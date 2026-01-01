/* File: utf8stringbuf.h; Copyright and License: see below */

#ifndef UTF8STRINGBUF_H_
#define UTF8STRINGBUF_H_

/*!
 *  \file utf8stringbuf.h
 *  \brief utf8stringbuf provides functions to build and search within c-strings.
 *
 *  utf8stringbuf provides a struct consisting of
 *  \li a pointer to an character array and
 *  \li the size of the array.
 *
 *  Initialize the struct:
 *  \code
 *      char myBuf[50] = "";
 *      utf8stringbuf_t myStrBuf = UTF8STRINGBUF(myBuf);
 *  \endcode
 *
 *  Use the utf8stringbuf functions like
 *  \code
 *      utf8stringbuf_append_str( &myStrBuf, "Hello" );
 *  \endcode
 *  to build strings, search strings and copy strings.
 *
 *  Get a standard, 0-terminated C-String whenever needed:
 *  \code
 *     char* cStr = utf8stringbuf_get_string( &myStrBuf );
 *  \endcode
 */

#include "utf8stringbuf/utf8codepoint.h"
#include "utf8stringbuf/utf8error.h"
#include "utf8stringbuf/utf8stringview.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

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
 *  \brief A string buffer is a pair of size and an array of that size.
 *
 *  \invariant size must not be zero.
 *             buf points to an array of size bytes.
 *             The array shall be valid and null-terminated and located on a writeable memory page (non-const).
 */
struct utf8stringbuf_struct {
    char* buf;
    size_t size;
};

/*!
 *  \typedef utf8stringbuf_t
 *  \brief pointer to own object attributes
 *
 *  It encapsulates memory-pointers (char*), memory-sizes (size_t) and indices (ptrdiff_t)
 *  so that the calling code only sees a pointer-like utf8stringbuf_t struct and integer-indices.
 *  Errors are reported via an utf8error_t, in some exceptional cases via -1 as special-index.
 */
typedef struct utf8stringbuf_struct utf8stringbuf_t;

/*!
 *  \def UTF8STRINGBUF(charArr)
 *  \brief Macro to facilitate static initialisation of an utf8stringbuf_t, can also be used as typed r_value
 *
 *  Example usage:
 *  \n
 *  If you need only a small, temporary stringbuffer, consider to create this dynamically on the stack:
 *  \code
 *      #include "utf8stringbuf/utf8stringbuf.h"
 *      void MySampleFunction() {
 *          char myArr[50] = "";
 *          utf8stringbuf_t myStrBuf = UTF8STRINGBUF(myArr);
 *  \endcode
 *  \n
 *  If you need a stringbuffer that is either big or permanent,
 *  and which is accessed only by one single thread,
 *  consider to create this in the data section of your compilation-unit:
 *  \code
 *      #include "utf8stringbuf/utf8stringbuf.h"
 *      static char mySqlArr[16384] = "";
 *      utf8stringbuf_t mySqlBuf = UTF8STRINGBUF(mySqlArr);
 *  \endcode
 *  \n
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 */
#define UTF8STRINGBUF(charArr) (utf8stringbuf_t){.buf=charArr,.size=sizeof(charArr)}

