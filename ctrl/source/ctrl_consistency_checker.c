/* File: ctrl_consistency_checker.c; Copyright and License: see below */

#include "ctrl_consistency_checker.h"
#include "storage/data_database_consistency_checker.h"
#include "data_id.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

void ctrl_consistency_checker_init ( ctrl_consistency_checker_t *this_, data_database_t *database, data_database_reader_t *db_reader, data_database_writer_t *db_writer )
{
    TRACE_BEGIN();
    assert( NULL != database );
    assert( NULL != db_reader );
    assert( NULL != db_writer );

    (*this_).database = database;
    (*this_).db_reader = db_reader;
    (*this_).db_writer = db_writer;

    data_database_consistency_checker_init( &((*this_).db_checker), database );

    TRACE_END();
}

void ctrl_consistency_checker_destroy ( ctrl_consistency_checker_t *this_ )
{
    TRACE_BEGIN();

    data_database_consistency_checker_destroy( &((*this_).db_checker) );

    (*this_).database = NULL;
    (*this_).db_reader = NULL;
    (*this_).db_writer = NULL;

    TRACE_END();
}

ctrl_error_t ctrl_consistency_checker_repair_database ( ctrl_consistency_checker_t *this_,
                                                        bool modify_db,
                                                        uint32_t *out_err,
                                                        uint32_t *out_fix,
                                                        utf8stringbuf_t out_report )
{
    TRACE_BEGIN();
    ctrl_error_t err_result = CTRL_ERROR_NONE;
    data_error_t data_err;
    uint32_t error_count = 0;
    uint32_t fix_count = 0;

    /* write report title */
    if ( modify_db )
    {
        utf8stringbuf_append_str( out_report, "TITLE: Repair database\n" );
    }
    else
    {
        utf8stringbuf_append_str( out_report, "TITLE: Check database\n" );
    }

    /* get all root diagrams */
    err_result |= ctrl_consistency_checker_private_ensure_single_root_diagram( this_, modify_db, &error_count, &fix_count, out_report );

    /* find unreferenced objects */
    err_result |= ctrl_consistency_checker_private_ensure_valid_diagram_parent( this_, modify_db, &error_count, &fix_count, out_report );

    /* prepare results and return */
    if ( NULL != out_err )
    {
        (*out_err) = error_count;
    }
    if ( NULL != out_fix )
    {
        (*out_fix) = fix_count;
    }

    /* write report summary */
    utf8stringbuf_append_str( out_report, "SUMMARY: Errors found: " );
    utf8stringbuf_append_int( out_report, error_count );
    utf8stringbuf_append_str( out_report, "\n         Errors fixed: " );
    utf8stringbuf_append_int( out_report, fix_count );
    utf8stringbuf_append_str( out_report, "\n" );

    TRACE_END_ERR( err_result );
    return err_result;
}

