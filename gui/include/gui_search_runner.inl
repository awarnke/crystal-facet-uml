/* File: gui_search_runner.inl; Copyright and License: see below */

static inline const pos_scroll_page_t* gui_search_runner_get_page_request( const gui_search_runner_t *this_ )
{
    return &((*this_).page_request);
}

static inline const data_search_result_list_t* gui_search_runner_get_result_list( const gui_search_runner_t *this_ )
{
    return &((*this_).result_list);
}

static inline uint32_t gui_search_runner_get_result_buffer_start( const gui_search_runner_t *this_ )
{
    return (*this_).result_buffer_start;
}

static inline bool gui_search_runner_get_result_buffer_more_after( const gui_search_runner_t *this_ )
{
    return (*this_).result_buffer_more_after;
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
