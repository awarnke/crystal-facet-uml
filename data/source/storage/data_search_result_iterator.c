/* File: data_search_result_iterator.c; Copyright and License: see below */

#include "storage/data_search_result_iterator.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>
#include <stdint.h>

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
    (*this_).is_at_relationship_end = true;
    data_search_result_init_void( &((*this_).next_search_result_buf) );

    data_rules_init ( &((*this_).data_rules) );

    (*this_).last_relationship_id = DATA_ROW_VOID;
    (*this_).last_relationship_was_scenario = false;

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
    (*this_).is_at_relationship_end = false;
    data_search_result_init_void( &((*this_).next_search_result_buf) );
    (*this_).last_relationship_id = DATA_ROW_VOID;
    (*this_).last_relationship_was_scenario = false;

    result |= data_search_result_iterator_private_step_to_next( this_ );

    data_rules_init ( &((*this_).data_rules) );

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_search_result_iterator_destroy ( data_search_result_iterator_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    (*this_).last_relationship_id = DATA_ROW_VOID;
    (*this_).last_relationship_was_scenario = false;

    data_rules_destroy ( &((*this_).data_rules) );

    result |= data_database_borrowed_stmt_destroy( &((*this_).diagram_statement) );
    result |= data_database_borrowed_stmt_destroy( &((*this_).classifier_statement) );
    result |= data_database_borrowed_stmt_destroy( &((*this_).feature_statement) );
    result |= data_database_borrowed_stmt_destroy( &((*this_).relationship_statement) );
    (*this_).is_at_diagram_end = true;
    (*this_).is_at_classifier_end = true;
    (*this_).is_at_feature_end = true;
    (*this_).is_at_relationship_end = true;
    data_search_result_destroy( &((*this_).next_search_result_buf) );

    U8_TRACE_END_ERR(result);
    return result;
}

bool data_search_result_iterator_has_next ( const data_search_result_iterator_t *this_ )
{
    const bool finished = (*this_).is_at_diagram_end && (*this_).is_at_classifier_end
                          && (*this_).is_at_feature_end && (*this_).is_at_relationship_end;
    assert( ( ! finished ) == data_search_result_is_valid( &((*this_).next_search_result_buf) ) );
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

    /* copy the next data_search_result_t to out */
    data_search_result_copy( out_search_result, &((*this_).next_search_result_buf) );

    /* search (and filter) for the next valid, unfiltered search result */
    data_search_result_iterator_private_step_to_next( this_ );

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

    /* invalidate the next_search_result_buf */
    data_search_result_init_void( &((*this_).next_search_result_buf) );

    if ( ! (*this_).is_at_diagram_end )
    {
        sqlite3_stmt *const sql_statement = data_database_borrowed_stmt_get_statement( &((*this_).diagram_statement) );

        /* do one step, check if is_at_end */
        U8_TRACE_INFO( "sqlite3_step() at diagrams" );
        const int sqlite_err = sqlite3_step( sql_statement );
        if ( SQLITE_DONE == sqlite_err )
        {
            U8_TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            (*this_).is_at_diagram_end = true;
        }
        else if ( SQLITE_ROW == sqlite_err )
        {
            (*this_).is_at_diagram_end = false;
            /* read next */
            result |= data_search_result_iterator_private_get_diagram( this_, &((*this_).next_search_result_buf) );
        }
        else
        {
            U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
            (*this_).is_at_diagram_end = true;
            result |= U8_ERROR_AT_DB;
        }
    }

    if ( (*this_).is_at_diagram_end && ( ! (*this_).is_at_classifier_end ))
    {
        sqlite3_stmt *const sql_statement = data_database_borrowed_stmt_get_statement( &((*this_).classifier_statement) );

        /* do one step, check if is_at_end */
        U8_TRACE_INFO( "sqlite3_step() at classifiers" );
        const int sqlite_err = sqlite3_step( sql_statement );
        if ( SQLITE_DONE == sqlite_err )
        {
            U8_TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            (*this_).is_at_classifier_end = true;
        }
        else if ( SQLITE_ROW == sqlite_err )
        {
            (*this_).is_at_classifier_end = false;
            /* read next */
            result |= data_search_result_iterator_private_get_classifier( this_, &((*this_).next_search_result_buf) );
        }
        else
        {
            U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
            (*this_).is_at_classifier_end = true;
            result |= U8_ERROR_AT_DB;
        }
    }

    while ( (*this_).is_at_classifier_end && ( ! (*this_).is_at_feature_end ) && ( ! data_search_result_is_valid( &((*this_).next_search_result_buf) ) ) )
    {
        sqlite3_stmt *const sql_statement = data_database_borrowed_stmt_get_statement( &((*this_).feature_statement) );

        /* do one step, check if is_at_end */
        U8_TRACE_INFO( "sqlite3_step() at features" );
        const int sqlite_err = sqlite3_step( sql_statement );
        if ( SQLITE_DONE == sqlite_err )
        {
            U8_TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            (*this_).is_at_feature_end = true;
        }
        else if ( SQLITE_ROW == sqlite_err )
        {
            (*this_).is_at_feature_end = false;
            /* read next */
            result |= data_search_result_iterator_private_get_feature( this_, &((*this_).next_search_result_buf) );
        }
        else
        {
            U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
            (*this_).is_at_feature_end = true;
            result |= U8_ERROR_AT_DB;
        }
    }

    while ( (*this_).is_at_feature_end && ( ! (*this_).is_at_relationship_end ) && ( ! data_search_result_is_valid( &((*this_).next_search_result_buf) ) ) )
    {
        sqlite3_stmt *const sql_statement = data_database_borrowed_stmt_get_statement( &((*this_).relationship_statement) );

        /* do one step, check if is_at_end */
        U8_TRACE_INFO( "sqlite3_step() at relationships" );
        const int sqlite_err = sqlite3_step( sql_statement );
        if ( SQLITE_DONE == sqlite_err )
        {
            U8_TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            (*this_).is_at_relationship_end = true;
        }
        else if ( SQLITE_ROW == sqlite_err )
        {
            (*this_).is_at_relationship_end = false;
            /* read next */
            result |= data_search_result_iterator_private_get_relationship( this_, &((*this_).next_search_result_buf) );
        }
        else
        {
            U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
            (*this_).is_at_relationship_end = true;
            result |= U8_ERROR_AT_DB;
        }
    }

    U8_TRACE_END_ERR(result);
    return result;
}

