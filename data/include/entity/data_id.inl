/* File: data_id.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "utf8stringbuf/utf8string.h"

static inline void data_id_init ( data_id_t *this_, data_table_t table, data_row_t row_id )
{
    (*this_).table = table;
    (*this_).row_id = row_id;
}

static inline void data_id_init_by_string ( data_id_t *this_, const char* string_to_parse )
{
    assert( string_to_parse != NULL );

    utf8stringview_t parse_me;
    utf8stringview_t ignore_me = UTF8STRINGVIEW_EMPTY;
    utf8stringview_init_str( &parse_me, string_to_parse );
    data_id_init_by_stringview( this_, &parse_me, &ignore_me );
    if ( utf8stringview_get_length( &ignore_me ) != 0 )
    {
        /* error: there are additional characters */
        data_id_reinit_void( this_ );
    }
    utf8stringview_destroy( &parse_me );
    utf8stringview_destroy( &ignore_me );
}

static inline void data_id_init_by_stringview ( data_id_t *this_,
                                                const utf8stringview_t* string_to_parse,
                                                utf8stringview_t *out_remainder )



{
    assert( string_to_parse != NULL );
    assert( out_remainder != NULL );

    if ( utf8stringview_get_length( string_to_parse ) == 0 )
    {
        (*this_).table = DATA_TABLE_VOID;
        (*this_).row_id = DATA_ROW_VOID;
        *out_remainder = UTF8STRINGVIEW_EMPTY;
    }
    else
    {
        const char start = *utf8stringview_get_start( string_to_parse );
        switch ( start )
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
            (*this_).row_id = DATA_ROW_VOID;
            *out_remainder = *string_to_parse;
        }
        else
        {
            const utf8stringview_t int_to_parse
                = UTF8STRINGVIEW( utf8stringview_get_start( string_to_parse )+1, utf8stringview_get_length( string_to_parse )-1 );
            int64_t number;
            const utf8error_t int_err = utf8stringview_parse_int( &int_to_parse, &number, out_remainder );
            const size_t byte_length = utf8stringview_get_length( &int_to_parse ) - utf8stringview_get_length( out_remainder );
            if (( int_err == UTF8ERROR_SUCCESS )&&( byte_length >= 4 )) {
                (*this_).row_id = number;
            }
            else {
                (*this_).table = DATA_TABLE_VOID;
                (*this_).row_id = DATA_ROW_VOID;
                *out_remainder = *string_to_parse;
            }
        }
    }
}

static inline void data_id_reinit ( data_id_t *this_, data_table_t table, data_row_t row_id )
{
    (*this_).table = table;
    (*this_).row_id = row_id;
}

static inline void data_id_init_void ( data_id_t *this_ )
{
    (*this_).table = DATA_TABLE_VOID;
    (*this_).row_id = DATA_ROW_VOID;
}

static inline void data_id_reinit_void ( data_id_t *this_ )
{
    (*this_).table = DATA_TABLE_VOID;
    (*this_).row_id = DATA_ROW_VOID;
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
    (*this_).row_id = DATA_ROW_VOID;
}

static inline data_table_t data_id_get_table ( const data_id_t *this_ )
{
    return (*this_).table;
}

static inline data_row_t data_id_get_row_id ( const data_id_t *this_ )
{
    return (*this_).row_id;
}

static inline void data_id_trace ( const data_id_t *this_ )
{
    const char prefix[] = "data_id_t: ";
    char id_buf[sizeof(prefix)+DATA_ID_MAX_UTF8STRING_LENGTH];
    utf8stringbuf_t id_str = UTF8STRINGBUF( id_buf );
    utf8stringbuf_copy_str( &id_str, prefix );
    data_id_to_utf8stringbuf( this_, id_str );
    U8_TRACE_INFO( utf8stringbuf_get_string( &id_str ) );
}

