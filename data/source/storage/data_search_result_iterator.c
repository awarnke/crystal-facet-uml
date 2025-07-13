/* File: data_search_result_iterator.c; Copyright and License: see below */

#include "storage/data_search_result_iterator.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>
#include <stdint.h>

/*!
 *  \brief predefined search statement to find TODO
 */
const char *const DATA_SEARCH_RESULT_ITERATOR_SELECT_BY_DIAGRAM_ID = "";

u8_error_t data_search_result_iterator_init_empty ( data_search_result_iterator_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    data_database_borrowed_stmt_init_void( &((*this_).diagram_statement) );
    data_database_borrowed_stmt_init_void( &((*this_).classifier_statement) );
    data_database_borrowed_stmt_init_void( &((*this_).feature_statement) );
    data_database_borrowed_stmt_init_void( &((*this_).relationship_statement) );
    (*this_).is_at_diagram_end = true;
    (*this_).is_at_classifier_end = true;
    (*this_).is_at_feature_end = true;
    (*this_).is_at_relathiship_end = true;

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_search_result_iterator_reinit ( data_search_result_iterator_t *this_,
                                                data_database_borrowed_stmt_t diagram_statement,
                                                data_database_borrowed_stmt_t classifier_statement,
                                                data_database_borrowed_stmt_t feature_statement,
                                                data_database_borrowed_stmt_t relationship_statement )
{
    U8_TRACE_BEGIN();
    assert( data_database_borrowed_stmt_is_valid( &diagram_statement ) );
    assert( data_database_borrowed_stmt_is_valid( &classifier_statement ) );
    assert( data_database_borrowed_stmt_is_valid( &feature_statement ) );
    assert( data_database_borrowed_stmt_is_valid( &relationship_statement ) );
    u8_error_t result = U8_ERROR_NONE;

    /* destroy old state */
    result = data_search_result_iterator_destroy( this_ );

    /* init new state */
    (*this_).diagram_statement = diagram_statement;
    (*this_).classifier_statement = classifier_statement;
    (*this_).feature_statement = feature_statement;
    (*this_).relationship_statement = relationship_statement;
    (*this_).is_at_diagram_end = false;
    (*this_).is_at_classifier_end = false;
    (*this_).is_at_feature_end = false;
    (*this_).is_at_relathiship_end = false;
    result |= data_search_result_iterator_private_step_to_next( this_ );

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_search_result_iterator_destroy ( data_search_result_iterator_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    result |= data_database_borrowed_stmt_destroy( &((*this_).diagram_statement) );
    result |= data_database_borrowed_stmt_destroy( &((*this_).classifier_statement) );
    result |= data_database_borrowed_stmt_destroy( &((*this_).feature_statement) );
    result |= data_database_borrowed_stmt_destroy( &((*this_).relationship_statement) );
    (*this_).is_at_diagram_end = true;
    (*this_).is_at_classifier_end = true;
    (*this_).is_at_feature_end = true;
    (*this_).is_at_relathiship_end = true;

    U8_TRACE_END_ERR(result);
    return result;
}

bool data_search_result_iterator_has_next ( const data_search_result_iterator_t *this_ )
{
    const bool finished = (*this_).is_at_diagram_end && (*this_).is_at_classifier_end
                          && (*this_).is_at_feature_end && (*this_).is_at_relathiship_end;
    return ( ! finished );
}

u8_error_t data_search_result_iterator_next ( data_search_result_iterator_t *this_, data_search_result_t *out_search_result )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_search_result );
    assert( data_database_borrowed_stmt_is_valid( &((*this_).diagram_statement) ) );
    assert( data_database_borrowed_stmt_is_valid( &((*this_).classifier_statement) ) );
    assert( data_database_borrowed_stmt_is_valid( &((*this_).feature_statement) ) );
    assert( data_database_borrowed_stmt_is_valid( &((*this_).relationship_statement) ) );
    u8_error_t result = U8_ERROR_NONE;


    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_search_result_iterator_private_step_to_next ( data_search_result_iterator_t *this_ )
{
    U8_TRACE_BEGIN();
    assert( data_database_borrowed_stmt_is_valid( &((*this_).diagram_statement) ) );
    assert( data_database_borrowed_stmt_is_valid( &((*this_).classifier_statement) ) );
    assert( data_database_borrowed_stmt_is_valid( &((*this_).feature_statement) ) );
    assert( data_database_borrowed_stmt_is_valid( &((*this_).relationship_statement) ) );
    u8_error_t result = U8_ERROR_NONE;


    U8_TRACE_END_ERR(result);
    return result;
}


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
