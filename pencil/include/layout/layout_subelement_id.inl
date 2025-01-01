/* File: layout_subelement_id.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include <assert.h>

static inline void layout_subelement_id_init( layout_subelement_id_t *this_ ,
                                              layout_subelement_kind_t kind,
                                              const data_full_id_t *id )
{
    (*this_).kind = kind;
    data_full_id_copy( &((*this_).id), id );
}

static inline void layout_subelement_id_init_void( layout_subelement_id_t *this_ )
{
    (*this_).kind = LAYOUT_SUBELEMENT_KIND_VOID;
    data_full_id_init_void( &((*this_).id) );
}

static inline void layout_subelement_id_reinit( layout_subelement_id_t *this_,
                                                layout_subelement_kind_t kind,
                                                const data_full_id_t *id )
{
    (*this_).kind = kind;
    data_full_id_replace( &((*this_).id ), id );
}

static inline void layout_subelement_id_init_solo( layout_subelement_id_t *this_,
                                                   layout_subelement_kind_t kind,
                                                   const data_id_t *id )
{
    (*this_).kind = kind;
    data_full_id_init_solo( &((*this_).id ), id );
}

static inline void layout_subelement_id_reinit_solo( layout_subelement_id_t *this_,
                                                     layout_subelement_kind_t kind,
                                                     const data_id_t *id )
{
    (*this_).kind = kind;
    data_full_id_reinit_solo( &((*this_).id ), id );
}

static inline void layout_subelement_id_copy( layout_subelement_id_t *this_, const layout_subelement_id_t *that )
{
    (*this_).kind = (*that).kind;
    data_full_id_copy( &((*this_).id), &((*that).id) );
}

static inline void layout_subelement_id_replace( layout_subelement_id_t *this_, const layout_subelement_id_t *that )
{
    (*this_).kind = (*that).kind;
    data_full_id_replace( &((*this_).id), &((*that).id) );
}

static inline void layout_subelement_id_destroy( layout_subelement_id_t *this_ )
{
    data_full_id_destroy( &((*this_).id) );
}

static inline bool layout_subelement_id_is_valid( const layout_subelement_id_t *this_ )
{
    return (( (*this_).kind != LAYOUT_SUBELEMENT_KIND_VOID )&&( data_full_id_is_valid( &((*this_).id) ) ));
}

static inline layout_subelement_kind_t layout_subelement_id_get_kind( const layout_subelement_id_t *this_ )
{
    return (*this_).kind;
}

static inline const data_full_id_t *layout_subelement_id_get_id_const( const layout_subelement_id_t *this_ )
{
    return &((*this_).id);
}


/*
Copyright 2024-2025 Andreas Warnke

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
