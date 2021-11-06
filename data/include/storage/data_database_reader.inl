/* File: data_database_reader.inl; Copyright and License: see below */

#include "tslog.h"
#include "trace.h"
#include <assert.h>

static inline bool data_database_reader_is_open( data_database_reader_t *this_ )
{
    bool result;
    result = (*this_).is_open;
    return result;
}


/*
Copyright 2016-2021 Andreas Warnke

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
