/* File: utf8stringviewiterator.inl; Copyright and License: see below */

#include <assert.h>

void utf8stringviewiterator_init ( utf8stringviewiterator_t *this_, utf8stringview_t element_list, utf8string_t separator )
{
    assert( utf8stringview_get_start( element_list ) != NULL );
    assert( separator != UTF8STRING_NULL );
    assert( utf8string_get_length( separator ) != 0 );
    (*this_).remaining = element_list;
    (*this_).separator = separator;
    utf8stringviewiterator_private_step_to_next( this_ );
    (*this_).has_next = true;  /* even an empty string contains one empty substring*/
}

void utf8stringviewiterator_destroy ( utf8stringviewiterator_t *this_ )
{
}

bool utf8stringviewiterator_has_next ( const utf8stringviewiterator_t *this_ )
{
    return (*this_).has_next;
}

utf8stringview_t utf8stringviewiterator_next ( utf8stringviewiterator_t *this_ )
{
    utf8stringview_t result = (*this_).next;
    utf8stringviewiterator_private_step_to_next( this_ );
    return result;
}

void utf8stringviewiterator_private_step_to_next ( utf8stringviewiterator_t *this_ )
{
    const size_t remaining_len = utf8stringview_get_length( (*this_).remaining );
    if ( remaining_len == 0 )
    {
        (*this_).has_next = false;
        (*this_).next = UTF8STRINGVIEW_NULL;
    }
    else
    {
        (*this_).has_next = true;
        const int next_sep = utf8stringview_find_first_str( (*this_).remaining, (*this_).separator );
        if ( next_sep == -1 )
        {
            (*this_).next = (*this_).remaining;
            (*this_).remaining = UTF8STRINGVIEW_NULL;
        }
        else
        {
            const size_t separator_len = utf8string_get_length( (*this_).separator );
            (*this_).next = utf8stringview_init_region( utf8stringview_get_start( (*this_).remaining ), 
                                                        0, 
                                                        next_sep 
                                                      );
            (*this_).remaining = utf8stringview_init_region( utf8stringview_get_start( (*this_).remaining ), 
                                                             (next_sep + separator_len ),
                                                             (remaining_len - next_sep - separator_len )
                                                           );
        }
    }
}


/*
Copyright 2021-2021 Andreas Warnke

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