static inline bool data_id_equals ( const data_id_t *this_, const data_id_t *that )
{
    return ( ( DATA_ROW_VOID != (*this_).row_id )&&( DATA_TABLE_VOID != (*this_).table )
             &&( (*this_).row_id == (*that).row_id )&&( (*this_).table == (*that).table ) );
}

static inline bool data_id_equals_or_both_void ( const data_id_t *this_, const data_id_t *that )
{
    const bool one_or_both_valid = data_id_is_valid( this_ ) || data_id_is_valid( that );
    return ( ! one_or_both_valid ) || (( (*this_).row_id == (*that).row_id )&&( (*this_).table == (*that).table ));
}

static inline bool data_id_is_valid ( const data_id_t *this_ )
{
    return (( DATA_ROW_VOID != (*this_).row_id )&&( DATA_TABLE_VOID != (*this_).table ));
}

static inline bool data_id_equals_id ( const data_id_t *this_, data_table_t table, data_row_t row_id )
{
    return ( ( DATA_ROW_VOID != (*this_).row_id )&&( DATA_TABLE_VOID != (*this_).table )
    &&( (*this_).row_id == row_id )&&( (*this_).table == table ) );
}

static inline utf8error_t data_id_to_utf8stringbuf ( const data_id_t *this_, utf8stringbuf_t out_str )
{
    assert( (*this_).table >= DATA_TABLE_VOID );
    assert( (*this_).table <= DATA_TABLE_DIAGRAM );
    utf8error_t result = UTF8ERROR_SUCCESS;
    switch ( (*this_).table )
    {
        default:
        case DATA_TABLE_VOID:
        {
            result |= utf8stringbuf_append_str( &out_str, "void" );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            result |= utf8stringbuf_append_str( &out_str, "C" );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            result |= utf8stringbuf_append_str( &out_str, "F" );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            result |= utf8stringbuf_append_str( &out_str, "R" );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            result |= utf8stringbuf_append_str( &out_str, "E" );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            result |= utf8stringbuf_append_str( &out_str, "D" );
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
                    result |= utf8stringbuf_append_str( &out_str, "000" );
                }
                else
                {
                    /* row_id is negative */
                }
            }
            else
            {
                result |= utf8stringbuf_append_str( &out_str, "00" );
            }
        }
        else
        {
            if ( 1000 > (*this_).row_id )
            {
                result |= utf8stringbuf_append_str( &out_str, "0" );
            }
            else
            {
                /* row_id is greater than 1000 */
            }
        }
        result |= utf8stringbuf_append_int( &out_str, (*this_).row_id );
    }

    return result;
}

static inline u8_error_t data_id_to_utf8_writer ( const data_id_t *this_, utf8stream_writer_t *out_writer )
{
    assert( (*this_).table >= DATA_TABLE_VOID );
    assert( (*this_).table <= DATA_TABLE_DIAGRAM );
    u8_error_t result = U8_ERROR_NONE;
    switch ( (*this_).table )
    {
        default:
        case DATA_TABLE_VOID:
        {
            result |= utf8stream_writer_write_str( out_writer, "void" );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            result |= utf8stream_writer_write_str( out_writer, "C" );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            result |= utf8stream_writer_write_str( out_writer, "F" );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            result |= utf8stream_writer_write_str( out_writer, "R" );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            result |= utf8stream_writer_write_str( out_writer, "E" );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            result |= utf8stream_writer_write_str( out_writer, "D" );
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
                    result |= utf8stream_writer_write_str( out_writer, "000" );
                }
                else
                {
                    /* row_id is negative */
                }
            }
            else
            {
                result |= utf8stream_writer_write_str( out_writer, "00" );
            }
        }
        else
        {
            if ( 1000 > (*this_).row_id )
            {
                result |= utf8stream_writer_write_str( out_writer, "0" );
            }
            else
            {
                /* row_id is greater than 1000 */
            }
        }
        result |= utf8stream_writer_write_int( out_writer, (*this_).row_id );
    }

    return result;
}


/*
Copyright 2016-2025 Andreas Warnke

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
