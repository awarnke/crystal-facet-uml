/* File: ctrl_undo_redo_entry.inl; Copyright and License: see below */

static inline void ctrl_undo_redo_entry_init_empty ( ctrl_undo_redo_entry_t *this_ )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY;
}

static inline void ctrl_undo_redo_entry_init ( ctrl_undo_redo_entry_t *this_, ctrl_undo_redo_entry_type_t action_type )
{
    (*this_).action_type = action_type;
}

static inline void ctrl_undo_redo_entry_reinit ( ctrl_undo_redo_entry_t *this_, ctrl_undo_redo_entry_type_t action_type )
{
    (*this_).action_type = action_type;
}

static inline void ctrl_undo_redo_entry_destroy ( ctrl_undo_redo_entry_t *this_ )
{
}

static inline ctrl_undo_redo_entry_type_t ctrl_undo_redo_entry_get_action_type ( ctrl_undo_redo_entry_t *this_ )
{
    return (*this_).action_type;
}

static inline data_classifier_t *ctrl_undo_redo_entry_get_classifier_before_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_before_action.classifier);
}

static inline data_diagram_t *ctrl_undo_redo_entry_get_diagram_before_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_before_action.diagram);
}

static inline data_diagramelement_t *ctrl_undo_redo_entry_get_diagramelement_before_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_before_action.diagramelement);
}

static inline data_relationship_t *ctrl_undo_redo_entry_get_relationship_before_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_before_action.relationship);
}

static inline data_feature_t *ctrl_undo_redo_entry_get_feature_before_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_before_action.feature);
}

static inline data_classifier_t *ctrl_undo_redo_entry_get_classifier_after_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_after_action.classifier);
}

static inline data_diagram_t *ctrl_undo_redo_entry_get_diagram_after_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_after_action.diagram);
}

static inline data_diagramelement_t *ctrl_undo_redo_entry_get_diagramelement_after_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_after_action.diagramelement);
}

static inline data_relationship_t *ctrl_undo_redo_entry_get_relationship_after_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_after_action.relationship);
}

static inline data_feature_t *ctrl_undo_redo_entry_get_feature_after_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_after_action.feature);
}


/*
Copyright 2016-2019 Andreas Warnke

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
