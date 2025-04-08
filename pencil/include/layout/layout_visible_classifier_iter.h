/* File: layout_visible_classifier_iter.h; Copyright and License: see below */

#ifndef LAYOUT_VISIBLE_CLASSIFIER_ITER_H
#define LAYOUT_VISIBLE_CLASSIFIER_ITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Iterator over sorted arrays of visible classifiers.
 */

#include "layout/layout_visible_classifier.h"
#include "layout/layout_visible_set.h"
#include "u8list/universal_array_index_sorter.h"
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief attributes of the iterator
 */
struct layout_visible_classifier_iter_struct {
    uint32_t next_idx;  /*!< next index on the order index_sorter, not the index in items */
    uint32_t length;  /*!< maximum entries to iterate over in index_sorter */
    layout_visible_set_t *items;
    const universal_array_index_sorter_t *order;
};

typedef struct layout_visible_classifier_iter_struct layout_visible_classifier_iter_t;

/*!
 *  \brief initializes the layout_visible_classifier_iter_t
 *
 *  \param this_ pointer to own object attributes
 *  \param items array of items to iterate over
 *  \param order sorted list of indexes which determine the order
 */
static inline void layout_visible_classifier_iter_init( layout_visible_classifier_iter_t *this_,
                                                        layout_visible_set_t *items,
                                                        const universal_array_index_sorter_t *order
                                                      );

/*!
 *  \brief initializes the layout_visible_classifier_iter_t to re-process the already processed items
 *
 *  it ignores the not-yet-processed items and iterates only over the already processed items
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to layout_visible_classifier_iter_t iterator, of which only the processed elements shall be re-processed
 */
static inline void layout_visible_classifier_iter_init_from_processed( layout_visible_classifier_iter_t *this_,
                                                                       const layout_visible_classifier_iter_t *that_
                                                                     );

/*!
 *  \brief destroys the layout_visible_classifier_iter_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void layout_visible_classifier_iter_destroy( layout_visible_classifier_iter_t *this_ );

/*!
 *  \brief checks if there are more elements
 *
 *  has_next() does not modify the iterator; multiple calls to has_next() do not skip elements.
 *
 *  \param this_ pointer to own object attributes
 *  \return true if there is at least one more element.
 */
static inline bool layout_visible_classifier_iter_has_next( const layout_visible_classifier_iter_t *this_ );

/*!
 *  \brief gets the next layout_visible_classifier_t if there are more, NULL otherwise.
 *
 *  \param this_ pointer to own object attributes
 *  \return the next layout_visible_classifier_t
 */
static inline layout_visible_classifier_t *layout_visible_classifier_iter_next_ptr( layout_visible_classifier_iter_t *this_ );

/*!
 *  \brief gets the number of already processed elements
 *
 *  \param this_ pointer to own object attributes
 *  \return number of successful layout_visible_classifier_iter_next_ptr() calls.
 */
static inline uint32_t layout_visible_classifier_iter_count_processed( const layout_visible_classifier_iter_t *this_ );

#include "layout_visible_classifier_iter.inl"

#endif  /* LAYOUT_VISIBLE_CLASSIFIER_ITER_H */


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
