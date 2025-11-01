/* File: io_data_file.c; Copyright and License: see below */

#include "io_data_file.h"
#include "io_exporter.h"
#include "io_file_format.h"
#include "io_importer.h"
#include "io_import_mode.h"
#include "entity/data_head.h"
#include "entity/data_head_key.h"
#include "storage/data_database_head.h"
#include "u8dir/u8dir_file.h"
#include "u8stream/universal_file_input_stream.h"
#include "u8stream/universal_null_output_stream.h"
#include "u8stream/universal_output_stream.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "u8/u8_u64.h"
#include <assert.h>

static const char *IO_DATA_FILE_TEMP_EXT = ".tmp-cfu";
static const char *IO_DATA_FILE_JSON_EXT = ".cfuJ";

void io_data_file_init ( io_data_file_t *this_ )
{
    U8_TRACE_BEGIN();

    data_database_init( &((*this_).database) );
    ctrl_controller_init( &((*this_).controller), &((*this_).database) );

    (*this_).data_file_name
        = utf8stringbuf_new( (*this_).private_data_file_name_buffer, sizeof((*this_).private_data_file_name_buffer) );
    utf8stringbuf_clear( &((*this_).data_file_name) );

    (*this_).db_file_name
        = utf8stringbuf_new( (*this_).private_db_file_name_buffer, sizeof((*this_).private_db_file_name_buffer) );
    utf8stringbuf_clear( &((*this_).db_file_name) );

    (*this_).auto_writeback_to_json = false;
    (*this_).delete_db_when_finished = false;
    (*this_).sync_revision = DATA_REVISION_VOID;

    U8_TRACE_END();
}

void io_data_file_destroy ( io_data_file_t *this_ )
{
    U8_TRACE_BEGIN();

    ctrl_controller_destroy( &((*this_).controller) );
    data_database_destroy( &((*this_).database) );

    U8_TRACE_END();
}

