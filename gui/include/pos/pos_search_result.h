/* File: pos_search_result.h; Copyright and License: see below */

#ifndef POS_SEARCH_RESULT_H
#define POS_SEARCH_RESULT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines location of a search result
 */

#include "shape/shape_int_rectangle.h"
#include "set/data_search_result.h"

/*!
 *  \brief location of a search result
 */
struct pos_search_result_struct {
    shape_int_rectangle_t icon_box;  /*!< The rectangle where the icon of the search result is drawn */
    shape_int_rectangle_t label_box;  /*!< bounding box of the label of the search result. */
    const data_search_result_t *data;  /*!< pointer to the data object of the search result */
};

typedef struct pos_search_result_struct pos_search_result_t;

/*!
 *  \brief initializes the pos_search_result_t struct with a pointer to a search result.
 *
 *  \param this_ pointer to own object attributes
 *  \param search_result_data pointer to data object which is layouted
 */
static inline void pos_search_result_init ( pos_search_result_t *this_,
                                            const data_search_result_t *search_result_data
                                          );

/*!
 *  \brief destroys the pos_search_result_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pos_search_result_destroy ( pos_search_result_t *this_ );

/*!
 *  \brief checks if the search result data is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return true if (*this_).data points to valid data
 */
static inline bool pos_search_result_is_valid ( const pos_search_result_t *this_ );

/*!
 *  \brief gets the bounding box of the search result
 *
 *  \param this_ pointer to own object attributes
 *  \return const-pointer to shape_int_rectangle_t.
 */
static inline const shape_int_rectangle_t *pos_search_result_get_icon_box_const ( const pos_search_result_t *this_ );

/*!
 *  \brief sets the icon_box of the search result
 *
 *  \param this_ pointer to own object attributes
 *  \param icon_box coordinates of new icon_box
 */
static inline void pos_search_result_set_icon_box ( pos_search_result_t *this_, const shape_int_rectangle_t *icon_box );

/*!
 *  \brief gets the label bounds of the search result
 *
 *  \param this_ pointer to own object attributes
 *  \return const-pointer to shape_int_rectangle_t.
 */
static inline const shape_int_rectangle_t *pos_search_result_get_label_box_const ( const pos_search_result_t *this_ );

/*!
 *  \brief sets the label bounds of the search result
 *
 *  \param this_ pointer to own object attributes
 *  \param label_box coordinates of new label bounds
 */
static inline void pos_search_result_set_label_box ( pos_search_result_t *this_, const shape_int_rectangle_t *label_box );

/*!
 *  \brief gets the data_feature_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to the corresponding data_feature_t object
 */
static inline const data_search_result_t *pos_search_result_get_data_const ( const pos_search_result_t *this_ );

/*!
 *  \brief gets the row id of the data_search_result
 *
 *  This is a shortcut for data_feature_get_row_id( pos_search_result_get_data_const ( X ) )
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the data_search_result
 */
static inline data_id_t pos_search_result_get_data_id ( const pos_search_result_t *this_ );

#include "pos_search_result.inl"

#endif  /* POS_SEARCH_RESULT_H */


/*
Copyright 2018-2025 Andreas Warnke

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
