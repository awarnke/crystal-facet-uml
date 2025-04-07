/* File: layout_feature_iter.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include <assert.h>

static inline void layout_feature_iter_init( layout_feature_iter_t *this_,
                                             layout_visible_set_t *items,
                                             const universal_array_index_sorter_t *order )
{
    (*this_).next_idx = 0;
    (*this_).items = items;
    (*this_).order = order;
}

static inline void layout_feature_iter_destroy( layout_feature_iter_t *this_ )
{
    (*this_).next_idx = 0;
    (*this_).items = NULL;
    (*this_).order = NULL;
}

static inline bool layout_feature_iter_has_next( const layout_feature_iter_t *this_ )
{
    return ( (*this_).next_idx < universal_array_index_sorter_get_count( (*this_).order ) );
}

static inline layout_feature_t *layout_feature_iter_next_ptr( layout_feature_iter_t *this_ )
{
    layout_feature_t *result = NULL;
    if ( layout_feature_iter_has_next( this_ ) )
    {
        uint32_t array_index = universal_array_index_sorter_get_array_index( (*this_).order, (*this_).next_idx );
        assert( array_index < layout_visible_set_get_feature_count( (*this_).items ) );
        result = layout_visible_set_get_feature_ptr( (*this_).items, array_index );
        (*this_).next_idx ++;
    }
    return result;
}


/*
Copyright 2025-2025 Andreas Warnke

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
