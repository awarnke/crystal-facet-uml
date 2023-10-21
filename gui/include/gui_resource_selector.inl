/* File: gui_resource_selector.inl; Copyright and License: see below */

#include <assert.h>

static inline gui_type_resource_t * gui_resource_selector_get_diagram_type ( gui_resource_selector_t *this_,
                                                                             data_diagram_type_t diag_type )
{
    gui_type_resource_t *result = &((*this_).type_undef);
    for ( unsigned int idx = 0; idx < (*this_).diagram_types_length; idx ++ )
    {
        gui_type_resource_t * const current = &((*this_).type_name_icon_list[(*this_).diagram_types_start+idx]);
        if ( gui_type_resource_get_type( current ).diagram == diag_type )
        {
            assert( gui_type_resource_get_context( current ) == DATA_TABLE_DIAGRAM );
            result = current;
        }
    }
    return result;
}

static inline gui_type_resource_t * gui_resource_selector_get_classifier_type ( gui_resource_selector_t *this_,
                                                                                data_classifier_type_t clas_type )
{
    gui_type_resource_t *result = &((*this_).type_undef);
    for ( unsigned int idx = 0; idx < (*this_).classifier_types_length; idx ++ )
    {
        gui_type_resource_t * const current = &((*this_).type_name_icon_list[(*this_).classifier_types_start+idx]);
        if ( gui_type_resource_get_type( current ).classifier == clas_type )
        {
            assert( gui_type_resource_get_context( current ) == DATA_TABLE_CLASSIFIER );
            result = current;
        }
    }
    return result;
}

static inline gui_type_resource_t * gui_resource_selector_get_feature_type ( gui_resource_selector_t *this_,
                                                                             data_feature_type_t feat_type )
{
    gui_type_resource_t *result = &((*this_).type_undef);
    for ( unsigned int idx = 0; idx < (*this_).feature_types_length; idx ++ )
    {
        gui_type_resource_t * const current = &((*this_).type_name_icon_list[(*this_).feature_types_start+idx]);
        if ( gui_type_resource_get_type( current ).feature == feat_type )
        {
            assert( gui_type_resource_get_context( current ) == DATA_TABLE_FEATURE );
            result = current;
        }
    }
    return result;
}

static inline gui_type_resource_t * gui_resource_selector_get_relationship_type ( gui_resource_selector_t *this_,
                                                                                  data_relationship_type_t rel_type )
{
    gui_type_resource_t *result = &((*this_).type_undef);
    for ( unsigned int idx = 0; idx < (*this_).relationship_types_length; idx ++ )
    {
        gui_type_resource_t * const current = &((*this_).type_name_icon_list[(*this_).relationship_types_start+idx]);
        if ( gui_type_resource_get_type( current ).relationship == rel_type )
        {
            assert( gui_type_resource_get_context( current ) == DATA_TABLE_RELATIONSHIP );
            result = current;
        }
    }
    return result;
}

static inline void gui_resource_selector_get_all_diagram_types ( const gui_resource_selector_t *this_,
                                                                 const gui_type_resource_t (**out_types)[],
                                                                 unsigned int *out_length )
{
    assert( out_types != NULL );
    assert( out_length != NULL );
    const gui_type_resource_t *start_element_ptr = &((*this_).type_name_icon_list[(*this_).diagram_types_start]);
    *out_types = (const gui_type_resource_t((*)[])) start_element_ptr;
    *out_length = (*this_).diagram_types_length;
}

static inline void gui_resource_selector_get_all_classifier_types ( const gui_resource_selector_t *this_,
                                                                    const gui_type_resource_t (**out_types)[],
                                                                    unsigned int *out_length )
{
    assert( out_types != NULL );
    assert( out_length != NULL );
    const gui_type_resource_t *start_element_ptr = &((*this_).type_name_icon_list[(*this_).classifier_types_start]);
    *out_types = (const gui_type_resource_t((*)[])) start_element_ptr;
    *out_length = (*this_).classifier_types_length;
}

static inline void gui_resource_selector_get_all_feature_types ( const gui_resource_selector_t *this_,
                                                                 const gui_type_resource_t (**out_types)[],
                                                                 unsigned int *out_length )
{
    assert( out_types != NULL );
    assert( out_length != NULL );
    const gui_type_resource_t *start_element_ptr = &((*this_).type_name_icon_list[(*this_).feature_types_start]);
    *out_types = (const gui_type_resource_t((*)[])) start_element_ptr;
    *out_length = (*this_).feature_types_length;
}

static inline void gui_resource_selector_get_all_relationship_types ( const gui_resource_selector_t *this_,
                                                                      const gui_type_resource_t (**out_types)[],
                                                                      unsigned int *out_length )
{
    assert( out_types != NULL );
    assert( out_length != NULL );
    const gui_type_resource_t *start_element_ptr = &((*this_).type_name_icon_list[(*this_).relationship_types_start]);
    *out_types = (const gui_type_resource_t((*)[])) start_element_ptr;
    *out_length = (*this_).relationship_types_length;
}


/*
Copyright 2020-2023 Andreas Warnke

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
