/* File: utf8stringbuf.h; Copyright and License: see below */

#ifndef UTF8STRINGBUF_H_
#define UTF8STRINGBUF_H_

/*!
 *  \file utf8stringbuf.h
 *  \brief utf8stringbuf provides functions to build, search and modify c-strings.
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
 *      utf8stringbuf_append_str( myStrBuf, "Hello" );
 *  \endcode
 *  to build strings, search strings and copy strings.
 *
 *  Get a standard, 0-terminated C-String whenever needed:
 *  \code
 *     char* cStr = utf8stringbuf_get_string( myStrBuf );
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
    size_t size;
    char* buf;
};

/*!
 *  \typedef utf8stringbuf_t
 *  \brief The string buffer object
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
#define UTF8STRINGBUF(charArr) (utf8stringbuf_t){.size=sizeof(charArr),.buf=charArr}

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
#define UTF8STRINGBUF_INIT(charArr) {.size=sizeof(charArr),.buf=charArr}

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
 *  \brief utf8stringbuf_init returns a stringbuf struct, buf is not modified
 *
 *  Use \link utf8stringbuf_clear(utf8stringbuf_t) utf8stringbuf_clear \endlink to empty the string buffer.
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param size size of the buf array. size shall be greater or equal 1.
 *  \param buf pointer to a non-const byte array. buf must not be NULL.
 *  \return A valid utf8stringbuf_t struct. Even if buf or size were NULL.
 */
static inline utf8stringbuf_t utf8stringbuf_init( size_t size, char *buf );

/*!
 *  \brief utf8stringbuf_clear clears the contents of the string buffer.
 *
 *  The complete buffer is erased by zeroes.
 *  \n
 *  This function may be called on an uninitialized buffer that possibly is not null-terminated.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:size
 *  \param this_ The string buffer object to be cleared.
 */
static inline void utf8stringbuf_clear( utf8stringbuf_t this_ );

/*!
 *  \brief Gets the pointer to the character array
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ The string buffer object
 *  \return Pointer to the character array, never NULL
 */
static inline char* utf8stringbuf_get_string( const utf8stringbuf_t this_ );

/*!
 *  \brief Gets the size of the character array (not the length of the current c string which is always shorter)
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ The string buffer object
 *  \return Size of the character array. This is always positive, never 0.
 */
static inline size_t utf8stringbuf_get_size( const utf8stringbuf_t this_ );

/*!
 *  \brief Gets the length of the string.
 *
 *  The 0 termination byte is not counted.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ A string buffer object
 *  \return Length of the string in bytes (not in utf-8 code-points)
 */
static inline unsigned int utf8stringbuf_get_length( const utf8stringbuf_t this_ );

/*!
 *  \brief Checks if two strings are equal.
 *
 *  Only the strings are compared, not the possibly trailing bytes after the string.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ A string buffer object
 *  \param that A 0-terminated c string. In case of NULL, this function returns 0.
 *  \return 1 if the strings are equal, 0 if not.
 */
static inline int utf8stringbuf_equals_str( const utf8stringbuf_t this_, const char *that );

/*!
 *  \brief Checks if two strings are equal.
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ A string buffer object
 *  \param that pointer to a string view object
 *  \return 1 if the strings are equal, 0 if not.
 */
static inline int utf8stringbuf_equals_view( const utf8stringbuf_t this_, const utf8stringview_t *that );

/*!
 *  \brief Checks if two strings are equal.
 *
 *  Only the strings are compared, not the trailing bytes after the string.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ A string buffer object
 *  \param that Another string buffer object
 *  \return 1 if the strings are equal, 0 if not.
 */
static inline int utf8stringbuf_equals_buf( const utf8stringbuf_t this_, const utf8stringbuf_t that );

/*!
 *  \brief Checks if the region equals the given string.
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ A string buffer object
 *  \param start the start position of the region to compare.
 *  \param that A 0-terminated c string. In case of NULL, this function returns 0.
 *  \return 1 if the region equals the given string, 0 if not.
 */
