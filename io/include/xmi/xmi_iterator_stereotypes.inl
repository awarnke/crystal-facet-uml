/* File: xmi_iterator_stereotypes.inl; Copyright and License: see below */

#include <assert.h>

void xmi_iterator_stereotypes_init ( xmi_iterator_stereotypes_t *this_, utf8string_t stereotype_list )
{
    assert( stereotype_list != UTF8STRING_NULL );
    (*this_).remaining = stereotype_list;
    xmi_iterator_stereotypes_private_step_to_next( this_ );
}

void xmi_iterator_stereotypes_destroy ( xmi_iterator_stereotypes_t *this_ )
{
}

bool xmi_iterator_stereotypes_has_next ( const xmi_iterator_stereotypes_t *this_ )
{
    return ( utf8stringview_get_length( (*this_).next ) != 0 );
}

utf8stringview_t xmi_iterator_stereotypes_next ( xmi_iterator_stereotypes_t *this_ )
{
    utf8stringview_t result = (*this_).next;
    xmi_iterator_stereotypes_private_step_to_next( this_ );
    return result;
}

void xmi_iterator_stereotypes_private_step_to_next ( xmi_iterator_stereotypes_t *this_ )
{
    /* TODO */
    utf8stringview_t noval = UTF8STRINGVIEW(NULL,0);
    (*this_).next = noval;
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
