/* File: utf8codepointiterator.inl; Copyright and License: see below */

static inline void utf8codepointiterator_init ( utf8codepointiterator_t *this_, utf8stringview_t string )
{
    (*this_).remaining = string;
    utf8codepointiterator_private_step_to_next( this_ );
}

static inline void utf8codepointiterator_destroy ( utf8codepointiterator_t *this_ )
{
}

static inline bool utf8codepointiterator_has_next ( const utf8codepointiterator_t *this_ )
{
    return utf8codepoint_is_valid( (*this_).next );
}

static inline utf8codepoint_t utf8codepointiterator_next ( utf8codepointiterator_t *this_ )
{
    utf8codepoint_t result = (*this_).next;
    utf8codepointiterator_private_step_to_next( this_ );
    return result;
}

static inline void utf8codepointiterator_private_step_to_next ( utf8codepointiterator_t *this_ )
{
    const size_t remaining_len = utf8stringview_get_length( (*this_).remaining );
    if ( remaining_len == 0 )
    {
        (*this_).next = UTF8CODEPOINT_INVAL_CHAR;
    }
    else
    {
        (*this_).next = utf8codepoint_init( utf8stringview_get_start( (*this_).remaining ), remaining_len );
        if ( utf8codepoint_is_valid( (*this_).next ) )
        {
            const unsigned int next_len = utf8codepoint_get_length( (*this_).next );
            (*this_).remaining = utf8stringview_init_region( utf8stringview_get_start( (*this_).remaining ), 
                                                             next_len,
                                                             (remaining_len - next_len )
                                                           );
        }
        else
        {
            (*this_).remaining = UTF8STRINGVIEW_NULL;
        }
    }
}


/*
Copyright 2021-2023 Andreas Warnke

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
