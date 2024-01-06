/* File: ctrl_classifier_trigger.inl; Copyright and License: see below */

#include <assert.h>

static inline void ctrl_classifier_trigger_init ( ctrl_classifier_trigger_t *this_,
                                                  consistency_drop_invisibles_t *drop_invisibles,
                                                  consistency_lifeline_t *lifeline )
{
    assert( drop_invisibles != NULL );
    assert( lifeline != NULL );
    (*this_).drop_invisibles = drop_invisibles;
    (*this_).lifeline = lifeline;
}

static inline void ctrl_classifier_trigger_destroy ( ctrl_classifier_trigger_t *this_ )
{
    (*this_).drop_invisibles = NULL;
    (*this_).lifeline = NULL;
}

static inline u8_error_t ctrl_classifier_trigger_post_delete_feature( ctrl_classifier_trigger_t *this_,
                                                                      const data_feature_t *deleted_feature )
{
    u8_error_t result_err = U8_ERROR_NONE;

    result_err |= consistency_lifeline_unlink_lifeline( (*this_).lifeline, deleted_feature );

    return result_err;
}


/*
Copyright 2018-2024 Andreas Warnke

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
