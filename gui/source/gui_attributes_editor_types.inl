/* File: gui_attributes_editor_types_types.inl; Copyright and License: see below */

static inline const GtkListStore* gui_attributes_editor_types_get_undef ( const gui_attributes_editor_types_t *this_ )
{
    return (*this_).no_types;
}

static inline const GtkListStore* gui_attributes_editor_types_get_diagrams ( const gui_attributes_editor_types_t *this_ )
{
    return (*this_).diagram_types;
}

static inline const GtkListStore* gui_attributes_editor_types_get_classifiers ( const gui_attributes_editor_types_t *this_ )
{
    return (*this_).classifier_types;
}

static inline const GtkListStore* gui_attributes_editor_types_get_relationships ( const gui_attributes_editor_types_t *this_ )
{
    return (*this_).relationship_types;
}

static inline const GtkListStore* gui_attributes_editor_types_get_features ( const gui_attributes_editor_types_t *this_ )
{
    return (*this_).feature_types;
}

static inline const GtkListStore* gui_attributes_editor_types_get_feature_lifeline ( const gui_attributes_editor_types_t *this_ )
{
    return (*this_).feature_lifeline_type;
}


/*
Copyright 2019-2023 Andreas Warnke

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
