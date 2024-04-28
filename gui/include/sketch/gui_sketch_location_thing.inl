/* File: gui_sketch_location_thing.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include <assert.h>

static inline void gui_sketch_location_thing_init( gui_sketch_location_thing_t *this_ ,
                                                   gui_sketch_location_thing_kind_t kind,
                                                   const data_full_id_t *id,
                                                   const data_full_id_t *surrounding_id )
{
    (*this_).kind = kind;
    data_full_id_copy( &((*this_).id ), id );
    data_full_id_copy( &((*this_).surrounding_id ), surrounding_id );
}

static inline void gui_sketch_location_thing_init_void( gui_sketch_location_thing_t *this_ )
{
    (*this_).kind = GUI_SKETCH_LOCATION_THING_KIND_VOID;
    data_full_id_init_void( &((*this_).id ) );
    data_full_id_init_void( &((*this_).surrounding_id ) );
}

static inline void gui_sketch_location_thing_destroy( gui_sketch_location_thing_t *this_ )
{
    data_full_id_destroy( &((*this_).id ) );
    data_full_id_destroy( &((*this_).surrounding_id ) );
}

static inline gui_sketch_location_thing_kind_t gui_sketch_location_thing_get_kind( const gui_sketch_location_thing_t *this_ )
{
    return (*this_).kind;
}

static inline const data_full_id_t *gui_sketch_location_thing_get_id_const( const gui_sketch_location_thing_t *this_ )
{
    return &((*this_).id );
}

static inline const data_full_id_t *gui_sketch_location_thing_get_surrounding_id_const( const gui_sketch_location_thing_t *this_ )
{
    return &((*this_).surrounding_id );
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
