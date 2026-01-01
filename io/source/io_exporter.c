/* File: io_exporter.c; Copyright and License: see below */

#include "io_exporter.h"
#include "document/document_css_writer.h"
#include "json/json_schema_writer.h"
#include "u8stream/universal_file_output_stream.h"
#include "u8stream/universal_output_stream.h"
#include "xmi/xmi_writer_pass.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "io_gtk.h"
#include <cairo-svg.h>
#include <cairo-pdf.h>
#include <cairo-ps.h>
#include <stdbool.h>
#include <assert.h>

enum io_exporter_max_enum {
    IO_EXPORTER_MAX_DIAGRAM_TREE_DEPTH = 16,
};

void io_exporter_init ( io_exporter_t *this_,
                        data_database_reader_t *db_reader )
{
    U8_TRACE_BEGIN();
    assert( NULL != db_reader );

    (*this_).db_reader = db_reader;

    (*this_).temp_filename = utf8stringbuf_new( (*this_).temp_filename_buf, sizeof((*this_).temp_filename_buf) );
    utf8stringbuf_clear( &((*this_).temp_filename) );

    U8_TRACE_END();
}

void io_exporter_destroy( io_exporter_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).db_reader = NULL;

    U8_TRACE_END();
}

u8_error_t io_exporter_export_files( io_exporter_t *this_,
                                     io_file_format_t export_type,
                                     const char *target_folder,
                                     const char *document_file_path,
                                     data_stat_t *io_export_stat )
{
    U8_TRACE_BEGIN();
    assert ( NULL != target_folder );
    assert ( NULL != document_file_path );
    assert ( NULL != io_export_stat );
    u8_error_t export_err = U8_ERROR_NONE;

    /* transform file path to name */
    char temp_filename_buf[48];
    utf8stringbuf_t temp_filename = UTF8STRINGBUF(temp_filename_buf);
    u8_error_t err = io_exporter_private_get_filename( this_, document_file_path, temp_filename );
    const char *const document_file_name = (err==0) ? utf8stringbuf_get_string( &temp_filename ) : "document";

    if ( NULL != target_folder )
    {
        if ( ( export_type & IO_FILE_FORMAT_SVG ) != 0 )
        {
            export_err |= io_exporter_private_export_image_files( this_, DATA_ID_VOID, IO_EXPORTER_MAX_DIAGRAM_TREE_DEPTH, IO_FILE_FORMAT_SVG, target_folder, io_export_stat );
        }

        if ( ( export_type & ( IO_FILE_FORMAT_PDF | IO_FILE_FORMAT_DOCBOOK ) ) != 0 )
        {
            export_err |= io_exporter_private_export_image_files( this_, DATA_ID_VOID, IO_EXPORTER_MAX_DIAGRAM_TREE_DEPTH, IO_FILE_FORMAT_PDF, target_folder, io_export_stat );
        }

        if ( ( export_type & IO_FILE_FORMAT_PS ) != 0 )
        {
            export_err |= io_exporter_private_export_image_files( this_, DATA_ID_VOID, IO_EXPORTER_MAX_DIAGRAM_TREE_DEPTH, IO_FILE_FORMAT_PS, target_folder, io_export_stat );
        }

        if ( ( export_type & ( IO_FILE_FORMAT_PNG | IO_FILE_FORMAT_DOCBOOK | IO_FILE_FORMAT_HTML ) ) != 0 )
        {
            export_err |= io_exporter_private_export_image_files( this_, DATA_ID_VOID, IO_EXPORTER_MAX_DIAGRAM_TREE_DEPTH, IO_FILE_FORMAT_PNG, target_folder, io_export_stat );
        }

        if ( ( export_type & IO_FILE_FORMAT_TXT ) != 0 )
        {
            export_err |= io_exporter_private_export_image_files( this_, DATA_ID_VOID, IO_EXPORTER_MAX_DIAGRAM_TREE_DEPTH, IO_FILE_FORMAT_TXT, target_folder, io_export_stat );
        }

        if ( ( export_type & IO_FILE_FORMAT_DOCBOOK ) != 0 )
        {
            export_err |= io_exporter_private_export_document_file( this_, IO_FILE_FORMAT_DOCBOOK, target_folder, document_file_name, io_export_stat );
        }

        if ( ( export_type & IO_FILE_FORMAT_HTML ) != 0 )
        {
            export_err |= io_exporter_private_export_document_file( this_,
                                                                    IO_FILE_FORMAT_HTML,
                                                                    target_folder,
                                                                    document_file_name,
                                                                    io_export_stat
                                                                  );
            export_err |= io_exporter_private_export_document_file( this_,
                                                                    IO_FILE_FORMAT_CSS,
                                                                    target_folder,
                                                                    document_file_name,
                                                                    io_export_stat
                                                                  );
        }

        if ( ( export_type & IO_FILE_FORMAT_JSON ) != 0 )
        {
            export_err |= io_exporter_private_export_document_file( this_,
                                                                    IO_FILE_FORMAT_JSON,
                                                                    target_folder,
                                                                    document_file_name,
                                                                    io_export_stat
                                                                  );
            export_err |= io_exporter_private_export_document_file( this_,
                                                                    IO_FILE_FORMAT_SCHEMA,
                                                                    target_folder,
                                                                    document_file_name,
                                                                    io_export_stat
                                                                  );
        }

        if ( ( export_type & IO_FILE_FORMAT_XMI2 ) != 0 )
        {
            export_err |= io_exporter_private_export_document_file( this_, IO_FILE_FORMAT_XMI2, target_folder, document_file_name, io_export_stat );
        }
    }
    else /* target_folder == NULL */
    {
        U8_LOG_WARNING("selected target folder was NULL.");
        export_err = -1;
    }

    U8_TRACE_END_ERR(export_err);
    return export_err;
}