/*!
 *  \def UTF8STRINGBUF_INIT(charArr)
 *  \brief Macro to facilitate static initialisation of an utf8stringbuf_t as struct member
 *  \n
 *  In a bigger project or in a multithreaded environment,
 *  consider to encapsulate your stringbuffer within a struct.
 *  You still have to cope with locking but all functions can work on a pointer
 *  to that struct instead of accessing global variables.
 *  \code
 *      #include "utf8stringbuf/utf8stringbuf.h"
 *      struct InitTestStruct {
 *          char urlArr[8192];
 *          utf8stringbuf_t url;
 *          pthread_mutex_t lock;
 *      };
 *      typedef struct InitTestStruct InitTest_t;
 *      static InitTest_t structTest = { "http://", UTF8STRINGBUF_INIT(structTest.urlArr), PTHREAD_MUTEX_INITIALIZER, };
 *  \endcode
 *  \n
 *  To statically initialize a whole array of the struct shown above, an own macro may help:
 *  \code
 *      #define INITTEST(testStr,this_) { testStr, UTF8STRINGBUF_INIT( this_.urlArr), PTHREAD_MUTEX_INITIALIZER, }
 *      static InitTest_t structArrTest[] = {
 *          INITTEST( "svn://first", structArrTest[0] ),
 *          INITTEST( "http://second", structArrTest[1] ),
 *          INITTEST( "ftp://third", structArrTest[2] ),
 *          INITTEST( "file://last", structArrTest[3] ),
 *      };
 *  \endcode
 *  \n
 *  To reduce footprint size while allocating static strings, put the character-arrays to bss-segment
 *  while initializing the utf8stringbufs in the data segment:
 *  \code
 *      static char ThousandPathNames[1000][256];  // no initialiation given here, will be located in bss and be initialized to zero
 *      #define PATH_INIT(x) UTF8STRINGBUF_INIT( ThousandPathNames[x] )
 *      #define FIVE_PATHS_INIT(x) PATH_INIT(x+0), PATH_INIT(x+1), PATH_INIT(x+2), PATH_INIT(x+3), PATH_INIT(x+4)
 *      #define TWENTY_PATHS_INIT(x) FIVE_PATHS_INIT(x+0), FIVE_PATHS_INIT(x+5), FIVE_PATHS_INIT(x+10), FIVE_PATHS_INIT(x+15)
 *      #define HUNDRED_PATHS_INIT(x) TWENTY_PATHS_INIT(x+0), TWENTY_PATHS_INIT(x+20), TWENTY_PATHS_INIT(x+40), TWENTY_PATHS_INIT(x+60), TWENTY_PATHS_INIT(x+80)
 *      static utf8stringbuf_t ThousandPaths[1000] = {
 *          HUNDRED_PATHS_INIT(0), HUNDRED_PATHS_INIT(100), HUNDRED_PATHS_INIT(200), HUNDRED_PATHS_INIT(300),
 *          HUNDRED_PATHS_INIT(400), HUNDRED_PATHS_INIT(500), HUNDRED_PATHS_INIT(600), HUNDRED_PATHS_INIT(700),
 *          HUNDRED_PATHS_INIT(800), HUNDRED_PATHS_INIT(900),
 *      };  // only these utf8stringbuf_t objects will exist in data segment. On a 32-bit platform, this should be 8kB.
 *  \endcode
 *  \n
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 */
#define UTF8STRINGBUF_INIT(charArr) {.buf=charArr,.size=sizeof(charArr)}

/*!
 *  \brief Creates a utf8stringbuf_t struct from a 0-terminated string
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param that Pointer to a 0-terminated string or NULL.
 *              The length of that string plus 1 is taken as buffer size.
 *              that must not point to a const character array.
 *  \return A utf8stringbuf_t struct. Even if that was NULL.
 */
static inline utf8stringbuf_t utf8stringbuf( char *that );

/*!
 *  \brief utf8stringbuf_wrong_init returns a stringbuf struct, buf is not modified
 *
 *  Use \link utf8stringbuf_clear(utf8stringbuf_t*) utf8stringbuf_clear \endlink to empty the string buffer.
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param buf pointer to a non-const byte array. buf must not be NULL.
 *  \param size size of the buf array. size shall be greater or equal 1.
 *  \return A valid utf8stringbuf_t struct. Even if buf or size were NULL.
 */
static inline utf8stringbuf_t utf8stringbuf_new( char *buf, size_t size );

/*!
 *  \brief utf8stringbuf_clear clears the contents of the string buffer.
 *
 *  The complete buffer is erased by zeroes.
 *  \n
 *  This function may be called on an uninitialized buffer that possibly is not null-terminated.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:size
 *  \param this_ pointer to own object attributes to be cleared.
 */
static inline void utf8stringbuf_clear( utf8stringbuf_t *this_ );

/*!
 *  \brief Gets the pointer to the character array
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ pointer to own object attributes
 *  \return Pointer to the character array, never NULL
 */
static inline char* utf8stringbuf_get_string( const utf8stringbuf_t *this_ );

/*!
 *  \brief Gets the size of the character array (not the length of the current c string which is always shorter)
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ pointer to own object attributes
 *  \return Size of the character array. This is always positive, never 0.
 */
static inline size_t utf8stringbuf_get_size( const utf8stringbuf_t *this_ );

