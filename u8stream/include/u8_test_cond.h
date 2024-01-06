/* File: u8/u8_test_cond.h; Copyright and License: see below */

#ifndef U8_TEST_COND_H
#define U8_TEST_COND_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief  Declares condition ids for fault injection tests in this u8stream module
 */

/*!
 *  \brief condition ids used to activate and deactivate fault injections during unit tests
 *
 *  Note that the 7538 prefix is the hexadecimal representation of the "u8" package name,
 */
enum u8_test_cond_enum {
    U8_TEST_COND_STRTOD = 0x75380001,  /*!< used in utf8string.inl at strtod */
    U8_TEST_COND_STRTOLL = 0x75380002,  /*!< used in utf8string.inl at strtoll */
};

#endif  /* U8_TEST_COND_H */


/*
Copyright 2023-2024 Andreas Warnke

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
