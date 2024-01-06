/* File: observer.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

static inline void observer_init( observer_t *this_,
                                  void *observer_instance,
                                  void (*observer_callback)(void *observer_instance, void *call_param),
                                  const char* callback_name )
{
    assert( NULL != observer_instance );
    assert( NULL != observer_callback );
    assert( NULL != callback_name );
    (*this_).observer_instance = observer_instance;
    (*this_).observer_callback = observer_callback;
    (*this_).callback_name = callback_name;
}

static inline void observer_destroy( observer_t *this_ )
{
    (*this_).observer_instance = NULL;
    (*this_).observer_callback = NULL;
}

static inline void observer_notify( observer_t *this_, void *call_param )
{
    assert( (*this_).observer_instance != NULL );
    assert( (*this_).observer_callback != NULL );
    U8_LOG_EVENT_STR("observer_notify() -->", (*this_).callback_name);
    ((*this_).observer_callback)( (*this_).observer_instance, call_param );
}


/*
Copyright 2016-2024 Andreas Warnke

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
