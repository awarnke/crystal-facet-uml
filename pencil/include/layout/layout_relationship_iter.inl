/* File: layout_relationship_iter.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include <assert.h>

static inline void layout_relationship_iter_init( layout_relationship_iter_t *this_,
                                                  layout_visible_set_t *items,
                                                  const universal_array_index_sorter_t *order )
{
    assert( items != NULL );
    assert( order != NULL );
    (*this_).next_idx = 0;
    (*this_).length = universal_array_index_sorter_get_count( order );
    (*this_).items = items;
    (*this_).order = order;
}

static inline void layout_relationship_iter_init_from_processed( layout_relationship_iter_t *this_,
                                                                 const layout_relationship_iter_t *that )
{
    (*this_).next_idx = 0;
    (*this_).length = (*that).next_idx;
    (*this_).items = (*that).items;
    (*this_).order = (*that).order;
}

static inline void layout_relationship_iter_copy( layout_relationship_iter_t *this_,
                                                  const layout_relationship_iter_t *original )
{
    (*this_).next_idx = (*original).next_idx;
    (*this_).length = (*original).length;
    (*this_).items = (*original).items;
    (*this_).order = (*original).order;
}

static inline void layout_relationship_iter_destroy( layout_relationship_iter_t *this_ )
{
    (*this_).next_idx = 0;
    (*this_).length = 0;
    (*this_).items = NULL;
    (*this_).order = NULL;
}

static inline bool layout_relationship_iter_has_next( const layout_relationship_iter_t *this_ )
{
    /* check that the size of the array has not shrinked */
    /* It should not have changed at all, but this cannot be checked here. */
    assert( (*this_).length <= universal_array_index_sorter_get_count( (*this_).order ) );

    return ( (*this_).next_idx < (*this_).length );
}

static inline layout_relationship_t *layout_relationship_iter_next_ptr( layout_relationship_iter_t *this_ )
{
    layout_relationship_t *result = NULL;
    if ( layout_relationship_iter_has_next( this_ ) )
    {
        uint32_t array_index = universal_array_index_sorter_get_array_index( (*this_).order, (*this_).next_idx );
        assert( array_index < layout_visible_set_get_relationship_count( (*this_).items ) );
        result = layout_visible_set_get_relationship_ptr( (*this_).items, array_index );
        (*this_).next_idx ++;
    }
    return result;
}


/*
Copyright 2025-2026 Andreas Warnke

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
