/* File: layout_visible_classifier.inl; Copyright and License: see below */

#include <assert.h>

static inline void layout_visible_classifier_init ( layout_visible_classifier_t *this_, data_visible_classifier_t *visible_classifier_data )
{
    assert ( NULL != visible_classifier_data );

    geometry_rectangle_init_empty( &((*this_).bounds) );
    geometry_rectangle_init_empty( &((*this_).space) );
    geometry_rectangle_init_empty( &((*this_).label_box) );
    (*this_).label_h_anchor = GEOMETRY_H_ALIGN_CENTER;  /* most labels are centered */
    (*this_).label_v_anchor = GEOMETRY_V_ALIGN_TOP;  /* most labels are fixed to the top */
    (*this_).data = visible_classifier_data;
}

static inline void layout_visible_classifier_destroy ( layout_visible_classifier_t *this_ )
{
    geometry_rectangle_destroy( &((*this_).bounds) );
    geometry_rectangle_destroy( &((*this_).space) );
    geometry_rectangle_destroy( &((*this_).label_box) );
    (*this_).data = NULL;
}

static inline bool layout_visible_classifier_is_valid ( layout_visible_classifier_t *this_ )
{
    bool result;
    if ( (*this_).data == NULL )
    {
        result = false;  /* cannot happen on initialized objects */
    }
    else
    {
        result = data_visible_classifier_is_valid( (*this_).data );
    }
    return result;
}

static inline geometry_rectangle_t *layout_visible_classifier_get_bounds_ptr ( layout_visible_classifier_t *this_ )
{
    return &((*this_).bounds);
}

static inline void layout_visible_classifier_set_bounds ( layout_visible_classifier_t *this_, const geometry_rectangle_t *bounds )
{
    geometry_rectangle_replace( &((*this_).bounds), bounds );
}

static inline geometry_rectangle_t *layout_visible_classifier_get_space_ptr ( layout_visible_classifier_t *this_ )
{
    return &((*this_).space);
}

static inline void layout_visible_classifier_set_space ( layout_visible_classifier_t *this_, const geometry_rectangle_t *space )
{
    geometry_rectangle_replace( &((*this_).space), space );
}

static inline geometry_rectangle_t *layout_visible_classifier_get_label_box_ptr ( layout_visible_classifier_t *this_ )
{
    return &((*this_).label_box);
}

static inline void layout_visible_classifier_set_label_box ( layout_visible_classifier_t *this_, const geometry_rectangle_t *label_box )
{
    geometry_rectangle_replace( &((*this_).label_box), label_box );
}

static inline void layout_visible_classifier_set_label_anchor ( layout_visible_classifier_t *this_,
                                                                geometry_h_align_t label_h_anchor,
                                                                geometry_v_align_t label_v_anchor )
{
    (*this_).label_h_anchor = label_h_anchor;
    (*this_).label_v_anchor = label_v_anchor;
}

static inline void layout_visible_classifier_shift ( layout_visible_classifier_t *this_, double delta_x, double delta_y )
{
    geometry_rectangle_shift( &((*this_).bounds), delta_x, delta_y );
    geometry_rectangle_shift( &((*this_).space), delta_x, delta_y );
    geometry_rectangle_shift( &((*this_).label_box), delta_x, delta_y );
}

static inline void layout_visible_classifier_expand ( layout_visible_classifier_t *this_, double delta_width, double delta_height )
{
    geometry_rectangle_expand( &((*this_).bounds), delta_width, delta_height );
    geometry_rectangle_expand( &((*this_).space), delta_width, delta_height );
    /* the label moves according to the defined anchor */
    double dx = geometry_h_align_get_left ( &((*this_).label_h_anchor), 0.0, 0.0, delta_width );
    double dy = geometry_v_align_get_top ( &((*this_).label_v_anchor), 0.0, 0.0, delta_height );
    geometry_rectangle_shift( &((*this_).label_box), dx, dy );
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
