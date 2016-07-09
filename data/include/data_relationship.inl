/* File: data_relationship.inl; Copyright and License: see below */

#include "data_id.h"

static inline void data_relationship_init_empty ( data_relationship_t *this_ )
{
    (*this_).id = DATA_ID_VOID_ID;
    (*this_).from_classifier_id = DATA_ID_VOID_ID;
    (*this_).to_classifier_id = DATA_ID_VOID_ID;
    (*this_).main_type = DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY;

    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    utf8stringbuf_clear( (*this_).name );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    utf8stringbuf_clear( (*this_).description );

    (*this_).list_order = 0;
}

static inline void data_relationship_reinit_empty ( data_relationship_t *this_ )
{
    /* data_relationship_destroy( this_ );  -- not necessary */
    data_relationship_init_empty( this_ );
}

static inline void data_relationship_destroy ( data_relationship_t *this_ )
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
