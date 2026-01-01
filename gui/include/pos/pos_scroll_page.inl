/* File: pos_scroll_page.inl; Copyright and License: see below */

#include <u8/u8_trace.h>
#include <assert.h>

static inline const pos_scroll_page_t pos_scroll_page_new ( uint32_t anchor_index,
                                                            bool backwards )
{
    return (pos_scroll_page_t){ .anchor_index = anchor_index, .backwards = backwards };
}

static inline uint32_t pos_scroll_page_get_anchor_index ( const pos_scroll_page_t *this_ )
{
    return (*this_).anchor_index;
}

static inline bool pos_scroll_page_get_backwards ( const pos_scroll_page_t *this_ )
{
    return (*this_).backwards;
}

static inline void pos_scroll_page_trace ( const pos_scroll_page_t *this_ )
{
    U8_TRACE_INFO_INT_INT( "pos_scroll_page_t: ", (*this_).anchor_index, (*this_).backwards );
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
