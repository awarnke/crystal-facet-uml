/* File: data_database_text_search.c; Copyright and License: see below */

#include "storage/data_database_text_search.h"
#include "trace.h"
#include "tslog.h"
#include "util/string/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>

/*!
 *  \brief translation table to encode strings for usage in LIKE search string literals
 */
const char *const DATA_DATABASE_TEXT_SEARCH_SQL_ENCODE[][2] = {
    { "%", "\\%" },
    { "_", "\\_" },
    { "\\", "\\\\" },
    { NULL, NULL }
};

data_error_t data_database_text_search_init ( data_database_text_search_t *this_, data_database_t *database )
{
    TRACE_BEGIN();
    assert( NULL != database );
    data_error_t result = DATA_ERROR_NONE;

    (*this_).database = database;
    (*this_).is_open = false;

    data_database_listener_init ( &((*this_).me_as_listener), this_, (void (*)(void*,data_database_listener_signal_t)) &data_database_text_search_db_change_callback );
    data_database_add_db_listener( database, &((*this_).me_as_listener) );

    if ( data_database_is_open( database ) )
    {
        /* if the database is open, open also the reader */
        result |= data_database_text_search_private_open( this_ );
    }

    data_rules_init ( &((*this_).data_rules) );

    TRACE_END_ERR(result);
    return result;
}

data_error_t data_database_text_search_destroy ( data_database_text_search_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    data_rules_destroy ( &((*this_).data_rules) );

    if ( (*this_).is_open )
    {
        result |= data_database_text_search_private_close( this_ );
    }

    data_database_remove_db_listener( (*this_).database, &((*this_).me_as_listener) );

    (*this_).database = NULL;

    TRACE_END_ERR(result);
    return result;
}

void data_database_text_search_db_change_callback ( data_database_text_search_t *this_, data_database_listener_signal_t signal_id )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    switch ( signal_id )
    {
        case DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE:
        {
            TRACE_INFO( "DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE" );
            if ( (*this_).is_open )
            {
                result |= data_database_text_search_private_close( this_ );
            }
        }
        break;

        case DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED:
        {
            TRACE_INFO( "DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED" );
            if ( (*this_).is_open )
            {
                result |= data_database_text_search_private_close( this_ );
            }
            result |= data_database_text_search_private_open( this_ );
        }
        break;

        default:
        {
            TSLOG_ERROR( "unexpected data_database_listener_signal_t" );
        }
    }

    TRACE_END();
}

data_error_t data_database_text_search_get_objects_by_textfragment ( data_database_text_search_t *this_,
                                                                     const char *textfragment,
                                                                     data_search_result_list_t *io_results )
{
    TRACE_BEGIN();
    assert( NULL != io_results );
    assert( NULL != textfragment );
    data_error_t result = DATA_ERROR_NONE;

    /* escape-encode textfragment */
    utf8error_t u8err = UTF8ERROR_SUCCESS;
    char like_search_buf[48] = "";
    utf8stringbuf_t like_search = UTF8STRINGBUF( like_search_buf );
    if ( 0 != utf8string_get_length( textfragment ) )
    {
        u8err |= utf8stringbuf_append_str( like_search, "%" );
        utf8stringbuf_t escape_me = utf8stringbuf_get_end( like_search );
        u8err |= utf8stringbuf_append_str( escape_me, textfragment );
        u8err |= utf8stringbuf_replace_all( escape_me, &DATA_DATABASE_TEXT_SEARCH_SQL_ENCODE );
        u8err |= utf8stringbuf_append_str( like_search, "%" );
    }
    else
    {
        /* no wildcards and no excaping if search string is empty */
        utf8stringbuf_clear( like_search );
    }
    TRACE_INFO_STR( "LIKE SEARCH:", utf8stringbuf_get_string( like_search ) );
    if ( u8err != UTF8ERROR_SUCCESS )
    {
        TSLOG_WARNING_STR( "error at escaping the search string", textfragment );
    }

    result |= data_database_text_search_private_get_diagrams_by_textfragment ( this_,
                                                                               utf8stringbuf_get_string( like_search ),
                                                                               io_results
                                                                             );

    result |= data_database_text_search_private_get_classifiers_by_textfragment ( this_,
                                                                                  utf8stringbuf_get_string( like_search ),
                                                                                  io_results
                                                                                );

    result |= data_database_text_search_private_get_features_by_textfragment ( this_,
                                                                               utf8stringbuf_get_string( like_search ),
                                                                               io_results
                                                                             );

    result |= data_database_text_search_private_get_relationships_by_textfragment ( this_,
                                                                                    utf8stringbuf_get_string( like_search ),
                                                                                    io_results
                                                                                  );

    TRACE_END_ERR( result );
    return result;
}

/* ================================ DIAGRAM ================================ */