/*!
 *  \brief Gets the length of the string.
 *
 *  The 0 termination byte is not counted.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ pointer to own object attributes
 *  \return Length of the string in bytes (not in utf-8 code-points)
 */
static inline unsigned int utf8stringbuf_get_length( const utf8stringbuf_t *this_ );

/*!
 *  \brief Gets a view on the character array, excluding the terminating zero
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ pointer to own object attributes
 *  \return A stringview instance
 */
static inline utf8stringview_t utf8stringbuf_get_view( const utf8stringbuf_t *this_ );

/*!
 *  \brief Checks if two strings are equal.
 *
 *  Only the strings are compared, not the possibly trailing bytes after the string.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ pointer to own object attributes
 *  \param that A 0-terminated c string. In case of NULL, this function returns 0.
 *  \return 1 if the strings are equal, 0 if not.
 */
static inline int utf8stringbuf_equals_str( const utf8stringbuf_t *this_, const char *that );

/*!
 *  \brief Checks if two strings are equal.
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ pointer to own object attributes
 *  \param that pointer to a string view object
 *  \return 1 if the strings are equal, 0 if not.
 */
static inline int utf8stringbuf_equals_view( const utf8stringbuf_t *this_, const utf8stringview_t *that );

/*!
 *  \brief Checks if two strings are equal.
 *
 *  Only the strings are compared, not the trailing bytes after the string.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ pointer to own object attributes
 *  \param that Another string buffer object
 *  \return 1 if the strings are equal, 0 if not.
 */
static inline int utf8stringbuf_equals_buf( const utf8stringbuf_t *this_, const utf8stringbuf_t *that );

/*!
 *  \brief Checks if the string buffer starts with the specified characters.
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ pointer to own object attributes
 *  \param that A 0-terminated c string. In case of NULL, this function returns 0.
 *  \return 1 if the string buffer starts with the characters in that, 0 if not.
 */
static inline int utf8stringbuf_starts_with_str( const utf8stringbuf_t *this_, const char *that );

/*!
 *  \brief Checks if the string buffer starts with the specified characters.
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ pointer to own object attributes
 *  \param that Another string buffer object.
 *  \return 1 if the string buffer starts with the characters in that, 0 if not.
 */
static inline int utf8stringbuf_starts_with_buf( const utf8stringbuf_t *this_, const utf8stringbuf_t *that );

/*!
 *  \brief Checks if the string buffer ends with the specified characters.
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ pointer to own object attributes
 *  \param that A 0-terminated c string. In case of NULL, this function returns 0.
 *  \return 1 if the string buffer ends with the characters in that, 0 if not.
 */
static inline int utf8stringbuf_ends_with_str( const utf8stringbuf_t *this_, const char *that );

/*!
 *  \brief Checks if the string buffer ends with the specified characters.
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ pointer to own object attributes
 *  \param that Another string buffer object.
 *  \return 1 if the string buffer ends with the characters in that, 0 if not.
 */
static inline int utf8stringbuf_ends_with_buf( const utf8stringbuf_t *this_, const utf8stringbuf_t *that );

/*!
 *  \brief Copies a string
 *
 *  If the source string does not fit into the destination buffer,
 *  the copied string gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ pointer to own object attributes.  It is valid to provide an uninitialized string buffer that possibly is not null-terminated.
 *  \param original The source string buffer. The buffers of this_ and original must not overlap!
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation.
 */
static inline utf8error_t utf8stringbuf_copy_buf( utf8stringbuf_t *this_, const utf8stringbuf_t *original );

/*!
 *  \brief Copies a string
 *
 *  If the source string does not fit into the destination buffer,
 *  the copied string gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ pointer to own object attributes. It is valid to provide an uninitialized string buffer that possibly is not null-terminated.
 *  \param original The 0-terminated source string. The buffers of this_ and original must not overlap!
 *                  NULL will cause the destination to be the empty string.
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation or UTF8ERROR_NULL_PARAM if that was NULL.
 */
static inline utf8error_t utf8stringbuf_copy_str( utf8stringbuf_t *this_, const char *original );

