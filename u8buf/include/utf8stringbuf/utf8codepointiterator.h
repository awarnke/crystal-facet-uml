/* File: utf8codepointiterator.h; Copyright and License: see below */

#ifndef UTF8CODEPOINTITERATOR_H
#define UTF8CODEPOINTITERATOR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Iterates over codepoints of e.g. a stringview
 */

#include "utf8stringbuf/utf8codepoint.h"
#include "utf8stringbuf/utf8string.h"
#include "utf8stringbuf/utf8stringview.h"
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the code point iterator functions
 * 
 *  The iterator works similar to the J2SE-ListIterator, hibernate-query-Iterator and QT-QListIterator:
 *  while ( hasNext() ) { element = next() };
 */
struct utf8codepointiterator_struct {
    utf8codepoint_t next;  /*!< the next codepoint-element */
    utf8stringview_t remaining;  /*!< remaining part of the element_list which is not yet processed */
};

typedef struct utf8codepointiterator_struct utf8codepointiterator_t;

/*!
 *  \brief initializes the utf8codepointiterator_t struct
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ pointer to own object attributes
 *  \param string list of code points
 */
static inline void utf8codepointiterator_init ( utf8codepointiterator_t *this_, utf8stringview_t string );

/*!
 *  \brief destroys the utf8codepointiterator_t struct
 *
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ pointer to own object attributes
 */
static inline void utf8codepointiterator_destroy ( utf8codepointiterator_t *this_ );

/*!
 *  \brief checks if a next code point-element exists in the iterator - does not modify the iterator state
 * 
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ pointer to own object attributes
 *  \return true if there is a next element in the iterator
 */
static inline bool utf8codepointiterator_has_next ( const utf8codepointiterator_t *this_ );

/*!
 *  \brief reads the next code point-element from the stringview.
 * 
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ pointer to own object attributes
 *  \return the next code point-element parsed from the stringview;
 *          in case there is no next code point-element, utf8codepoint_is_valid() of the result is false
 */
static inline utf8codepoint_t utf8codepointiterator_next ( utf8codepointiterator_t *this_ );

/*!
 *  \brief moves the iterator to the next code point-element, updates (*this_).next
 *
 *  \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ pointer to own object attributes
 */
static inline void utf8codepointiterator_private_step_to_next ( utf8codepointiterator_t *this_ );

#include "utf8codepointiterator.inl"

#endif  /* UTF8CODEPOINTITERATOR_H */


/*
Copyright 2021-2022 Andreas Warnke

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
