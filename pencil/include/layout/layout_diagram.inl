/* File: layout_diagram.inl; Copyright and License: see below */

#include <assert.h>

static inline void layout_diagram_init_empty ( layout_diagram_t *this_, data_diagram_t *diagram_data )
{
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

static inline geometry_rectangle_t *layout_diagram_get_bounds_ptr ( layout_diagram_t *this_ )
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

static inline const data_diagram_t *layout_diagram_get_data_ptr ( const layout_diagram_t *this_ )
{
    assert ( NULL != (*this_).data );
    return (*this_).data;
}


/*
Copyright 2018-2018 Andreas Warnke

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
