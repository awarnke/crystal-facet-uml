/* File: test_result.inl; Copyright and License: see below */

#include <assert.h>

static inline void test_result_init( test_result_t *this_ )
{
    (*this_).total = 0;
    (*this_).failed = 0;
}

static inline void test_result_destroy( test_result_t *this_ )
{
}

static inline void test_result_add_test_case_result( test_result_t *this_, bool success )
{
    (*this_).total ++;
    if ( ! success ) 
    {
        (*this_).failed ++;
    }
}

static inline unsigned int test_result_get_total( test_result_t *this_ )
{
    return (*this_).total;
}

static inline unsigned int test_result_get_failed( test_result_t *this_ )
{
    return (*this_).failed;
}


/*
Copyright 2019-2021 Andreas Warnke

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
