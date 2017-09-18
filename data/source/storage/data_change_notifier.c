/* File: data_change_notifier.c; Copyright and License: see below */

#include "data_id.h"
#include "storage/data_change_notifier.h"
#include "trace.h"
#include "tslog.h"
#include <glib-object.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

static bool data_change_notifier_glib_signal_initialized = false;
static guint data_change_notifier_glib_signal_id = 0;
const char *DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME = "cfu_data_changed";

void data_change_notifier_init ( data_change_notifier_t *this_ )
{
    TRACE_BEGIN();

    (*this_).num_listeners = 0;
    memset( (*this_).listener_array, '\0', sizeof( (*this_).listener_array ) );

    /* define a new signal */
    if ( ! data_change_notifier_glib_signal_initialized )
    {
        data_change_notifier_glib_signal_id = g_signal_new (
            DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME,
            G_TYPE_OBJECT,
            G_SIGNAL_RUN_FIRST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__POINTER,
            G_TYPE_NONE,
            1,
            G_TYPE_POINTER /* data_id_t*: id of the record that was created, updated or deleted */
        );
        data_change_notifier_glib_signal_initialized = true;
        TRACE_INFO_INT( "g_signal_new(\"cfu_data_changed\") returned new signal id", data_change_notifier_glib_signal_id );
    }

    TRACE_END();
}

void data_change_notifier_destroy ( data_change_notifier_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void data_change_notifier_emit_signal ( data_change_notifier_t *this_, data_table_t table, int64_t row_id )
{
    TRACE_BEGIN();

    data_id_t modified_id;
    data_id_init( &modified_id, table, row_id );

    for ( int32_t pos = 0; pos < (*this_).num_listeners; pos ++ )
    {
        TRACE_INFO_INT( "g_signal_emit to listener", pos );
        g_signal_emit( (*this_).listener_array[pos], data_change_notifier_glib_signal_id, 0, &modified_id );
    }

    TRACE_END();
}

data_error_t data_change_notifier_add_listener ( data_change_notifier_t *this_, GObject *new_listener )
{
    TRACE_BEGIN();
    assert( NULL != new_listener );
    data_error_t result = DATA_ERROR_NONE;

    /* check for duplicates */
    bool duplicate = false;
    for ( int32_t pos = 0; pos < (*this_).num_listeners; pos ++ )
    {
        if( new_listener == (*this_).listener_array[pos] )
        {
            duplicate = true;
            result = DATA_ERROR_INVALID_REQUEST;
            TSLOG_ERROR( "duplicate call to data_change_notifier_add_listener for same listener." );
        }
    }

    /* if not a duplicate */
    if ( ! duplicate ) {
        if ( (*this_).num_listeners + 1 <= DATA_CHANGE_NOTIFIER_MAX_LISTENERS )
        {
            (*this_).listener_array[(*this_).num_listeners] = new_listener;  /* possibly increases reference count */
            (*this_).num_listeners ++;
        }
        else
        {
            result = DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
            TSLOG_ERROR( "data_change_notifier_add_listener has too many listeners." );
        }
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_change_notifier_remove_listener ( data_change_notifier_t *this_, GObject *no_listener )
{
    TRACE_BEGIN();
    assert( NULL != no_listener );
    data_error_t result = DATA_ERROR_NONE;

    /* search listener to remove */
    int32_t found_at_pos = -1;  /* -1 means: not found */
    for ( int32_t pos = 0; pos < (*this_).num_listeners; pos ++ )
    {
        if( no_listener == (*this_).listener_array[pos] )
        {
            found_at_pos = pos;
        }
    }

    /* remove if found */
    if ( found_at_pos != -1 ) {
        (*this_).listener_array[found_at_pos] = NULL;
        if ( found_at_pos + 1 != (*this_).num_listeners )
        {
            (*this_).listener_array[found_at_pos] = (*this_).listener_array[(*this_).num_listeners-1];
            (*this_).listener_array[(*this_).num_listeners-1] = NULL;
        }
        (*this_).num_listeners --;
    }
    else
    {
        result = DATA_ERROR_INVALID_REQUEST;
        TSLOG_ERROR( "data_change_notifier_remove_listener did not find listener to remove." );
    }

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2016-2017 Andreas Warnke

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