ctrl_error_t ctrl_consistency_checker_private_ensure_single_root_diagram ( ctrl_consistency_checker_t *this_,
                                                                           bool modify_db,
                                                                           uint32_t *io_err,
                                                                           uint32_t *io_fix,
                                                                           utf8stringbuf_t out_report )
{
    TRACE_BEGIN();
    assert ( NULL != io_err );
    assert ( NULL != io_fix );
    ctrl_error_t err_result = CTRL_ERROR_NONE;
    data_error_t data_err;

    /* write report title */
    utf8stringbuf_append_str( out_report, "STEP: Ensure a single root diagram\n" );

    /* get all root diagrams */
    uint32_t out_diagram_count;
    data_err = data_database_reader_get_diagrams_by_parent_id ( (*this_).db_reader,
                                                                DATA_ID_VOID_ID,
                                                                CTRL_CONSISTENCY_CHECKER_MAX_DIAG_BUFFER,
                                                                &((*this_).temp_diagram_buffer),
                                                                & out_diagram_count
                                                              );

    if ( DATA_ERROR_NONE == data_err )
    {
        utf8stringbuf_append_str( out_report, "    ROOT DIAGRAM COUNT: " );
        utf8stringbuf_append_int( out_report, out_diagram_count );
        utf8stringbuf_append_str( out_report, "\n" );
        for ( int list_pos = 0; list_pos < out_diagram_count; list_pos ++ )
        {
            utf8stringbuf_append_str( out_report, "    ROOT DIAGRAM: " );
            utf8stringbuf_append_int( out_report, data_diagram_get_id( &((*this_).temp_diagram_buffer[list_pos]) ) );
            utf8stringbuf_append_str( out_report, ": " );
            utf8stringbuf_append_str( out_report, data_diagram_get_name_ptr( &((*this_).temp_diagram_buffer[list_pos]) ) );
            utf8stringbuf_append_str( out_report, "\n" );
        }
        if ( out_diagram_count == 0 )
        {
            (*io_err) ++;
            utf8stringbuf_append_str( out_report, "    PROPOSED FIX: Create a diagram via the GUI.\n" );
        }
        else if ( out_diagram_count > 1 )
        {
            (*io_err) += (out_diagram_count-1) ;
            int64_t proposed_root_diagram_id = data_diagram_get_id( &((*this_).temp_diagram_buffer[0]) );
            if ( ! modify_db )
            {
                utf8stringbuf_append_str( out_report, "    PROPOSED FIX: Attach additional root diagrams below the first: " );
                utf8stringbuf_append_int( out_report, proposed_root_diagram_id );
                utf8stringbuf_append_str( out_report, "\n" );
            }
            else
            {
                for ( int list_pos = 1; list_pos < out_diagram_count; list_pos ++ )
                {
                    data_err = data_database_writer_update_diagram_parent_id ( (*this_).db_writer,
                                                                               data_diagram_get_id( &((*this_).temp_diagram_buffer[list_pos]) ),
                                                                               proposed_root_diagram_id,
                                                                               NULL
                                                                             );
                    if ( DATA_ERROR_NONE == data_err )
                    {
                        utf8stringbuf_append_str( out_report, "    FIX: Diagram " );
                        utf8stringbuf_append_int( out_report, data_diagram_get_id( &((*this_).temp_diagram_buffer[list_pos]) ) );
                        utf8stringbuf_append_str( out_report, " attached to " );
                        utf8stringbuf_append_int( out_report, proposed_root_diagram_id);
                        utf8stringbuf_append_str( out_report, "\n" );
                        (*io_fix) ++;
                    }
                    else
                    {
                        utf8stringbuf_append_str( out_report, "ERROR WRITING DATABASE.\n" );
                        err_result |= (ctrl_error_t) data_err;
                    }
                }
            }
        }
    }
    else
    {
        utf8stringbuf_append_str( out_report, "ERROR READING DATABASE.\n" );
        err_result |= (ctrl_error_t) data_err;
    }

    TRACE_END_ERR( err_result );
    return err_result;
}

ctrl_error_t ctrl_consistency_checker_private_ensure_valid_diagram_parent ( ctrl_consistency_checker_t *this_,
                                                                            bool modify_db,
                                                                            uint32_t *io_err,
                                                                            uint32_t *io_fix,
                                                                            utf8stringbuf_t out_report )
{
    TRACE_BEGIN();
    assert ( NULL != io_err );
    assert ( NULL != io_fix );
    ctrl_error_t err_result = CTRL_ERROR_NONE;
    data_error_t data_err;

    /* write report title */
    utf8stringbuf_append_str( out_report, "STEP: Ensure that diagrams have valid parents\n" );

    data_small_set_t unref;
    data_small_set_init( &unref );
    data_err = data_database_consistency_checker_find_unreferenced_diagrams ( &((*this_).db_checker), &unref );
    if ( DATA_ERROR_NONE == data_err )
    {
        uint32_t unref_count = data_small_set_get_count( &unref );

        utf8stringbuf_append_str( out_report, "    UNREFERENCED DIAGRAM COUNT: " );
        utf8stringbuf_append_int( out_report, unref_count );
        utf8stringbuf_append_str( out_report, "\n" );

        if ( unref_count != 0 )
        {
            (*io_err) += unref_count;

            if ( ! modify_db )
            {
                utf8stringbuf_append_str( out_report, "    PROPOSED FIX: Delete " );
                utf8stringbuf_append_int( out_report, unref_count );
                utf8stringbuf_append_str( out_report, " diagrams.\n" );
            }
            else
            {
                for ( int list_pos = 0; list_pos < unref_count; list_pos ++ )
                {
                    data_id_t diagram_id = data_small_set_get_id( &unref, list_pos );
                    int64_t diagram_row_id = data_id_get_row_id( &diagram_id );
                    data_err = data_database_writer_delete_diagram ( (*this_).db_writer, diagram_row_id, NULL );
                    if ( DATA_ERROR_NONE == data_err )
                    {
                        utf8stringbuf_append_str( out_report, "    FIX: Diagram " );
                        utf8stringbuf_append_int( out_report, diagram_row_id );
                        utf8stringbuf_append_str( out_report, " deleted.\n" );
                        (*io_fix) ++;
                    }
                    else
                    {
                        utf8stringbuf_append_str( out_report, "ERROR WRITING DATABASE.\n" );
                        err_result |= (ctrl_error_t) data_err;
                    }
                }
            }
        }
    }
    else
    {
        utf8stringbuf_append_str( out_report, "ERROR READING DATABASE.\n" );
        err_result |= (ctrl_error_t) data_err;
    }

    TRACE_END_ERR( err_result );
    return err_result;
}


/*
Copyright 2016-2016 Andreas Warnke

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
