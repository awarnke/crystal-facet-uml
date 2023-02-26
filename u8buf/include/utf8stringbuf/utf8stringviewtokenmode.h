/* File: utf8stringviewtokenmode.h; Copyright and License: see below */

#ifndef UTF8STRINGVIEWTOKENMODE_H
#define UTF8STRINGVIEWTOKENMODE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Iterates over a character-separated list of a stringview
 */

#include "utf8stringbuf/utf8string.h"
#include "utf8stringbuf/utf8stringview.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \enum utf8stringviewtokenmode_enum
 *
 *  For tokenizers, this enumeration presents modes to detect the borders of the tokens.
 *
 *  These modes have in common
 *  - whitespaces and control-characters are not tokens, but they separate tokens
 *  - special characters are single codepoint tokens
 *  - special characters are recognized only in ascii area, not in whole unicode range
 *  - no support for escaped characters
 *  - underscore is treated like alphanumeric characters
 *  - numbers attached to alphanumerical characters are treated as same token
 *  - integers are recognized as tokens, e.g. 05, +10, -80
 *
 *  These modes differ in
 *  - floating point numbers are recognized as tokens, e.g. 0.0, +1.e-5, -03.23E+14, NaN, -Infinity
 *    for use in the stdlib function double strtod(...)
 */
enum utf8stringviewtokenmode_enum {
    UTF8STRINGVIEWTOKENMODE_INT = 0,    /*!< alphanumerical, special-char and integer tokens are recognized */
    UTF8STRINGVIEWTOKENMODE_FLOAT = 1,  /*!< alphanumerical, special-char and int/float number tokens are recognized */
};

/*!
 *  \typedef utf8stringviewtokenmode_t
 *  \brief Modes to detect the borders of tokens
 */
typedef enum utf8stringviewtokenmode_enum utf8stringviewtokenmode_t;

#ifdef __cplusplus
}
#endif

#endif  /* UTF8STRINGVIEWTOKENMODE_H */


/*
Copyright 2023-2023 Andreas Warnke

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
