/* File: utf8stringviewiterator.h; Copyright and License: see below */

#ifndef UTF8STRINGVIEWITERATOR_H
#define UTF8STRINGVIEWITERATOR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Iterates over a character-separated list of a stringview
 */

#include "util/string/utf8string.h"
#include "util/string/utf8stringview.h"
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the character-separated iterator functions
 * 
 *  The iterator works similar to the J2SE-ListIterator, hibernate-query-Iterator and QT-QListIterator:
 *  while ( hasNext() ) { element = next() };
 */
struct utf8stringviewiterator_struct {
    bool has_next;  /*!< triue if there is a next stringview-element */
    utf8stringview_t next;  /*!< the next stringview-element */
    utf8stringview_t remaining;  /*!< remaining part of the element_list which is not yet processed */
    utf8string_t separator;  /*!< character-sequence that separates the stringviews */
};

typedef struct utf8stringviewiterator_struct utf8stringviewiterator_t;

/*!
 *  \brief initializes the utf8stringviewiterator_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param element_list character-separated list of stringviews
 *  \param separator character-sequence that separates the stringviews to retrieve; length must not be 0
 */
void utf8stringviewiterator_init ( utf8stringviewiterator_t *this_, utf8stringview_t element_list, utf8string_t separator );

/*!
 *  \brief destroys the utf8stringviewiterator_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void utf8stringviewiterator_destroy ( utf8stringviewiterator_t *this_ );

/*!
 *  \brief checks if a next stringview-element exists in the iterator - does not modify the iterator state
 * 
 *  Note that even an empty string contains one empty stringview-element
 *
 *  \param this_ pointer to own object attributes
 */
bool utf8stringviewiterator_has_next ( const utf8stringviewiterator_t *this_ );

/*!
 *  \brief reads the next stringview-element from the character-separated list of stringviews.
 * 
 *  Internally advances to the stringview-element after next.
 *
 *  \param this_ pointer to own object attributes
 *  \return the next stringview-element parsed from the character-separated list of stringviews, 
 *          in case there is no next stringview-element, utf8stringview_get_length() of the result is 0
 */
utf8stringview_t utf8stringviewiterator_next ( utf8stringviewiterator_t *this_ );

/*!
 *  \brief moves the iterator to the next stringview-element, updates (*this_).next and (*this_).has_next
 *
 *  \param this_ pointer to own object attributes
 */
void utf8stringviewiterator_private_step_to_next ( utf8stringviewiterator_t *this_ );

#include "utf8stringviewiterator.inl"

#endif  /* UTF8STRINGVIEWITERATOR_H */


/*
Copyright 2021-2021 Andreas Warnke

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
