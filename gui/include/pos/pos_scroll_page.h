/* File: pos_scroll_page.h; Copyright and License: see below */

#ifndef POS_SCROLL_PAGE_H
#define POS_SCROLL_PAGE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines the visible window on a list
 *
 *  This data struct defines a request to show a window
 *  without knowing the list size or the window size.
 */

#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief parent and list order of gap between diagrams
 */
struct pos_scroll_page_struct {
    uint32_t anchor_index;  /*!< An index within the list that shall be shown */
    bool backwards;  /*!< A direction starting at anchor_index where the list items shall be shown */
};

typedef struct pos_scroll_page_struct pos_scroll_page_t;

/*!
 *  \brief initializes a const pos_scroll_page_t struct with index and diretion.
 *
 *  \param anchor_index An index within the list that shall be shown
 *  \param backwards A direction starting at anchor_index where the list items shall be shown
 *  \return an instance of a newly initilized pos_scroll_page_t
 */
static inline const pos_scroll_page_t pos_scroll_page_new ( uint32_t anchor_index,
                                                            bool backwards
                                                          );

/*!
 *  \brief gets an index within the list that shall be shown
 *
 *  \param this_ pointer to own object attributes
 *  \return the anchor_index.
 */
static inline uint32_t pos_scroll_page_get_anchor_index ( const pos_scroll_page_t *this_ );

/*!
 *  \brief gets the page direction starting at anchor_index
 *
 *  \param this_ pointer to own object attributes
 *  \return true if backwards
 */
static inline bool pos_scroll_page_get_backwards ( const pos_scroll_page_t *this_ );

/*!
 *  \brief prints the pos_scroll_page_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pos_scroll_page_trace ( const pos_scroll_page_t *this_ );

#include "pos_scroll_page.inl"

#endif  /* POS_SCROLL_PAGE_H */


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
