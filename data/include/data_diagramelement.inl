/* File: data_diagramelement.inl; Copyright and License: see below */

static inline void data_diagramelement_init_empty ( data_diagramelement_t *this_ )
{
    (*this_).id = DATA_ID_VOID_ID;
    (*this_).diagram_id = DATA_ID_VOID_ID;
    (*this_).classifier_id = DATA_ID_VOID_ID;
}

static inline void data_diagramelement_reinit_empty ( data_diagramelement_t *this_ )
{
    /* data_diagramelement_destroy( this_ );  -- not necessary */
    data_diagramelement_init_empty( this_ );
}

static inline void data_diagramelement_init_new ( data_diagramelement_t *this_, int64_t diagram_id, int64_t classifier_id )
{
    (*this_).id = DATA_ID_VOID_ID;
    (*this_).diagram_id = diagram_id;
    (*this_).classifier_id = classifier_id;
}

static inline void data_diagramelement_destroy ( data_diagramelement_t *this_ )
{
    (*this_).id = DATA_ID_VOID_ID;
}


/*
Copyright 2016-2016 Andreas Warnke

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