/* ================================ DIAGRAM ================================ */

/*!
 *  \brief predefined search statement to find diagrams by textfragment
 *
 *  note: name is needed for debugging only
 */
const char *const DATA_SEARCH_RESULT_ITERATOR_SELECT_DIAGRAM_BY_TEXTFRAGMENT =
"SELECT id,diagram_type,name "
"FROM diagrams "
"WHERE name LIKE ? ESCAPE \"\\\" "
"OR stereotype LIKE ? ESCAPE \"\\\" "
"OR description LIKE ? ESCAPE \"\\\";";

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_DIAGRAM_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: diagram_type
 */
static const int RESULT_DIAGRAM_TYPE_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: name
 */
static const int RESULT_DIAGRAM_NAME_COLUMN = 2;


u8_error_t data_search_result_iterator_private_get_diagram( data_search_result_iterator_t *this_,
                                                            data_search_result_t *out_search_result )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_search_result );
    u8_error_t result = U8_ERROR_NONE;

    sqlite3_stmt *const prepared_statement = data_database_borrowed_stmt_get_statement( &((*this_).diagram_statement) );

    data_search_result_init_diagram( out_search_result,
                                     sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_ID_COLUMN ),
                                     sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_TYPE_COLUMN ),
                                     (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_NAME_COLUMN )
                                   );
    data_search_result_trace( out_search_result );

    U8_TRACE_END_ERR( result );
    return result;
}


/* ================================ CLASSIFIER ================================ */

/*!
 *  \brief predefined search statement to find classifiers by textfragment
 *
 *  note: classifiers.name is needed for debugging only
 */
const char *const DATA_SEARCH_RESULT_ITERATOR_SELECT_CLASSIFIER_BY_TEXTFRAGMENT =
"SELECT classifiers.id,classifiers.main_type,classifiers.name,diagrams.id "
"FROM classifiers "
"INNER JOIN diagramelements ON diagramelements.classifier_id=classifiers.id "
"INNER JOIN diagrams ON diagramelements.diagram_id=diagrams.id "
"WHERE classifiers.name LIKE ? ESCAPE \"\\\" "
"OR classifiers.stereotype LIKE ? ESCAPE \"\\\" "
"OR classifiers.description LIKE ? ESCAPE \"\\\" "
"GROUP BY classifiers.id,diagrams.id;";  /* no duplicates if a classifier is twice in a diagram */

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_CLASSIFIER_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: main_type
 */
static const int RESULT_CLASSIFIER_MAIN_TYPE_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: name
 */
