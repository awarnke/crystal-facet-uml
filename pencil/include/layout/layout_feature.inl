/* File: layout_feature.inl; Copyright and License: see below */

#include <assert.h>

static inline void layout_feature_init ( layout_feature_t *this_,
                                         const data_feature_t *feature_data,
                                         layout_visible_classifier_t *classifier )
{
    assert ( NULL != feature_data );
    assert ( NULL != classifier );

    geometry_rectangle_init_empty( &((*this_).symbol_box) );
    geometry_rectangle_init_empty( &((*this_).label_box) );
    (*this_).icon_direction = GEOMETRY_DIRECTION_CENTER;
    (*this_).data = feature_data;
    (*this_).classifier = classifier;
}

static inline void layout_feature_destroy ( layout_feature_t *this_ )
{
    geometry_rectangle_destroy( &((*this_).symbol_box) );
    geometry_rectangle_destroy( &((*this_).label_box) );
    (*this_).data = NULL;
}

static inline bool layout_feature_is_valid ( const layout_feature_t *this_ )
{
    bool result;
    if (( (*this_).data == NULL )||( (*this_).classifier == NULL ))
    {
        result = false;  /* cannot happen on initialized objects */
    }
    else
    {
        result = data_feature_is_valid( (*this_).data )
            && layout_visible_classifier_is_valid( (*this_).classifier )
            && ( data_feature_get_classifier_row_id( (*this_).data ) == layout_visible_classifier_get_classifier_id( (*this_).classifier ));
    }
    return result;
}

static inline const geometry_rectangle_t *layout_feature_get_symbol_box_const ( const layout_feature_t *this_ )
{
    return &((*this_).symbol_box);
}

static inline geometry_point_t layout_feature_get_symbol_middle ( const layout_feature_t *this_ )
{
    geometry_point_t result;
    geometry_point_init( &result,
                         geometry_rectangle_get_center_x( &((*this_).symbol_box) ),
                         geometry_rectangle_get_center_y( &((*this_).symbol_box) )
                       );
    return result;
}

static inline void layout_feature_set_symbol_box ( layout_feature_t *this_, const geometry_rectangle_t *feature_symbol_box )
{
    geometry_rectangle_replace( &((*this_).symbol_box), feature_symbol_box );
}

static inline geometry_direction_t layout_feature_get_icon_direction ( const layout_feature_t *this_ )
{
    return (*this_).icon_direction;
}

static inline void layout_feature_set_icon_direction ( layout_feature_t *this_, geometry_direction_t direction )
{
    (*this_).icon_direction = direction;
}

static inline const geometry_rectangle_t *layout_feature_get_label_box_const ( const layout_feature_t *this_ )
{
    return &((*this_).label_box);
}

static inline void layout_feature_set_label_box ( layout_feature_t *this_, const geometry_rectangle_t *label_box )
{
    geometry_rectangle_replace( &((*this_).label_box), label_box );
}

static inline const data_feature_t *layout_feature_get_data_const ( const layout_feature_t *this_ )
{
    return (*this_).data;
}

static inline layout_visible_classifier_t *layout_feature_get_classifier_ptr ( layout_feature_t *this_ )
{
    return (*this_).classifier;
}

static inline const layout_visible_classifier_t *layout_feature_get_classifier_const ( const layout_feature_t *this_ )
{
    return (*this_).classifier;
}

static inline data_row_id_t layout_feature_get_feature_id ( const layout_feature_t *this_ )
{
    return data_feature_get_row_id( (*this_).data );
}


/*
Copyright 2018-2022 Andreas Warnke

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
