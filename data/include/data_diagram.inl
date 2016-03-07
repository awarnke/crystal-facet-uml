/* File: data_diagram.inl; Copyright and License: see below */

#include "log.h"
#include "trace.h"

static inline void data_diagram_init_new ( data_diagram_t *this_, int64_t parent_diagram_id, data_diagram_type_t diagram_type, const char* diagram_name, const char* diagram_description )
{
    utf8error_t strerr;

    (*this_).id = DATA_DIAGRAM_ID_NEW_ID;
    (*this_).parent_id = parent_diagram_id;
    (*this_).diagram_type = diagram_type;

    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).name, diagram_name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
    }

    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).description, diagram_description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
    }

    (*this_).list_order = 0;
}

static inline void data_diagram_init_empty ( data_diagram_t *this_ )
{
    (*this_).id = DATA_DIAGRAM_ID_UNINITIALIZED_ID;
    (*this_).parent_id = 0;
    (*this_).diagram_type = DATA_DIAGRAM_TYPE_LIST;

    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    utf8stringbuf_clear( (*this_).name );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    utf8stringbuf_clear( (*this_).description );

    (*this_).list_order = 0;
}

static inline void data_diagram_init ( data_diagram_t *this_, int64_t diagram_id, int64_t parent_diagram_id, data_diagram_type_t diagram_type, const char* diagram_name, const char* diagram_description )
{
    utf8error_t strerr;

    (*this_).id = diagram_id;
    (*this_).parent_id = parent_diagram_id;
    (*this_).diagram_type = diagram_type;

    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).description, diagram_description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
    }
}

static inline void data_diagram_destroy ( data_diagram_t *this_ )
{
    (*this_).id = DATA_DIAGRAM_ID_UNINITIALIZED_ID;
    (*this_).parent_id = DATA_DIAGRAM_ID_UNINITIALIZED_ID;
    (*this_).diagram_type = 0;

    utf8stringbuf_clear( (*this_).name );

    utf8stringbuf_clear( (*this_).description );
}

static inline void data_diagram_trace ( data_diagram_t *this_ )
{
    TRACE_INFO( "data_diagram_t" );
    TRACE_INFO_INT( "- id:", (*this_).id );
    TRACE_INFO_INT( "- parent_id:", (*this_).parent_id );
    TRACE_INFO_INT( "- diagram_type:", (*this_).diagram_type );
    TRACE_INFO_STR( "- name:", utf8stringbuf_get_string((*this_).name) );
    TRACE_INFO_STR( "- description:", utf8stringbuf_get_string((*this_).description) );
    TRACE_INFO_INT( "- list_order:", (*this_).list_order );
}


/*
Copyright 2016-2016 Andreas Warnke

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
