#ifndef UTF8_STRING_H_
#define UTF8_STRING_H_

/*!
 *  \file utf8_string.h
 *  \brief utf8_string provides functions to search and compare c-strings.
 * 
 *  \note License: Use this code according to the license: Apache 2.0.
 *  \author (c) 2012-2016 A.Warnke; Email-contact: utf8stringbuf-at-andreaswarnke-dot-de
 */ 

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "util/string/utf8_code_point.h"
#include "util/string/utf8_string_buf.h"

#ifdef __cplusplus
extern "C" {
#endif
    
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
static inline unsigned int utf8_string_get_size( const char *this_ );

/*!
 * \brief Gets the length of the string.
 *
 * The 0 termination byte is not counted.
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen   
 * \param this_ A string object
 * \return Length of the string in bytes (not in utf-8 code-points)
 */
static inline unsigned int utf8_string_get_length( const char *this_ );

/*!
 * \brief Checks if two strings are equal.
 *
 * NULL never equals anything.
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen   
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param that Another 0-terminated c string. In case of NULL, this function returns 0.
 * \return 1 if the strings are equal, 0 if not.
 */
static inline int utf8_string_equals_str( const char *this_, const char *that );

/*!
 * \brief Checks if two strings are equal.
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen   
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param that A string buffer object
 * \return 1 if the strings are equal, 0 if not.
 */
static inline int utf8_string_equals_buf( const char *this_, const utf8_string_buf_t that );

/*!
 * \brief Checks if the region equals the given string.
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen   
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param start the start position of the region to compare.
 * \param that A 0-terminated c string. In case of NULL, this function returns 0.
 * \return 1 if the region equals the given string, 0 if not.
 */
static inline int utf8_string_equals_region_str( const char *this_, int start, const char *that );

/*!
 * \brief Checks if the region equals the given string.
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen   
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param start the start position of the region to compare.
 * \param that A string buffer object
 * \return 1 if the region equals the given string, 0 if not.
 */
static inline int utf8_string_equals_region_buf( const char *this_, int start, const utf8_string_buf_t that );

/*!
 * \brief Checks if the string buffer starts with the specified characters.
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen   
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param that A 0-terminated c string. In case of NULL, this function returns 0.
 * \return 1 if the string starts with the characters in that, 0 if not.
 */
static inline int utf8_string_starts_with_str( const char *this_, const char *that );

/*!
 * \brief Checks if the string buffer starts with the specified characters.
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen   
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param that A string buffer object.
 * \return 1 if the string starts with the characters in that, 0 if not.
 */
static inline int utf8_string_starts_with_buf( const char *this_, const utf8_string_buf_t that );

/*!
 * \brief Checks if the string buffer ends with the specified characters.
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen   
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param that A 0-terminated c string. In case of NULL, this function returns 0.
 * \return 1 if the string ends with the characters in that, 0 if not.
 */
static inline int utf8_string_ends_with_str( const char *this_, const char *that );

/*!
 * \brief Checks if the string buffer ends with the specified characters.
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen   
 * \param this_ A 0-terminated c string. In case of NULL, this function returns 0.
 * \param that A string buffer object.
 * \return 1 if the string ends with the characters in that, 0 if not.
 */
static inline int utf8_string_ends_with_buf( const char *this_, const utf8_string_buf_t that );

/*!
 * \brief Searches a pattern within a string
 * 
 * Example:
 * \code
 *     utf8_string_find_first_buf( "hasta la vista", utf8_string_buf( "sta" ));
 * \endcode
 * will return index 2.
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen   
 * \param this_ The 0-terminated string within which to search
 * \param pattern The string buffer containing the byte-sequence to search
 * \return Index of the first occurrence within the string. 
 *         -1 if there is no match.
 */
static inline int utf8_string_find_first_buf( const char *this_, const utf8_string_buf_t pattern );

/*!
 * \brief Searches a pattern within a string
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen   
 * \param this_ The 0-terminated string within which to search
 * \param pattern The 0-terminated string to search
 * \return Index of the first occurrence within the string. 
 *         -1 if there is no match.
 */
static inline int utf8_string_find_first_str( const char *this_, const char *pattern );

/*!
 * \brief Searches a pattern within a string starting at the end
 * 
 * Example:
 * \code
 *     utf8_string_find_last_buf( "hasta la vista", utf8_string_buf( "sta" ));
 * \endcode
 * will return index 11.
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [ ]medium   [x]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen   
 * \param this_ The 0-terminated string within which to search
 * \param pattern The string buffer containing the byte-sequence to search
 * \return Index of the first occurrence within the string. 
 *         -1 if there is no match.
 */
static inline int utf8_string_find_last_buf( const char *this_, const utf8_string_buf_t pattern );

/*!
 * \brief Searches a pattern within a string starting at the end
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [ ]medium   [x]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen   
 * \param this_ The 0-terminated string within which to search
 * \param pattern The 0-terminated string to search
 * \return Index of the first occurrence within the string. 
 *         -1 if there is no match.
 */
static inline int utf8_string_find_last_str( const char *this_, const char *pattern );

/*!
 * \brief Searches a pattern within a string
 * 
 * Example:
 * \code
 *     utf8_string_find_next_buf( "hasta la vista", utf8_string_buf( "sta" ), 3);
 * \endcode
 * will return index 11.
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen   
 * \param this_ The 0-terminated string within which to search
 * \param pattern The string buffer containing the byte-sequence to search
 * \param start_index Index where to start the search.
 * \return Index of the next occurrence within the string equal or greater than start_index. 
 *         -1 if there is no match.
 */
static inline int utf8_string_find_next_buf( const char *this_, const utf8_string_buf_t pattern, int start_index );

/*!
 * \brief Searches a pattern within a string
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen   
 * \param this_ The 0-terminated string within which to search
 * \param pattern The 0-terminated string to search
 * \param start_index Index where to start the search.
 * \return Index of the next occurrence within the string equal or greater than start_index. 
 *         -1 if there is no match.
 */
static inline int utf8_string_find_next_str( const char *this_, const char *pattern, int start_index );

/*!
 * \brief Gets the code point at a given byte index.
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)   
 * \param this_ The 0-terminated string object
 * \param byte_index index where to read the character from.
 * \return A unicode codepoint.
 *         utf8_code_point_is_valid will state 0 if there is an illegal byte-sequence within the string
 *         or if byte_index is out of range.
 *         The terminating zero of a string is a valid character.
 */
static inline utf8_code_point_t utf8_string_get_char_at( const char *this_, unsigned int byte_index );

#ifdef __cplusplus
}
#endif

#include "util/string/utf8_string.inl"

#endif /*UTF8_STRING_H_*/
