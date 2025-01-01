/* File: utf8string.h; Copyright and License: see below */

#ifndef UTF8STRING_H_
#define UTF8STRING_H_

/*!
 *  \file utf8string.h
 *  \brief utf8string provides functions to search and compare c-strings.
 */

#include "utf8stringbuf/utf8codepoint.h"
#include "utf8stringbuf/utf8error.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \def UTF8STRING_NULL
 *  \brief NULL value of an utf8string_t
 *
 *  \n
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 */
#define UTF8STRING_NULL (NULL)

/*!
 *  \typedef utf8string_t
 *  \brief The string object is of type const char (array of const char would have been more accurate).
 *
 *  utf8string_t is a const char to avoid typecasts whenever exchanging strings with POSIX, ANSI-C or other libraries.
 *
 *  utf8string_t objects are immutable strings, use a utf8stringbuf_t to modify the string object.
 */
typedef const char utf8string_t;

/* Note: If optimization level is zero, inline might not work. */
/*       You possibly have to append the following string      */
/*       after every function prototype for gcc:               */
/*       __attribute__((always_inline))                        */
/*       Or prepend the following string                       */
/*       to every function prototype for Visual Studio:        */
/*       __forceinline                                         */

/*!
 * \brief Gets the size of the string: the length plus 1 for the terminating zero
 * \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 * \param this_ The string object
 * \return Size of the string. This is always positive, never 0.
 */
static inline size_t utf8string_get_size( utf8string_t *this_ );

/*!
 * \brief Gets the length of the string.
 *
 * The 0 termination byte is not counted.
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ A string object
 * \return Length of the string in bytes (not in utf-8 code-points)
 */
static inline unsigned int utf8string_get_length( utf8string_t *this_ );

/*!
 * \brief Checks if two strings are equal.
 *
 * NULL never equals anything.
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param that Another 0-terminated c string. In case of NULL, this function returns 0.
 * \return 1 if the strings are equal, 0 if not.
 */
static inline int utf8string_equals_str( utf8string_t *this_, utf8string_t *that );

/*!
 * \brief Checks if the string buffer starts with the specified characters.
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param that A 0-terminated c string. In case of NULL, this function returns 0.
 * \return 1 if the string starts with the characters in that, 0 if not.
 */
static inline int utf8string_starts_with_str( utf8string_t *this_, utf8string_t *that );

/*!
 * \brief Checks if the string buffer ends with the specified characters.
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param that A 0-terminated c string. In case of NULL, this function returns 0.
 * \return 1 if the string ends with the characters in that, 0 if not.
 */
static inline int utf8string_ends_with_str( utf8string_t *this_, utf8string_t *that );

/*!
 * \brief Searches a pattern within a string
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen
 * \param this_ The 0-terminated string within which to search
 * \param pattern The 0-terminated string to search
 * \return true if there is a match
 */
static inline bool utf8string_contains_str( utf8string_t *this_, utf8string_t *pattern );

/*!
 * \brief Parses a signed integer from a string in decimal format
 *
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ A 0-terminated c string. In case of NULL, this function returns UTF8ERROR_NULL_PARAM.
 * \param out_byte_length The number of bytes parsed. (In utf8, this is identical to the number of code points)
 * \param out_number The parsed integer
 * \return UTF8ERROR_SUCCESS in case of success: An integer number has been parsed.
 *         UTF8ERROR_NOT_FOUND in case there is no decimal integer.
 *         UTF8ERROR_NULL_PARAM in this_ or out_number is NULL
 *         UTF8ERROR_OUT_OF_RANGE in case there is a decimal integer which does not fit into int64_t.
 */
utf8error_t utf8string_parse_int( utf8string_t *this_, unsigned int *out_byte_length, int64_t *out_number );

/*!
 * \brief Parses a floating point number from a string in decimal mantissa, optional fraction and optional exponent format
 *
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ A 0-terminated c string. In case of NULL, this function returns UTF8ERROR_NULL_PARAM.
 * \param out_byte_length The number of bytes parsed. (In utf8, this is identical to the number of code points)
 * \param out_number The parsed floating point number
 * \return UTF8ERROR_SUCCESS in case of success: An integer number has been parsed.
 *         UTF8ERROR_NOT_FOUND in case there is no number.
 *         UTF8ERROR_NULL_PARAM in this_ or out_number is NULL
 *         UTF8ERROR_OUT_OF_RANGE in case there is a number which does not fit into double.
 */
utf8error_t utf8string_parse_float( utf8string_t *this_, unsigned int *out_byte_length, double *out_number );

#ifdef __cplusplus
}
#endif

#include "utf8stringbuf/utf8string.inl"

#endif /*UTF8STRING_H_*/


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