u8_error_t io_exporter_private_get_filename( io_exporter_t *this_,
                                             const char* path,
                                             utf8stringbuf_t out_base_filename )
{
    U8_TRACE_BEGIN();
    assert ( NULL != path );
    u8_error_t err = U8_ERROR_NONE;

    utf8stringview_t path_view = UTF8STRINGVIEW_STR( path );

    utf8stringview_t before_winpath_sep;
    utf8stringview_t after_winpath_sep;
    const utf8error_t err_w = utf8stringview_split_at_last_str( &path_view, "\\", &before_winpath_sep, &after_winpath_sep );
    if ( err_w != UTF8ERROR_SUCCESS )
    {
        after_winpath_sep = path_view;
    }
    utf8stringview_t before_unixpath_sep;
    utf8stringview_t after_unixpath_sep;
    const utf8error_t err_u = utf8stringview_split_at_last_str( &after_winpath_sep, "/", &before_unixpath_sep, &after_unixpath_sep );
    if ( err_u != UTF8ERROR_SUCCESS )
    {
        after_unixpath_sep = after_winpath_sep;
    }
    utf8stringview_t before_dot;
    utf8stringview_t after_dot;
    const utf8error_t err_d = utf8stringview_split_at_last_str( &after_unixpath_sep, ".", &before_dot, &after_dot );
    if ( ( err_d != UTF8ERROR_SUCCESS )
        || ( utf8stringview_get_length( &before_dot ) == 0 )
        || ( utf8stringview_get_length( &after_dot ) == 0 ) )
    {
        /* either no dot found or the filename begins with dot or the filename ends on dot */
        before_dot = after_unixpath_sep;
    }

    err = utf8stringbuf_copy_view( &out_base_filename, &before_dot );
    if ( utf8stringbuf_get_length( &out_base_filename ) == 0 )
    {
        err = U8_ERROR_INPUT_EMPTY;
    }

    U8_TRACE_END_ERR(err);
    return err;
}

