/* File: data_id.inl; Copyright and License: see below */

#include "trace.h"
#include "tslog.h"
#include "util/string/utf8string.h"

static inline void data_id_init ( data_id_t *this_, data_table_t table, data_row_id_t row_id )
{
    (*this_).table = table;
    (*this_).row_id = row_id;
}

static inline void data_id_init_by_string ( data_id_t *this_, const char* string_to_parse )
{
    size_t dummy_length;
    data_id_init_by_string_region ( this_, string_to_parse, 0, &dummy_length );
}

static inline void data_id_init_by_string_region ( data_id_t *this_,
                                                   const char* string_to_parse,
                                                   size_t start,
                                                   size_t *out_length )
{
    if ( string_to_parse == NULL )
    {
        (*this_).table = DATA_TABLE_VOID;
        (*this_).row_id = DATA_ROW_ID_VOID;
    }
    else
    {
        switch ( string_to_parse[start] )
        {
            case DATA_TABLE_ALPHANUM_CLASSIFIER:
            {
                (*this_).table = DATA_TABLE_CLASSIFIER;
            }
            break;

            case DATA_TABLE_ALPHANUM_FEATURE:
            {
                (*this_).table = DATA_TABLE_FEATURE;
            }
            break;

            case DATA_TABLE_ALPHANUM_RELATIONSHIP:
            {
                (*this_).table = DATA_TABLE_RELATIONSHIP;
            }
            break;

            case DATA_TABLE_ALPHANUM_DIAGRAMELEMENT:
            {
                (*this_).table = DATA_TABLE_DIAGRAMELEMENT;
            }
            break;

            case DATA_TABLE_ALPHANUM_DIAGRAM:
            {
                (*this_).table = DATA_TABLE_DIAGRAM;
            }
            break;

            default:
            {
                (*this_).table = DATA_TABLE_VOID;
            }
            break;
        }

        if ( (*this_).table == DATA_TABLE_VOID )
        {
            (*this_).row_id = DATA_ROW_ID_VOID;
        }
        else
        {
            const char* int_to_parse = string_to_parse+start+1;
            unsigned int byte_length;
            int64_t number;
            utf8error_t u8err = utf8string_parse_int( int_to_parse, &byte_length, &number );
            if (( u8err == UTF8ERROR_SUCCESS )&&( byte_length >= 4 )) {
                (*this_).row_id = number;
                *out_length = byte_length+1;
            }
            else {
                (*this_).table = DATA_TABLE_VOID;
                (*this_).row_id = DATA_ROW_ID_VOID;
                *out_length = 0;
            }
        }
    }
}

static inline void data_id_reinit ( data_id_t *this_, data_table_t table, data_row_id_t row_id )
{
    (*this_).table = table;
    (*this_).row_id = row_id;
}

static inline void data_id_init_void ( data_id_t *this_ )
{
    (*this_).table = DATA_TABLE_VOID;
    (*this_).row_id = DATA_ROW_ID_VOID;
}

static inline void data_id_reinit_void ( data_id_t *this_ )
{
    (*this_).table = DATA_TABLE_VOID;
    (*this_).row_id = DATA_ROW_ID_VOID;
}

static inline void data_id_copy ( data_id_t *this_, const data_id_t *that )
{
    (*this_) = (*that);
}

static inline void data_id_replace ( data_id_t *this_, const data_id_t *that )
{
    (*this_) = (*that);
}

static inline void data_id_destroy ( data_id_t *this_ )
{
    (*this_).table = DATA_TABLE_VOID;
    (*this_).row_id = DATA_ROW_ID_VOID;
}

static inline data_table_t data_id_get_table ( const data_id_t *this_ )
{
    return (*this_).table;
}

static inline data_row_id_t data_id_get_row_id ( const data_id_t *this_ )
{
    return (*this_).row_id;
}

static inline void data_id_trace ( const data_id_t *this_ )
{
    const char prefix[] = "data_id_t: ";
    char id_buf[sizeof(prefix)+DATA_ID_MAX_UTF8STRING_LENGTH];
    utf8stringbuf_t id_str = UTF8STRINGBUF( id_buf );
    utf8stringbuf_copy_str( id_str, prefix );
    data_id_to_utf8stringbuf( this_, id_str );
    TRACE_INFO( utf8stringbuf_get_string( id_str ) );
}

static inline bool data_id_equals ( const data_id_t *this_, const data_id_t *that )
{
    return ( ( DATA_ROW_ID_VOID != (*this_).row_id )&&( DATA_TABLE_VOID != (*this_).table )
             &&( (*this_).row_id == (*that).row_id )&&( (*this_).table == (*that).table ) );
}

static inline bool data_id_equals_or_both_void ( const data_id_t *this_, const data_id_t *that )
{
    const bool one_or_both_valid = data_id_is_valid( this_ ) || data_id_is_valid( that );
    return ( ! one_or_both_valid ) || (( (*this_).row_id == (*that).row_id )&&( (*this_).table == (*that).table ));
}

static inline bool data_id_is_valid ( const data_id_t *this_ )
{
    return (( DATA_ROW_ID_VOID != (*this_).row_id )&&( DATA_TABLE_VOID != (*this_).table ));
}

static inline bool data_id_equals_id ( const data_id_t *this_, data_table_t table, data_row_id_t row_id )
{
    return ( ( DATA_ROW_ID_VOID != (*this_).row_id )&&( DATA_TABLE_VOID != (*this_).table )
    &&( (*this_).row_id == row_id )&&( (*this_).table == table ) );
}

static inline utf8error_t data_id_to_utf8stringbuf ( const data_id_t *this_, utf8stringbuf_t out_str )
{
    utf8error_t result = UTF8ERROR_SUCCESS;
    switch ( (*this_).table )
    {
        case DATA_TABLE_VOID:
        {
            result |= utf8stringbuf_append_str( out_str, "void" );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            result |= utf8stringbuf_append_str( out_str, "C" );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            result |= utf8stringbuf_append_str( out_str, "F" );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            result |= utf8stringbuf_append_str( out_str, "R" );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            result |= utf8stringbuf_append_str( out_str, "E" );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            result |= utf8stringbuf_append_str( out_str, "D" );
        }
        break;

        default:
        {
            TSLOG_ERROR( "data_id_to_utf8stringbuf has incomplete switch on data_table_t" );
        }
        break;
    }

    if ( (*this_).table != DATA_TABLE_VOID )
    {
        if ( 100 > (*this_).row_id )
        {
            if ( 10 > (*this_).row_id )
            {
                if ( 0 <= (*this_).row_id )
                {
                    result |= utf8stringbuf_append_str( out_str, "000" );
                }
                else
                {
                    /* row_id is negative */
                }
            }
            else
            {
                result |= utf8stringbuf_append_str( out_str, "00" );
            }
        }
        else
        {
            if ( 1000 > (*this_).row_id )
            {
                result |= utf8stringbuf_append_str( out_str, "0" );
            }
            else
            {
                /* row_id is greater than 1000 */
            }
        }
        result |= utf8stringbuf_append_int( out_str, (*this_).row_id );
    }

    return result;
}


/*
Copyright 2016-2021 Andreas Warnke

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
