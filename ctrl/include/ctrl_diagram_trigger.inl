/* File: ctrl_diagram_trigger.inl; Copyright and License: see below */

#include <assert.h>

static inline void ctrl_diagram_trigger_init ( ctrl_diagram_trigger_t *this_,
                                               consistency_classifier_t *classifier,
                                               consistency_feature_t *feature,
                                               consistency_lifeline_t *lifeline,
                                               consistency_relationship_t *relationship )
{
    assert( classifier != NULL );
    assert( feature != NULL );
    assert( lifeline != NULL );
    assert( relationship != NULL );
    (*this_).classifier = classifier;
    (*this_).feature = feature;
    (*this_).lifeline = lifeline;
    (*this_).relationship = relationship;
}

static inline void ctrl_diagram_trigger_destroy ( ctrl_diagram_trigger_t *this_ )
{
    (*this_).classifier = NULL;
    (*this_).feature = NULL;
    (*this_).lifeline = NULL;
    (*this_).relationship = NULL;
}

static inline u8_error_t ctrl_diagram_trigger_post_update_diagram_type( ctrl_diagram_trigger_t *this_,
                                                                        const data_diagram_t *updated_diagram )
{
    u8_error_t result = U8_ERROR_NONE;
    result |= consistency_lifeline_delete_lifelines( (*this_).lifeline, updated_diagram );
    result |= consistency_lifeline_create_lifelines( (*this_).lifeline, updated_diagram );
    result |= consistency_feature_delete_invisibles_in_diagram( (*this_).feature, updated_diagram );
    result |= consistency_relationship_delete_invisibles_in_diagram( (*this_).relationship, updated_diagram );
    return result;
}

static inline u8_error_t ctrl_diagram_trigger_post_create_diagramelement( ctrl_diagram_trigger_t *this_,
                                                                          const data_diagramelement_t *new_diagramelement )
{
    u8_error_t result = U8_ERROR_NONE;
    if ( DATA_ROW_VOID == data_diagramelement_get_focused_feature_row_id( new_diagramelement ) )
    {
        result = consistency_lifeline_create_a_lifeline( (*this_).lifeline, new_diagramelement );
    }
    return result;
}

static inline u8_error_t ctrl_diagram_trigger_post_delete_diagramelement( ctrl_diagram_trigger_t *this_,
                                                                          const data_diagramelement_t *deleted_diagramelement )
{
    u8_error_t result_err = U8_ERROR_NONE;
    result_err |= consistency_lifeline_delete_a_lifeline( (*this_).lifeline, deleted_diagramelement );
    result_err |= consistency_classifier_delete_unreferenced_classifier( (*this_).classifier, deleted_diagramelement );
    result_err |= consistency_feature_delete_invisibles_of_classifier( (*this_).feature, deleted_diagramelement );
    result_err |= consistency_relationship_delete_invisibles_at_classifier( (*this_).relationship, deleted_diagramelement );
    return result_err;
}


/*
Copyright 2018-2026 Andreas Warnke

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
