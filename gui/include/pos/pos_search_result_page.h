/* File: pos_search_result_page.h; Copyright and License: see below */

#ifndef POS_SEARCH_RESULT_PAGE_H
#define POS_SEARCH_RESULT_PAGE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines location of a search result
 */

#include "pos/pos_search_result.h"
#include "shape/shape_int_rectangle.h"
#include "set/data_search_result.h"

/*!
 *  \brief constants for maximum values of gui_sketch_result_list_t
 */
enum pos_search_result_page_max_enum {
    POS_SEARCH_RESULT_PAGE_MAX_PAGE_SIZE = 100,  /*!< maximum number of search results on one page */
};

/*!
 *  \brief location of a search result
 */
struct pos_search_result_page_struct {
    /* search result data */
    data_search_result_t result_list_buf[POS_SEARCH_RESULT_PAGE_MAX_PAGE_SIZE];  /*!< list of search results on one page */

    /* search result layout information */
    pos_search_result_t element_pos[POS_SEARCH_RESULT_PAGE_MAX_PAGE_SIZE];  /*!< layout positions of search results on current visible page */

    /* data buffer information */
    uint32_t buffer_start;  /*!< offset of start element in result_list_buf and element_pos list (relative to absolute start of result list) */
    uint32_t buffer_length;  /*!< number of elements in result_list_buf and element_pos list */
    bool buffer_more_after;  /*!< This flag indicates if more results follow after the result_buffer. */

    /* visible page information */
    uint32_t page_start;  /*!< offset of first visible element (relative to absolute start of result list) */
    uint32_t page_length;  /*!< number of visible elements */

    bool has_prev_page;  /*!< indicates if the prev page button is displayed ( equals page_start != 0 )*/
    shape_int_rectangle_t button_prev_box;  /*!< The rectangle where the icon of the search result is drawn */
    bool has_next_page;  /*!< indicates if the next page button is displayed */
    shape_int_rectangle_t button_next_box;  /*!< bounding box of the label of the search result. */
};

typedef struct pos_search_result_page_struct pos_search_result_page_t;

/*!
 *  \brief initializes the pos_search_result_page_t struct to empty.
 *
 *  \param this_ pointer to own object attributes
 *  \param buffer_start offset of start element in result_list_buf and element_pos list (relative to absolute start of result list)
 */
static inline void pos_search_result_page_init ( pos_search_result_page_t *this_, uint32_t buffer_start );

/*!
 *  \brief re-initializes the pos_search_result_page_t struct to empty.
 *
 *  \param this_ pointer to own object attributes
 *  \param buffer_start offset of start element in result_list_buf and element_pos list (relative to absolute start of result list)
 */
static inline void pos_search_result_page_reinit ( pos_search_result_page_t *this_, uint32_t buffer_start );

/*!
 *  \brief destroys the pos_search_result_page_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pos_search_result_page_destroy ( pos_search_result_page_t *this_ );

/*!
 *  \brief gets the layout object of type pos_search_result_t
 *
 *  \param this_ pointer to own object attributes
 *  \param index offset of the search_result relative to the absolute start of the result list
 *  \return a pointer to the corresponding data_feature_t object
 */
static inline const pos_search_result_t *pos_search_result_page_get_search_result_layout_const ( const pos_search_result_page_t *this_, uint32_t index );

/*!
 *  \brief gets the layout object of type pos_search_result_t
 *
 *  \param this_ pointer to own object attributes
 *  \param index offset of the search_result relative to the absolute start of the result list
 *  \return a pointer to the corresponding data_feature_t object
 */
static inline pos_search_result_t *pos_search_result_page_get_search_result_layout_ptr ( pos_search_result_page_t *this_, uint32_t index );

/*!
 *  \brief appends a search_result to the page
 *
 *  \param this_ pointer to own object attributes
 *  \param search_result another search result to be shown on the current page:
 *                       this is copied to result_list_buf[] and an element_pos[] is initialized.
 *  \return U8_ERROR_NONE if success; U8_ERROR_ARRAY_BUFFER_EXCEEDED if buffer full.
 */
static inline u8_error_t pos_search_result_page_add_search_result ( pos_search_result_page_t *this_, const data_search_result_t *search_result );

/*!
 *  \brief gets the buffer_start
 *
 *  \param this_ pointer to own object attributes
 *  \return attribute buffer_start
 */
