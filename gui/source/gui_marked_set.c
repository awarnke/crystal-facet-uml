/* File: gui_marked_set.c; Copyright and License: see below */

#include "gui_marked_set.h"
#include "data_table.h"
#include "data_id.h"
#include "trace/trace.h"
#include "tslog/tslog.h"
#include <gdk/gdk.h>

static bool gui_marked_set_glib_signal_initialized = false;
static guint gui_marked_set_glib_signal_id = 0;
const char *GUI_MARKED_SET_GLIB_SIGNAL_NAME = "cfu_object_selected";

void gui_marked_set_init( gui_marked_set_t *this_, GObject *signal_source )
{
    TRACE_BEGIN();
    assert( NULL != signal_source );

    data_full_id_init_void( &((*this_).focused) );
    data_id_init_void( &((*this_).focused_diagram) );
    data_id_init_void( &((*this_).highlighted) );
    data_id_init_void( &((*this_).highlighted_diagram) );
    (*this_).highlighted_button = GUI_SKETCH_ACTION_NONE;
    data_small_set_init( &((*this_).selected_set) );

    /* define a new signal */
    if ( ! gui_marked_set_glib_signal_initialized )
    {
        gui_marked_set_glib_signal_id = g_signal_new (
            GUI_MARKED_SET_GLIB_SIGNAL_NAME,
            G_TYPE_OBJECT,
            G_SIGNAL_RUN_FIRST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__POINTER,
            G_TYPE_NONE,
            1,
            G_TYPE_POINTER /* data_id_t* */
        );
        gui_marked_set_glib_signal_initialized = true;
        TRACE_INFO_INT( "g_signal_new(\"cfu_object_selected\") returned new signal id", gui_marked_set_glib_signal_id );
    }

    (*this_).signal_source = signal_source;

    TRACE_END();
}

void gui_marked_set_reinit( gui_marked_set_t *this_ )
{
    TRACE_BEGIN();
    assert( NULL != (*this_).signal_source );

    data_full_id_init_void( &((*this_).focused) );
    data_id_init_void( &((*this_).focused_diagram) );
    data_id_init_void( &((*this_).highlighted) );
    data_id_init_void( &((*this_).highlighted_diagram) );
    (*this_).highlighted_button = GUI_SKETCH_ACTION_NONE;
    data_small_set_reinit( &((*this_).selected_set) );

    TRACE_END();
}

void gui_marked_set_destroy( gui_marked_set_t *this_ )
{
    TRACE_BEGIN();

    data_full_id_destroy( &((*this_).focused) );
    data_id_destroy( &((*this_).focused_diagram) );
    data_id_destroy( &((*this_).highlighted) );
    data_id_destroy( &((*this_).highlighted_diagram) );
    data_small_set_destroy( &((*this_).selected_set) );
    (*this_).signal_source = NULL;

    TRACE_END();
}

void gui_marked_set_private_notify_listeners( gui_marked_set_t *this_, data_id_t modified_real_object_id )
{
    TRACE_BEGIN();

    TRACE_INFO( "g_signal_emit to listeners" );
    g_signal_emit( (*this_).signal_source, gui_marked_set_glib_signal_id, 0, &modified_real_object_id );

    TRACE_END();
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