static inline int utf8stringbuf_equals_region_str( const utf8stringbuf_t this_, int start, const char *that );

/*!
 *  \brief Checks if the region equals the given string.
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ A string buffer object
 *  \param start the start position of the region to compare.
 *  \param that Another string buffer object
 *  \return 1 if the region equals the given string, 0 if not.
 */
static inline int utf8stringbuf_equals_region_buf( const utf8stringbuf_t this_, int start, const utf8stringbuf_t that );

/*!
 *  \brief Checks if the string buffer starts with the specified characters.
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ A string buffer object
 *  \param that A 0-terminated c string. In case of NULL, this function returns 0.
 *  \return 1 if the string buffer starts with the characters in that, 0 if not.
 */
static inline int utf8stringbuf_starts_with_str( const utf8stringbuf_t this_, const char *that );

/*!
 *  \brief Checks if the string buffer starts with the specified characters.
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ A string buffer object
 *  \param that Another string buffer object.
 *  \return 1 if the string buffer starts with the characters in that, 0 if not.
 */
static inline int utf8stringbuf_starts_with_buf( const utf8stringbuf_t this_, const utf8stringbuf_t that );

/*!
 *  \brief Checks if the string buffer ends with the specified characters.
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ A string buffer object
 *  \param that A 0-terminated c string. In case of NULL, this function returns 0.
 *  \return 1 if the string buffer ends with the characters in that, 0 if not.
 */
static inline int utf8stringbuf_ends_with_str( const utf8stringbuf_t this_, const char *that );

/*!
 *  \brief Checks if the string buffer ends with the specified characters.
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ A string buffer object
 *  \param that Another string buffer object.
 *  \return 1 if the string buffer ends with the characters in that, 0 if not.
 */
static inline int utf8stringbuf_ends_with_buf( const utf8stringbuf_t this_, const utf8stringbuf_t that );

/*!
 *  \brief Searches a pattern within a string
 *
 *  Example:
 *  \code
 *      utf8stringbuf_find_first_buf( utf8stringbuf( "hasta la vista" ), utf8stringbuf( "sta" ));
 *  \endcode
 *  will return index 2.
 *  \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen
 *  \param this_ The string buffer within which to search
 *  \param pattern The string buffer containing the byte-sequence to search
 *  \return Index of the first occurrence within the string buffer.
 *          -1 if there is no match.
 */
static inline int utf8stringbuf_find_first_buf( const utf8stringbuf_t this_, const utf8stringbuf_t pattern );

/*!
 *  \brief Searches a pattern within a string
 *  \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen
 *  \param this_ The string buffer within which to search
 *  \param pattern The 0-terminated string to search
 *  \return Index of the first occurrence within the string buffer.
 *          -1 if there is no match.
 */
static inline int utf8stringbuf_find_first_str( const utf8stringbuf_t this_, const char *pattern );

/*!
 *  \brief Searches a pattern within a string starting at the end
 *
 *  Example:
 *  \code
 *      utf8stringbuf_find_last_buf( utf8stringbuf( "hasta la vista" ), utf8stringbuf( "sta" ));
 *  \endcode
 *  will return index 11.
 *  \note Performance-Rating: [ ]single-operation   [ ]fast   [ ]medium   [x]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen
 *  \param this_ The string buffer within which to search
 *  \param pattern The string buffer containing the byte-sequence to search
 *  \return Index of the first occurrence within the string buffer.
 *          -1 if there is no match.
 */
static inline int utf8stringbuf_find_last_buf( const utf8stringbuf_t this_, const utf8stringbuf_t pattern );

/*!
 *  \brief Searches a pattern within a string starting at the end
 *  \note Performance-Rating: [ ]single-operation   [ ]fast   [ ]medium   [x]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen
 *  \param this_ The string buffer within which to search
 *  \param pattern The 0-terminated string to search
 *  \return Index of the first occurrence within the string buffer.
 *          -1 if there is no match.
 */
