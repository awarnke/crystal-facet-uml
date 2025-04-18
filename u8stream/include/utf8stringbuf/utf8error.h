/* File: utf8error.h; Copyright and License: see below */

#ifndef UTF8ERROR_H_
#define UTF8ERROR_H_

/*!
 *  \file utf8error.h
 *  \brief utf8error is an enumeration of error constants.
 */

#include "u8/u8_error.h"

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
    UTF8ERROR_SUCCESS         = U8_ERROR_NONE,
                                /*!< success, there was no error */
    UTF8ERROR_NOT_FOUND       = U8_ERROR_NOT_FOUND,
                                /*!< pattern not found */
    UTF8ERROR_NULL_PARAM      = U8_ERROR_PARAM_MISSING,
                                /*!< NULL was provided as parameter instead of a valid pointer */
    UTF8ERROR_OUT_OF_RANGE    = U8_ERROR_PARAM_OUT_OF_RANGE,
                                /*!< some integer parameter was out of range */
    UTF8ERROR_TRUNCATED       = U8_ERROR_STRING_BUFFER_EXCEEDED,
                                /*!< the resulting string did not fit into the buffer, the string was truncated */
    UTF8ERROR_NOT_A_CODEPOINT = U8_ERROR_INVALID_ENCODING,
                                /*!< a codepoint was out of range: only 0x00000000 to 0x0010ffff are valid in utf8 */
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
 * Copyright 2012-2025 Andreas Warnke
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
