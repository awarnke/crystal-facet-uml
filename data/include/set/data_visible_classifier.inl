/* File: data_visible_classifier.inl; Copyright and License: see below */

static inline void data_visible_classifier_init_empty ( data_visible_classifier_t *this_ )
{
    data_classifier_init_empty( &((*this_).classifier) );
    data_diagramelement_init_empty( &((*this_).diagramelement) );
}

static inline void data_visible_classifier_destroy ( data_visible_classifier_t *this_ )
{
    data_classifier_destroy( &((*this_).classifier) );
    data_diagramelement_destroy( &((*this_).diagramelement) );
}

static inline data_classifier_t *data_visible_classifier_get_classifier_ptr ( data_visible_classifier_t *this_ )
{
    return &((*this_).classifier);
}

static inline const data_classifier_t *data_visible_classifier_get_classifier_const ( const data_visible_classifier_t *this_ )
{
    return &((*this_).classifier);
}

static inline data_diagramelement_t *data_visible_classifier_get_diagramelement_ptr ( data_visible_classifier_t *this_ )
{
    return &((*this_).diagramelement);
}

static inline const data_diagramelement_t *data_visible_classifier_get_diagramelement_const ( const data_visible_classifier_t *this_ )
{
    return &((*this_).diagramelement);
}

static inline bool data_visible_classifier_is_valid ( const data_visible_classifier_t *this_ )
{
    bool result;
    result = data_classifier_is_valid( &((*this_).classifier) ) && data_diagramelement_is_valid( &((*this_).diagramelement) );
    return result;
}


/*
Copyright 2016-2021 Andreas Warnke

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