static inline int utf8stringbuf_find_last_str( const utf8stringbuf_t this_, const char *pattern );

/*!
 *  \brief Searches a pattern within a string
 *
 *  Example:
 *  \code
 *      utf8stringbuf_find_next_buf( utf8stringbuf( "hasta la vista" ), utf8stringbuf( "sta" ), 3);
 *  \endcode
 *  will return index 11.
 *  \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen
 *  \param this_ The string buffer within which to search
 *  \param pattern The string buffer containing the byte-sequence to search
 *  \param start_index Index where to start the search.
 *  \return Index of the next occurrence within the string buffer equal or greater than start_index.
 *          -1 if there is no match.
 */
static inline int utf8stringbuf_find_next_buf( const utf8stringbuf_t this_, const utf8stringbuf_t pattern, int start_index );

/*!
 *  \brief Searches a pattern within a string
 *  \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen
 *  \param this_ The string buffer within which to search
 *  \param pattern The 0-terminated string to search
 *  \param start_index Index where to start the search.
 *  \return Index of the next occurrence within the string buffer equal or greater than start_index.
 *          -1 if there is no match.
 */
static inline int utf8stringbuf_find_next_str( const utf8stringbuf_t this_, const char *pattern, int start_index );

/*!
 *  \brief Gets the code point at a given byte index.
 *
 *  Example usage:
 *  \n
 *  You can read a codepoint at a given byte index,
 *  e.g. within a trademark information, where C2 AE is the (R) sign:
 *  \code
 *      utf8codepoint_t result;
 *      char testArr[] = "\xC2\xAE 2005 A.A. \xC2\xAE 2006-2012 B.B.";
 *      utf8stringbuf_t testBuf = UTF8STRINGBUF(testArr);
 *      result = utf8stringbuf_get_char_at( testBuf, 13 );
 *      printf( "Code Point: %x", utf8codepoint_get_char(result) );
 *  \endcode
 *  This code will print "Code Point: ae" because the utf-8 sequence C2 AE is the unicode character 00AE.
 *  \n
 *
 *  You can iterate over all code points:
 *  \code
 *      size_t byteSize = utf8stringbuf_get_size( testBuf );
 *      unsigned int currentIndex = 0;
 *      for ( int loopCount = 0; loopCount < byteSize; loopCount ++ ) {
 *          result = utf8stringbuf_get_char_at( testBuf, currentIndex );
 *          if ( utf8codepoint_is_valid(result) && ( utf8codepoint_get_char(result) != '\0' )) {
 *              // do something with the current code point here
 *              currentIndex += utf8codepoint_get_length(result);
 *          }
 *          else {
 *              break;
 *          }
 *      }
 *  \endcode
 *  Note: the loopCount variable is just an unnecessary fallback to ensure that this is no endless loop, an unconditional for(;;) will also succeed.
 *  \n
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ The string buffer object
 *  \param byte_index index where to read the character from.
 *  \return A unicode codepoint. The result is undefined if byte_index is out of range.
 *          utf8codepoint_is_valid will state 0 if there is an illegal byte-sequence within the string buffer.
 *          The terminating zero of a string buffer is a valid character.
 */
static inline utf8codepoint_t utf8stringbuf_get_char_at( const utf8stringbuf_t this_, unsigned int byte_index );

/*!
 *  \brief Copies a string
 *
 *  If the source string does not fit into the destination buffer,
 *  the copied string gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ The destination string buffer.  It is valid to provide an uninitialized string buffer that possibly is not null-terminated.
 *  \param original The source string buffer. The buffers of this_ and original must not overlap!
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation.
 */
static inline utf8error_t utf8stringbuf_copy_buf( utf8stringbuf_t this_, const utf8stringbuf_t original );

