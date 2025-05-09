/* File: test_case_result.h; Copyright and License: see below */

#ifndef TEST_CASE_RESULT_H
#define TEST_CASE_RESULT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief provides an enumeration for test case results: ok and error
 */

/*!
 *  \brief enumeration of test case results
 */
enum test_case_result_enum {
    TEST_CASE_RESULT_OK,  /*!< indicates success */
    TEST_CASE_RESULT_ERR,  /*!< indicates an error */
};

typedef enum test_case_result_enum test_case_result_t;

#endif  /* TEST_CASE_RESULT_H */


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
