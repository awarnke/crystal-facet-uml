/* File: test_case.inl; Copyright and License: see below */

#include <stddef.h>
#include <assert.h>

static inline void test_case_init( test_case_t *this_,
                                   const char *name,
                                   test_category_t category,
                                   test_case_result_t (*execute) (test_fixture_t *test_env) )
{
    assert( name != NULL );
    (*this_).name = name;
    (*this_).category = category;
    (*this_).execute = execute;
}

static inline void test_case_destroy( test_case_t *this_ )
{
}

static inline const char * test_case_get_name( test_case_t *this_ )
{
    return (*this_).name;
}

static inline test_category_t test_case_get_catgegory( test_case_t *this_ )
{
    return (*this_).category;
}

static inline test_case_result_t test_case_execute( test_case_t *this_, test_fixture_t *test_env )
{
    return (*((*this_).execute))(test_env);
}


/*
Copyright 2024-2024 Andreas Warnke

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
