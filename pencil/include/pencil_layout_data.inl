/* File: pencil_layout_data.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

static inline void pencil_layout_data_reinit( pencil_layout_data_t *this_, const data_visible_set_t *input_data )
{
    pencil_layout_data_destroy( this_ );
    pencil_layout_data_init( this_, input_data );
}

static inline void pencil_layout_data_resync( pencil_layout_data_t *this_ )
{
    pencil_layout_data_reinit( this_, (*this_).input_data );
}

/* ================================ diagram ================================ */

static inline layout_diagram_t *pencil_layout_data_get_diagram_ptr ( pencil_layout_data_t *this_ )
{
    /*assert ( (*this_).diagram_valid );*/ /* we return the pointer even if diagram_layout is not yet initialized */
    return &((*this_).diagram_layout);
}

static inline const layout_diagram_t *pencil_layout_data_get_diagram_const ( const pencil_layout_data_t *this_ )
{
    /*assert ( (*this_).diagram_valid );*/ /* we return the pointer even if diagram_layout is not yet initialized */
    return &((*this_).diagram_layout);
}

/* ================================ classifiers ================================ */

static inline uint32_t pencil_layout_data_get_classifier_count ( const pencil_layout_data_t *this_ )
{
    assert ( (*this_).visible_classifier_count <= PENCIL_LAYOUT_DATA_MAX_CLASSIFIERS );
    return (*this_).visible_classifier_count;
}

static inline layout_visible_classifier_t *pencil_layout_data_get_classifier_ptr ( pencil_layout_data_t *this_, uint32_t index )
{
    assert( index < (*this_).visible_classifier_count );
    assert ( (*this_).visible_classifier_count <= PENCIL_LAYOUT_DATA_MAX_CLASSIFIERS );
    return &((*this_).visible_classifier_layout[index]);
}

static inline const layout_visible_classifier_t *pencil_layout_data_get_classifier_const ( const pencil_layout_data_t *this_, uint32_t index )
{
    assert( index < (*this_).visible_classifier_count );
    assert ( (*this_).visible_classifier_count <= PENCIL_LAYOUT_DATA_MAX_CLASSIFIERS );
    return &((*this_).visible_classifier_layout[index]);
}

/* ================================ features ================================ */

static inline uint32_t pencil_layout_data_get_feature_count ( const pencil_layout_data_t *this_ )
{
    assert( (*this_).feature_count <= PENCIL_LAYOUT_DATA_MAX_FEATURES );
    return (*this_).feature_count;
}

static inline layout_feature_t *pencil_layout_data_get_feature_ptr ( pencil_layout_data_t *this_, uint32_t index )
{
    assert( index < (*this_).feature_count );
    assert( (*this_).feature_count <= PENCIL_LAYOUT_DATA_MAX_FEATURES );
    return &((*this_).feature_layout[index]);
}

static inline const layout_feature_t *pencil_layout_data_get_feature_const ( const pencil_layout_data_t *this_, uint32_t index )
{
    assert( index < (*this_).feature_count );
    assert( (*this_).feature_count <= PENCIL_LAYOUT_DATA_MAX_FEATURES );
    return &((*this_).feature_layout[index]);
}

/* ================================ relationships ================================ */

static inline pencil_visibility_t pencil_layout_data_get_relationship_visibility ( const pencil_layout_data_t *this_, uint32_t index )
{
    assert( index < (*this_).relationship_count );
    assert( (*this_).relationship_count <= PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS );
    return layout_relationship_get_visibility ( &((*this_).relationship_layout[index]) );
}

static inline void pencil_layout_data_set_relationship_visibility ( pencil_layout_data_t *this_, uint32_t index, pencil_visibility_t visible )
{
    assert( index < (*this_).relationship_count );
    assert( (*this_).relationship_count <= PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS );
    layout_relationship_set_visibility ( &((*this_).relationship_layout[index]), visible );
}

static inline uint32_t pencil_layout_data_get_relationship_count ( const pencil_layout_data_t *this_ )
{
    assert( (*this_).relationship_count <= PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS );
    return (*this_).relationship_count;
}

static inline layout_relationship_t *pencil_layout_data_get_relationship_ptr ( pencil_layout_data_t *this_, uint32_t index )
{
    assert( index < (*this_).relationship_count );
    assert( (*this_).relationship_count <= PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS );
    return &((*this_).relationship_layout[index]);
}

static inline const layout_relationship_t *pencil_layout_data_get_relationship_const ( const pencil_layout_data_t *this_, uint32_t index )
{
    assert( index < (*this_).relationship_count );
    assert( (*this_).relationship_count <= PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS );
    return &((*this_).relationship_layout[index]);
}

static inline bool pencil_layout_data_is_ancestor ( const pencil_layout_data_t *this_,
                                                    const layout_visible_classifier_t *ancestor,
                                                    const layout_visible_classifier_t *descendant )
{
    assert ( NULL != ancestor );
    assert ( NULL != descendant );

    /* get index */
    uint32_t ancestor_index;
    uint32_t descendant_index;
    ancestor_index = data_visible_set_get_classifier_index_from_pointer ( (*this_).input_data, layout_visible_classifier_get_data_const(ancestor) );
    descendant_index = data_visible_set_get_classifier_index_from_pointer ( (*this_).input_data, layout_visible_classifier_get_data_const(descendant) );

    /* ask input_data */
    return data_visible_set_is_ancestor_by_index ( (*this_).input_data, ancestor_index, descendant_index );
}

static inline uint32_t pencil_layout_data_count_ancestors ( const pencil_layout_data_t *this_,
                                                            const layout_visible_classifier_t *classifier )
{
    assert ( NULL != classifier );

    /* get index */
    uint32_t classifier_index;
    classifier_index = data_visible_set_get_classifier_index_from_pointer ( (*this_).input_data, layout_visible_classifier_get_data_const(classifier) );

    /* ask input_data */
    return data_visible_set_count_ancestors_of_index ( (*this_).input_data, classifier_index );
}

static inline uint32_t pencil_layout_data_count_descendants ( const pencil_layout_data_t *this_,
                                                              const layout_visible_classifier_t *classifier )
{
    assert ( NULL != classifier );

    /* get index */
    uint32_t classifier_index;
    classifier_index = data_visible_set_get_classifier_index_from_pointer ( (*this_).input_data, layout_visible_classifier_get_data_const(classifier) );

    /* ask input_data */
    return data_visible_set_count_descendants_of_index ( (*this_).input_data, classifier_index );
}


/*
Copyright 2017-2020 Andreas Warnke

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