/*!
 *  \brief Copies a stringview
 *
 *  If the source stringview does not fit into the destination buffer,
 *  the copied stringview gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ pointer to own object attributes. It is valid to provide an uninitialized string buffer that possibly is not null-terminated.
 *  \param original pointer to the source string view. The buffers of this_ and original must not overlap!
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation
 */
static inline utf8error_t utf8stringbuf_copy_view( utf8stringbuf_t *this_, const utf8stringview_t *original );

/*!
 *  \brief Splits a string buffer into an ignored first part and the unfilled terminating part
 *
 *  This function may be useful when building an string using the append functions:
 *  Future append calls are faster because the utf8stringbuf is smaller.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ pointer to own object attributes
 *  \return an utf8stringbuf containing only the end
 */
static inline utf8stringbuf_t utf8stringbuf_get_end( utf8stringbuf_t *this_ );

/*!
 *  \brief Appends a string to a string buffer
 *
 *  If the source string does not fit into the destination buffer,
 *  the copied string gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n+a), n:strlen, a:appendlen
 *  \param this_ pointer to own object attributes
 *  \param appendix The 0-terminated source string.
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation or
 *          UTF8ERROR_NULL_PARAM if appendix was NULL.
 */
static inline utf8error_t utf8stringbuf_append_str( utf8stringbuf_t *this_, const char *appendix );

/*!
 *  \brief Appends a string buffer to a string buffer
 *
 *  If the source string does not fit into the destination buffer,
 *  the copied string gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n+a), n:strlen, a:appendlen
 *  \param this_ pointer to own object attributes
 *  \param appendix The source string buffer
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation.
 */
static inline utf8error_t utf8stringbuf_append_buf( utf8stringbuf_t *this_, const utf8stringbuf_t *appendix );

/*!
 *  \brief Appends a signed integer to a string buffer in decimal format
 *
 *  If the integer not fit to the destination buffer,
 *  the copied string gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ pointer to own object attributes
 *  \param appendix The integer to be appended
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation.
 */
static inline utf8error_t utf8stringbuf_append_int( utf8stringbuf_t *this_, const int64_t appendix );

/*!
 *  \brief Appends an unsigned integer to a string buffer in hexadecimal format
 *
 *  If the integer not fit to the destination buffer,
 *  the copied string gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ pointer to own object attributes
 *  \param appendix The integer to be appended
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation.
 */
static inline utf8error_t utf8stringbuf_append_hex( utf8stringbuf_t *this_, const uint64_t appendix );

/*!
 *  \brief Appends an unicode character to a string buffer
 *
 *  If the character does not fit to the destination buffer,
 *  the destination buffer is not modified.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ pointer to own object attributes
 *  \param appendix The unicode codepoint to be appended
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED if the character could not be appended.
 *          UTF8ERROR_NOT_A_CODEPOINT if appendix is greater than 0x10ffff
 */
extern utf8error_t utf8stringbuf_append_char( utf8stringbuf_t *this_, const uint32_t appendix );

/*!
 *  \brief Appends a string of wchar_t to a string buffer
 *
 *  If the source string does not fit into the destination buffer,
 *  the copied string gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *  \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n+a), n:strlen, a:appendlen
 *  \param this_ pointer to own object attributes
 *  \param appendix The 0-terminated source string.
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation or
 *          UTF8ERROR_NULL_PARAM if appendix was NULL or
 *          UTF8ERROR_NOT_A_CODEPOINT if one or more characters of appendix are greater than 0x7fffffff
 */
extern utf8error_t utf8stringbuf_append_wstr( utf8stringbuf_t *this_, const wchar_t *appendix );

/*!
 *  \brief Appends a stringview to a string buffer
 *
 *  If the source string does not fit into the destination buffer,
 *  the copied string gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n+a), n:strlen, a:appendlen
 *  \param this_ pointer to own object attributes
 *  \param appendix pointer to the source stringview.
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation or
 *          UTF8ERROR_NULL_PARAM if appendix was NULL.
 */
static inline utf8error_t utf8stringbuf_append_view( utf8stringbuf_t *this_, const utf8stringview_t *appendix );

#ifdef __cplusplus
}
#endif

#include "utf8stringbuf/utf8stringbuf.inl"

#endif /*UTF8STRINGBUF_H_*/


/*
 * Copyright 2012-2026 Andreas Warnke
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