u8_error_t io_data_file_open ( io_data_file_t *this_,
                               const char* requested_file_path,
                               bool read_only,
                               u8_error_info_t *out_err_info )
{
    U8_TRACE_BEGIN();
    assert( requested_file_path != NULL );
    assert( out_err_info != NULL );
    u8_error_info_init_void( out_err_info );
    const utf8stringview_t req_file_path = UTF8STRINGVIEW_STR(requested_file_path);
    utf8stringview_t req_file_parent;
    utf8stringview_t req_file_name;
    utf8stringview_t req_file_basename;
    utf8stringview_t req_file_extension;
    io_data_file_private_split_path( this_, &req_file_path, &req_file_parent, &req_file_name );
    io_data_file_private_split_extension( this_, &req_file_name, &req_file_basename, &req_file_extension );

    const bool temp_requested = utf8string_ends_with_str( requested_file_path, IO_DATA_FILE_TEMP_EXT );
    bool is_json;
    const u8_error_t file_not_readable = io_data_file_private_guess_db_type( this_, requested_file_path, &is_json );
    u8_error_t err = U8_ERROR_NONE;

    if ( file_not_readable != U8_ERROR_NONE )
    {
        if (( temp_requested )||( ! is_json ))
        {
            U8_TRACE_INFO( "CASE: use temp db file that does not exist, is not accessible or has wrong format" );
            U8_TRACE_INFO( read_only ? "read_only" : "writeable" );
            /* This is a strange request, but we can create such an sqlite file. */
            /* To be consistent with the case of opening an existing temporary file, also this is exported to json later */
            (*this_).auto_writeback_to_json = ( ! read_only );
            (*this_).delete_db_when_finished = ( ! read_only );

            err |= utf8stringbuf_copy_view( &((*this_).data_file_name), &req_file_parent );
            err |= utf8stringbuf_append_view( &((*this_).data_file_name), &req_file_basename );
            err |= utf8stringbuf_append_str( &((*this_).data_file_name), IO_DATA_FILE_JSON_EXT );

            err |= utf8stringbuf_copy_str( &((*this_).db_file_name), requested_file_path );
        }
        else
        {
            U8_TRACE_INFO( "CASE: use json file that does not exist or is not accessible" );
            U8_TRACE_INFO( read_only ? "read_only" : "writeable" );
            /* A new json file shall be created */
            (*this_).auto_writeback_to_json = ( ! read_only );
            (*this_).delete_db_when_finished = ( ! read_only );
            err |= utf8stringbuf_copy_str( &((*this_).data_file_name), requested_file_path );
            err |= utf8stringbuf_copy_view( &((*this_).db_file_name), &req_file_parent );
            err |= utf8stringbuf_append_view( &((*this_).db_file_name), &req_file_basename );
            err |= utf8stringbuf_append_str( &((*this_).db_file_name), IO_DATA_FILE_TEMP_EXT );
            u8dir_file_remove( utf8stringbuf_get_string( &((*this_).db_file_name) ) );  /* ignore possible errors */
        }
    }
    else
    {
        if ( temp_requested )
        {
            U8_TRACE_INFO_STR( "CASE: use existing temp file", read_only ? "read_only" : "writeable" );
            /* A temporary sqlite file shall be used and later be exported to json */
            (*this_).auto_writeback_to_json = ( ! read_only );
            (*this_).delete_db_when_finished = ( ! read_only );
            err |= utf8stringbuf_copy_view( &((*this_).data_file_name), &req_file_parent );
            err |= utf8stringbuf_append_view( &((*this_).data_file_name), &req_file_basename );
            err |= utf8stringbuf_append_str( &((*this_).data_file_name), IO_DATA_FILE_JSON_EXT );
            err |= utf8stringbuf_copy_str( &((*this_).db_file_name), requested_file_path );
        }
        else if ( is_json )
        {
            U8_TRACE_INFO_STR( "CASE: use existing json file", read_only ? "read_only" : "writeable" );
            /* An existing json file shall be used */
            (*this_).auto_writeback_to_json = ( ! read_only );
            (*this_).delete_db_when_finished = true;
            err |= utf8stringbuf_copy_str( &((*this_).data_file_name), requested_file_path );
            err |= utf8stringbuf_copy_view( &((*this_).db_file_name), &req_file_parent );
            err |= utf8stringbuf_append_view( &((*this_).db_file_name), &req_file_basename );
            err |= utf8stringbuf_append_str( &((*this_).db_file_name), IO_DATA_FILE_TEMP_EXT );
            u8dir_file_remove( utf8stringbuf_get_string( &((*this_).db_file_name) ) );  /* ignore possible errors */

            err |= data_database_open( &((*this_).database), utf8stringbuf_get_string( &((*this_).db_file_name) ) );
            if ( err != U8_ERROR_NONE )
            {
                U8_LOG_ERROR("An error occurred at creating a temporary database file, possibly the parent directory is read-only.")
                U8_LOG_WARNING("Changes will not be written back to not accidentally overwrite the data source")
                (*this_).auto_writeback_to_json = false;
                (*this_).delete_db_when_finished = true;  /* do not keep .tmp-cfu files when import was not successful */
            }
            else
            {
                /* import */
                err |= io_data_file_private_import( this_, utf8stringbuf_get_string( &((*this_).data_file_name) ), out_err_info );

                /* update head data */
                data_database_head_t head_table;
                data_database_head_init( &head_table, &((*this_).database) );
                {
                    /* DATA_HEAD_KEY_DATA_FILE_NAME */
                    data_head_t head1;
                    const char *const requested_file_name = utf8stringview_get_start( &req_file_name );  /* This view is null terminated */
                    data_head_init_new( &head1, DATA_HEAD_KEY_DATA_FILE_NAME, requested_file_name );
                    err |= data_database_head_create_value( &head_table, &head1, NULL );
                    data_head_destroy( &head1 );
                    U8_TRACE_INFO_STR( "io_data_file_open/DATA_FILE_NAME", requested_file_name );

                    /* DATA_HEAD_KEY_DATA_FILE_LAST_SYNC_MOD_TIME */
                    uint64_t mod_time;
                    u8_error_t mtime_err = u8dir_file_get_modification_time( requested_file_path, &mod_time );
                    if ( mtime_err == U8_ERROR_NONE )
                    {
                        u8_u64_hex_t hex_time;
                        u8_u64_get_hex( mod_time, &hex_time );
                        data_head_t head2;
                        data_head_init_new( &head2, DATA_HEAD_KEY_DATA_FILE_LAST_SYNC_MOD_TIME, &(hex_time[0]) );
                        err |= data_database_head_create_value( &head_table, &head2, NULL );
                        data_head_destroy( &head2 );
                        U8_TRACE_INFO_STR( "io_data_file_open/DATA_FILE_LAST_SYNC_MOD_TIME", &(hex_time[0]) );
                    }
                }
                data_database_head_destroy( &head_table );

                /* finish import */
                err |= data_database_close( &((*this_).database) );

                if ( err != U8_ERROR_NONE )
                {
                    U8_LOG_ERROR("An error occurred at reading a json data file")
                    u8dir_file_remove( utf8stringbuf_get_string( &((*this_).db_file_name) ) );  /* ignore possible additional errors */
                    U8_LOG_WARNING("Changes will not be written back to not accidentally overwrite the data source")
                    (*this_).auto_writeback_to_json = false;
                    (*this_).delete_db_when_finished = true;  /* do not keep .tmp-cfu files when import was not successful */
                }
            }
        }
        else
        {
            U8_TRACE_INFO_STR( "CASE: use existing sqlite file", read_only ? "read_only" : "writeable" );
            /* An sqlite file shall be used */
            (*this_).auto_writeback_to_json = false;
            (*this_).delete_db_when_finished = false;
            err |= utf8stringbuf_copy_str( &((*this_).data_file_name), requested_file_path );
            err |= utf8stringbuf_copy_str( &((*this_).db_file_name), requested_file_path );
        }
    }
    U8_TRACE_INFO_STR( "data_file_name:", utf8stringbuf_get_string( &((*this_).data_file_name) ) );
    U8_TRACE_INFO_STR( "db_file_name:  ", utf8stringbuf_get_string( &((*this_).db_file_name) ) );

    if ( err == U8_ERROR_NONE )
    {
        if ( read_only )
        {
            err |= data_database_open_read_only( &((*this_).database), utf8stringbuf_get_string( &((*this_).db_file_name) ) );
        }
        else
        {
            err |= data_database_open( &((*this_).database), utf8stringbuf_get_string( &((*this_).db_file_name) ) );
        }

        /* get the sync revision */
        if ( file_not_readable != U8_ERROR_NONE )
        {
            /* new file or access problem */
            (*this_).sync_revision = DATA_REVISION_VOID;
        }
        else
        {
            if (( temp_requested )||( ! is_json ))
            {
                /* temp file is not in sync by definition */
                (*this_).sync_revision = DATA_REVISION_VOID;
            }
            else
            {
                (*this_).sync_revision = data_database_get_revision( &((*this_).database) );
                U8_TRACE_INFO_INT( "sync_revision", (*this_).sync_revision );
            }
        }
    }

    /* Reading the DATA_HEAD_KEY_DATA_FILE_NAME from the just opened (*this_).db_file_name */
    /* If found, update (*this_).data_file_name */
    if ( err == U8_ERROR_NONE )
    {
        data_database_head_t head_table;
        data_database_head_init( &head_table, &((*this_).database) );

        data_head_t head;
        u8_error_t key_err = data_database_head_read_value_by_key( &head_table, DATA_HEAD_KEY_DATA_FILE_NAME, &head );
        if ( key_err == U8_ERROR_NONE )
        {
            /* case: recovery after abnormal program termination */
            U8_TRACE_INFO_STR( "DATA_FILE_NAME:", data_head_get_value_const( &head ) );
            /* set the data_file_name to the read head value */
            err |= utf8stringbuf_copy_view( &((*this_).data_file_name), &req_file_parent );
            err |= utf8stringbuf_append_str( &((*this_).data_file_name), data_head_get_value_const( &head ) );
        }

        data_database_head_destroy( &head_table );
    }

    U8_TRACE_END_ERR( err );
    return err;
}

