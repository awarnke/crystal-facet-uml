/* File: layout_diagram.inl; Copyright and License: see below */

#include <assert.h>

static inline void layout_diagram_init ( layout_diagram_t *this_, const data_diagram_t *diagram_data )
{
    assert ( NULL != diagram_data );

    geometry_rectangle_init_empty( &((*this_).bounds) );
    geometry_rectangle_init_empty( &((*this_).draw_area) );
    (*this_).data = diagram_data;
}

static inline void layout_diagram_destroy ( layout_diagram_t *this_ )
{
    geometry_rectangle_destroy( &((*this_).bounds) );
    geometry_rectangle_destroy( &((*this_).draw_area) );
    (*this_).data = NULL;
}

static inline bool layout_diagram_is_valid ( const layout_diagram_t *this_ )
{
    bool result;
    if ( (*this_).data == NULL )
    {
        result = false;  /* cannot happen on initialized objects */
    }
    else
    {
        result = data_diagram_is_valid( (*this_).data );
    }
    return result;
}

static inline geometry_rectangle_t *layout_diagram_get_bounds_ptr ( layout_diagram_t *this_ )
{
    return &((*this_).bounds);
}

static inline const geometry_rectangle_t *layout_diagram_get_bounds_const ( const layout_diagram_t *this_ )
{
    return &((*this_).bounds);
}

static inline void layout_diagram_set_bounds ( layout_diagram_t *this_, const geometry_rectangle_t *diagram_bounds )
{
    geometry_rectangle_replace( &((*this_).bounds), diagram_bounds );
}

static inline geometry_rectangle_t *layout_diagram_get_draw_area_ptr ( layout_diagram_t *this_ )
{
    return &((*this_).draw_area);
}

static inline const geometry_rectangle_t *layout_diagram_get_draw_area_const ( const layout_diagram_t *this_ )
{
    return &((*this_).draw_area);
}

static inline const data_diagram_t *layout_diagram_get_data_const ( const layout_diagram_t *this_ )
{
    assert ( NULL != (*this_).data );
    return (*this_).data;
}

static inline data_row_id_t layout_diagram_get_diagram_id ( const layout_diagram_t *this_ )
{
    return data_diagram_get_id( (*this_).data );
}


/*
Copyright 2018-2020 Andreas Warnke

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
