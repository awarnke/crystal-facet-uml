/* File: test_suite.inl; Copyright and License: see below */

#include "test_environment_assert.h"

static inline void test_suite_init( test_suite_t *this_,
                                    const char *name,
                                    test_category_t default_category,
                                    test_fixture_t * (*setup) (void),
                                    void (*teardown) ( test_fixture_t *test_env ) )
{
    TEST_ENVIRONMENT_ASSERT( NULL != name );
    TEST_ENVIRONMENT_ASSERT( NULL != setup );
    TEST_ENVIRONMENT_ASSERT( NULL != teardown );
    (*this_).name = name;
    (*this_).default_category = default_category;
    (*this_).setup = setup;
    (*this_).teardown = teardown;
    (*this_).test_case_count = 0;
}

static inline void test_suite_destroy( test_suite_t *this_ )
{
    (*this_).name = NULL;
    (*this_).default_category = 0x0;
    (*this_).setup = NULL;
    (*this_).teardown = NULL;
    (*this_).test_case_count = 0;
}

static inline void test_suite_add_test_case( test_suite_t *this_,
                                             const char *name,
                                             test_case_result_t (*test_case) ( test_fixture_t *test_env ) )
{
    TEST_ENVIRONMENT_ASSERT( (*this_).test_case_count < TEST_SUITE_MAX_TEST_CASES );
    TEST_ENVIRONMENT_ASSERT( NULL != test_case );
    const int index = (*this_).test_case_count;
    test_case_init( &((*this_).test_case[index]), name, (*this_).default_category, test_case );
    (*this_).test_case_count ++;
}

static inline void test_suite_add_special_test_case( test_suite_t *this_,
                                                     const char *name,
                                                     test_category_t category,
                                                     test_case_result_t (*test_case) ( test_fixture_t *test_env ) )
{
    TEST_ENVIRONMENT_ASSERT( (*this_).test_case_count < TEST_SUITE_MAX_TEST_CASES );
    TEST_ENVIRONMENT_ASSERT( NULL != test_case );
    const int index = (*this_).test_case_count;
    test_case_init( &((*this_).test_case[index]), name, category, test_case );
    (*this_).test_case_count ++;
}

static inline unsigned int test_suite_get_test_case_count( test_suite_t *this_ )
{
    TEST_ENVIRONMENT_ASSERT( (*this_).test_case_count <= TEST_SUITE_MAX_TEST_CASES );
    return (*this_).test_case_count;
}

static inline test_case_result_t test_suite_run_test_case( test_suite_t *this_, unsigned int index )
{
    TEST_ENVIRONMENT_ASSERT( (*this_).test_case_count <= TEST_SUITE_MAX_TEST_CASES );
    TEST_ENVIRONMENT_ASSERT( index < (*this_).test_case_count );
    test_fixture_t *const test_env = (*((*this_).setup))();
    const test_case_result_t result = test_case_execute( &((*this_).test_case[index]), test_env );
    (*((*this_).teardown))( test_env );
    return result;
}

static inline const char* test_suite_get_name( test_suite_t *this_ )
{
    return (*this_).name;
}

static inline const test_case_t* test_suite_get_test_case( test_suite_t *this_, unsigned int index )
{
    TEST_ENVIRONMENT_ASSERT( (*this_).test_case_count <= TEST_SUITE_MAX_TEST_CASES );
    TEST_ENVIRONMENT_ASSERT( index < (*this_).test_case_count );
    return &((*this_).test_case[index]);
}


/*
Copyright 2019-2026 Andreas Warnke

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