/*!
 *  \brief Copies a string
 *
 *  If the source string does not fit into the destination buffer,
 *  the copied string gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ The destination string buffer. It is valid to provide an uninitialized string buffer that possibly is not null-terminated.
 *  \param original The 0-terminated source string. The buffers of this_ and original must not overlap!
 *                  NULL will cause the destination to be the empty string.
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation or UTF8ERROR_NULL_PARAM if that was NULL.
 */
static inline utf8error_t utf8stringbuf_copy_str( utf8stringbuf_t this_, const char *original );

/*!
 *  \brief Copies a stringview
 *
 *  If the source stringview does not fit into the destination buffer,
 *  the copied stringview gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ The destination string buffer. It is valid to provide an uninitialized string buffer that possibly is not null-terminated.
 *  \param original pointer to the source string view. The buffers of this_ and original must not overlap!
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation
 */
static inline utf8error_t utf8stringbuf_copy_view( utf8stringbuf_t this_, const utf8stringview_t *original );

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
/*!
 * \brief Copies a substring
 *
 * If the source string does not fit into the destination buffer,
 * the copied string gets truncated. This function ensures, that
 * truncation does not split an utf8 code-point in half.
 * \n
 * This function does not check, that the provided range in the source string is a valid utf8 sequence.
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ The destination string buffer.  It is valid to provide an uninitialized buffer that possibly is not null-terminated.
 * \param that The source string buffer. The buffers of this_ and that must not overlap!
 * \param start Start index within the source string, from which to copy. 0 is the index of the first byte.
 * \param length Length in bytes to be copied.
 * \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *         UTF8ERROR_TRUNCATED in case of truncation or
 *         UTF8ERROR_OUT_OF_RANGE in case of illegal range.
 */
extern utf8error_t utf8stringbuf_copy_region_from_buf( utf8stringbuf_t this_, const utf8stringbuf_t that, int start, int length );
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
/*!
 * \brief Copies a substring
 *
 * If the source string does not fit into the destination buffer,
 * the copied string gets truncated. This function ensures, that
 * truncation does not split an utf8 code-point in half.
 * \n
 * This function does not check, that the provided range in the source string is a valid utf8 sequence.
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ The destination string buffer.  It is valid to provide an uninitialized string buffer that possibly is not null-terminated.
 * \param that The 0-terminated source string. The buffers of this_ and that must not overlap!
 * \param start Start index within the source string, from which to copy. 0 is the index of the first byte.
 * \param length Length in bytes to be copied.
 * \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *         UTF8ERROR_TRUNCATED in case of truncation or
 *         UTF8ERROR_OUT_OF_RANGE in case of illegal range or
 *         UTF8ERROR_NULL_PARAM if that was NULL.
 */
extern utf8error_t utf8stringbuf_copy_region_from_str( utf8stringbuf_t this_, const char *that, int start, int length );
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

/*!
 *  \brief Replaces all occurrences of pattern by replacement.
 *  \note Performance-Rating: [ ]single-operation   [ ]fast   [ ]medium   [x]slow ;   Performance-Class: O(n*p+n*(n+r)), n:strlen, p:patternlen, r:replacelen
 *  \param this_ The string buffer within which to search
 *  \param pattern The 0-terminated string to search. Empty or NULL patterns are not replaced.
 *  \param replacement The 0-terminated string to replace the pattern with.
 *  \return UTF8ERROR_SUCCESS in case of success,
 *          UTF8ERROR_TRUNCATED if the string buffer was truncated or
 *          UTF8ERROR_NULL_PARAM if pattern is NULL.
 */
static inline utf8error_t utf8stringbuf_replace_all_str_by_str( const utf8stringbuf_t this_, const char *pattern, const char *replacement );

/*!
 *  \brief Replaces all occurrences of pattern by replacement.
 *  \note Performance-Rating: [ ]single-operation   [ ]fast   [ ]medium   [x]slow ;   Performance-Class: O(n*p+n*(n+r)), n:strlen, p:patternlen, r:replacelen
 *  \param this_ The string buffer within which to search
 *  \param pattern The string to search. Empty patterns are not replaced.
 *  \param replacement The string to replace the pattern with.
 *  \return UTF8ERROR_SUCCESS in case of success,
 *          UTF8ERROR_TRUNCATED if the string buffer was truncated.
 */