u8_error_t io_data_file_close ( io_data_file_t *this_ )
{
    U8_TRACE_BEGIN();

    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).auto_writeback_to_json && ( ! io_data_file_is_in_sync( this_ ) ) )
    {
        U8_TRACE_INFO( "CASE: auto_writeback_to_json == true" );
        result |= io_data_file_private_export( this_, utf8stringbuf_get_string( &((*this_).data_file_name) ) );
    }

    result |= data_database_close( &((*this_).database) );

    if ( (*this_).delete_db_when_finished )
    {
        U8_TRACE_INFO( "CASE: delete_db_when_finished == true" );
        u8dir_file_remove( utf8stringbuf_get_string( &((*this_).db_file_name) ) );  /* ignore possible errors */
    }

    (*this_).auto_writeback_to_json = false;
    (*this_).delete_db_when_finished = false;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t io_data_file_sync_to_disk ( io_data_file_t *this_ )
{
    U8_TRACE_BEGIN();

    u8_error_t result = data_database_flush_caches( &((*this_).database) );

    if ( (*this_).auto_writeback_to_json )  /* ignore if already in_sync - if explicitly requested, simply do sync (again) */
    {
        result |= io_data_file_private_export( this_, utf8stringbuf_get_string( &((*this_).data_file_name) ) );
    }

    /* get sync revision */
    (*this_).sync_revision = data_database_get_revision( &((*this_).database) );
    U8_TRACE_INFO_INT( "sync_revision", (*this_).sync_revision );

    /* update head data */
    data_database_head_t head_table;
    data_database_head_init( &head_table, &((*this_).database) );
    {
        /* DATA_HEAD_KEY_DATA_FILE_LAST_SYNC_MOD_TIME */
        uint64_t mod_time;
        const char *const json_file_path = utf8stringbuf_get_string( &((*this_).data_file_name) );
        u8_error_t mtime_err = u8dir_file_get_modification_time( json_file_path, &mod_time );
        if ( mtime_err == U8_ERROR_NONE )
        {
            u8_u64_hex_t hex_time;
            u8_u64_get_hex( mod_time, &hex_time );
            result |= data_database_head_update_value_by_key( &head_table,
                                                              DATA_HEAD_KEY_DATA_FILE_LAST_SYNC_MOD_TIME,
                                                              &(hex_time[0]),
                                                              NULL
                                                            );
            U8_TRACE_INFO_STR( "io_data_file_sync_to_disk/DATA_FILE_LAST_SYNC_MOD_TIME", &(hex_time[0]) );
        }
    }
    data_database_head_destroy( &head_table );

    /* restore sync revision so that undo_redo history and this_ refer to the same revision */
    data_database_set_revision( &((*this_).database), (*this_).sync_revision );

    U8_TRACE_END_ERR( result );
    return result;
}

