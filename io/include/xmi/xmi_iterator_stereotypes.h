/* File: xmi_iterator_stereotypes.h; Copyright and License: see below */

#ifndef XMI_ITERATOR_STEREOTYPES_H
#define XMI_ITERATOR_STEREOTYPES_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Iterates over a comma-separated list of stereotypes
 */

#include "util/string/utf8string.h"
#include "util/string/utf8stringview.h"
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the stereotype iterator functions
 * 
 *  The iterator works similar to the J2SE-ListIterator, hibernate-query-Iterator and QT-QListIterator:
 *  while ( hasNext() ) { element = next() };
 */
struct xmi_iterator_stereotypes_struct {
    utf8stringview_t next;  /*!< the next stringview, if utf8stringview_get_length()==0, there is no next stereotype */
    utf8string_t remaining;  /*!< pointer to external database. Currently unused, but in future, e.g. is_open could be queried */
};

typedef struct xmi_iterator_stereotypes_struct xmi_iterator_stereotypes_t;

/*!
 *  \brief initializes the xmi_iterator_stereotypes_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param stereotype_list comma-separated list of stereotypes
 */
void xmi_iterator_stereotypes_init ( xmi_iterator_stereotypes_t *this_, utf8string_t stereotype_list );

/*!
 *  \brief destroys the xmi_iterator_stereotypes_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void xmi_iterator_stereotypes_destroy ( xmi_iterator_stereotypes_t *this_ );

/*!
 *  \brief checks if a next element exists in the iterator - does not modify the iterator state
 *
 *  \param this_ pointer to own object attributes
 */
bool xmi_iterator_stereotypes_has_next ( const xmi_iterator_stereotypes_t *this_ );

/*!
 *  \brief reads the next classifier from the comma-separated list of stereotypes (and internally advances to the classifier after).
 *
 *  \param this_ pointer to own object attributes
 *  \return the next stereotype parsed from the comma-separated list of stereotypes, 
 *          in case there is no next stereotype, utf8stringview_get_length() of the result is 0
 */
utf8stringview_t xmi_iterator_stereotypes_next ( xmi_iterator_stereotypes_t *this_ );

/*!
 *  \brief moves the iterator to the next element, updates (*this_).next
 *
 *  \param this_ pointer to own object attributes
 */
void xmi_iterator_stereotypes_private_step_to_next ( xmi_iterator_stereotypes_t *this_ );

#include "xmi_iterator_stereotypes.inl"

#endif  /* XMI_ITERATOR_STEREOTYPES_H */


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
