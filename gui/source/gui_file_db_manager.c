/* File: gui_file_db_manager.c; Copyright and License: see below */

#include "gui_file_db_manager.h"
#include "trace/trace.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

void gui_file_db_manager_init ( gui_file_db_manager_t *this_,
                                ctrl_controller_t *controller,
                                data_database_t *database,
                                gui_simple_message_to_user_t *message_to_user )
{
    TRACE_BEGIN();
    assert( NULL != controller );
    assert( NULL != database );
    assert( NULL != message_to_user );

    (*this_).controller = controller;
    (*this_).database = database;
    (*this_).message_to_user = message_to_user;

    TRACE_END();
}

void gui_file_db_manager_destroy( gui_file_db_manager_t *this_ )
{
    TRACE_BEGIN();

    (*this_).controller = NULL;
    (*this_).database = NULL;
    (*this_).message_to_user = NULL;

    TRACE_END();
}

u8_error_t gui_file_db_manager_use_db( gui_file_db_manager_t *this_, const char *filename )
{
    TRACE_BEGIN();

    const u8_error_t error = ctrl_controller_switch_database ( (*this_).controller, filename );

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
        if ( data_database_is_open( (*this_).database ) )
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
