#ifndef UTF8CODEPOINT_H_
#define UTF8CODEPOINT_H_

/*!
 *  \file utf8codepoint.h
 *  \brief utf8codepoint is a struct of utf8-byte-length and the code-point of a character.
 * 
 *  You typically do not create a code point object yourself.
 *  It is simply the return value of \link utf8stringbuf_get_char_at(utf8stringbuf_t,unsigned int) utf8stringbuf_get_char_at \endlink
 * 
 *  \note License: Use this code according to the license: Apache 2.0.
 *  \author (c) 2012-2018 A.Warnke; Email-contact: utf8stringbuf-at-andreaswarnke-dot-de
 */ 

#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
    
/*! 
 *  \brief A character is a codepoint and a utf8-byte-count. 
 * 
 *  Most code points are represented by a positive length and a positive code point.
 *  The 0-termination of strings is represented as byte_length 1 and code_point 0.
 *  A byte_length of 0 indcates an invalid code_point.
 */
struct utf8codepoint_struct {
    unsigned int byte_length;
    uint32_t code_point;
};

/*!
 *  \typedef utf8codepoint_t
 *  \brief An utf-8 code point object
 */
typedef struct utf8codepoint_struct utf8codepoint_t;

/*!
 *  \brief utf8codepoint returns a code point struct 
 */
static inline utf8codepoint_t utf8codepoint( uint32_t code_point );

/*!
 * \brief Creates a utf8codepoint_t struct from a pointer to an utf-8 character
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)   
 * \param that Pointer to an utf-8 character or NULL. 
 * \param max_size maximum number of bytes to read. 
 * \return A utf8codepoint_t struct. Even if that was NULL.
 */
static inline utf8codepoint_t utf8codepoint_init( const char *that, unsigned int max_size );

/*!
 * \brief Gets the unicode code point
 * \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)   
 * \param this_ The code point object
 * \return Unicode code point
 */
static inline uint32_t utf8codepoint_get_char( const utf8codepoint_t this_ );

/*!
 * \brief Gets the length of the utf8 encoded character in bytes. 
 * \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)   
 * \param this_ The code point object
 * \return length of the utf8 encoded character in bytes. 0 if the code point is invalid.
 */
static inline unsigned int utf8codepoint_get_length( const utf8codepoint_t this_ );

/*!
 * \brief Determines if this_ object is a valid unicode codepoint. 
 * 
 * Unicode defines code points in the range from 0x000000 to 0x10ffff. 
 * Even within this range, the following integers are illegal:
 * 0xd800,..,0xdfff, 0xfdd0,..,0xfdef,
 * 0xfffe,0xffff,0x1fffe,0x1ffff,0x2fffe,0x2ffff,...,0x10fffe,0x10ffff.
 * \n
 * The 0x0 character which terminates strings is a valid unicode character.
 * 
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)   
 * \param this_ The code point object
 * \return 1 if this object is valid and encodes a valid unicode code point, 0 otherwise.
 */
static inline int utf8codepoint_is_unicode( const utf8codepoint_t this_ );

/*!
 * \brief Determines if this_ object is valid. 
 * 
 * Utf8 can encode any integer from 0x00000000 to 0x7fffffff. For all these integers,
 * this function will return 1. Only illegal byte sequences in the input byte array will
 * result in 0. 
 * \n
 * Unicode only defines code points from 0x000000 to 0x10ffff. And even within this range,
 * some code points are invalid. Call
 * \link utf8codepoint_is_unicode(const utf8codepoint_t this_) utf8codepoint_is_unicode \endlink
 * to check if the code point encodes a valid unicode character.
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)   
 * \param this_ The code point object
 * \return 1 if this object is valid, 0 otherwise.
 */
static inline int utf8codepoint_is_valid( const utf8codepoint_t this_ );

extern const utf8codepoint_t UTF8CODEPOINT_INVAL_CHAR;

#ifdef __cplusplus
}
#endif

#include "util/string/utf8codepoint.inl"

#endif /*UTF8CODEPOINT_H_*/
