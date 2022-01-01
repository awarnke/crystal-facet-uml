/* File: utf8string.h; Copyright and License: see below */

#ifndef UTF8STRING_H_
#define UTF8STRING_H_

/*!
 *  \file utf8string.h
 *  \brief utf8string provides functions to search and compare c-strings.
 *
 *  \note License: Use this code according to the license: Apache 2.0.
 *  \author Copyright 2012-2022 A.Warnke; Email-contact: utf8stringbuf-at-andreaswarnke-dot-de
 */

#include "util/string/utf8codepoint.h"
#include "util/string/utf8stringbuf.h"
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
 *  \brief The string object: A const char pointer.
 * 
 *  utf8string_t objects point to immutable strings, use a utf8stringbuf_t to modify the string object.
 */
typedef const char* utf8string_t;
    
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
static inline size_t utf8string_get_size( const char *this_ );

/*!
 * \brief Gets the length of the string.
 *
 * The 0 termination byte is not counted.
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ A string object
 * \return Length of the string in bytes (not in utf-8 code-points)
 */
static inline unsigned int utf8string_get_length( const char *this_ );

/*!
 * \brief Checks if two strings are equal.
 *
 * NULL never equals anything.
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param that Another 0-terminated c string. In case of NULL, this function returns 0.
 * \return 1 if the strings are equal, 0 if not.
 */
static inline int utf8string_equals_str( const char *this_, const char *that );

/*!
 * \brief Checks if two strings are equal.
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param that A string buffer object
 * \return 1 if the strings are equal, 0 if not.
 */
static inline int utf8string_equals_buf( const char *this_, const utf8stringbuf_t that );

/*!
 * \brief Checks if the region equals the given string.
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param start the start position of the region to compare.
 * \param that A 0-terminated c string. In case of NULL, this function returns 0.
 * \return 1 if the region equals the given string, 0 if not.
 */
static inline int utf8string_equals_region_str( const char *this_, int start, const char *that );

/*!
 * \brief Checks if the region equals the given string.
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param start the start position of the region to compare.
 * \param that A string buffer object
 * \return 1 if the region equals the given string, 0 if not.
 */
static inline int utf8string_equals_region_buf( const char *this_, int start, const utf8stringbuf_t that );

/*!
 * \brief Checks if the string buffer starts with the specified characters.
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param that A 0-terminated c string. In case of NULL, this function returns 0.
 * \return 1 if the string starts with the characters in that, 0 if not.
 */
static inline int utf8string_starts_with_str( const char *this_, const char *that );

/*!
 * \brief Checks if the string buffer starts with the specified characters.
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param that A string buffer object.
 * \return 1 if the string starts with the characters in that, 0 if not.
 */
static inline int utf8string_starts_with_buf( const char *this_, const utf8stringbuf_t that );

/*!
 * \brief Checks if the string buffer ends with the specified characters.
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param that A 0-terminated c string. In case of NULL, this function returns 0.
 * \return 1 if the string ends with the characters in that, 0 if not.
 */
static inline int utf8string_ends_with_str( const char *this_, const char *that );

/*!
 * \brief Checks if the string buffer ends with the specified characters.
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param that A string buffer object.
 * \return 1 if the string ends with the characters in that, 0 if not.
 */
static inline int utf8string_ends_with_buf( const char *this_, const utf8stringbuf_t that );

/*!
 * \brief Searches a pattern within a string
 *
 * Example:
 * \code
 *     utf8string_find_first_buf( "hasta la vista", utf8stringbuf( "sta" ));
 * \endcode
 * will return index 2.
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen
 * \param this_ The 0-terminated string within which to search
 * \param pattern The string buffer containing the byte-sequence to search
 * \return Index of the first occurrence within the string.
 *         -1 if there is no match.
 */
static inline int utf8string_find_first_buf( const char *this_, const utf8stringbuf_t pattern );

/*!
 * \brief Searches a pattern within a string
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen
 * \param this_ The 0-terminated string within which to search
 * \param pattern The 0-terminated string to search
 * \return Index of the first occurrence within the string.
 *         -1 if there is no match.
 */
static inline int utf8string_find_first_str( const char *this_, const char *pattern );

/*!
 * \brief Searches a pattern within a string starting at the end
 *
 * Example:
 * \code
 *     utf8string_find_last_buf( "hasta la vista", utf8stringbuf( "sta" ));
 * \endcode
 * will return index 11.
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [ ]medium   [x]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen
 * \param this_ The 0-terminated string within which to search
 * \param pattern The string buffer containing the byte-sequence to search
 * \return Index of the first occurrence within the string.
 *         -1 if there is no match.
 */
static inline int utf8string_find_last_buf( const char *this_, const utf8stringbuf_t pattern );

/*!
 * \brief Searches a pattern within a string starting at the end
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [ ]medium   [x]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen
 * \param this_ The 0-terminated string within which to search
 * \param pattern The 0-terminated string to search
 * \return Index of the first occurrence within the string.
 *         -1 if there is no match.
 */
static inline int utf8string_find_last_str( const char *this_, const char *pattern );

/*!
 * \brief Searches a pattern within a string
 *
 * Example:
 * \code
 *     utf8string_find_next_buf( "hasta la vista", utf8stringbuf( "sta" ), 3);
 * \endcode
 * will return index 11.
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen
 * \param this_ The 0-terminated string within which to search
 * \param pattern The string buffer containing the byte-sequence to search
 * \param start_index Index where to start the search.
 * \return Index of the next occurrence within the string equal or greater than start_index.
 *         -1 if there is no match.
 */
static inline int utf8string_find_next_buf( const char *this_, const utf8stringbuf_t pattern, int start_index );

/*!
 * \brief Searches a pattern within a string
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen
 * \param this_ The 0-terminated string within which to search
 * \param pattern The 0-terminated string to search
 * \param start_index Index where to start the search.
 * \return Index of the next occurrence within the string equal or greater than start_index.
 *         -1 if there is no match.
 */
static inline int utf8string_find_next_str( const char *this_, const char *pattern, int start_index );

/*!
 * \brief Gets the code point at a given byte index.
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 * \param this_ The 0-terminated string object
 * \param byte_index index where to read the character from.
 * \return A unicode codepoint.
 *         utf8codepoint_is_valid will state 0 if there is an illegal byte-sequence within the string
 *         or if byte_index is out of range.
 *         The terminating zero of a string is a valid character.
 */
static inline utf8codepoint_t utf8string_get_char_at( const char *this_, unsigned int byte_index );

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
static inline utf8error_t utf8string_parse_int( const char *this_, unsigned int *out_byte_length, int64_t *out_number );

#ifdef __cplusplus
}
#endif

#include "util/string/utf8string.inl"

#endif /*UTF8STRING_H_*/


/*
 * Copyright 2012-2022 Andreas Warnke
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
