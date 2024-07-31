/* File: ctrl_undo_redo_iterator.h; Copyright and License: see below */

#ifndef CTRL_UNDO_REDO_ITERATOR_H
#define CTRL_UNDO_REDO_ITERATOR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Iterates over a set of ctrl_undo_redo_entry_t of that are stored in a ring buffer
 */

#include "ctrl_undo_redo_entry.h"
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the code point iterator functions
 *
 *  The iterator works similar to the J2SE-ListIterator, hibernate-query-Iterator and QT-QListIterator:
 *  while ( hasNext() ) { element = next() };
 */
struct ctrl_undo_redo_iterator_struct {
    const ctrl_undo_redo_entry_t *next;  /*!< the next codepoint-element */
};

typedef struct ctrl_undo_redo_iterator_struct ctrl_undo_redo_iterator_t;

/*!
 *  \brief initializes the ctrl_undo_redo_iterator_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param next next ctrl_undo_redo_entry_t to return by the iterator
 */
static inline void ctrl_undo_redo_iterator_init ( ctrl_undo_redo_iterator_t *this_, const ctrl_undo_redo_entry_t *next );

/*!
 *  \brief destroys the ctrl_undo_redo_iterator_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void ctrl_undo_redo_iterator_destroy ( ctrl_undo_redo_iterator_t *this_ );

/*!
 *  \brief checks if a next code point-element exists in the iterator - does not modify the iterator state
 *
 *  \param this_ pointer to own object attributes
 *  \return true if there is a next element in the iterator
 */
static inline bool ctrl_undo_redo_iterator_has_next ( const ctrl_undo_redo_iterator_t *this_ );

/*!
 *  \brief reads the next code point-element from the stringview.
 *
 *  \param this_ pointer to own object attributes
 *  \return the next code point-element parsed from the stringview;
 *          in case there is no next code point-element, utf8codepoint_is_valid() of the result is false
 */
static inline const ctrl_undo_redo_entry_t * ctrl_undo_redo_iterator_next ( ctrl_undo_redo_iterator_t *this_ );

/*!
 *  \brief moves the iterator to the next code point-element, updates (*this_).next
 *
 *  \param this_ pointer to own object attributes
 */
static inline void ctrl_undo_redo_iterator_private_step_to_next ( ctrl_undo_redo_iterator_t *this_ );

#include "ctrl_undo_redo_iterator.inl"

#endif  /* CTRL_UNDO_REDO_ITERATOR_H */


/*
Copyright 2024-2024 Andreas Warnke

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
