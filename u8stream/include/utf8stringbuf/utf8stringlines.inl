/* File: utf8stringlines.inl; Copyright and License: see below */

#ifdef __cplusplus
extern "C" {
#endif

static inline void utf8stringlines_init ( utf8stringlines_t *this_, const utf8stringview_t *lines_list, uint32_t line_length )
{
    assert( lines_list != NULL );
    (*this_).remaining = *lines_list;
    (*this_).line_length = line_length;
    (*this_).next_is_end = false;
    (*this_).has_next = true;
    utf8stringlines_private_step_to_next( this_ );
}

static inline void utf8stringlines_destroy ( utf8stringlines_t *this_ )
{
}

static inline bool utf8stringlines_has_next ( const utf8stringlines_t *this_ )
{
    return (*this_).has_next;
}

static inline utf8stringview_t utf8stringlines_next ( utf8stringlines_t *this_ )
{
    utf8stringview_t result = (*this_).next;
    utf8stringlines_private_step_to_next( this_ );
    return result;
}

static inline void utf8stringlines_private_step_to_next ( utf8stringlines_t *this_ )
{
    if ( (*this_).next_is_end )
    {
        (*this_).has_next = false;
        (*this_).next = UTF8STRINGVIEW_EMPTY;
    }
    else
    {
        utf8stringview_t before = UTF8STRINGVIEW_EMPTY;
        utf8stringview_t after = UTF8STRINGVIEW_EMPTY;
        const utf8error_t has_next
            = utf8stringview_split_at_first_str( &((*this_).remaining), "\n", &before, &after );
        if ( has_next != UTF8ERROR_SUCCESS )
        {
            (*this_).next_is_end = true;
            (*this_).next = (*this_).remaining;
            (*this_).remaining = UTF8STRINGVIEW_EMPTY;
        }
        else
        {
            (*this_).next = before;
            (*this_).remaining = after;
        }
    }
}

#ifdef __cplusplus
}
#endif


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