/*!
 *  \brief predefined search statement to find diagrams by textfragment
 *
 *  note: name is needed for debugging only
 */
static const char data_database_text_search_SELECT_DIAGRAM_BY_TEXTFRAGMENT[] =
    "SELECT id,diagram_type,name "
    "FROM diagrams "
    "WHERE name LIKE ? ESCAPE \"\\\" "
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


data_error_t data_database_text_search_private_get_diagrams_by_textfragment ( data_database_text_search_t *this_,
                                                                              const char *textfragment,
                                                                              data_search_result_list_t *io_results )
{
    TRACE_BEGIN();
    assert( NULL != io_results );
    assert( NULL != textfragment );
    data_error_t result = DATA_ERROR_NONE;

    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_diagram_ids_by_textfragment;

        result |= data_database_text_search_private_bind_two_texts_to_statement( this_, prepared_statement, textfragment, textfragment );

        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (DATA_ERROR_NONE == result); row_index ++ )
        {
            TRACE_INFO_INT( "sqlite3_step():", (row_index+1) );
            sqlite_err = sqlite3_step( prepared_statement );
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
            else if ( SQLITE_ROW != sqlite_err )
            {
                TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            else
            {
                data_search_result_t current_result;

                data_search_result_init_diagram( &current_result,
                                                 sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_ID_COLUMN ),
                                                 sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_TYPE_COLUMN ),
                                                 (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_NAME_COLUMN )
                                               );

                int err_full = data_search_result_list_add( io_results, &current_result );
                if ( err_full != 0 )
                {
                    TSLOG_ANOMALY_INT( "io_results list full:", data_search_result_list_get_length( io_results ) );
                    result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
                }

                data_search_result_trace( &current_result );
                data_search_result_destroy( &current_result );
            }
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}


/* ================================ CLASSIFIER ================================ */

/*!
 *  \brief predefined search statement to find classifiers by textfragment
 *
 *  note: classifiers.name is needed for debugging only
 */
static const char data_database_text_search_SELECT_CLASSIFIER_BY_TEXTFRAGMENT[] =
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

data_error_t data_database_text_search_private_get_classifiers_by_textfragment ( data_database_text_search_t *this_,
                                                                                 const char *textfragment,
                                                                                 data_search_result_list_t *io_results )
{
    TRACE_BEGIN();
    assert( NULL != io_results );
    assert( NULL != textfragment );
    data_error_t result = DATA_ERROR_NONE;

    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_classifier_ids_by_textfragment;

        result |= data_database_text_search_private_bind_three_texts_to_statement( this_, prepared_statement, textfragment, textfragment, textfragment );

        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (DATA_ERROR_NONE == result); row_index ++ )
        {
            TRACE_INFO_INT( "sqlite3_step():", (row_index+1) );
            sqlite_err = sqlite3_step( prepared_statement );
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
            else if ( SQLITE_ROW != sqlite_err )
            {
                TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            else
            {
                data_search_result_t current_result;

                data_search_result_init_classifier( &current_result,
                                                    sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIER_ID_COLUMN ),
                                                    sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_MAIN_TYPE_COLUMN ),
                                                    (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_NAME_COLUMN ),
                                                    sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIER_DIAGRAM_ID_COLUMN )
                                                  );

                int err_full = data_search_result_list_add( io_results, &current_result );
                if ( err_full != 0 )
                {
                    TSLOG_ANOMALY_INT( "io_results list full:", data_search_result_list_get_length( io_results ) );
                    result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
                }

                data_search_result_trace( &current_result );
                data_search_result_destroy( &current_result );
            }
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

/* ================================ DIAGRAMELEMENT ================================ */

/* ================================ FEATURE ================================ */

/*!
 *  \brief predefined search statement to find features by textfragment
 *
 *  note: features.key is needed for debugging only
 */
static const char data_database_text_search_SELECT_FEATURE_BY_TEXTFRAGMENT[] =
    "SELECT DISTINCT features.id,features.main_type,features.key,features.classifier_id,"
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

