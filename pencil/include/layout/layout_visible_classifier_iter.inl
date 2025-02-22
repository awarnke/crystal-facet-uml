/* File: layout_visible_classifier_iter.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include <assert.h>

static inline void layout_visible_classifier_iter_init( layout_visible_classifier_iter_t *this_,
                                                        const universal_array_list_t *items,
                                                        const universal_array_index_sorter_t *order )
{
    (*this_).next_idx = 0;
    (*this_).items = items;
    (*this_).order = order;
}

static inline void layout_visible_classifier_iter_destroy( layout_visible_classifier_iter_t *this_ )
{
    (*this_).next_idx = 0;
    (*this_).items = NULL;
    (*this_).order = NULL;
}

static inline bool layout_visible_classifier_iter_has_next( const layout_visible_classifier_iter_t *this_ )
{
    return ( (*this_).next_idx < universal_array_index_sorter_get_count( (*this_).order ) );
}

static inline const layout_visible_classifier_t *layout_visible_classifier_iter_next_const( layout_visible_classifier_iter_t *this_ )
{
    const layout_visible_classifier_t *result = NULL;
    if ( layout_visible_classifier_iter_has_next( this_ ) )
    {
        uint32_t array_index = universal_array_index_sorter_get_array_index( (*this_).order, (*this_).next_idx );
        assert( array_index <= universal_array_list_get_length( (*this_).items ) );
        result = (const layout_visible_classifier_t*) universal_array_list_get_const( (*this_).items, array_index );
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
