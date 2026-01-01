/* File: u8/u8_test_cond.h; Copyright and License: see below */

#ifndef U8_TEST_COND_H
#define U8_TEST_COND_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief  Declares condition ids for fault injection tests in this u8stream module
 */

/*!
 *  \brief condition ids used to activate and deactivate fault injections during tests
 *
 *  Note that the 7538 prefix is the hexadecimal representation of the "u8" package name,
 */
enum u8_test_cond_enum {
    U8_TEST_COND_STRTOD = 0x75380001,  /*!< used in utf8string.c at strtod */
    U8_TEST_COND_STRTOLL = 0x75380002,  /*!< used in utf8string.c at strtoll */
    U8_TEST_COND_FSEEK = 0x75380003,  /*!< used in universal_file_input_stream.c at fseek */
    U8_TEST_COND_FCLOSE = 0x75380004,  /*!< used in universal_file_input/output_stream.c at fclose */
    U8_TEST_COND_FFLUSH = 0x75380005,  /*!< used in universal_file/stream_output_stream.c at fflush */
    U8_TEST_COND_FWRITE = 0x75380006,  /*!< used in universal_file/stream_output_stream.c at fwrite */
};

#endif  /* U8_TEST_COND_H */


/*
Copyright 2023-2026 Andreas Warnke

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
