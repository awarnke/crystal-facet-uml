/* File: universal_array_index_sorter.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

static inline void universal_array_index_sorter_init( universal_array_index_sorter_t *this_ )
{
    (*this_).entries_count = 0;
}

static inline void universal_array_index_sorter_reinit( universal_array_index_sorter_t *this_ )
{
    (*this_).entries_count = 0;
}

static inline void universal_array_index_sorter_destroy( universal_array_index_sorter_t *this_ )
{
}

static inline int universal_array_index_sorter_insert( universal_array_index_sorter_t *this_, uint32_t array_index, int64_t weight )
{
    assert( (*this_).entries_count <= UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE );
    int result;
    if ( (*this_).entries_count < UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE )
    {
        bool already_inserted = false;
        for ( uint32_t sorted_index = (*this_).entries_count; (sorted_index > 0) && ( ! already_inserted ); sorted_index -- )
        {
            if ( weight < (*this_).weights[sorted_index-1] )
            {
                /* shift */
                (*this_).entries[sorted_index] = (*this_).entries[sorted_index-1];
                (*this_).weights[sorted_index] = (*this_).weights[sorted_index-1];
            }
            else
            {
                /* insert */
                (*this_).entries[sorted_index] = array_index;
                (*this_).weights[sorted_index] = weight;
                already_inserted = true;
            }
        }
        if ( ! already_inserted )
        {
            (*this_).entries[0] = array_index;
            (*this_).weights[0] = weight;
        }
        (*this_).entries_count ++;
        result = 0;
    }
    else
    {
        result = -1;
    }
    return result;
}

static inline uint32_t universal_array_index_sorter_get_count( const universal_array_index_sorter_t *this_ )
{
    return (*this_).entries_count;
}

static inline uint32_t universal_array_index_sorter_get_array_index( const universal_array_index_sorter_t *this_, uint32_t sort_index )
{
    assert( (*this_).entries_count <= UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE );
    assert( sort_index <= (*this_).entries_count );
    return (*this_).entries[sort_index];
}


/*
Copyright 2017-2022 Andreas Warnke

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