static inline utf8error_t utf8stringbuf_replace_all_buf_by_buf( const utf8stringbuf_t this_, const utf8stringbuf_t pattern, const utf8stringbuf_t replacement );

/*!
 *  \brief Replaces all occurrences of patterns by the corresponding replacement strings
 *
 *  This function is intended to escape character sequences.
 *  \n
 *  Examples:
 *  \code
 *      const char *const SQL_ENCODE[][2] = {
 *          { "\0", "\\0" },  //  within strings, null cannot be represented.
 *          { "\x09", "\\t" },
 *          { "\x0a", "\\n" },
 *          { "\x0d", "\\r" },
 *          { "\x0e", "\\b" },
 *          { "\x1a", "\\z" },
 *          { "\"", "\\\"" },
 *          { "'", "\\'" },
 *          { "\\", "\\\\" },
 *          { "%", "\\%" },  //  % replacement only needed in searches by LIKE operator
 *          { "_", "\\_" },  //  _ replacement only needed in searches by LIKE operator
 *          { NULL, NULL }
 *      };
 *      utf8stringbuf_replace_all( mySqlBuf, &SQL_ENCODE );
 *      const char *const XML_ENCODE[][2] = {
 *          { "<", "&lt;" },
 *          { ">", "&gt;" },
 *          { "&", "&amp;" },
 *          { "\"", "&quot;" },  //  " replacement only needed in attribute values
 *          { "'", "&apos;" },  //  ' replacement only needed in attribute values
 *          { NULL, NULL }
 *      };
 *      utf8stringbuf_replace_all( myXmlBuf, &XML_ENCODE );
 *  \endcode
 *  \see https://dev.mysql.com/doc/refman/5.6/en/string-literals.html
 *  \note Performance-Rating: [ ]single-operation   [ ]fast   [ ]medium   [x]slow ;   Performance-Class: O(n*p+n*(n+r)), n:strlen, p:sumOfPatternlen, r:maxOfReplacelen
 *  \param this_ The string buffer within which to search
 *  \param patterns_and_replacements The string array containing the byte-sequences to search and to replace,
 *                                   terminated by a NULL-pair. Empty patterns are not replaced.
 *                                   The string buffer this_ is processed from start to end;
 *                                   if multiple patterns match, the first pattern is replaced.
 *  \return UTF8ERROR_SUCCESS in case of success,
 *          UTF8ERROR_TRUNCATED if the string buffer was truncated or
 *          UTF8ERROR_NULL_PARAM if patterns_and_replacements is NULL.
 */
extern utf8error_t utf8stringbuf_replace_all( const utf8stringbuf_t this_, const char *const ((*patterns_and_replacements)[][2]) );

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
/*!
 * \brief Replaces a region within a string buffer
 *
 * If the resulting string does not fit into the destination buffer,
 * the destination string gets truncated. This function ensures, that
 * truncation does not split an utf8 code-point in half.
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n+r), n:strlen, r:replacelen
 * \param this_ The destination string buffer
 * \param start Start index within the string buffer, where to start the replacement. 0 is the index of the first byte.
 * \param length Length in bytes to be replaced. Provide 0 to only insert characters without deleting.
 * \param replacement The 0-terminated replacement string. The buffers of this_ and replacement must not overlap!
 *        Provide the empty string or NULL to simply delete a region within a string buffer.
 * \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *         UTF8ERROR_OUT_OF_RANGE in case of illegal ranges or UTF8ERROR_TRUNCATED in case of truncation.
 */
