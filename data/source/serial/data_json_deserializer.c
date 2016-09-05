/* File: data_json_deserializer.c; Copyright and License: see below */

#include "serial/data_json_deserializer.h"
#include "trace.h"
#include "log.h"
#include <assert.h>

void data_json_deserializer_init ( data_json_deserializer_t *this_ )
{
    TRACE_BEGIN();

    data_json_tokenizer_init( &((*this_).tokenizer) );

    (*this_).container_stack_size = 0;
    (*this_).root_object_count = 0;
    (*this_).last_token = DATA_JSON_DESERIALIZER_STATE_START;

    TRACE_END();
}

void data_json_deserializer_destroy ( data_json_deserializer_t *this_ )
{
    TRACE_BEGIN();

    data_json_tokenizer_destroy( &((*this_).tokenizer) );

    TRACE_END();
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
