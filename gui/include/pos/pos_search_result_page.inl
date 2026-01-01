/* File: pos_search_result_page.inl; Copyright and License: see below */

#include <assert.h>

static inline void pos_search_result_page_init ( pos_search_result_page_t *this_, uint32_t buffer_start )
{
    (*this_).buffer_start = buffer_start;
    (*this_).buffer_length = 0;
    (*this_).buffer_more_after = false;
    (*this_).page_start = buffer_start;
    (*this_).page_length = 0;
    (*this_).has_prev_page = false;
    shape_int_rectangle_init_empty( &((*this_).button_prev_box) );
    (*this_).has_next_page = false;
    shape_int_rectangle_init_empty( &((*this_).button_next_box) );
}

static inline void pos_search_result_page_reinit ( pos_search_result_page_t *this_, uint32_t buffer_start )
{
    pos_search_result_page_destroy( this_ );
    pos_search_result_page_init( this_, buffer_start );
}

static inline void pos_search_result_page_destroy ( pos_search_result_page_t *this_ )
{
    (*this_).buffer_length = 0;
    shape_int_rectangle_destroy( &((*this_).button_prev_box) );
    shape_int_rectangle_destroy( &((*this_).button_next_box) );
}

static inline const pos_search_result_t *pos_search_result_page_get_search_result_layout_const ( const pos_search_result_page_t *this_, uint32_t index )
{
    assert( index >= (*this_).buffer_start );
    assert( index < (*this_).buffer_start + (*this_).buffer_length );
    return &((*this_).element_pos[ index - (*this_).buffer_start ]);
}

static inline pos_search_result_t *pos_search_result_page_get_search_result_layout_ptr ( pos_search_result_page_t *this_, uint32_t index )
{
    assert( index >= (*this_).buffer_start );
    assert( index < (*this_).buffer_start + (*this_).buffer_length );
    return &((*this_).element_pos[ index - (*this_).buffer_start ]);
}

static inline u8_error_t pos_search_result_page_add_search_result ( pos_search_result_page_t *this_, const data_search_result_t *search_result )
{
    u8_error_t result = U8_ERROR_NONE;
    if ( (*this_).buffer_length < POS_SEARCH_RESULT_PAGE_MAX_PAGE_SIZE )
    {
        data_search_result_copy( &((*this_).result_list_buf[(*this_).buffer_length]), search_result );
        pos_search_result_init( &((*this_).element_pos[(*this_).buffer_length]),
                                &((*this_).result_list_buf[(*this_).buffer_length])
                              );
        (*this_).buffer_length ++;
    }
    else
    {
        result = U8_ERROR_ARRAY_BUFFER_EXCEEDED;
    }
    return result;
}

static inline uint32_t pos_search_result_page_get_buffer_start ( const pos_search_result_page_t *this_ )
{
    return (*this_).buffer_start;
}

static inline uint32_t pos_search_result_page_get_buffer_length ( const pos_search_result_page_t *this_ )
{
    return (*this_).buffer_length;
}

static inline bool pos_search_result_page_get_buffer_more_after ( const pos_search_result_page_t *this_ )
{
    return (*this_).buffer_more_after;
}

static inline void pos_search_result_page_set_buffer_more_after ( pos_search_result_page_t *this_, bool buffer_more_after )
{
    (*this_).buffer_more_after = buffer_more_after;
}

static inline uint32_t pos_search_result_page_get_page_start ( const pos_search_result_page_t *this_ )
{
    return (*this_).page_start;
}

static inline void pos_search_result_page_set_page_start ( pos_search_result_page_t *this_, uint32_t page_start )
{
    (*this_).page_start = page_start;
}

static inline uint32_t pos_search_result_page_get_page_length ( const pos_search_result_page_t *this_ )
{
    return (*this_).page_length;
}

static inline void pos_search_result_page_set_page_length ( pos_search_result_page_t *this_, uint32_t page_length )
{
    (*this_).page_length = page_length;
}

static inline bool pos_search_result_page_has_prev_page ( const pos_search_result_page_t *this_ )
{
    return (*this_).has_prev_page;
}

static inline void pos_search_result_page_set_has_prev_page ( pos_search_result_page_t *this_, bool has_prev_page )
{
    (*this_).has_prev_page = has_prev_page;
}

static inline const shape_int_rectangle_t *pos_search_result_page_get_button_prev_box_const ( const pos_search_result_page_t *this_ )
{
     return &((*this_).button_prev_box);
}

static inline void pos_search_result_page_set_button_prev_box ( pos_search_result_page_t *this_, const shape_int_rectangle_t *button_prev_box )
{
    shape_int_rectangle_replace( &((*this_).button_prev_box), button_prev_box );
}

static inline bool pos_search_result_page_has_next_page ( const pos_search_result_page_t *this_ )
{
    return (*this_).has_next_page;
}

static inline void pos_search_result_page_set_has_next_page ( pos_search_result_page_t *this_, bool has_next_page )
{
    (*this_).has_next_page = has_next_page;
}

static inline const shape_int_rectangle_t *pos_search_result_page_get_button_next_box_const ( const pos_search_result_page_t *this_ )
{
    return &((*this_).button_next_box);
}

static inline void pos_search_result_page_set_button_next_box ( pos_search_result_page_t *this_, const shape_int_rectangle_t *button_next_box )
{
    shape_int_rectangle_replace( &((*this_).button_next_box), button_next_box );
}


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