static const int RESULT_CLASSIFIER_NAME_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: diagrams.id
 */
static const int RESULT_CLASSIFIER_DIAGRAM_ID_COLUMN = 3;

u8_error_t data_search_result_iterator_private_get_classifier( data_search_result_iterator_t *this_,
                                                               data_search_result_t *out_search_result )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_search_result );
    u8_error_t result = U8_ERROR_NONE;

    sqlite3_stmt *const prepared_statement = data_database_borrowed_stmt_get_statement( &((*this_).classifier_statement) );

    data_search_result_init_classifier( out_search_result,
                                        sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIER_ID_COLUMN ),
                                        sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_MAIN_TYPE_COLUMN ),
                                        (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_NAME_COLUMN ),
                                        sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIER_DIAGRAM_ID_COLUMN )
                                      );
    data_search_result_trace( out_search_result );

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ DIAGRAMELEMENT ================================ */

/* ================================ FEATURE ================================ */

/*!
 *  \brief predefined search statement to find features by textfragment
 *
 *  note: features.key is needed for debugging only
 */
const char *const DATA_SEARCH_RESULT_ITERATOR_SELECT_FEATURE_BY_TEXTFRAGMENT =
"SELECT features.id,features.main_type,features.key,features.classifier_id,"
"classifiers.main_type,diagrams.id,diagrams.diagram_type "
"FROM features "
"INNER JOIN classifiers ON features.classifier_id=classifiers.id "
"INNER JOIN diagramelements ON diagramelements.classifier_id=classifiers.id "
"INNER JOIN diagrams ON diagramelements.diagram_id=diagrams.id "
"WHERE features.key LIKE ? ESCAPE \"\\\" "
"OR features.value LIKE ? ESCAPE \"\\\" "
"OR features.description LIKE ? ESCAPE \"\\\" "
"GROUP BY features.id,diagrams.id;";  /* no duplicates if a classifier is twice in a diagram */

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_FEATURE_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: main_type
 */
static const int RESULT_FEATURE_MAIN_TYPE_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: key
 */
static const int RESULT_FEATURE_KEY_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: classifiers.id
 */
static const int RESULT_FEATURE_CLASSIFIER_ID_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: classifiers.main_type
 */
static const int RESULT_FEATURE_CLASSIFIER_MAIN_TYPE_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: diagrams.id
 */
static const int RESULT_FEATURE_DIAGRAM_ID_COLUMN = 5;

/*!
 *  \brief the column id of the result where this parameter is stored: diagrams.diagram_type
 */
static const int RESULT_FEATURE_DIAGRAM_TYPE_COLUMN = 6;

u8_error_t data_search_result_iterator_private_get_feature( data_search_result_iterator_t *this_,
                                                            data_search_result_t *out_search_result )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_search_result );
    u8_error_t result = U8_ERROR_NONE;

    sqlite3_stmt *const prepared_statement = data_database_borrowed_stmt_get_statement( &((*this_).feature_statement) );

    data_search_result_init_feature( out_search_result,
                                     sqlite3_column_int64( prepared_statement, RESULT_FEATURE_ID_COLUMN ),
                                     sqlite3_column_int( prepared_statement, RESULT_FEATURE_MAIN_TYPE_COLUMN ),
                                     (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_KEY_COLUMN ),
                                     sqlite3_column_int64( prepared_statement, RESULT_FEATURE_CLASSIFIER_ID_COLUMN ),
                                     sqlite3_column_int64( prepared_statement, RESULT_FEATURE_DIAGRAM_ID_COLUMN )
                                   );
    const data_type_t current_type = data_search_result_get_match_type( out_search_result );
    const data_feature_type_t f_type = data_type_get_feature_type( &current_type );
    const data_classifier_type_t c_type = sqlite3_column_int( prepared_statement, RESULT_FEATURE_CLASSIFIER_MAIN_TYPE_COLUMN );
    const data_diagram_type_t d_type = sqlite3_column_int( prepared_statement, RESULT_FEATURE_DIAGRAM_TYPE_COLUMN );
    U8_TRACE_INFO_INT( "- c_type:", c_type );
    U8_TRACE_INFO_INT( "- d_type:", d_type );

    bool filter = false;
    const bool is_scenario_feat = data_rules_feature_is_scenario_cond( &((*this_).data_rules), f_type );
    if ( is_scenario_feat )
    {
        /* text search never returns lifelines, independant of data_rules_diagram_shows_scenario_features */
        filter = true;
    }
    else
    {
        /* evaluate filter */
        const bool vis_by_classifier = data_rules_classifier_has_uncond_features ( &((*this_).data_rules), c_type );
        const bool vis_by_diagram = data_rules_diagram_shows_uncond_features ( &((*this_).data_rules), d_type );
        filter = !( vis_by_classifier && vis_by_diagram );
    }

    if ( filter )
    {
        /* invalidate the out_search_result */
        data_search_result_init_void( out_search_result );
    }

    data_search_result_trace( out_search_result );

    U8_TRACE_END_ERR( result );
    return result;
}


