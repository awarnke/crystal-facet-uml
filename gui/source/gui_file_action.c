/* File: gui_file_action.c; Copyright and License: see below */

#include "gui_file_action.h"
#include "u8/u8_error_info.h"
#include "u8/u8_trace.h"
#ifndef NDEBUG
#include "u8stream/universal_stream_output_stream.h"
#include "utf8stream/utf8stream_writer.h"
#endif  /* not NDEBUG */
#include "gui_gtk.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

void gui_file_action_init( gui_file_action_t *this_,
                           ctrl_controller_t *controller,
                           io_data_file_t *data_file,
                           gui_simple_message_to_user_t *message_to_user )
{
    U8_TRACE_BEGIN();
    assert( NULL != controller );
    assert( NULL != data_file );
    assert( NULL != message_to_user );

    (*this_).controller = controller;
    (*this_).data_file = data_file;
    (*this_).message_to_user = message_to_user;

    U8_TRACE_END();
}

void gui_file_action_destroy( gui_file_action_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).controller = NULL;
    (*this_).data_file = NULL;
    (*this_).message_to_user = NULL;

    U8_TRACE_END();
}

u8_error_t gui_file_action_use_db( gui_file_action_t *this_, const char *filename )
{
    U8_TRACE_BEGIN();

    if ( io_data_file_is_open( (*this_).data_file ) )
    {
        const u8_error_t close_err = io_data_file_close( (*this_).data_file );
        if ( close_err != U8_ERROR_NONE )
        {
            U8_LOG_ERROR( "Closing the database was not possible" );
            U8_TRACE_INFO_STR( "Closing the database was not possible:", io_data_file_get_filename_const( (*this_).data_file ) );
        }
    }

    /* react immediately */
    gui_simple_message_to_user_show_message_with_name( (*this_).message_to_user,
                                                       GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                       GUI_SIMPLE_MESSAGE_CONTENT_LOADING_WAIT,
                                                       filename
                                                     );
    bool events_handled = true;
    for ( uint_fast8_t max_loop = 40; events_handled && ( max_loop > 0 ); max_loop-- )
    {
        events_handled = g_main_context_iteration( NULL, /*may_block*/ FALSE );
    }

    u8_error_info_t err_info;
    const u8_error_t error = io_data_file_open_writeable ( (*this_).data_file, filename, &err_info );

    if ( U8_ERROR_NONE == error )
    {
        /* ensure that at least one diagram exists - otherwise the first window looks a bit empty */
        ctrl_diagram_controller_t *diag_control;
        diag_control = ctrl_controller_get_diagram_control_ptr( (*this_).controller );
        ctrl_diagram_controller_create_root_diagram_if_not_exists( diag_control,
                                                                   DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM,
                                                                   "New Diagram",
                                                                   NULL
                                                                 );

        /* remove the loading message if not overwritten: */
        if ( GUI_SIMPLE_MESSAGE_TYPE_INFO == gui_simple_message_to_user_get_type_id( (*this_).message_to_user ) )
        {
            gui_simple_message_to_user_hide( (*this_).message_to_user );
        }
    }
    else if ( U8_ERROR_NO_DB == error )
    {
        /* Most likely the parent directory of database is read only */
        gui_simple_message_to_user_show_message_with_name( (*this_).message_to_user,
                                                           GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                           GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_NOT_CREATEABLE,
                                                           filename
                                                         );
    }
    else
    {
        if ( u8_error_info_is_error( &err_info ) )
        {
            gui_simple_message_to_user_show_error_info( (*this_).message_to_user, &err_info );
        }
        else if ( io_data_file_is_open( (*this_).data_file ) )
        {
            gui_simple_message_to_user_show_message_with_name( (*this_).message_to_user,
                                                               GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                               GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_OPENED_WITH_ERROR,
                                                               filename
                                                             );
        }
        else
        {
            gui_simple_message_to_user_show_message_with_name( (*this_).message_to_user,
                                                               GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                               GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_NOT_OPENED,
                                                               filename
                                                             );
        }
    }

    U8_TRACE_END_ERR( error );
    return error;
}

u8_error_t gui_file_action_save( gui_file_action_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t err = U8_ERROR_NONE;

    u8_error_t d_err;
    d_err = U8_ERROR_NONE;
    d_err |= io_data_file_trace_stats( (*this_).data_file );
    d_err |= io_data_file_sync_to_disk( (*this_).data_file );
    d_err |= io_data_file_trace_stats( (*this_).data_file );
    if ( U8_ERROR_NONE != d_err )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_WRITE_ERROR
                                               );
    }
    else
    {
        gui_simple_message_to_user_show_message_with_name( (*this_).message_to_user,
                                                           GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                           GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_WRITTEN,
                                                           io_data_file_get_filename_const( (*this_).data_file )
                                                         );
#ifndef NDEBUG
        /* in debug mode, also check consistency of database */
        universal_stream_output_stream_t out_stream;
        universal_stream_output_stream_init( &out_stream, stdout );
        universal_output_stream_t *const out_base = universal_stream_output_stream_get_output_stream( &out_stream );
        utf8stream_writer_t out_report;
        utf8stream_writer_init( &out_report, out_base );
        uint32_t found_errors;
        uint32_t fixed_errors;
        ctrl_controller_repair_database( (*this_).controller,
                                         false /* no repair, just test */,
                                         &found_errors,
                                         &fixed_errors,
                                         &out_report
                                       );
        if (( found_errors != 0 ) || ( fixed_errors != 0 ))
        {
            gui_simple_message_to_user_show_message_with_quantity( (*this_).message_to_user,
                                                                   GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                   GUI_SIMPLE_MESSAGE_CONTENT_DB_INCONSISTENT,
                                                                   found_errors
                                                                 );
        }
        utf8stream_writer_destroy( &out_report );
        universal_stream_output_stream_destroy( &out_stream );
#endif
    }

    U8_TRACE_END_ERR( err );
    return err;
}


/*
Copyright 2016-2025 Andreas Warnke

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
