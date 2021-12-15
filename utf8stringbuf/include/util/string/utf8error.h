/* File: utf8error.h; Copyright and License: see below */

#ifndef UTF8ERROR_H_
#define UTF8ERROR_H_

/*!
 *  \file utf8error.h
 *  \brief utf8error is an enumeration of error constants.
 *
 *  \note License: Use this code according to the license: Apache 2.0.
 *  \author Copyright 2012-2021 A.Warnke; Email-contact: utf8stringbuf-at-andreaswarnke-dot-de
 */

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \enum utf8error_enum
 *
 *  Enumeration for success and error states.
 *
 *  It is possible to bitwise-or multiple errors to collect errors over several statements,
 *  e.g. { strerr |= utf8stringbuf_copy_str(...);  strerr |= utf8stringbuf_append_str(...); },
 *  and evaluate multiple errors only once at the end.
 *
 *  The bitmasks are compatible with u8_error_t
 */
enum utf8error_enum {
    UTF8ERROR_SUCCESS         = 0x00000000, /*!< success, there was no error */
    UTF8ERROR_NOT_FOUND       = -2147483648, /*!< pattern not found */
    UTF8ERROR_NULL_PARAM      = 0x00401000, /*!< NULL was provided as parameter instead of a valid pointer */
    UTF8ERROR_OUT_OF_RANGE    = 0x00402000, /*!< some integer parameter was out of range */
    UTF8ERROR_TRUNCATED       = 0x00404000, /*!< the resulting string did not fit into the buffer, the string was truncated */
    UTF8ERROR_NOT_A_CODEPOINT = 0x00408000, /*!< a codepoint was out of range: only 0x00000000 to 0x0010ffff are valid in utf8 (since 2003) */
};

/*!
 *  \typedef utf8error_t
 *  \brief Enumeration of error constants
 */
typedef enum utf8error_enum utf8error_t;

#ifdef __cplusplus
}
#endif

#endif /*UTF8ERROR_H_*/


/*
 * Copyright 2012-2021 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
