/* File: ctrl_diagram_policy_enforcer.inl; Copyright and License: see below */

static inline u8_error_t ctrl_diagram_policy_enforcer_post_update_diagram_type( ctrl_diagram_policy_enforcer_t *this_,
                                                                                const data_diagram_t *updated_diagram )
{
    return ctrl_diagram_policy_enforcer_private_create_lifelines( this_, updated_diagram );
}

static inline u8_error_t ctrl_diagram_policy_enforcer_post_create_diagramelement( ctrl_diagram_policy_enforcer_t *this_,
                                                                                  const data_diagramelement_t *new_diagramelement )
{
    u8_error_t result = U8_ERROR_NONE;
    if ( DATA_ROW_ID_VOID == data_diagramelement_get_focused_feature_row_id( new_diagramelement ) )
    {
        result = ctrl_diagram_policy_enforcer_private_create_a_lifeline( this_, new_diagramelement );
    }
    return result;
}

static inline u8_error_t ctrl_diagram_policy_enforcer_post_delete_diagramelement( ctrl_diagram_policy_enforcer_t *this_,
                                                                                  const data_diagramelement_t *deleted_diagramelement )
{
    u8_error_t result_err = U8_ERROR_NONE;
    result_err |= ctrl_diagram_policy_enforcer_private_delete_a_lifeline ( this_, deleted_diagramelement );
    result_err |= ctrl_diagram_policy_enforcer_private_delete_unreferenced_classifier ( this_, deleted_diagramelement );
    result_err |= ctrl_diagram_policy_enforcer_private_delete_invisible_relationships ( this_, deleted_diagramelement );
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
