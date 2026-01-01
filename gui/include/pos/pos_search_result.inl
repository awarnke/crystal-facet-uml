/* File: pos_search_result.inl; Copyright and License: see below */

#include <assert.h>

static inline void pos_search_result_init ( pos_search_result_t *this_,
                                         const data_search_result_t *search_result_data )
{
    assert ( NULL != search_result_data );

    shape_int_rectangle_init_empty( &((*this_).icon_box) );
    shape_int_rectangle_init_empty( &((*this_).label_box) );
    (*this_).data = search_result_data;
}

static inline void pos_search_result_destroy ( pos_search_result_t *this_ )
{
    shape_int_rectangle_destroy( &((*this_).icon_box) );
    shape_int_rectangle_destroy( &((*this_).label_box) );
    (*this_).data = NULL;
}

static inline bool pos_search_result_is_valid ( const pos_search_result_t *this_ )
{
    bool result;
    if (( (*this_).data == NULL ))
    {
        assert(false);
        result = false;  /* cannot happen on initialized objects */
    }
    else
    {
        const data_id_t data_id = data_search_result_get_match_id( (*this_).data );
        result = data_id_is_valid( &data_id );
    }
    return result;
}

static inline const shape_int_rectangle_t *pos_search_result_get_icon_box_const ( const pos_search_result_t *this_ )
{
    return &((*this_).icon_box);
}

static inline void pos_search_result_set_icon_box ( pos_search_result_t *this_, const shape_int_rectangle_t *icon_box )
{
    shape_int_rectangle_replace( &((*this_).icon_box), icon_box );
}

static inline const shape_int_rectangle_t *pos_search_result_get_label_box_const ( const pos_search_result_t *this_ )
{
    return &((*this_).label_box);
}

static inline void pos_search_result_set_label_box ( pos_search_result_t *this_, const shape_int_rectangle_t *label_box )
{
    shape_int_rectangle_replace( &((*this_).label_box), label_box );
}

static inline const data_search_result_t *pos_search_result_get_data_const ( const pos_search_result_t *this_ )
{
    return (*this_).data;
}

static inline data_id_t pos_search_result_get_data_id ( const pos_search_result_t *this_ )
{
    return data_search_result_get_match_id( (*this_).data );
}

static inline data_id_t pos_search_result_get_diagram_id ( const pos_search_result_t *this_ )
{
    return data_search_result_get_diagram_id( (*this_).data );
}


/*
Copyright 2021-2026 Andreas Warnke

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
