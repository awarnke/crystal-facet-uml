/* File: gui_sketch_location_thing.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include <assert.h>

static inline void gui_sketch_location_thing_init( gui_sketch_location_thing_t *this_ ,
                                                   gui_sketch_location_thing_kind_t kind,
                                                   const data_full_id_t *id )
{
    (*this_).kind = kind;
    data_full_id_copy( &((*this_).id), id );
}

static inline void gui_sketch_location_thing_init_void( gui_sketch_location_thing_t *this_ )
{
    (*this_).kind = GUI_SKETCH_LOCATION_THING_KIND_VOID;
    data_full_id_init_void( &((*this_).id) );
}

static inline void gui_sketch_location_thing_reinit( gui_sketch_location_thing_t *this_,
                                                     gui_sketch_location_thing_kind_t kind,
                                                     const data_full_id_t *id )
{
    (*this_).kind = kind;
    data_full_id_replace( &((*this_).id ), id );
}

static inline void gui_sketch_location_thing_init_solo( gui_sketch_location_thing_t *this_,
                                                        gui_sketch_location_thing_kind_t kind,
                                                        const data_id_t *id )
{
    (*this_).kind = kind;
    data_full_id_init_solo( &((*this_).id ), *id );
}

static inline void gui_sketch_location_thing_reinit_solo( gui_sketch_location_thing_t *this_,
                                                          gui_sketch_location_thing_kind_t kind,
                                                          const data_id_t *id )
{
    (*this_).kind = kind;
    data_full_id_reinit_solo( &((*this_).id ), *id );
}

static inline void gui_sketch_location_thing_copy( gui_sketch_location_thing_t *this_, const gui_sketch_location_thing_t *that )
{
    (*this_).kind = (*that).kind;
    data_full_id_copy( &((*this_).id), &((*that).id) );
}

static inline void gui_sketch_location_thing_replace( gui_sketch_location_thing_t *this_, const gui_sketch_location_thing_t *that )
{
    (*this_).kind = (*that).kind;
    data_full_id_replace( &((*this_).id), &((*that).id) );
}

static inline void gui_sketch_location_thing_destroy( gui_sketch_location_thing_t *this_ )
{
    data_full_id_destroy( &((*this_).id) );
}

static inline bool gui_sketch_location_thing_is_valid( const gui_sketch_location_thing_t *this_ )
{
    return (( (*this_).kind != GUI_SKETCH_LOCATION_THING_KIND_VOID )&&( data_full_id_is_valid( &((*this_).id) ) ));
}

static inline gui_sketch_location_thing_kind_t gui_sketch_location_thing_get_kind( const gui_sketch_location_thing_t *this_ )
{
    return (*this_).kind;
}

static inline const data_full_id_t *gui_sketch_location_thing_get_id_const( const gui_sketch_location_thing_t *this_ )
{
    return &((*this_).id);
}


/*
Copyright 2024-2024 Andreas Warnke

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
