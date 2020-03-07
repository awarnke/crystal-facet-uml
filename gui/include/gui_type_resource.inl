/* File: gui_type_resource.inl; Copyright and License: see below */

static inline void gui_type_resource_init_diagram ( gui_type_resource_t *this_,
                                                    data_diagram_type_t type,
                                                    const char * name,
                                                    const GdkPixbuf * icon )
{
    (*this_).context = DATA_TABLE_DIAGRAM;
    (*this_).type.diagram = type;
    (*this_).name = name;
    (*this_).icon = icon;
}

static inline void gui_type_resource_init_classifier ( gui_type_resource_t *this_,
                                                       data_classifier_type_t type,
                                                       const char * name,
                                                       const GdkPixbuf * icon )
{
    (*this_).context = DATA_TABLE_CLASSIFIER;
    (*this_).type.classifier = type;
    (*this_).name = name;
    (*this_).icon = icon;
}

static inline void gui_type_resource_init_feature ( gui_type_resource_t *this_,
                                                    data_feature_type_t type,
                                                    const char * name,
                                                    const GdkPixbuf * icon )
{
    (*this_).context = DATA_TABLE_FEATURE;
    (*this_).type.feature = type;
    (*this_).name = name;
    (*this_).icon = icon;
}

static inline void gui_type_resource_init_relationship ( gui_type_resource_t *this_,
                                                         data_relationship_type_t type,
                                                         const char * name,
                                                         const GdkPixbuf * icon )
{
    (*this_).context = DATA_TABLE_RELATIONSHIP;
    (*this_).type.relationship = type;
    (*this_).name = name;
    (*this_).icon = icon;
}

static inline void gui_type_resource_destroy ( gui_type_resource_t *this_ )
{
}

static inline data_table_t gui_type_resource_get_context ( const gui_type_resource_t *this_ )
{
    return ( (*this_).context );
}

static inline union gui_type_resource_union gui_type_resource_get_type ( const gui_type_resource_t *this_ )
{
    return ( (*this_).type );
}

static inline const char * gui_type_resource_get_name ( const gui_type_resource_t *this_ )
{
    return ( (*this_).name );
}

static inline const GdkPixbuf * gui_type_resource_get_icon ( const gui_type_resource_t *this_ )
{
    return ( (*this_).icon );
}


/*
Copyright 2020-2020 Andreas Warnke

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
