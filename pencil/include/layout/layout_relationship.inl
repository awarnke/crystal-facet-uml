/* File: layout_relationship.inl; Copyright and License: see below */

#include <assert.h>

static inline void layout_relationship_init_empty ( layout_relationship_t *this_, data_relationship_t *relationship_data )
{
    (*this_).visible = PENCIL_VISIBILITY_HIDE;
    geometry_connector_init_empty( &((*this_).shape) );
    (*this_).relationship_data = relationship_data;
}

static inline void layout_relationship_destroy ( layout_relationship_t *this_ )
{
    geometry_connector_destroy( &((*this_).shape) );
    (*this_).relationship_data = NULL;
}

static inline pencil_visibility_t layout_relationship_get_visibility ( const layout_relationship_t *this_ )
{
    return (*this_).visible;
}

static inline void layout_relationship_set_visibility ( layout_relationship_t *this_, pencil_visibility_t visible )
{
    (*this_).visible = visible;
}

static inline geometry_connector_t *layout_relationship_get_shape_ptr ( layout_relationship_t *this_ )
{
    return &((*this_).shape);
}

static inline data_relationship_t *layout_relationship_get_data_ptr ( const layout_relationship_t *this_ )
{
    assert ( NULL != (*this_).relationship_data );
    return (*this_).relationship_data;
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
