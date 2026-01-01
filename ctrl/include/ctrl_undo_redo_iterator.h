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
 *  \brief all data attributes needed for ctrl_undo_redo_entry_t iterator functions
 *
 *  The iterator works similar to the J2SE-ListIterator, hibernate-query-Iterator and QT-QListIterator:
 *  while ( has_next() ) { element = next() };
 */
struct ctrl_undo_redo_iterator_struct {
    const ctrl_undo_redo_entry_t (*ring_buf)[];  /*!< the ring buffer address */
    uint32_t ring_buf_size;  /*!< the ring buffer size, unit is array-element */
    bool iterate_upwards;  /*!< true if next is (current+1)%ring_buf_size, false if downwards direction */
    uint32_t current;  /*!< index of current element, range 0..(ring_buf_size-1) */
    uint32_t length;  /*!< remaining number of elements to iterate over, range 0..ring_buf_size */
};

typedef struct ctrl_undo_redo_iterator_struct ctrl_undo_redo_iterator_t;

/*!
 *  \brief initializes the ctrl_undo_redo_iterator_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param ring_buf the ring buffer address
 *  \param ring_buf_size the ring buffer size, unit is array-element
 *  \param iterate_upwards true if next is (current+1)%ring_buf_size, false if downwards direction
 *  \param current index of current element, range 0..(ring_buf_size-1)
 *  \param length remaining number of elements to iterate over, range 0..ring_buf_size
 */
static inline void ctrl_undo_redo_iterator_init ( ctrl_undo_redo_iterator_t *this_,
                                                  const ctrl_undo_redo_entry_t (*ring_buf)[],
                                                  uint32_t ring_buf_size,
                                                  bool iterate_upwards,
                                                  uint32_t current,
                                                  uint32_t length
                                                );

/*!
 *  \brief re-initializes the ctrl_undo_redo_iterator_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param ring_buf the ring buffer address
 *  \param ring_buf_size the ring buffer size, unit is array-element
 *  \param iterate_upwards true if next is (current+1)%ring_buf_size, false if downwards direction
 *  \param current index of current element, range 0..(ring_buf_size-1)
 *  \param length remaining number of elements to iterate over, range 0..ring_buf_size
 */
static inline void ctrl_undo_redo_iterator_reinit ( ctrl_undo_redo_iterator_t *this_,
                                                    const ctrl_undo_redo_entry_t (*ring_buf)[],
                                                    uint32_t ring_buf_size,
                                                    bool iterate_upwards,
                                                    uint32_t current,
                                                    uint32_t length
                                                  );

/*!
 *  \brief initializes the ctrl_undo_redo_iterator_t struct to an empty iterator
 *
 *  \param this_ pointer to own object attributes
 */
static inline void ctrl_undo_redo_iterator_init_empty ( ctrl_undo_redo_iterator_t *this_ );

/*!
 *  \brief destroys the ctrl_undo_redo_iterator_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void ctrl_undo_redo_iterator_destroy ( ctrl_undo_redo_iterator_t *this_ );

/*!
 *  \brief checks if a next ctrl_undo_redo_entry_t exists in the iterator - does not modify the iterator state
 *
 *  \param this_ pointer to own object attributes
 *  \return true if there is a next element in the iterator
 */
static inline bool ctrl_undo_redo_iterator_has_next ( const ctrl_undo_redo_iterator_t *this_ );

/*!
 *  \brief reads the next ctrl_undo_redo_entry_t from the iterator.
 *
 *  \param this_ pointer to own object attributes
 *  \return the next ctrl_undo_redo_entry_t from the iterator
 */
static inline const ctrl_undo_redo_entry_t * ctrl_undo_redo_iterator_next ( ctrl_undo_redo_iterator_t *this_ );

/*!
 *  \brief iterates through the iterator and collects statistics
 *
 *  This function finds types DATA_STAT_SERIES_CREATED, DATA_STAT_SERIES_MODIFIED, DATA_STAT_SERIES_DELETED.
 *  When finished, no more elements are left in the iterator.
 *
 *  \param this_ pointer to own object attributes
 *  \param undo true if the action shall be counted as undone, false if it shall be counted as redone
 *  \param[in,out] io_stat *io_stat shall be initialized by caller, statistics are added to initial values
 */
static inline void ctrl_undo_redo_iterator_collect_statistics ( ctrl_undo_redo_iterator_t *this_,
                                                                bool undo,
                                                                data_stat_t *io_stat
                                                              );

#include "ctrl_undo_redo_iterator.inl"

#endif  /* CTRL_UNDO_REDO_ITERATOR_H */


/*
Copyright 2024-2026 Andreas Warnke

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