/* ================================ RELATIONSHIP ================================ */

/*!
 *  \brief predefined search statement to find relationships by textfragment
 *
 *  note: relationships.name is needed for debugging only
 */
const char *const DATA_SEARCH_RESULT_ITERATOR_SELECT_RELATIONSHIP_BY_TEXTFRAGMENT =
"SELECT DISTINCT relationships.id,relationships.main_type,relationships.name,"
"relationships.from_classifier_id,relationships.to_classifier_id,"
"relationships.from_feature_id,relationships.to_feature_id,"
"source.focused_feature_id,dest.focused_feature_id,"  /* challenge: the group by clause may randomly select ids here */
"diagrams.id,diagrams.diagram_type "
"FROM relationships "
"INNER JOIN diagramelements AS source "
"ON source.classifier_id=relationships.from_classifier_id "
"INNER JOIN diagramelements AS dest "
"ON (dest.classifier_id=relationships.to_classifier_id)AND(dest.diagram_id==source.diagram_id) "
"INNER JOIN diagrams ON source.diagram_id=diagrams.id "
"WHERE relationships.name LIKE ? ESCAPE \"\\\" "
"OR relationships.stereotype LIKE ? ESCAPE \"\\\" "
"OR relationships.description LIKE ? ESCAPE \"\\\" "
//"GROUP BY relationships.id,diagrams.id "  /* good: no duplicates if a classifier is twice in a diagram / bad: randomly chosen source and dest --> use DISTINCT */
"ORDER BY relationships.id,((source.focused_feature_id ISNULL)AND(dest.focused_feature_id ISNULL)) ASC;";  /* start with interactions/scenarios */

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_RELATIONSHIP_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: main_type
 */
static const int RESULT_RELATIONSHIP_MAIN_TYPE_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: name
 */
static const int RESULT_RELATIONSHIP_NAME_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: from_classifier_id
 */
static const int RESULT_RELATIONSHIP_FROM_CLASSIFIER_ID_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: to_classifier_id
 */
static const int RESULT_RELATIONSHIP_TO_CLASSIFIER_ID_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: from_feature_id
 */
static const int RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN = 5;

/*!
 *  \brief the column id of the result where this parameter is stored: to_feature_id
 */
static const int RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN = 6;

/*!
 *  \brief the column id of the result where this parameter is stored: source.focused_feature_id
 *
 *  In case of a GROUP BY clause, this id may not be related to the RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN due to random selection
 *  One may consider to remove the GROUP BY and accept more search results
 */
static const int RESULT_RELATIONSHIP_FROM_FOCUSED_ID_COLUMN = 7;

/*!
 *  \brief the column id of the result where this parameter is stored: dest.focused_feature_id
 *
 *  In case of a GROUP BY clause, this id may not be related to the RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN due to random selection
 *  One may consider to remove the GROUP BY and accept more search results
 */
static const int RESULT_RELATIONSHIP_TO_FOCUSED_ID_COLUMN = 8;

/*!
 *  \brief the column id of the result where this parameter is stored: diagrams.id
 */
static const int RESULT_RELATIONSHIP_DIAGRAM_ID_COLUMN = 9;

/*!
 *  \brief the column id of the result where this parameter is stored: diagrams.diagram_type
 */
static const int RESULT_RELATIONSHIP_DIAGRAM_TYPE_COLUMN = 10;

