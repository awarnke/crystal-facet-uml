/* File: test_case.h; Copyright and License: see below */

#ifndef TEST_CASE_H
#define TEST_CASE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides a test case definition.
 */

#include "test_case_result.h"
#include "test_category.h"
#include "test_fixture.h"
#include <stdbool.h>

/*!
 *  \brief attributes of a test case
 */
struct test_case_struct {
    const char *name;  /*!< name of the test case */
    test_category_t category;  /*!< category of the test case: goal, execution cycle, further attributes */
    test_case_result_t (*execute) (test_fixture_t *test_env);  /*!< pointer to test case execution function */
};

typedef struct test_case_struct test_case_t;

/*!
 *  \brief initializes the test_case_t
 *
 *  \param this_ pointer to own object attributes
 *  \param name name of the test case
 *  \param category category of the test case: goal, execution cycle, further attributes
 *  \param execute pointer to test case execution function
 */
static inline void test_case_init( test_case_t *this_,
                                   const char *name,
                                   test_category_t category,
                                   test_case_result_t (*execute) (test_fixture_t *test_env)
                                 );

/*!
 *  \brief destroys the test_case_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void test_case_destroy( test_case_t *this_ );

/*!
 *  \brief returns the name of the test case
 *
 *  \param this_ pointer to own object attributes
 */
static inline const char * test_case_get_name( const test_case_t *this_ );

/*!
 *  \brief returns the category of the test case
 *
 *  \param this_ pointer to own object attributes
 */
static inline test_category_t test_case_get_catgegory( const test_case_t *this_ );

/*!
 *  \brief returns the number of failed test cases
 *
 *  \param this_ pointer to own object attributes
 *  \param test_env pointer to the context(fixture) for the test execution
 */
static inline test_case_result_t test_case_execute( test_case_t *this_, test_fixture_t *test_env );

#include "test_case.inl"

#endif  /* TEST_CASE_H */


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
