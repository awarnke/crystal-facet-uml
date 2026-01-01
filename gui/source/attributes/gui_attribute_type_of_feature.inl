/* File: gui_attribute_type_of_feature.inl; Copyright and License: see below */

static inline void gui_attribute_type_of_feature_init ( gui_attribute_type_of_feature_t *this_,
                                                        data_feature_type_t selected_type,
                                                        gui_attributes_editor_t *editor )
{
    (*this_).selected_type = selected_type;
    (*this_).editor = editor;
}


static inline void gui_attribute_type_of_feature_destroy ( gui_attribute_type_of_feature_t *this_ )
{
    (*this_).editor = NULL;
}


static inline data_feature_type_t gui_attribute_type_of_feature_get_selected_type ( const gui_attribute_type_of_feature_t *this_ )
{
    return (*this_).selected_type;
}


static inline gui_attributes_editor_t* gui_attribute_type_of_feature_get_editor ( const gui_attribute_type_of_feature_t *this_ )
{
    return (*this_).editor;
}


/*
Copyright 2023-2026 Andreas Warnke

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
