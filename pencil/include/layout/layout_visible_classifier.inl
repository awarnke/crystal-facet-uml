/* File: layout_visible_classifier.inl; Copyright and License: see below */

#include <assert.h>

static inline void layout_visible_classifier_init ( layout_visible_classifier_t *this_, data_visible_classifier_t *visible_classifier_data )
{
    assert ( NULL != visible_classifier_data );

    geometry_rectangle_init_empty( &((*this_).bounds) );
    geometry_rectangle_init_empty( &((*this_).space) );
    (*this_).data = visible_classifier_data;
}

static inline void layout_visible_classifier_destroy ( layout_visible_classifier_t *this_ )
{
    geometry_rectangle_destroy( &((*this_).bounds) );
    geometry_rectangle_destroy( &((*this_).space) );
    (*this_).data = NULL;
}

static inline geometry_rectangle_t *layout_visible_classifier_get_bounds_ptr ( layout_visible_classifier_t *this_ )
{
    return &((*this_).bounds);
}

static inline geometry_rectangle_t *layout_visible_classifier_get_space_ptr ( layout_visible_classifier_t *this_ )
{
    return &((*this_).space);
}

static inline const data_visible_classifier_t *layout_visible_classifier_get_data_ptr ( const layout_visible_classifier_t *this_ )
{
    return (*this_).data;
}

static inline const data_classifier_t *layout_visible_classifier_get_classifier_ptr ( const layout_visible_classifier_t *this_ )
{
    return data_visible_classifier_get_classifier_const ( (*this_).data );
}

static inline const data_diagramelement_t *layout_visible_classifier_get_diagramelement_ptr ( const layout_visible_classifier_t *this_ )
{
    return data_visible_classifier_get_diagramelement_const ( (*this_).data );
}

static inline bool layout_visible_classifier_is_fix_sized_symbol ( const layout_visible_classifier_t *this_ )
{
    data_classifier_type_t classifier_type;
    classifier_type = data_classifier_get_main_type( data_visible_classifier_get_classifier_const ( (*this_).data ) );
    bool result;
    result = (  ( DATA_CLASSIFIER_TYPE_UML_ACTOR == classifier_type )
                || ( DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE == classifier_type )
                || ( DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE == classifier_type )
                || ( DATA_CLASSIFIER_TYPE_DYN_FORK_NODE == classifier_type )
                || ( DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE == classifier_type )
                || ( DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY == classifier_type )
                || ( DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY == classifier_type )
                || ( DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT == classifier_type )
             );
             
    return result;
}

static inline int64_t layout_visible_classifier_get_classifier_id ( const layout_visible_classifier_t *this_ )
{
    return data_classifier_get_id( data_visible_classifier_get_classifier_const ( (*this_).data ) );
}

static inline int64_t layout_visible_classifier_get_diagramelement_id ( const layout_visible_classifier_t *this_ )
{
    return data_diagramelement_get_id( data_visible_classifier_get_diagramelement_const ( (*this_).data ) );
}

/*
Copyright 2018-2019 Andreas Warnke

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
