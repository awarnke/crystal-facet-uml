/* File: data_database_listener.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include <assert.h>

static inline void data_database_listener_init ( data_database_listener_t *this_,
                                                 void *listener_instance,
                                                 void (*listener_callback)(void* listener_instance, data_database_listener_signal_t signal_id) )
{
    (*this_).listener_instance = listener_instance;
    (*this_).listener_callback = listener_callback;
}

static inline void data_database_listener_destroy ( data_database_listener_t *this_ )
{
    (*this_).listener_instance = NULL;
    (*this_).listener_callback = NULL;
}

static inline void data_database_listener_notify ( data_database_listener_t *this_, data_database_listener_signal_t signal_id )
{
    assert( (*this_).listener_instance != NULL );
    assert( (*this_).listener_callback != NULL );
    U8_TRACE_INFO("data_database_listener_notify()");
    ((*this_).listener_callback)( (*this_).listener_instance, signal_id );
}


/*
Copyright 2016-2023 Andreas Warnke

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