u8_error_t data_search_result_iterator_private_get_relationship( data_search_result_iterator_t *this_,
                                                                 data_search_result_t *out_search_result )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_search_result );
    u8_error_t result = U8_ERROR_NONE;

    sqlite3_stmt *const prepared_statement = data_database_borrowed_stmt_get_statement( &((*this_).relationship_statement) );

    data_search_result_init_relationship( out_search_result,
                                          sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_ID_COLUMN ),
                                          sqlite3_column_int( prepared_statement, RESULT_RELATIONSHIP_MAIN_TYPE_COLUMN ),
                                          (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_NAME_COLUMN ),
                                          sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_FROM_CLASSIFIER_ID_COLUMN ),
                                          sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_TO_CLASSIFIER_ID_COLUMN ),
                                          sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_DIAGRAM_ID_COLUMN )
                                        );
    const data_row_t from_feat
         = ( SQLITE_NULL != sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN ) )
         ? sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN )
         : DATA_ROW_VOID;
         const data_row_t to_feat
         = ( SQLITE_NULL != sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN ) )
         ? sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN )
         : DATA_ROW_VOID;
         const data_row_t from_focused
         = ( SQLITE_NULL != sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIP_FROM_FOCUSED_ID_COLUMN ) )
         ? sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_FROM_FOCUSED_ID_COLUMN )
         : DATA_ROW_VOID;
         const data_row_t to_focused
         = ( SQLITE_NULL != sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIP_TO_FOCUSED_ID_COLUMN ) )
         ? sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_TO_FOCUSED_ID_COLUMN )
         : DATA_ROW_VOID;
    const data_diagram_type_t d_type = sqlite3_column_int( prepared_statement, RESULT_RELATIONSHIP_DIAGRAM_TYPE_COLUMN );
    U8_TRACE_INFO_INT( "- from_feat:", from_feat );
    U8_TRACE_INFO_INT( "- to_feat:", to_feat );
    U8_TRACE_INFO_INT( "- d_type:", d_type );
    const data_row_t rel_row_id = data_id_get_row_id( data_search_result_get_match_id_const( out_search_result ) );

    bool filter = false;
    const bool is_scenario_diag = data_rules_diagram_is_scenario ( &((*this_).data_rules), d_type );
    /*const bool is_scenario_rel = data_rules_relationship_is_scenario_cond( &((*this_).data_rules), from_feature_type, to_feature_type);*/
    if ( is_scenario_diag )
    {
        /* in theory, we would need to know the from_feature_type and the to_feature_type to determine the visibility: */
        /*
         * const bool is_shown = data_rules_relationship_is_scenario_cond( const data_rules_t *this_,
         *                                                                 data_feature_type_t from_feature_type,
         *                                                                 data_feature_type_t to_feature_type
         *                                                               );
         */
        /* but we only have the data_relationship_type_t and the data_diagramelement_t here and we control the order of results: */
        const bool from_is_lifeline = ( DATA_ROW_VOID != from_feat )&&( from_focused == from_feat );
        const bool to_is_lifeline = ( DATA_ROW_VOID != to_feat )&&( to_focused == to_feat );
        const bool visible = from_is_lifeline && to_is_lifeline;
        if ( visible )
        {
            filter = false;
            (*this_).last_relationship_was_scenario = true;
            (*this_).last_relationship_id = rel_row_id;
            U8_TRACE_INFO_INT( "data_search_result_iterator: in scenario found relationship", rel_row_id );
        }
        else
        {
            filter = true;
            U8_TRACE_INFO_INT( "data_search_result_iterator: in scenario skipped invisible relationship", rel_row_id );
        }
    }
    else
    {
        /* in theory, we would need to know the from_feature_type and the to_feature_type to determine the visibility: */
        /*
         * const bool is_shown = data_rules_relationship_is_scenario_cond( const data_rules_t *this_,
         *                                                                 data_feature_type_t from_feature_type,
         *                                                                 data_feature_type_t to_feature_type
         *                                                               );
         */
        /* but we only have the data_relationship_type_t and the data_diagramelement_t here and we control the order of results: */
        const bool is_scenario = ( (*this_).last_relationship_id == rel_row_id )&&( (*this_).last_relationship_was_scenario );
        const bool vis_by_diagram = data_rules_diagram_shows_uncond_relationships ( &((*this_).data_rules), d_type );
        if ( vis_by_diagram && ( ! is_scenario ) )
        {
            filter = false;
            (*this_).last_relationship_was_scenario = false;
            (*this_).last_relationship_id = rel_row_id;
            U8_TRACE_INFO_INT( "data_search_result_iterator: non-scenario found relationship", rel_row_id );
        }
        else
        {
            filter = true;
            U8_TRACE_INFO_INT( "data_search_result_iterator: non-scenario skipped scenario-specific relationship", rel_row_id );
        }
    }

    if ( filter )
    {
        /* invalidate the out_search_result */
        data_search_result_init_void( out_search_result );
    }

    data_search_result_trace( out_search_result );

    U8_TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2020-2025 Andreas Warnke

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