bool io_data_file_is_externally_modified ( io_data_file_t *this_ )
{
    U8_TRACE_BEGIN();
    bool result = false;

    if ( data_database_is_open( &((*this_).database) ) )
    {
        data_database_head_t head_table;
        data_database_head_init( &head_table, &((*this_).database) );
        {
            uint64_t mod_time;
            const char *const json_file_path = utf8stringbuf_get_string( &((*this_).data_file_name) );
            u8_error_t mtime_err = u8dir_file_get_modification_time( json_file_path, &mod_time );
            if ( mtime_err == U8_ERROR_NONE )
            {
                u8_u64_hex_t hex_time;
                u8_u64_get_hex( mod_time, &hex_time );

                data_head_t head_val;
                const u8_error_t db_err
                    = data_database_head_read_value_by_key( &head_table,
                                                            DATA_HEAD_KEY_DATA_FILE_LAST_SYNC_MOD_TIME,
                                                            &head_val
                                                        );
                if ( db_err == U8_ERROR_NONE )
                {
                    if ( ! utf8string_equals_str( &(hex_time[0]), data_head_get_value_const( &head_val ) ) )
                    {
                        result = true;
                    }
                }
            }
        }
        data_database_head_destroy( &head_table );
    }

    U8_TRACE_END();
    return result;
}