u8_error_t io_exporter_private_export_image_files( io_exporter_t *this_,
                                                   data_id_t diagram_id,
                                                   uint32_t max_recursion,
                                                   io_file_format_t export_type,
                                                   const char *target_folder,
                                                   data_stat_t *io_export_stat )
{
    U8_TRACE_BEGIN();
    assert ( NULL != target_folder );
    assert ( NULL != io_export_stat );
    U8_TRACE_INFO_STR("target_folder:", target_folder );
    const data_row_t diagram_row_id = data_id_get_row_id( &diagram_id );
    u8_error_t result = U8_ERROR_NONE;

    /* draw current diagram */
    if ( DATA_ROW_VOID != diagram_row_id )
    {
        assert( data_id_get_table( &diagram_id ) == DATA_TABLE_DIAGRAM );

        /* determine filename */
        utf8stringbuf_copy_str( &((*this_).temp_filename), target_folder );
        utf8stringbuf_append_str( &((*this_).temp_filename), "/" );
        result |= io_exporter_private_get_filename_for_diagram( this_, diagram_id, utf8stringbuf_get_end( &((*this_).temp_filename) ) );

        if ( IO_FILE_FORMAT_SVG == export_type )
        {
            utf8stringbuf_append_str( &((*this_).temp_filename), ".svg" );
        }
        else if ( IO_FILE_FORMAT_PNG == export_type )
        {
            utf8stringbuf_append_str( &((*this_).temp_filename), ".png" );
        }
        else if ( IO_FILE_FORMAT_PDF == export_type )
        {
            utf8stringbuf_append_str( &((*this_).temp_filename), ".pdf" );
        }
        else if ( IO_FILE_FORMAT_PS == export_type )
        {
            utf8stringbuf_append_str( &((*this_).temp_filename), ".ps" );
        }
        else /* IO_FILE_FORMAT_TXT */
        {
            utf8stringbuf_append_str( &((*this_).temp_filename), ".txt" );
        }
        U8_LOG_EVENT_STR( "exporting diagram to file:", utf8stringbuf_get_string( &((*this_).temp_filename) ) );

        result |= io_exporter_export_image_file( this_,
                                                 diagram_id,
                                                 export_type,
                                                 utf8stringbuf_get_string( &((*this_).temp_filename) ),
                                                 io_export_stat
                                               );
    }

    /* recursion to children */
    if (( result == 0 )&&( max_recursion > 0 ))
    {
        u8_error_t db_err;
        data_small_set_t the_set;
        data_small_set_init( &the_set );
        db_err = data_database_reader_get_diagram_ids_by_parent_id ( (*this_).db_reader, diagram_row_id, &the_set );
        if ( db_err != U8_ERROR_NONE )
        {
            U8_LOG_ERROR("error reading database.");
            result = -1;
        }
        else
        {
            for ( uint32_t pos = 0; pos < data_small_set_get_count( &the_set ); pos ++ )
            {
                data_id_t probe_id;
                probe_id = data_small_set_get_id( &the_set, pos );

                result |= io_exporter_private_export_image_files( this_, probe_id, max_recursion-1, export_type, target_folder, io_export_stat );

                data_id_destroy( &probe_id );
            }
        }
        data_small_set_destroy( &the_set );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t io_exporter_export_image_file( io_exporter_t *this_,
                                          data_id_t diagram_id,
                                          io_file_format_t export_type,
                                          const char *file_path,
                                          data_stat_t *io_export_stat )
{
    U8_TRACE_BEGIN();
    assert ( data_id_is_valid( &diagram_id ) );
    assert ( NULL != file_path );
    assert ( NULL != io_export_stat );
    U8_TRACE_INFO_STR("file_path:", file_path );
    u8_error_t result = U8_ERROR_NONE;

    /* create surface */
    if (( IO_FILE_FORMAT_SVG == export_type )
        || ( IO_FILE_FORMAT_PDF == export_type )
        || ( IO_FILE_FORMAT_PS == export_type )
        || ( IO_FILE_FORMAT_PNG == export_type ) )
    {
        image_format_writer_init( &((*this_).temp_image_format_exporter ),
                                  (*this_).db_reader,
                                  &((*this_).temp_input_data),
                                  &((*this_).temp_profile)
                                );
        result |= image_format_writer_render_diagram_to_file( &((*this_).temp_image_format_exporter ),
                                                              diagram_id,
                                                              export_type,
                                                              file_path,
                                                              io_export_stat
                                                            );
        image_format_writer_destroy( &((*this_).temp_image_format_exporter ) );
    }
    else /* IO_FILE_FORMAT_TXT */
    {
        universal_file_output_stream_t text_output;
        universal_file_output_stream_init( &text_output );
        universal_output_stream_t *output = universal_file_output_stream_get_output_stream( &text_output );

        /* open file */
        result |= universal_file_output_stream_open( &text_output, file_path );
        if ( result == 0 )
        {
            u8_error_t write_err = U8_ERROR_NONE;

            /* temporarily use the temp_model_traversal */
            /* write file */
            document_element_writer_init( &((*this_).temp_format_writer ),
                                          (*this_).db_reader,
                                          IO_FILE_FORMAT_TXT,
                                          io_export_stat,
                                          output
                                        );
            io_export_diagram_traversal_init( &((*this_).temp_diagram_traversal),
                                              (*this_).db_reader,
                                              &((*this_).temp_input_data),
                                              io_export_stat,
                                              document_element_writer_get_element_writer( &((*this_).temp_format_writer) )
                                            );
            write_err |= document_element_writer_write_header( &((*this_).temp_format_writer), "DUMMY_TITLE" );
            write_err |= io_export_diagram_traversal_begin_and_walk_diagram ( &((*this_).temp_diagram_traversal), diagram_id, "NO_IMAGE_FILE" );
            write_err |= io_export_diagram_traversal_end_diagram ( &((*this_).temp_diagram_traversal), diagram_id );
            write_err |= document_element_writer_write_footer( &((*this_).temp_format_writer) );
            io_export_diagram_traversal_destroy( &((*this_).temp_diagram_traversal) );
            document_element_writer_destroy( &((*this_).temp_format_writer ) );

            if ( 0 != write_err )
            {
                U8_LOG_ERROR("error writing txt.");
                result = -1;
            }

            /* close file */
            result |= universal_file_output_stream_close( &text_output );
        }

        result |= universal_file_output_stream_destroy( &text_output );

        data_stat_inc_count ( io_export_stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_EXPORTED );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t io_exporter_private_export_document_file( io_exporter_t *this_,
                                                     io_file_format_t export_type,
                                                     const char *target_folder,
                                                     const char *document_file_name,
                                                     data_stat_t *io_export_stat )
{
    U8_TRACE_BEGIN();
    assert ( NULL != target_folder );
    assert ( NULL != document_file_name );
    assert ( NULL != io_export_stat );
    U8_TRACE_INFO_STR("target_folder:", target_folder );
    U8_TRACE_INFO_STR("document_file_name:", document_file_name );
    u8_error_t export_err = U8_ERROR_NONE;

    /* open file */
    utf8stringbuf_copy_str( &((*this_).temp_filename), target_folder );
    utf8stringbuf_append_str( &((*this_).temp_filename), "/" );
    utf8stringbuf_append_str( &((*this_).temp_filename), document_file_name );
    switch ( export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            utf8stringbuf_append_str( &((*this_).temp_filename), ".xml" );
        }
        break;

        case IO_FILE_FORMAT_HTML:
        {
            utf8stringbuf_append_str( &((*this_).temp_filename), ".html" );
        }
        break;

        case IO_FILE_FORMAT_CSS:
        {
            utf8stringbuf_append_str( &((*this_).temp_filename), ".css" );
        }
        break;

        case IO_FILE_FORMAT_JSON:
        {
            utf8stringbuf_append_str( &((*this_).temp_filename), ".json" );
        }
        break;

        case IO_FILE_FORMAT_SCHEMA:
        {
            utf8stringbuf_append_str( &((*this_).temp_filename), ".schema" );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            utf8stringbuf_append_str( &((*this_).temp_filename), ".xmi" );
        }
        break;

        default:
        {
            utf8stringbuf_append_str( &((*this_).temp_filename), ".unknown_format" );
            U8_LOG_ERROR("error: unknown_format.");
        }
        break;
    }
    U8_LOG_EVENT_STR( "exporting diagrams to document file:", utf8stringbuf_get_string( &((*this_).temp_filename) ) );

    export_err |= io_exporter_export_document_file( this_,
                                                    export_type,
                                                    document_file_name,
                                                    utf8stringbuf_get_string( &((*this_).temp_filename) ),
                                                    io_export_stat
                                                  );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t io_exporter_export_document_file( io_exporter_t *this_,
                                             io_file_format_t export_type,
                                             const char *document_title,
                                             const char *file_path,
                                             data_stat_t *io_export_stat )
{
    U8_TRACE_BEGIN();
    assert ( NULL != document_title );
    assert ( NULL != file_path );
    assert ( NULL != io_export_stat );
    U8_TRACE_INFO_STR("file_path:", file_path );
    u8_error_t export_err = U8_ERROR_NONE;
    universal_file_output_stream_t file_output;
    universal_file_output_stream_init( &file_output );
    universal_output_stream_t *output = universal_file_output_stream_get_output_stream( &file_output );

    export_err |= universal_file_output_stream_open( &file_output, file_path );
    if ( export_err == 0 )
    {
        /* write file */
        if ( IO_FILE_FORMAT_CSS == export_type )
        {
            document_css_writer_t css_writer;
            document_css_writer_init( &css_writer, output );
            export_err |= document_css_writer_write_stylesheet( &css_writer );
            document_css_writer_destroy( &css_writer );
        }
        else if ( IO_FILE_FORMAT_SCHEMA == export_type )
        {
            json_schema_writer_t schema_writer;
            json_schema_writer_init( &schema_writer, output );
            export_err |= json_schema_writer_write_schema( &schema_writer );
            json_schema_writer_destroy( &schema_writer );
        }
        else if ( IO_FILE_FORMAT_XMI2 == export_type )
        {
            xmi_element_writer_init( &((*this_).temp_xmi_writer ), io_export_stat, output );
            /* init the model_traversal */
            io_export_model_traversal_init( &((*this_).temp_model_traversal),
                                            (*this_).db_reader,
                                            &((*this_).temp_input_data),
                                            io_export_stat,
                                            xmi_element_writer_get_element_writer( &((*this_).temp_xmi_writer) )
                                          );
            /* write the document */
            export_err |= xmi_element_writer_write_header( &((*this_).temp_xmi_writer), document_title );
            export_err |= xmi_element_writer_start_main( &((*this_).temp_xmi_writer), document_title );
            xmi_element_writer_set_mode( &((*this_).temp_xmi_writer ), XMI_WRITER_PASS_BASE );
            export_err |= io_export_model_traversal_walk_model_nodes( &((*this_).temp_model_traversal) );
            export_err |= xmi_element_writer_end_main( &((*this_).temp_xmi_writer) );
            xmi_element_writer_set_mode( &((*this_).temp_xmi_writer ), XMI_WRITER_PASS_PROFILE );
            export_err |= io_export_model_traversal_walk_model_nodes( &((*this_).temp_model_traversal) );
            export_err |= xmi_element_writer_write_footer( &((*this_).temp_xmi_writer) );

            io_export_model_traversal_destroy( &((*this_).temp_model_traversal) );
            xmi_element_writer_destroy( &((*this_).temp_xmi_writer ) );
        }
        else if ( IO_FILE_FORMAT_JSON == export_type )
        {
            json_element_writer_init( &((*this_).temp_json_writer ), io_export_stat, output );
            export_err |= json_element_writer_write_header( &((*this_).temp_json_writer), document_title );

            /* init the diagram_traversal */
            {
                io_export_diagram_traversal_init( &((*this_).temp_diagram_traversal),
                                                  (*this_).db_reader,
                                                  &((*this_).temp_input_data),
                                                  io_export_stat,
                                                  json_element_writer_get_element_writer( &((*this_).temp_json_writer) )
                                                );
                /* write the document */
                json_element_writer_set_mode( &((*this_).temp_json_writer ), JSON_WRITER_PASS_VIEWS );
                export_err |= json_element_writer_start_main( &((*this_).temp_json_writer), document_title );
                export_err |= io_exporter_private_export_document_part( this_, DATA_ID_VOID, IO_EXPORTER_MAX_DIAGRAM_TREE_DEPTH, io_export_stat );
                export_err |= json_element_writer_end_main( &((*this_).temp_json_writer) );

                io_export_diagram_traversal_destroy( &((*this_).temp_diagram_traversal) );
            }

            /* init the model_traversal */
            {
                io_export_flat_traversal_init( &((*this_).temp_flat_traversal),
                                               (*this_).db_reader,
                                               io_export_stat,
                                               json_element_writer_get_element_writer( &((*this_).temp_json_writer) )
                                             );
                /* write the document */
                json_element_writer_set_mode( &((*this_).temp_json_writer ), JSON_WRITER_PASS_NODES );
                export_err |= json_element_writer_start_main( &((*this_).temp_json_writer), document_title );
                export_err |= io_export_flat_traversal_iterate_classifiers( &((*this_).temp_flat_traversal), false );
                export_err |= json_element_writer_end_main( &((*this_).temp_json_writer) );

                json_element_writer_set_mode( &((*this_).temp_json_writer ), JSON_WRITER_PASS_EDGES );
                export_err |= json_element_writer_start_main( &((*this_).temp_json_writer), document_title );
                export_err |= io_export_flat_traversal_iterate_classifiers( &((*this_).temp_flat_traversal), false );
                export_err |= json_element_writer_end_main( &((*this_).temp_json_writer) );

                io_export_flat_traversal_destroy( &((*this_).temp_flat_traversal) );
            }

            export_err |= json_element_writer_write_footer( &((*this_).temp_json_writer) );
            json_element_writer_destroy( &((*this_).temp_json_writer ) );
        }
        else
        {
            document_element_writer_init( &((*this_).temp_format_writer ),
                                          (*this_).db_reader,
                                          export_type,
                                          io_export_stat,
                                          output
                                        );
            /* init the diagram_traversal */
            io_export_diagram_traversal_init( &((*this_).temp_diagram_traversal),
                                              (*this_).db_reader,
                                              &((*this_).temp_input_data),
                                              io_export_stat,
                                              document_element_writer_get_element_writer( &((*this_).temp_format_writer) )
                                            );
            /* write the document */
            export_err |= document_element_writer_write_header( &((*this_).temp_format_writer), document_title );
            export_err |= document_element_writer_start_toc( &((*this_).temp_format_writer) );
            export_err |= io_exporter_private_export_table_of_contents( this_,
                                                                        DATA_ID_VOID,
                                                                        IO_EXPORTER_MAX_DIAGRAM_TREE_DEPTH,
                                                                        &((*this_).temp_format_writer)
                                                                      );
            export_err |= document_element_writer_end_toc( &((*this_).temp_format_writer) );
            export_err |= document_element_writer_start_main( &((*this_).temp_format_writer), document_title );
            export_err |= io_exporter_private_export_document_part( this_, DATA_ID_VOID,
                                                                    IO_EXPORTER_MAX_DIAGRAM_TREE_DEPTH,
                                                                    io_export_stat
                                                                  );
            export_err |= document_element_writer_end_main( &((*this_).temp_format_writer) );
            export_err |= document_element_writer_write_footer( &((*this_).temp_format_writer) );

            io_export_diagram_traversal_destroy( &((*this_).temp_diagram_traversal) );
            document_element_writer_destroy( &((*this_).temp_format_writer ) );
        }

        /* close file */
        export_err |= universal_file_output_stream_close( &file_output );
    }

    export_err |= universal_file_output_stream_destroy( &file_output );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t io_exporter_private_export_document_part( io_exporter_t *this_,
                                                     data_id_t diagram_id,
                                                     uint32_t max_recursion,
                                                     data_stat_t *io_export_stat )
{
    U8_TRACE_BEGIN();
    assert ( NULL != io_export_stat );
    const data_row_t diagram_row_id = data_id_get_row_id( &diagram_id );
    u8_error_t export_err = U8_ERROR_NONE;

    /* write part for current diagram */
    if ( DATA_ROW_VOID != diagram_row_id )
    {
        assert( data_id_get_table( &diagram_id ) == DATA_TABLE_DIAGRAM );

        /* determine filename */
        export_err |= io_exporter_private_get_filename_for_diagram( this_, diagram_id, (*this_).temp_filename );


        /* write doc part */
        export_err |= io_export_diagram_traversal_begin_and_walk_diagram( &((*this_).temp_diagram_traversal),
                                                                          diagram_id,
                                                                          utf8stringbuf_get_string( &((*this_).temp_filename) )
                                                                        );
    }

    /* recursion to children */
    if (( export_err == 0 )&&( max_recursion > 0 ))
    {
        u8_error_t db_err;
        data_small_set_t the_set;
        data_small_set_init( &the_set );
        db_err = data_database_reader_get_diagram_ids_by_parent_id ( (*this_).db_reader, diagram_row_id, &the_set );
        if ( db_err != U8_ERROR_NONE )
        {
            U8_LOG_ERROR("error reading database.");
            export_err |= -1;
        }
        else
        {
            const uint32_t child_count = data_small_set_get_count( &the_set );
            for ( uint32_t pos = 0; pos < child_count; pos ++ )
            {
                data_id_t probe_id = data_small_set_get_id( &the_set, pos );

                export_err |= io_exporter_private_export_document_part( this_, probe_id, max_recursion-1, io_export_stat );

                data_id_destroy( &probe_id );
            }
        }
        data_small_set_destroy( &the_set );
    }

    /* end diagram section */
    if ( DATA_ROW_VOID != diagram_row_id )
    {
        /* write end of doc part */
        export_err |= io_export_diagram_traversal_end_diagram( &((*this_).temp_diagram_traversal), diagram_id );
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t io_exporter_private_export_table_of_contents( io_exporter_t *this_,
                                                         data_id_t diagram_id,
                                                         uint32_t max_recursion,
                                                         document_element_writer_t *format_writer )
{
    U8_TRACE_BEGIN();
    assert ( NULL != format_writer );
    const data_row_t diagram_row_id = data_id_get_row_id( &diagram_id );
    u8_error_t export_err = U8_ERROR_NONE;

    /* write entry for current diagram */
    if ( DATA_ROW_VOID != diagram_row_id )
    {
        assert( data_id_get_table( &diagram_id ) == DATA_TABLE_DIAGRAM );

        export_err |= document_element_writer_start_toc_entry( format_writer );

        /* load data to be drawn */
        u8_error_t db_err;
        db_err = data_database_reader_get_diagram_by_id ( (*this_).db_reader, diagram_row_id, &((*this_).temp_diagram) );
        if ( db_err != U8_ERROR_NONE )
        {
            U8_LOG_ERROR("error reading database.");
            export_err |= -1;
        }
        else
        {
            export_err |= document_element_writer_write_toc_entry ( format_writer, &((*this_).temp_diagram) );
            data_diagram_destroy( &((*this_).temp_diagram) );
        }
    }

    /* recursion to children */
    if (( export_err == 0 )&&( max_recursion > 0 ))
    {
        u8_error_t db_err;
        data_small_set_t the_set;
        data_small_set_init( &the_set );
        db_err = data_database_reader_get_diagram_ids_by_parent_id ( (*this_).db_reader, diagram_row_id, &the_set );
        if ( db_err != U8_ERROR_NONE )
        {
            U8_LOG_ERROR("error reading database.");
            export_err |= -1;
        }
        else
        {
            const uint32_t child_count = data_small_set_get_count( &the_set );
            if ( child_count != 0 )
            {
                export_err |= document_element_writer_start_toc_sublist( format_writer );
                for ( uint32_t pos = 0; pos < child_count; pos ++ )
                {
                    data_id_t probe_id = data_small_set_get_id( &the_set, pos );

                    export_err |= io_exporter_private_export_table_of_contents( this_, probe_id, max_recursion-1, format_writer );

                    data_id_destroy( &probe_id );
                }
                export_err |= document_element_writer_end_toc_sublist ( format_writer );
            }
        }
        data_small_set_destroy( &the_set );
    }

    /* end toc entry */
    if ( DATA_ROW_VOID != diagram_row_id )
    {
        export_err |= document_element_writer_end_toc_entry( format_writer );
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t io_exporter_private_get_filename_for_diagram( io_exporter_t *this_,
                                                         data_id_t diagram_id,
                                                         utf8stringbuf_t filename )
{
    U8_TRACE_BEGIN();
    assert( data_id_get_table( &diagram_id ) == DATA_TABLE_DIAGRAM );
    u8_error_t result = U8_ERROR_NONE;
    utf8stringbuf_clear( &filename );

    u8_error_t db_err;
    db_err = data_database_reader_get_diagram_by_id ( (*this_).db_reader, data_id_get_row_id( &diagram_id ), &((*this_).temp_diagram) );
    if ( db_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR("error reading database.");
        result |= -1;
    }
    else
    {
        const char *diag_name;
        diag_name = data_diagram_get_name_const( &((*this_).temp_diagram) );

        /* determine filename */
        data_id_to_utf8stringbuf( &diagram_id, filename );
        utf8stringbuf_append_str( &filename, "_" );
        io_exporter_private_append_valid_chars_to_filename( this_, diag_name, filename );
        data_diagram_destroy( &((*this_).temp_diagram) );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

void io_exporter_private_append_valid_chars_to_filename( io_exporter_t *this_,
                                                         const char* name,
                                                         utf8stringbuf_t filename )
{
    U8_TRACE_BEGIN();
    assert( NULL != name );
    U8_TRACE_INFO_STR( "name:", name );

    bool finished = false;
    static const int MAX_APPEND_CHARS = 64;
    for ( int pos = 0; ( pos < MAX_APPEND_CHARS ) && ( ! finished ); pos ++ )
    {
        char probe = name[pos];
        if ( probe == '\0' )
        {
            finished = true;
        }
        else if (( 'A' <= probe ) && ( probe <= 'Z' ))
        {
            utf8stringbuf_append_char( &filename, probe );
        }
        else if (( 'a' <= probe ) && ( probe <= 'z' ))
        {
            utf8stringbuf_append_char( &filename, probe );
        }
        else if (( '0' <= probe ) && ( probe <= '9' ))
        {
            utf8stringbuf_append_char( &filename, probe );
        }
        else if ( '-' == probe )
        {
            utf8stringbuf_append_char( &filename, probe );
        }
        else if ( '_' == probe )
        {
            utf8stringbuf_append_char( &filename, probe );
        }
        else if ( ' ' == probe )
        {
            utf8stringbuf_append_char( &filename, '_' );
        }
    }

    U8_TRACE_END();
}


/*
Copyright 2016-2026 Andreas Warnke

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