data_error_t data_database_text_search_private_get_features_by_textfragment ( data_database_text_search_t *this_,
                                                                              const char *textfragment,
                                                                              data_search_result_list_t *io_results )
{
    TRACE_BEGIN();
    assert( NULL != io_results );
    assert( NULL != textfragment );
    data_error_t result = DATA_ERROR_NONE;

    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_feature_ids_by_textfragment;

        result |= data_database_text_search_private_bind_three_texts_to_statement( this_, prepared_statement, textfragment, textfragment, textfragment );

        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (DATA_ERROR_NONE == result); row_index ++ )
        {
            TRACE_INFO_INT( "sqlite3_step():", (row_index+1) );
            sqlite_err = sqlite3_step( prepared_statement );
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
            else if ( SQLITE_ROW != sqlite_err )
            {
                TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            else
            {
                data_search_result_t current_result;

                data_search_result_init_feature( &current_result,
                                                 sqlite3_column_int64( prepared_statement, RESULT_FEATURE_ID_COLUMN ),
                                                 sqlite3_column_int( prepared_statement, RESULT_FEATURE_MAIN_TYPE_COLUMN ),
                                                 (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_KEY_COLUMN ),
                                                 sqlite3_column_int64( prepared_statement, RESULT_FEATURE_CLASSIFIER_ID_COLUMN ),
                                                 sqlite3_column_int64( prepared_statement, RESULT_FEATURE_DIAGRAM_ID_COLUMN )
                                               );
                const data_feature_type_t f_type = data_search_result_get_match_type( &current_result );
                const data_classifier_type_t c_type = sqlite3_column_int( prepared_statement, RESULT_FEATURE_CLASSIFIER_MAIN_TYPE_COLUMN );
                const data_diagram_type_t d_type = sqlite3_column_int( prepared_statement, RESULT_FEATURE_DIAGRAM_TYPE_COLUMN );
                TRACE_INFO_INT( "- c_type:", c_type );
                TRACE_INFO_INT( "- d_type:", d_type );

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

                if ( ! filter )
                {
                    int err_full = data_search_result_list_add( io_results, &current_result );
                    if ( err_full != 0 )
                    {
                        TSLOG_ANOMALY_INT( "io_results list full:", data_search_result_list_get_length( io_results ) );
                        result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
                    }
                }

                data_search_result_trace( &current_result );
                data_search_result_destroy( &current_result );
            }
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }


    TRACE_END_ERR( result );
    return result;
}


/* ================================ RELATIONSHIP ================================ */

/*!
 *  \brief predefined search statement to find relationships by textfragment
 *
 *  note: relationships.name is needed for debugging only
 */
static const char data_database_text_search_SELECT_RELATIONSHIP_BY_TEXTFRAGMENT[] =
    "SELECT DISTINCT relationships.id,relationships.main_type,relationships.name,"
    "relationships.from_classifier_id,relationships.to_classifier_id,"
    "relationships.from_feature_id,relationships.to_feature_id,"
    "diagrams.id,diagrams.diagram_type "
    "FROM relationships "
    "INNER JOIN diagramelements AS source "
    "ON source.classifier_id=relationships.from_classifier_id "
    "INNER JOIN diagramelements AS dest "
    "ON (dest.classifier_id=relationships.to_classifier_id)AND(dest.diagram_id==source.diagram_id) "
    "INNER JOIN diagrams ON source.diagram_id=diagrams.id "
    "WHERE relationships.name LIKE ? ESCAPE \"\\\" "
    "OR relationships.description LIKE ? ESCAPE \"\\\" "
    "GROUP BY relationships.id,diagrams.id;";  /* no duplicates if a classifier is twice in a diagram */

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
 *  \brief the column id of the result where this parameter is stored: diagrams.id
 */
static const int RESULT_RELATIONSHIP_DIAGRAM_ID_COLUMN = 7;

/*!
 *  \brief the column id of the result where this parameter is stored: diagrams.diagram_type
 */
static const int RESULT_RELATIONSHIP_DIAGRAM_TYPE_COLUMN = 8;

