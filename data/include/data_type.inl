/* File: data_type.inl; Copyright and License: see below */

static inline void data_type_init_void ( data_type_t *this_ )
{
    (*this_).context = DATA_TABLE_VOID;
    (*this_).specific_type = -1;
}

static inline void data_type_init_diagram ( data_type_t *this_, data_diagram_type_t type )
{
    (*this_).context = DATA_TABLE_DIAGRAM;
    (*this_).specific_type = (int) type;
}

static inline void data_type_init_classifier ( data_type_t *this_, data_classifier_type_t type )
{
    (*this_).context = DATA_TABLE_CLASSIFIER;
    (*this_).specific_type = (int) type;
}

static inline void data_type_init_feature ( data_type_t *this_, data_feature_type_t type )
{
    (*this_).context = DATA_TABLE_FEATURE;
    (*this_).specific_type = (int) type;
}

static inline void data_type_init_relationship ( data_type_t *this_, data_relationship_type_t type )
{
    (*this_).context = DATA_TABLE_RELATIONSHIP;
    (*this_).specific_type = (int) type;
}

static inline void data_type_destroy ( data_type_t *this_ )
{
}

static inline data_table_t data_type_get_context ( const data_type_t *this_ )
{
    return ( (*this_).context );
}

static inline int data_type_get_type_as_int ( const data_type_t *this_ )
{
    return ( (*this_).specific_type );
}

static inline const data_diagram_type_t data_type_get_diagram_type ( const data_type_t *this_ )
{
    return ( ((*this_).context == DATA_TABLE_DIAGRAM)
        ? (data_diagram_type_t) (*this_).specific_type
        : DATA_DIAGRAM_TYPE_VOID );
}

static inline data_classifier_type_t data_type_get_classifier_type ( const data_type_t *this_ )
{
    return ( ((*this_).context == DATA_TABLE_CLASSIFIER)
        ? (data_diagram_type_t) (*this_).specific_type
        : DATA_CLASSIFIER_TYPE_VOID );
}

static inline data_feature_type_t data_type_get_feature_type ( const data_type_t *this_ )
{
    return ( ((*this_).context == DATA_TABLE_FEATURE)
        ? (data_diagram_type_t) (*this_).specific_type
        : DATA_FEATURE_TYPE_VOID );
}

static inline data_relationship_type_t data_type_get_relationship_type ( const data_type_t *this_ )
{
    return ( ((*this_).context == DATA_TABLE_RELATIONSHIP)
        ? (data_diagram_type_t) (*this_).specific_type
        : DATA_RELATIONSHIP_TYPE_VOID );
}


/*
Copyright 2023-2024 Andreas Warnke

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
