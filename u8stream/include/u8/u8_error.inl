/* File: u8_error.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

static inline bool u8_error_contains ( const u8_error_t this_,  const u8_error_t that )
{
    return ( that == (this_ & that) );
}

static inline bool u8_error_more_than ( const u8_error_t this_,  const u8_error_t that )
{
    return ( 0 != (this_ & ~(that)) );
}


/*
Copyright 2023-2023 Andreas Warnke

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
