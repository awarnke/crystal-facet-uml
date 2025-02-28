/* File: layout_visible_set.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

static inline void layout_visible_set_reinit( layout_visible_set_t *this_, const data_visible_set_t *input_data )
{
    layout_visible_set_destroy( this_ );
    layout_visible_set_init( this_, input_data );
}

static inline void layout_visible_set_resync( layout_visible_set_t *this_ )
{
    layout_visible_set_reinit( this_, (*this_).input_data );
}

static inline bool layout_visible_set_is_valid( const layout_visible_set_t *this_ )
{
    return (*this_).diagram_valid;
}

/* ================================ diagram ================================ */

static inline layout_diagram_t *layout_visible_set_get_diagram_ptr ( layout_visible_set_t *this_ )
{
    /*assert ( (*this_).diagram_valid );*/ /* we return the pointer even if diagram_layout is not yet initialized */
    return &((*this_).diagram_layout);
}

static inline const layout_diagram_t *layout_visible_set_get_diagram_const ( const layout_visible_set_t *this_ )
{
    /*assert ( (*this_).diagram_valid );*/ /* we return the pointer even if diagram_layout is not yet initialized */
    return &((*this_).diagram_layout);
}

/* ================================ classifiers ================================ */

static inline uint32_t layout_visible_set_get_visible_classifier_count ( const layout_visible_set_t *this_ )
{
    assert ( (*this_).visible_classifier_count <= LAYOUT_VISIBLE_SET_MAX_CLASSIFIERS );
    return (*this_).visible_classifier_count;
}

static inline layout_visible_classifier_t *layout_visible_set_get_visible_classifier_ptr ( layout_visible_set_t *this_, uint32_t index )
{
    assert( index < (*this_).visible_classifier_count );
    assert ( (*this_).visible_classifier_count <= LAYOUT_VISIBLE_SET_MAX_CLASSIFIERS );
    return &((*this_).visible_classifier_layout[index]);
}

static inline const layout_visible_classifier_t *layout_visible_set_get_visible_classifier_const ( const layout_visible_set_t *this_, uint32_t index )
{
    assert( index < (*this_).visible_classifier_count );
    assert ( (*this_).visible_classifier_count <= LAYOUT_VISIBLE_SET_MAX_CLASSIFIERS );
    return &((*this_).visible_classifier_layout[index]);
}

/* ================================ features ================================ */

static inline uint32_t layout_visible_set_get_feature_count ( const layout_visible_set_t *this_ )
{
    assert( (*this_).feature_count <= LAYOUT_VISIBLE_SET_MAX_FEATURES );
    return (*this_).feature_count;
}

static inline layout_feature_t *layout_visible_set_get_feature_ptr ( layout_visible_set_t *this_, uint32_t index )
{
    assert( index < (*this_).feature_count );
    assert( (*this_).feature_count <= LAYOUT_VISIBLE_SET_MAX_FEATURES );
    return &((*this_).feature_layout[index]);
}

static inline const layout_feature_t *layout_visible_set_get_feature_const ( const layout_visible_set_t *this_, uint32_t index )
{
    assert( index < (*this_).feature_count );
    assert( (*this_).feature_count <= LAYOUT_VISIBLE_SET_MAX_FEATURES );
    return &((*this_).feature_layout[index]);
}

/* ================================ relationships ================================ */

static inline pencil_visibility_t layout_visible_set_get_relationship_visibility ( const layout_visible_set_t *this_, uint32_t index )
{
    assert( index < (*this_).relationship_count );
    assert( (*this_).relationship_count <= LAYOUT_VISIBLE_SET_MAX_RELATIONSHIPS );
    return layout_relationship_get_visibility ( &((*this_).relationship_layout[index]) );
}

static inline void layout_visible_set_set_relationship_visibility ( layout_visible_set_t *this_, uint32_t index, pencil_visibility_t visible )
{
    assert( index < (*this_).relationship_count );
    assert( (*this_).relationship_count <= LAYOUT_VISIBLE_SET_MAX_RELATIONSHIPS );
    layout_relationship_set_visibility ( &((*this_).relationship_layout[index]), visible );
}

static inline uint32_t layout_visible_set_get_relationship_count ( const layout_visible_set_t *this_ )
{
    assert( (*this_).relationship_count <= LAYOUT_VISIBLE_SET_MAX_RELATIONSHIPS );
    return (*this_).relationship_count;
}

static inline layout_relationship_t *layout_visible_set_get_relationship_ptr ( layout_visible_set_t *this_, uint32_t index )
{
    assert( index < (*this_).relationship_count );
    assert( (*this_).relationship_count <= LAYOUT_VISIBLE_SET_MAX_RELATIONSHIPS );
    return &((*this_).relationship_layout[index]);
}

static inline const layout_relationship_t *layout_visible_set_get_relationship_const ( const layout_visible_set_t *this_, uint32_t index )
{
    assert( index < (*this_).relationship_count );
    assert( (*this_).relationship_count <= LAYOUT_VISIBLE_SET_MAX_RELATIONSHIPS );
    return &((*this_).relationship_layout[index]);
}

static inline bool layout_visible_set_is_ancestor ( const layout_visible_set_t *this_,
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

static inline uint32_t layout_visible_set_count_ancestors ( const layout_visible_set_t *this_,
                                                            const layout_visible_classifier_t *classifier )
{
    assert ( NULL != classifier );

    /* get index */
    uint32_t classifier_index;
    classifier_index = data_visible_set_get_classifier_index_from_pointer ( (*this_).input_data, layout_visible_classifier_get_data_const(classifier) );

    /* ask input_data */
    return data_visible_set_count_ancestors_of_index ( (*this_).input_data, classifier_index );
}

static inline uint32_t layout_visible_set_count_descendants ( const layout_visible_set_t *this_,
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
Copyright 2017-2025 Andreas Warnke

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
