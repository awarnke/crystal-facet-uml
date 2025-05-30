/* File: utf8stringlines.h; Copyright and License: see below */

#ifndef UTF8STRINGLINES_H
#define UTF8STRINGLINES_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Iterates over the lines of a stringview
 */

#include "utf8stringbuf/utf8string.h"
#include "utf8stringbuf/utf8stringview.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \brief all data attributes needed for the character-separated iterator functions
 *
 *  The iterator works similar to the J2SE-ListIterator, hibernate-query-Iterator and QT-QListIterator:
 *  while ( hasNext() ) { element = next() };
 */
struct utf8stringlines_struct {
    bool has_next;  /*!< true if there is a next stringview-element, even in case of a possibly empty end */
    bool next_is_end;  /*!< true if next stringview-element is the last one; needed to distinguish empty next elements from the end */
    utf8stringview_t next;  /*!< the next stringview-element */
    utf8stringview_t remaining;  /*!< remaining part of the element_list which is not yet processed */
    uint32_t line_length;  /*!< proposal for a line length of the result stringviews */
};

typedef struct utf8stringlines_struct utf8stringlines_t;

/*!
 *  \brief initializes the utf8stringlines_t struct
 *
 *  \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n), n:lines_list length
 *  \param this_ pointer to own object attributes
 *  \param lines_list list of lines as stringview
 *  \param line_length proposed line length in code points. The retrieved lines may be shorter or longer.
 */
static inline void utf8stringlines_init ( utf8stringlines_t *this_, const utf8stringview_t *lines_list, uint32_t line_length );

/*!
 *  \brief destroys the utf8stringlines_t struct
 *
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ pointer to own object attributes
 */
static inline void utf8stringlines_destroy ( utf8stringlines_t *this_ );

/*!
 *  \brief checks if a next stringview-element exists in the iterator - does not modify the iterator state
 *
 *  Note that even an empty string contains one empty stringview-element
 *
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 *  \param this_ pointer to own object attributes
 *  \return true if there is a next element in the iterator
 */
static inline bool utf8stringlines_has_next ( const utf8stringlines_t *this_ );

/*!
 *  \brief reads the next stringview line from the list of lines.
 *
 *  Internally advances to the stringview-line after next.
 *
 *  \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n), n:lines_list length
 *  \param this_ pointer to own object attributes
 *  \return the next stringview-element parsed from the character-separated list of stringviews,
 *          in case there is no next stringview-element, utf8stringview_get_length() of the result is 0
 */
static inline utf8stringview_t utf8stringlines_next ( utf8stringlines_t *this_ );

/*!
 *  \brief moves the iterator to the next stringview line, updates (*this_).next and (*this_).has_next
 *
 *  \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n), n:lines_list length
 *  \param this_ pointer to own object attributes
 */
static inline void utf8stringlines_private_step_to_next ( utf8stringlines_t *this_ );

#ifdef __cplusplus
}
#endif

#include "utf8stringlines.inl"

#endif  /* UTF8STRINGLINES_H */


/*
Copyright 2025-2025 Andreas Warnke

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