u8_error_t io_data_file_trace_stats ( io_data_file_t *this_ )
{
    U8_TRACE_BEGIN();

    U8_TRACE_INFO_STR( "io_data_file_t:", utf8stringbuf_get_string( &((*this_).data_file_name) ) );

    const u8_error_t result = data_database_trace_stats( &((*this_).database) );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t io_data_file_private_guess_db_type ( const io_data_file_t *this_, const char *filename, bool *out_json )
{
    U8_TRACE_BEGIN();
    assert( filename != NULL );
    assert( out_json != NULL );
    u8_error_t scan_head_error = U8_ERROR_NONE;

    /* open file */
    universal_file_input_stream_t in_file;
    universal_file_input_stream_init( &in_file );
    scan_head_error |= universal_file_input_stream_open( &in_file, filename );

    /* import from stream */
    if ( scan_head_error == U8_ERROR_NONE )
    {
        char file_prefix[16];
        size_t prefix_size;
        assert( sizeof(file_prefix) == sizeof(DATA_DATABASE_SQLITE3_MAGIC) );
        scan_head_error = universal_file_input_stream_read( &in_file, &file_prefix, sizeof(file_prefix), &prefix_size );
        if (( scan_head_error == U8_ERROR_NONE )&&( prefix_size == sizeof(file_prefix) )
            &&( 0 == memcmp( &file_prefix, &DATA_DATABASE_SQLITE3_MAGIC, sizeof(file_prefix) ) ))
        {
            U8_TRACE_INFO_STR("File exists and starts with sqlite3 magic:", filename);
            *out_json = false;
        }
        else
        {
            U8_TRACE_INFO_STR("File exists and is not of type sqlite3:", filename);
            *out_json = true;
        }

        /* close file */
        scan_head_error |= universal_file_input_stream_close( &in_file );
    }
    else
    {
        U8_TRACE_INFO_STR("File does not exist", filename);
        *out_json = true;
    }

    /* cleanup */
    scan_head_error |= universal_file_input_stream_destroy( &in_file );

    U8_TRACE_END_ERR( scan_head_error );
    return scan_head_error;
}

u8_error_t io_data_file_private_import ( io_data_file_t *this_, const char *src_file, u8_error_info_t *out_err_info )
{
    U8_TRACE_BEGIN();
    assert( src_file != NULL );
    assert( out_err_info != NULL );
    u8_error_info_init_void( out_err_info );
    u8_error_t import_err = U8_ERROR_NONE;
    static const io_import_mode_t import_mode = IO_IMPORT_MODE_CREATE|IO_IMPORT_MODE_LINK;
    universal_null_output_stream_t dev_null;
    universal_null_output_stream_init( &dev_null );
    utf8stream_writer_t out_null;
    utf8stream_writer_init( &out_null, universal_null_output_stream_get_output_stream( &dev_null ) );

    U8_TRACE_INFO_STR( "importing file:", src_file );
    if ( io_data_file_is_open( this_ ) )
    {
        static data_database_reader_t db_reader;
        data_database_reader_init( &db_reader, &((*this_).database) );
        static io_importer_t importer;
        io_importer_init( &importer, &db_reader, &((*this_).controller) );
        {
            data_stat_t import_stat;
            data_stat_init ( &import_stat );
            import_err = io_importer_import_file( &importer, import_mode, src_file, &import_stat, out_err_info, &out_null );
            data_stat_trace( &import_stat );
            data_stat_destroy ( &import_stat );
        }
        io_importer_destroy( &importer );
        data_database_reader_destroy( &db_reader );
    }
    else
    {
        import_err = U8_ERROR_NO_DB;
    }

    utf8stream_writer_destroy( &out_null );
    universal_null_output_stream_destroy( &dev_null );
    U8_TRACE_END_ERR( import_err );
    return import_err;
}

u8_error_t io_data_file_private_export ( io_data_file_t *this_, const char *dst_file )
{
    U8_TRACE_BEGIN();
    assert( dst_file != NULL );
    u8_error_t export_err = U8_ERROR_NONE;

    U8_TRACE_INFO_STR( "exporting file:", dst_file );
    const char *document_filename = io_data_file_get_filename_const( this_ );
    if ( io_data_file_is_open( this_ ) )
    {
        static data_database_reader_t db_reader;
        data_database_reader_init( &db_reader, &((*this_).database) );
        static io_exporter_t exporter;
        io_exporter_init( &exporter, &db_reader );
        {
            data_stat_t export_stat;
            data_stat_init ( &export_stat );
            export_err = io_exporter_export_document_file( &exporter, IO_FILE_FORMAT_JSON, "title", document_filename, &export_stat );
            data_stat_trace( &export_stat );
            data_stat_destroy ( &export_stat );
        }
        io_exporter_destroy( &exporter );
        data_database_reader_destroy( &db_reader );
    }
    else
    {
        export_err = U8_ERROR_NO_DB;
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}


/*
Copyright 2022-2025 Andreas Warnke

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