static inline utf8error_t utf8stringbuf_replace_region_by_str( utf8stringbuf_t this_, int start, int length, const char *replacement );
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
/*!
 * \brief Replaces a region within a string buffer
 *
 * If the resulting string does not fit into the destination buffer,
 * the destination string gets truncated. This function ensures, that
 * truncation does not split an utf8 code-point in half.
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n+r), n:strlen, r:replacelen
 * \param this_ The destination string buffer
 * \param start Start index within the string buffer, where to start the replacement. 0 is the index of the first byte.
 * \param length Length in bytes to be replaced. Provide 0 to only insert characters without deleting.
 * \param replacement The replacement string buffer. The buffers of this_ and replacement must not overlap!
 * \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *         UTF8ERROR_OUT_OF_RANGE in case of illegal ranges or UTF8ERROR_TRUNCATED in case of truncation.
 */
static inline utf8error_t utf8stringbuf_replace_region_by_buf( utf8stringbuf_t this_, int start, int length, const utf8stringbuf_t replacement );
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
/*!
 * \brief Deletes a region within a string buffer
 *
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ The string buffer
 * \param start Start index within the string buffer, where to start deleting. 0 is the index of the first byte.
 * \param length Length in bytes to be deleted.
 * \return UTF8ERROR_SUCCESS in case of success: All bytes have been deleted.
 *         UTF8ERROR_OUT_OF_RANGE in case of illegal ranges.
 */
static inline utf8error_t utf8stringbuf_delete( utf8stringbuf_t this_, int start, int length );
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
/*!
 * \brief Deletes a region at the end of a string buffer
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ The string buffer
 * \param length Length in bytes to be deleted.
 * \return UTF8ERROR_SUCCESS in case of success: All bytes have been deleted.
 *         UTF8ERROR_OUT_OF_RANGE in case of illegal ranges.
 */
static inline utf8error_t utf8stringbuf_delete_from_end( utf8stringbuf_t this_, int length );
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
/*!
 * \brief Truncates a string buffer
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param this_ The string buffer
 * \param start Start index within the string buffer, where to start deleting. 0 is the index of the first byte.
 * \return UTF8ERROR_SUCCESS in case of success: All bytes have been deleted.
 *         UTF8ERROR_OUT_OF_RANGE in case of illegal ranges.
 */
static inline utf8error_t utf8stringbuf_delete_to_end( utf8stringbuf_t this_, int start );
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
/*!
 * \brief Inserts a string to a string buffer
 *
 * If the resulting string does not fit into the buffer,
 * the string gets truncated. This function ensures, that
 * truncation does not split an utf8 code-point in half.
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n+i), n:strlen, i:insertlen
 * \param this_ The string buffer
 * \param start Start index within the string buffer, where to start the insertation. 0 is the index of the first byte.
 * \param insert The 0-terminated inseration string. The buffers of this_ and insert must not overlap!
 * \return UTF8ERROR_SUCCESS in case of success: All bytes have been inserted.
 *         UTF8ERROR_OUT_OF_RANGE in case of illegal ranges or UTF8ERROR_TRUNCATED in case of truncation.
 */
static inline utf8error_t utf8stringbuf_insert_str( utf8stringbuf_t this_, int start, const char *insert );
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
/*!
 * \brief Inserts a string buffer to a string buffer
 *
 * If the resulting string does not fit into the buffer,
 * the string gets truncated. This function ensures, that
 * truncation does not split an utf8 code-point in half.
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n+i), n:strlen, i:insertlen
 * \param this_ The string buffer
 * \param start Start index within the string buffer, where to start the insertation. 0 is the index of the first byte.
 * \param insert The inseration string buffer. The buffers of this_ and insert must not overlap!
 * \return UTF8ERROR_SUCCESS in case of success: All bytes have been inserted.
 *         UTF8ERROR_OUT_OF_RANGE in case of illegal ranges or UTF8ERROR_TRUNCATED in case of truncation.
 */
static inline utf8error_t utf8stringbuf_insert_buf( utf8stringbuf_t this_, int start, const utf8stringbuf_t insert );
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

