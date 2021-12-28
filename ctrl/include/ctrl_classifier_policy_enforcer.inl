/* File: ctrl_classifier_policy_enforcer.inl; Copyright and License: see below */

static inline u8_error_t ctrl_classifier_policy_enforcer_post_delete_feature( ctrl_classifier_policy_enforcer_t *this_,
                                                                              const data_feature_t *deleted_feature )
{
    u8_error_t result_err = U8_ERROR_NONE;

    result_err |= ctrl_classifier_policy_enforcer_private_unlink_lifeline( this_, deleted_feature );

    return result_err;
}


/*
Copyright 2018-2021 Andreas Warnke

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
