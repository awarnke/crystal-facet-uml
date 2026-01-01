/* File: gui_type_resource.inl; Copyright and License: see below */

static inline void gui_type_resource_init_diagram ( gui_type_resource_t *this_,
                                                    data_diagram_type_t type,
                                                    const char * name,
                                                    GdkTexture * icon )
{
    data_type_init_diagram( &((*this_).type_id), type );
    (*this_).name = name;
    (*this_).icon = icon;
}

static inline void gui_type_resource_init_classifier ( gui_type_resource_t *this_,
                                                       data_classifier_type_t type,
                                                       const char * name,
                                                       GdkTexture * icon )
{
    data_type_init_classifier( &((*this_).type_id), type );
    (*this_).name = name;
    (*this_).icon = icon;
}

static inline void gui_type_resource_init_feature ( gui_type_resource_t *this_,
                                                    data_feature_type_t type,
                                                    const char * name,
                                                    GdkTexture * icon )
{
    data_type_init_feature( &((*this_).type_id), type );
    (*this_).name = name;
    (*this_).icon = icon;
}

static inline void gui_type_resource_init_relationship ( gui_type_resource_t *this_,
                                                         data_relationship_type_t type,
                                                         const char * name,
                                                         GdkTexture * icon )
{
    data_type_init_relationship( &((*this_).type_id), type );
    (*this_).name = name;
    (*this_).icon = icon;
}

static inline void gui_type_resource_replace ( gui_type_resource_t *this_, const gui_type_resource_t *original )
{
    (*this_).type_id = (*original).type_id;
    (*this_).name = (*original).name;
    (*this_).icon = (*original).icon;
}

static inline void gui_type_resource_destroy ( gui_type_resource_t *this_ )
{
}

static inline const data_type_t * gui_type_resource_get_type_id ( const gui_type_resource_t *this_ )
{
    return ( &((*this_).type_id) );
}

static inline const char * gui_type_resource_get_name ( const gui_type_resource_t *this_ )
{
    return ( (*this_).name );
}

static inline GdkTexture * gui_type_resource_get_icon ( const gui_type_resource_t *this_ )
{
    return ( (*this_).icon );
}


/*
Copyright 2020-2026 Andreas Warnke

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