/*!
 *  \brief Splits a string buffer into an ignored first part and the unfilled terminating part
 *
 *  This function may be useful when building an string using the append functions:
 *  Future append calls are faster because the utf8stringbuf is smaller.
 *  Also you may modify the last appended part by the replace functions without touching the first part.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ The string buffer
 *  \return an utf8stringbuf containing only the end
 */
static inline utf8stringbuf_t utf8stringbuf_get_end( utf8stringbuf_t this_ );

/*!
 *  \brief Appends a string to a string buffer
 *
 *  If the source string does not fit into the destination buffer,
 *  the copied string gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n+a), n:strlen, a:appendlen
 *  \param this_ The destination string buffer
 *  \param appendix The 0-terminated source string.
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation or
 *          UTF8ERROR_NULL_PARAM if appendix was NULL.
 */
static inline utf8error_t utf8stringbuf_append_str( utf8stringbuf_t this_, const char *appendix );

/*!
 *  \brief Appends a string buffer to a string buffer
 *
 *  If the source string does not fit into the destination buffer,
 *  the copied string gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n+a), n:strlen, a:appendlen
 *  \param this_ The destination string buffer
 *  \param appendix The source string buffer
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation.
 */
static inline utf8error_t utf8stringbuf_append_buf( utf8stringbuf_t this_, const utf8stringbuf_t appendix );

/*!
 *  \brief Appends a signed integer to a string buffer in decimal format
 *
 *  If the integer not fit to the destination buffer,
 *  the copied string gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ The destination string buffer
 *  \param appendix The integer to be appended
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation.
 */
static inline utf8error_t utf8stringbuf_append_int( utf8stringbuf_t this_, const int64_t appendix );

/*!
 *  \brief Appends an unsigned integer to a string buffer in hexadecimal format
 *
 *  If the integer not fit to the destination buffer,
 *  the copied string gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ The destination string buffer
 *  \param appendix The integer to be appended
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation.
 */
static inline utf8error_t utf8stringbuf_append_hex( utf8stringbuf_t this_, const uint64_t appendix );

/*!
 *  \brief Appends an unicode character to a string buffer
 *
 *  If the character does not fit to the destination buffer,
 *  the destinatoin buffer is not modified.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 *  \param this_ The destination string buffer
 *  \param appendix The unicode codepoint to be appended
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED if the character could not be appended.
 *          UTF8ERROR_NOT_A_CODEPOINT if appendix is greater than 0x7fffffff
 */
extern utf8error_t utf8stringbuf_append_char( utf8stringbuf_t this_, const uint32_t appendix );

/*!
 *  \brief Appends a string of wchar_t to a string buffer
 *
 *  If the source string does not fit into the destination buffer,
 *  the copied string gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *  \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n+a), n:strlen, a:appendlen
 *  \param this_ The destination string buffer
 *  \param appendix The 0-terminated source string.
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation or
 *          UTF8ERROR_NULL_PARAM if appendix was NULL or
 *          UTF8ERROR_NOT_A_CODEPOINT if one or more characters of appendix are greater than 0x7fffffff
 */
extern utf8error_t utf8stringbuf_append_wstr( utf8stringbuf_t this_, const wchar_t *appendix );

/*!
 *  \brief Appends a stringview to a string buffer
 *
 *  If the source string does not fit into the destination buffer,
 *  the copied string gets truncated. This function ensures, that
 *  truncation does not split an utf8 code-point in half.
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n+a), n:strlen, a:appendlen
 *  \param this_ The destination string buffer
 *  \param appendix pointer to the source stringview.
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED in case of truncation or
 *          UTF8ERROR_NULL_PARAM if appendix was NULL.
 */
static inline utf8error_t utf8stringbuf_append_view( utf8stringbuf_t this_, const utf8stringview_t *appendix );

#ifdef __cplusplus
}
#endif

#include "utf8stringbuf/utf8stringbuf.inl"

#endif /*UTF8STRINGBUF_H_*/


/*
 * Copyright 2012-2024 Andreas Warnke
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