data_error_t data_database_text_search_private_get_relationships_by_textfragment ( data_database_text_search_t *this_,
                                                                                   const char *textfragment,
                                                                                   data_search_result_list_t *io_results )
{
    TRACE_BEGIN();
    assert( NULL != io_results );
    assert( NULL != textfragment );
    data_error_t result = DATA_ERROR_NONE;

    int sqlite_err;
    sqlite3_stmt *prepared_statement;
    unsigned int dropped_scenario_rel = 0;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_relationship_ids_by_textfragment;

        result |= data_database_text_search_private_bind_two_texts_to_statement( this_, prepared_statement, textfragment, textfragment );

        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (DATA_ERROR_NONE == result); row_index ++ )
        {
            TRACE_INFO_INT( "sqlite3_step():", (row_index+1) );
            sqlite_err = sqlite3_step( prepared_statement );
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
            else if ( SQLITE_ROW != sqlite_err )
            {
                TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            else
            {
                data_search_result_t current_result;

                data_search_result_init_relationship( &current_result,
                                                      sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_ID_COLUMN ),
                                                      sqlite3_column_int( prepared_statement, RESULT_RELATIONSHIP_MAIN_TYPE_COLUMN ),
                                                      (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_NAME_COLUMN ),
                                                      sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_FROM_CLASSIFIER_ID_COLUMN ),
                                                      sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_TO_CLASSIFIER_ID_COLUMN ),
                                                      sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_DIAGRAM_ID_COLUMN )
                                                    );
                const data_row_id_t from_feat = sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN );
                const data_row_id_t to_feat = sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN );
                const data_diagram_type_t d_type = sqlite3_column_int( prepared_statement, RESULT_RELATIONSHIP_DIAGRAM_TYPE_COLUMN );
                TRACE_INFO_INT( "- from_feat:", from_feat );
                TRACE_INFO_INT( "- to_feat:", to_feat );
                TRACE_INFO_INT( "- d_type:", d_type );

                bool filter = false;
                const bool is_scenario_diag = data_rules_diagram_is_scenario ( &((*this_).data_rules), d_type );
                /*const bool is_scenario_rel = data_rules_relationship_is_scenario_cond( &((*this_).data_rules), from_feature_type, to_feature_type);*/
                if ( is_scenario_diag )
                {
                    /* there could be valid relationships that are visible and match the search. */
                    /* but it is quite difficult to determine if the relationship is visible in the current diagram */
                    /* --> drop the result and write a not to the log */
                    dropped_scenario_rel ++;
                    filter = true;
                }
                else
                {
                    /* there could be hidden scenario-typed relationships in a non-scenario diagram. */
                    /* but it is quite difficult to determine if the relationship is scenario-only */
                    /* --> show the result anyway */
                    const bool vis_by_diagram = data_rules_diagram_shows_uncond_relationships ( &((*this_).data_rules), d_type );
                    filter = ! vis_by_diagram;
                }

                if ( ! filter )
                {
                    int err_full = data_search_result_list_add( io_results, &current_result );
                    if ( err_full != 0 )
                    {
                        TSLOG_ANOMALY_INT( "io_results list full:", data_search_result_list_get_length( io_results ) );
                        result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
                    }
                }

                data_search_result_trace( &current_result );
                data_search_result_destroy( &current_result );
            }
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    if ( dropped_scenario_rel != 0 )
    {
        TSLOG_ANOMALY_INT( "Full text search does not work on relationships in scenario based diagrams. Possibly missed relationships:",
                           dropped_scenario_rel
                         );
    }

    TRACE_END_ERR( result );
    return result;
}


/* ================================ private ================================ */

data_error_t data_database_text_search_private_open ( data_database_text_search_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( ! (*this_).is_open )
    {
        result |= data_database_text_search_private_prepare_statement ( this_,
                                                                        data_database_text_search_SELECT_DIAGRAM_BY_TEXTFRAGMENT,
                                                                        sizeof( data_database_text_search_SELECT_DIAGRAM_BY_TEXTFRAGMENT ),
                                                                        &((*this_).private_prepared_query_diagram_ids_by_textfragment)
                                                                      );
        result |= data_database_text_search_private_prepare_statement ( this_,
                                                                        data_database_text_search_SELECT_CLASSIFIER_BY_TEXTFRAGMENT,
                                                                        sizeof( data_database_text_search_SELECT_CLASSIFIER_BY_TEXTFRAGMENT ),
                                                                        &((*this_).private_prepared_query_classifier_ids_by_textfragment)
                                                                      );
        result |= data_database_text_search_private_prepare_statement ( this_,
                                                                        data_database_text_search_SELECT_FEATURE_BY_TEXTFRAGMENT,
                                                                        sizeof( data_database_text_search_SELECT_FEATURE_BY_TEXTFRAGMENT ),
                                                                        &((*this_).private_prepared_query_feature_ids_by_textfragment)
                                                                      );
        result |= data_database_text_search_private_prepare_statement ( this_,
                                                                        data_database_text_search_SELECT_RELATIONSHIP_BY_TEXTFRAGMENT,
                                                                        sizeof( data_database_text_search_SELECT_RELATIONSHIP_BY_TEXTFRAGMENT ),
                                                                        &((*this_).private_prepared_query_relationship_ids_by_textfragment)
                                                                      );

        (*this_).is_open = true;
    }
    else
    {
        result |= DATA_ERROR_INVALID_REQUEST;
        TSLOG_WARNING( "Database is already open." );
    }

    TRACE_END_ERR(result);
    return result;
}

data_error_t data_database_text_search_private_close ( data_database_text_search_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result |= data_database_text_search_private_finalize_statement( this_, (*this_).private_prepared_query_relationship_ids_by_textfragment );
        result |= data_database_text_search_private_finalize_statement( this_, (*this_).private_prepared_query_feature_ids_by_textfragment );
        result |= data_database_text_search_private_finalize_statement( this_, (*this_).private_prepared_query_classifier_ids_by_textfragment );
        result |= data_database_text_search_private_finalize_statement( this_, (*this_).private_prepared_query_diagram_ids_by_textfragment );

        (*this_).is_open = false;
    }
    else
    {
        result |= DATA_ERROR_INVALID_REQUEST;
        TSLOG_WARNING( "Database was not open." );
    }

    TRACE_END_ERR(result);
    return result;
}


/*
Copyright 2020-2021 Andreas Warnke

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
