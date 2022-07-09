/* File: gui_file_db_manager.c; Copyright and License: see below */

#include "gui_file_db_manager.h"
#include "u8stream/universal_file_input_stream.h"
#include "trace/trace.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

void gui_file_db_manager_init( gui_file_db_manager_t *this_,
                               ctrl_controller_t *controller,
                               io_data_file_t *data_file,
                               gui_simple_message_to_user_t *message_to_user )
{
    TRACE_BEGIN();
    assert( NULL != controller );
    assert( NULL != data_file );
    assert( NULL != message_to_user );

    (*this_).controller = controller;
    (*this_).data_file = data_file;
    (*this_).message_to_user = message_to_user;

    TRACE_END();
}

void gui_file_db_manager_destroy( gui_file_db_manager_t *this_ )
{
    TRACE_BEGIN();

    (*this_).controller = NULL;
    (*this_).data_file = NULL;
    (*this_).message_to_user = NULL;

    TRACE_END();
}

u8_error_t gui_file_db_manager_use_db( gui_file_db_manager_t *this_, const char *filename )
{
    TRACE_BEGIN();

    bool is_json;
    gui_file_db_manager_private_guess_db_type( this_, filename, &is_json );

    if ( io_data_file_is_open( (*this_).data_file ) )
    {
        const u8_error_t close_err = io_data_file_close( (*this_).data_file );
        if ( close_err != U8_ERROR_NONE )
        {
            TSLOG_ERROR( "Closing the database was not possible" );
            TRACE_INFO_STR( "Closing the database was not possible:", io_data_file_get_filename_ptr( (*this_).data_file ) );
        }
    }
    const u8_error_t error = io_data_file_open ( (*this_).data_file, filename );

    if ( U8_ERROR_NONE == error )
    {
        /* ensure that at least one diagram exists - otherwise the first window looks a bit empty */
        ctrl_diagram_controller_t *diag_control;
        diag_control = ctrl_controller_get_diagram_control_ptr( (*this_).controller );
        ctrl_diagram_controller_create_root_diagram_if_not_exists( diag_control,
                                                                   DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM,
                                                                   "New Overview",
                                                                   NULL
                                                                 );
    }
    else
    {
        if ( io_data_file_is_open( (*this_).data_file ) )
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

    TRACE_END_ERR( error );
    return error;
}

u8_error_t gui_file_db_manager_private_guess_db_type( gui_file_db_manager_t *this_, const char *filename, bool *out_json )
{
    TRACE_BEGIN();
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
            TRACE_INFO_STR("File exists and starts with sqlite3 magic:", filename);
            *out_json = false;
        }
        else
        {
            TRACE_INFO_STR("File exists and is not of type sqlite3:", filename);
            *out_json = true;
        }
    }
    else
    {
        if( 1 == utf8string_ends_with_str( filename, ".json" ) )
        {
            TRACE_INFO_STR("File does not exist and is of type json:", filename);
            *out_json = true;
        }
        else
        {
            TRACE_INFO_STR("File does not exist and type defaults to sqlite3:", filename);
            *out_json = false;
        }
    }

    /* close file */
    scan_head_error |= universal_file_input_stream_close( &in_file );
    scan_head_error |= universal_file_input_stream_destroy( &in_file );

    TRACE_END_ERR( scan_head_error );
    return scan_head_error;
}


/*
Copyright 2016-2022 Andreas Warnke

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
