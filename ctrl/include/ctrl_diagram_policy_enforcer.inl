/* File: ctrl_diagram_policy_enforcer.inl; Copyright and License: see below */

static inline ctrl_error_t ctrl_diagram_policy_enforcer_post_update_diagram_type ( ctrl_diagram_policy_enforcer_t *this_,
                                                                           const data_diagram_t *updated_diagram )
{
    return ctrl_diagram_policy_enforcer_private_create_lifelines ( this_, updated_diagram );
}

static inline ctrl_error_t ctrl_diagram_policy_enforcer_post_create_diagramelement ( ctrl_diagram_policy_enforcer_t *this_,
                                                                             const data_diagramelement_t *new_diagramelement )
{
    return ctrl_diagram_policy_enforcer_private_create_a_lifeline ( this_, new_diagramelement );
}

static inline ctrl_error_t ctrl_diagram_policy_enforcer_post_delete_diagramelement ( ctrl_diagram_policy_enforcer_t *this_,
                                                                             const data_diagramelement_t *deleted_diagramelement )
{
    ctrl_error_t result_err = CTRL_ERROR_NONE;
    result_err |= ctrl_diagram_policy_enforcer_private_delete_a_lifeline ( this_, deleted_diagramelement );
    result_err |= ctrl_diagram_policy_enforcer_private_delete_unreferenced_classifier ( this_, deleted_diagramelement );
    return result_err;
}


/*
Copyright 2018-2018 Andreas Warnke

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
