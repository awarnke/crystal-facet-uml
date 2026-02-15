/* File: universal_array_index_iterator.h; Copyright and License: see below */

#include <assert.h>

static inline void universal_array_index_iterator_init( universal_array_index_iterator_t *this_,
                                                        const universal_array_index_sorter_t *index_list )
{
    (*this_).next_index = 0;
    (*this_).index_list = index_list;
}

static inline void universal_array_index_iterator_reset( universal_array_index_iterator_t *this_ )
{
    (*this_).next_index = 0;
}

static inline void universal_array_index_iterator_destroy( universal_array_index_iterator_t *this_ )
{
    (*this_).index_list = NULL;
}

static inline bool universal_array_index_iterator_has_next( const universal_array_index_iterator_t *this_ )
{
    return (*this_).next_index < universal_array_index_sorter_get_count( (*this_).index_list );
}

static inline uint32_t universal_array_index_iterator_next( universal_array_index_iterator_t *this_ )
{
    uint32_t result_index = universal_array_index_sorter_get_array_index( (*this_).index_list, (*this_).next_index );
    (*this_).next_index ++;
    return result_index;
}


/*
 * Copyright 2026-2026 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
