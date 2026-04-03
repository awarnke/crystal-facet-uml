/* File: ctrl_classifier_trigger.inl; Copyright and License: see below */

#include <assert.h>

static inline void ctrl_classifier_trigger_init ( ctrl_classifier_trigger_t *this_,
                                                  consistency_feature_t *feature,
                                                  consistency_lifeline_t *lifeline,
                                                  consistency_relationship_t *relationship )
{
    assert( feature != NULL );
    assert( lifeline != NULL );
    assert( relationship != NULL );
    (*this_).feature = feature;
    (*this_).lifeline = lifeline;
    (*this_).relationship = relationship;
}

static inline void ctrl_classifier_trigger_destroy ( ctrl_classifier_trigger_t *this_ )
{
    (*this_).feature = NULL;
    (*this_).lifeline = NULL;
    (*this_).relationship = NULL;
}

static inline u8_error_t ctrl_classifier_trigger_post_delete_feature( ctrl_classifier_trigger_t *this_,
                                                                      const data_feature_t *deleted_feature )
{
    u8_error_t result_err = U8_ERROR_NONE;

    const data_feature_type_t f_type = data_feature_get_main_type( deleted_feature );
    if ( f_type == DATA_FEATURE_TYPE_LIFELINE )
    {
        /* if a lifeline was deleted, check if to unlink the lifeline from the diagramelement */
        result_err |= consistency_lifeline_unlink_lifeline( (*this_).lifeline, deleted_feature );
    }

    return result_err;
}

static inline u8_error_t ctrl_classifier_trigger_post_create_feature( ctrl_classifier_trigger_t *this_,
                                                                      const data_feature_t *create_feature )
{
    u8_error_t result_err = U8_ERROR_NONE;

    const data_feature_type_t f_type = data_feature_get_main_type( create_feature );
    if ( f_type != DATA_FEATURE_TYPE_LIFELINE )
    {
        /* if a non-lifeline was created, check if it is visible */
#if 0
        const data_row_t classifier_id = data_feature_get_classifier_row_id( create_feature );
        result_err |= consistency_feature_delete_invisibles_of_classifier( (*this_).feature, classifier_id );
#endif
    }

    return result_err;
}

static inline u8_error_t ctrl_classifier_trigger_post_create_relationship( ctrl_classifier_trigger_t *this_,
                                                                           const data_relationship_t *create_relationship )
{
    u8_error_t result_err = U8_ERROR_NONE;

#if 0
    const data_row_t classifier_id = data_relationship_get_from_classifier_row_id( create_relationship );
    result_err |= consistency_relationship_delete_invisibles_at_classifier( (*this_).relationship, classifier_id );
#endif

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
