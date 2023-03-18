/* File: utf8stringviewtokenizer.h; Copyright and License: see below */

#ifndef UTF8STRINGVIEWTOKENIZER_H
#define UTF8STRINGVIEWTOKENIZER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Iterates over a character-separated list of a stringview
 */

#include "utf8stringbuf/utf8string.h"
#include "utf8stringbuf/utf8stringview.h"
#include "utf8stringbuf/utf8stringviewtokenmode.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \brief defines all data attributes needed for the token iterator functions
 *
 *  Usage example:
 *  while ( hasNext() ) { element = next() };
 */
struct utf8stringviewtokenizer_struct {
    utf8stringview_t remaining_input_text;  /*!< remaining part of the input_text which is not yet processed */
    utf8stringviewtokenmode_t mode;  /*!< mode defines how tokens are separated */
    uint32_t last_token_line;  /*!< line of last token */
    uint32_t current_line;  /*!< current line */
};

typedef struct utf8stringviewtokenizer_struct utf8stringviewtokenizer_t;

/*!
 *  \brief initializes the utf8stringviewtokenizer_t struct
 *
 *  \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n), n:remaining_input_text_len
 *  \param this_ pointer to own object attributes
 *  \param input_text the input text to be tokenized
 *  \param mode defines how tokens are separated
 */
static inline void utf8stringviewtokenizer_init ( utf8stringviewtokenizer_t *this_,
                                                  utf8stringview_t input_text,
                                                  utf8stringviewtokenmode_t mode );

/*!
 *  \brief destroys the utf8stringviewtokenizer_t struct
 *
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ pointer to own object attributes
 */
static inline void utf8stringviewtokenizer_destroy ( utf8stringviewtokenizer_t *this_ );

/*!
 *  \brief checks if a next stringview-token exists in the iterator
 *
 *  Note that an empty string is not a token
 *
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ pointer to own object attributes
 *  \return true if there is a next element in the iterator
 */
static inline bool utf8stringviewtokenizer_has_next ( const utf8stringviewtokenizer_t *this_ );

/*!
 *  \brief reads the next stringview-element from the character-separated list of stringviews.
 *
 *  Internally advances to the remaining_input_text to the next token.
 *
 *  \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n), n:remaining_input_text_len
 *  \param this_ pointer to own object attributes
 *  \return the next token parsed from the remaining_input_text,
 *          in case there is no next stringview-element, utf8stringview_get_length() of the result is 0
 */
static inline utf8stringview_t utf8stringviewtokenizer_next ( utf8stringviewtokenizer_t *this_ );

/*!
 *  \brief gets the line number of the last read token
 *
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ pointer to own object attributes
 *  \return line nubmer, the first line is 1, 0 if no token was read yet
 */
static inline uint32_t utf8stringviewtokenizer_get_line ( const utf8stringviewtokenizer_t *this_ );

/*!
 *  \brief sets the token mode
 *
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ pointer to own object attributes
 *  \param mode defines how tokens are separated
 */
static inline void utf8stringviewtokenizer_set_mode ( utf8stringviewtokenizer_t *this_, utf8stringviewtokenmode_t mode );

/*!
 *  \brief checks if the provideed ascii code point is a space or a control character, this includes 0x7f
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ pointer to own object attributes
 *  \param ascii an ascii character, which is an utf8 character in range 0x00..0x7f
 *  \return true if the character is a space and therefor not (part of) a token.
 */
static inline bool utf8stringviewtokenizer_private_is_space( utf8stringviewtokenizer_t *this_, char ascii );

/*!
 *  \brief checks if the provideed ascii code point is a standalone token, see utf8stringviewtokenmode_enum
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ pointer to own object attributes
 *  \param ascii an ascii character, which is an utf8 character in range 0x00..0x7f
 *  \return true if the character is a standalone token like '/' which neither part of the preceding nor of the following token.
 */
static inline bool utf8stringviewtokenizer_private_is_standalone( utf8stringviewtokenizer_t *this_, char ascii );

/*!
 *  \brief checks if the next token is a number and returns its length
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:remaining_input_text_len
 *  \param this_ pointer to own object attributes
 *  \return length of the integer or floatingpoint number, 0 if there is no number
 */
static inline size_t utf8stringviewtokenizer_private_get_number_len( utf8stringviewtokenizer_t *this_ );

/*!
 *  \brief moves the iterator to the next token start, updates (*this_).remaining_input_text
 *
 *  \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n), n:remaining_input_text_len
 *  \param this_ pointer to own object attributes
 */
static inline void utf8stringviewtokenizer_private_skip_space ( utf8stringviewtokenizer_t *this_ );

#ifdef __cplusplus
}
#endif

#include "utf8stringviewtokenizer.inl"

#endif  /* UTF8STRINGVIEWTOKENIZER_H */


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
