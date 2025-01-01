/* File: test_fixture.h; Copyright and License: see below */

#ifndef TEST_FIXTURE_H
#define TEST_FIXTURE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief provides a type def for a test fixture
 */

/*!
 *  \brief a forward declaration of a test fixture structure.
 *
 *  Because the struct is specific to the test suite, the type can be declared here only as incomplete type.
 */
struct test_fixture_struct;

/*!
 *  \brief a type declaration of a test fixture.
 *
 *  A test fixture provides the context for the function under test.
 *  This is on one hand the test environment like database, file system, controller;
 *  on the other hand it also hosts the own data structures needed by the function under test.
 */
typedef struct test_fixture_struct test_fixture_t;

#endif  /* TEST_FIXTURE_H */


/*
Copyright 2023-2025 Andreas Warnke

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