static inline uint32_t pos_search_result_page_get_buffer_start ( const pos_search_result_page_t *this_ );

/*!
 *  \brief gets the buffer_length
 *
 *  \param this_ pointer to own object attributes
 *  \return attribute buffer_length
 */
static inline uint32_t pos_search_result_page_get_buffer_length ( const pos_search_result_page_t *this_ );

/*!
 *  \brief gets the buffer_more_after
 *
 *  \param this_ pointer to own object attributes
 *  \return attribute buffer_more_after
 */
static inline bool pos_search_result_page_get_buffer_more_after ( const pos_search_result_page_t *this_ );

/*!
 *  \brief gets the buffer_more_after
 *
 *  \param this_ pointer to own object attributes
 *  \param buffer_more_after new value of attribute buffer_more_after
 */
static inline void pos_search_result_page_set_buffer_more_after ( pos_search_result_page_t *this_, bool buffer_more_after );

/*!
 *  \brief gets the page_start
 *
 *  \param this_ pointer to own object attributes
 *  \return attribute page_start
 */
static inline uint32_t pos_search_result_page_get_page_start ( const pos_search_result_page_t *this_ );

/*!
 *  \brief sets the page_start
 *
 *  \param this_ pointer to own object attributes
 *  \param page_start new value of attribute page_start
 */
static inline void pos_search_result_page_set_page_start ( pos_search_result_page_t *this_, uint32_t page_start );

/*!
 *  \brief gets the page_length
 *
 *  \param this_ pointer to own object attributes
 *  \return attribute page_length
 */
static inline uint32_t pos_search_result_page_get_page_length ( const pos_search_result_page_t *this_ );

/*!
 *  \brief sets the page_length
 *
 *  \param this_ pointer to own object attributes
 *  \param page_length new value of attribute page_length
 */
static inline void pos_search_result_page_set_page_length ( pos_search_result_page_t *this_, uint32_t page_length );

/*!
 *  \brief gets the has_prev_page
 *
 *  \param this_ pointer to own object attributes
 *  \return attribute has_prev_page
 */
static inline bool pos_search_result_page_has_prev_page ( const pos_search_result_page_t *this_ );

/*!
 *  \brief sets the has_prev_page
 *
 *  \param this_ pointer to own object attributes
 *  \param has_prev_page new value of attribute has_prev_page
 */
static inline void pos_search_result_page_set_has_prev_page ( pos_search_result_page_t *this_, bool has_prev_page );

/*!
 *  \brief gets the button_prev_box
 *
 *  \param this_ pointer to own object attributes
 *  \return const-pointer to shape_int_rectangle_t.
 */
static inline const shape_int_rectangle_t *pos_search_result_page_get_button_prev_box_const ( const pos_search_result_page_t *this_ );

/*!
 *  \brief sets the button_prev_box
 *
 *  \param this_ pointer to own object attributes
 *  \param button_prev_box coordinates of button_prev_box
 */
static inline void pos_search_result_page_set_button_prev_box ( pos_search_result_page_t *this_, const shape_int_rectangle_t *button_prev_box );

/*!
 *  \brief gets the has_next_page
 *
 *  \param this_ pointer to own object attributes
 *  \return attribute has_next_page
 */
static inline bool pos_search_result_page_has_next_page ( const pos_search_result_page_t *this_ );

/*!
 *  \brief sets the has_next_page
 *
 *  \param this_ pointer to own object attributes
 *  \param has_next_page new value of attribute has_next_page
 */
static inline void pos_search_result_page_set_has_next_page ( pos_search_result_page_t *this_, bool has_next_page );

/*!
 *  \brief gets the button_next_box
 *
 *  \param this_ pointer to own object attributes
 *  \return const-pointer to shape_int_rectangle_t.
 */
static inline const shape_int_rectangle_t *pos_search_result_page_get_button_next_box_const ( const pos_search_result_page_t *this_ );

/*!
 *  \brief sets the button_next_box
 *
 *  \param this_ pointer to own object attributes
 *  \param button_next_box coordinates of button_next_box
 */
static inline void pos_search_result_page_set_button_next_box ( pos_search_result_page_t *this_, const shape_int_rectangle_t *button_next_box );

#include "pos_search_result_page.inl"

#endif  /* POS_SEARCH_RESULT_PAGE_H */


/*
Copyright 2025-2026 Andreas Warnke

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
