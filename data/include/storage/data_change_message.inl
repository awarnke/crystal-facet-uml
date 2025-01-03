/* File: data_change_message.inl; Copyright and License: see below */

#include "u8/u8_log.h"

static inline void data_change_message_init ( data_change_message_t *this_,
                                              data_change_event_type_t event,
                                              data_id_t modified,
                                              data_id_t parent )
{
    (*this_).event = event;
    (*this_).modified = modified;
    (*this_).parent = parent;
}

static inline void data_change_message_reinit ( data_change_message_t *this_,
                                                data_change_event_type_t event,
                                                data_id_t modified,
                                                data_id_t parent )
{
    (*this_).event = event;
    (*this_).modified = modified;
    (*this_).parent = parent;
}

static inline void data_change_message_destroy ( data_change_message_t *this_ )
{
}

static inline data_change_event_type_t data_change_message_get_event ( const data_change_message_t *this_ )
{
    return (*this_).event;
}

static inline data_id_t data_change_message_get_modified ( const data_change_message_t *this_ )
{
    return (*this_).modified;
}

static inline data_id_t data_change_message_get_parent ( const data_change_message_t *this_ )
{
    return (*this_).parent;
}

static inline void data_change_message_trace ( const data_change_message_t *this_ )
{
    U8_TRACE_INFO( "data_change_message_t" );
    switch ( (*this_).event )
    {
        case DATA_CHANGE_EVENT_TYPE_CREATE:
        {
            U8_TRACE_INFO("- event: DATA_CHANGE_EVENT_TYPE_CREATE");
        }
        break;

        case DATA_CHANGE_EVENT_TYPE_UPDATE:
        {
            U8_TRACE_INFO("- event: DATA_CHANGE_EVENT_TYPE_UPDATE");
        }
        break;

        case DATA_CHANGE_EVENT_TYPE_DELETE:
        {
            U8_TRACE_INFO("- event: DATA_CHANGE_EVENT_TYPE_DELETE");
        }
        break;

        case DATA_CHANGE_EVENT_TYPE_MULTI:
        {
            U8_TRACE_INFO("- event: DATA_CHANGE_EVENT_TYPE_MULTI");
        }
        break;

        case DATA_CHANGE_EVENT_TYPE_DB_OPENED:
        {
            U8_TRACE_INFO("- event: DATA_CHANGE_EVENT_TYPE_DB_OPENED");
        }
        break;

        case DATA_CHANGE_EVENT_TYPE_DB_PREPARE_CLOSE:
        {
            U8_TRACE_INFO("- event: DATA_CHANGE_EVENT_TYPE_DB_PREPARE_CLOSE");
        }
        break;

        case DATA_CHANGE_EVENT_TYPE_DB_CLOSED:
        {
            U8_TRACE_INFO("- event: DATA_CHANGE_EVENT_TYPE_DB_CLOSED");
        }
        break;

        default:
        {
            U8_LOG_ERROR("- event: out of range");
        }
        break;
    }
    U8_TRACE_INFO( "- modified:" );
    data_id_trace( &((*this_).modified) );
    U8_TRACE_INFO( "- parent:" );
    data_id_trace( &((*this_).parent) );
}


/*
Copyright 2018-2025 Andreas Warnke

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
