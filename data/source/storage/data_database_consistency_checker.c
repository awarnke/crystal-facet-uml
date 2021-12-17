/* File: data_database_consistency_checker.c; Copyright and License: see below */

#include "storage/data_database_consistency_checker.h"
#include "trace.h"
#include "tslog.h"
#include "util/string/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>

u8_error_t data_database_consistency_checker_init ( data_database_consistency_checker_t *this_, data_database_t *database )
{
    TRACE_BEGIN();
    assert( NULL != database );
    u8_error_t result = U8_ERROR_NONE;

    (*this_).database = database;

    TRACE_END_ERR(result);
    return result;
}


u8_error_t data_database_consistency_checker_destroy ( data_database_consistency_checker_t *this_ )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    (*this_).database = NULL;

    TRACE_END_ERR(result);
    return result;
}

/*!
 *  \brief the length of an sql query shall be determined by the terminating 0 of the string
 */
static const int AUTO_DETECT_SQL_LENGTH = -1;

/*!
 *  \brief the last evaluated sql string character shall not be reported to the caller
 */
static void* NO_SQL_DEBUG_INFORMATION = NULL;

/*!
 *  \brief break the loop of evaluating search results after a predefined high number
 */
static const int MAX_ROWS_TO_CHECK = 1000000;

/*!
 *  \brief search statement to find diagrams and their parent ids
 */
static const char SELECT_DIAGRAMS_IDS[] =
    "SELECT diagrams.id,diagrams.parent_id FROM diagrams;";

/*!
 *  \brief the column id of the result where this parameter is stored: diagrams.id
 */
static const int RESULT_DIAGRAMS_DIAG_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: diagrams.parent_id
 */
static const int RESULT_DIAGRAMS_DIAG_PARENT_ID_COLUMN = 1;

/*!
 *  \brief search statement to find diagramelements with illegal related diagrams and classifiers
 */
static const char SELECT_DIAGRAMELEMENTS_AND_RELATED[] =
    "SELECT diagramelements.id,diagramelements.diagram_id,diagramelements.classifier_id,diagrams.id,classifiers.id "
    "FROM diagramelements "
    "LEFT JOIN diagrams ON diagramelements.diagram_id=diagrams.id "
    "LEFT JOIN classifiers ON diagramelements.classifier_id=classifiers.id;";

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.id
 */
static const int RESULT_DIAGRAMELEMENTS_DIAGELE_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.diagram_id
 */
static const int RESULT_DIAGRAMELEMENTS_DIAGELE_DIAGRAM_ID_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.classifier_id
 */
static const int RESULT_DIAGRAMELEMENTS_DIAGELE_CLASSIFIER_ID_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: diagrams.id
 */
static const int RESULT_DIAGRAMELEMENTS_DIAGRAM_ID_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: classifiers.id
 */
static const int RESULT_DIAGRAMELEMENTS_CLASSIFIER_ID_COLUMN = 4;

/*!
 *  \brief search statement to find diagramelements with illegal focused features
 */
static const char SELECT_FOCUSED_FEATURES[] =
    "SELECT diagramelements.id,diagramelements.classifier_id,diagramelements.focused_feature_id,"
    "features.id,features.classifier_id "
    "FROM diagramelements "
    "LEFT JOIN features ON diagramelements.focused_feature_id=features.id;";

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.id
 */
static const int RESULT_FOCUSED_FEATURES_DIAGELE_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.classifier_id
 */
static const int RESULT_FOCUSED_FEATURES_DIAGELE_CLASSIFIER_ID_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.focused_feature_id
 */
static const int RESULT_FOCUSED_FEATURES_DIAGELE_FEATURE_ID_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: features.id
 */
static const int RESULT_FOCUSED_FEATURES_FEATURE_ID_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: features.classifier_id
 */
static const int RESULT_FOCUSED_FEATURES_FEATURE_CLASSIFIER_ID_COLUMN = 4;

/*!
 *  \brief search statement to find classifiers that are not referenced
 */
static const char SELECT_CLASSIFIERS_AND_DIAGRAMELEMENTS[] =
    "SELECT classifiers.id,diagramelements.classifier_id,"
    "diagramelements.id " /* diagramelements.id needed only for debugging */
    "FROM classifiers LEFT JOIN diagramelements ON classifiers.id=diagramelements.classifier_id;";

/*!
 *  \brief the column id of the result where this parameter is stored: classifiers.id
 */
static const int RESULT_CLASSIFIERS_CLASSIFIER_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.classifier_id
 */
static const int RESULT_CLASSIFIERS_DIAGELE_CLASSIFIER_ID_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.id
 */
static const int RESULT_CLASSIFIERS_DIAGELE_ID_COLUMN = 2;

/*!
 *  \brief search statement to find features that are not referenced
 */
static const char SELECT_FEATURES_AND_CLASSIFIERS[] =
    "SELECT features.id,features.classifier_id,classifiers.id "
    "FROM features "
    "LEFT JOIN classifiers ON features.classifier_id=classifiers.id;";

/*!
 *  \brief the column id of the result where this parameter is stored: features.id
 */
static const int RESULT_FEATURES_FEATURE_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: features.classifier_id
 */
static const int RESULT_FEATURES_FEATURE_CLASSIFIER_ID_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: classifiers.id
 */
static const int RESULT_FEATURES_CLASSIFIER_ID_COLUMN = 2;

/*!
 *  \brief search statement to find relationships that contain invalid classifier references
 */
static const char SELECT_RELATIONSHIPS_AND_CLASSIFIERS[] =
    "SELECT relationships.id,relationships.from_classifier_id,relationships.to_classifier_id,source.id,dest.id "
    "FROM relationships "
    "LEFT JOIN classifiers AS source ON relationships.from_classifier_id=source.id "
    "LEFT JOIN classifiers AS dest ON relationships.to_classifier_id=dest.id;";

/*!
 *  \brief the column id of the result where this parameter is stored: relationships.id
 */
static const int RESULT_RELATIONSHIPS_RELATIONSHIP_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: relationships.from_classifier_id
 */
static const int RESULT_RELATIONSHIPS_RELATIONSHIP_FROM_ID_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: relationships.to_classifier_id
 */
static const int RESULT_RELATIONSHIPS_RELATIONSHIP_TO_ID_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: source.id
 */
static const int RESULT_RELATIONSHIPS_SOURCE_ID_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: dest.id
 */
static const int RESULT_RELATIONSHIPS_DEST_ID_COLUMN = 4;

/*!
 *  \brief search statement to find relationships that contain invalid feature references
 */
static const char SELECT_FEATURE_RELATIONSHIPS[] =
    "SELECT relationships.id,relationships.from_classifier_id,relationships.to_classifier_id,"
    "relationships.from_feature_id,relationships.to_feature_id,source.id,source.classifier_id,dest.id,dest.classifier_id "
    "FROM relationships "
    "LEFT JOIN features AS source ON relationships.from_feature_id=source.id "
    "LEFT JOIN features AS dest ON relationships.to_feature_id=dest.id;";

/*!
 *  \brief the column id of the result where this parameter is stored: relationships.id
 */
static const int RESULT_FEATURE_RELATIONSHIPS_RELATION_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: relationships.from_classifier_id
 */
static const int RESULT_FEATURE_RELATIONSHIPS_RELATION_FROM_CLASSIFIER_ID_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: relationships.to_classifier_id
 */
static const int RESULT_FEATURE_RELATIONSHIPS_RELATION_TO_CLASIFIER_ID_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: relationships.from_feature_id
 */
static const int RESULT_FEATURE_RELATIONSHIPS_RELATION_FROM_FEATURE_ID_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: relationships.to_feature_id
 */
static const int RESULT_FEATURE_RELATIONSHIPS_RELATION_TO_FEATURE_ID_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: source.id
 */
static const int RESULT_FEATURE_RELATIONSHIPS_SOURCE_FEATURE_ID_COLUMN = 5;

/*!
 *  \brief the column id of the result where this parameter is stored: source.classifier_id
 */
static const int RESULT_FEATURE_RELATIONSHIPS_SOURCE_FEATURE_CLASSIFIER_ID_COLUMN = 6;

/*!
 *  \brief the column id of the result where this parameter is stored: dest.id
 */
static const int RESULT_FEATURE_RELATIONSHIPS_DEST_FEATURE_ID_COLUMN = 7;

/*!
 *  \brief the column id of the result where this parameter is stored: dest.classifier_id
 */
static const int RESULT_FEATURE_RELATIONSHIPS_DEST_FEATURE_CLASSIFIER_ID_COLUMN = 8;

u8_error_t data_database_consistency_checker_find_circular_diagram_parents ( data_database_consistency_checker_t *this_, data_small_set_t *io_set )
{
    TRACE_BEGIN();
    assert( NULL != io_set );
    u8_error_t result = U8_ERROR_NONE;

    /* fetch all diagram ids from the database */
    uint32_t diagram_id_pair_count;
    result = data_database_consistency_checker_private_get_diagram_ids ( this_,
                                                                         DATA_DATABASE_CONSISTENCY_CHECKER_MAX_TEMP_DIAG_IDS,
                                                                         &((*this_).private_temp_diagram_ids_buf),
                                                                         &diagram_id_pair_count
                                                                       );

    /* delete all parent-ids from diagrams that have parents who have empty parent ids */
    bool finished = false;
    for ( uint_fast32_t tree_depth = 0; ( tree_depth < diagram_id_pair_count ) && ( ! finished ); tree_depth ++ )
    {
        uint_fast32_t found_children = 0;
        for ( uint_fast32_t child_idx = 0; child_idx < diagram_id_pair_count; child_idx ++ )
        {
            bool child_has_parent;
            child_has_parent = ( DATA_ROW_ID_VOID != ((*this_).private_temp_diagram_ids_buf)[child_idx][1] );
            if ( child_has_parent )
            {
                for ( uint_fast32_t probe_parent_idx = 0; probe_parent_idx < diagram_id_pair_count; probe_parent_idx ++ )
                {
                    bool is_parent;
                    is_parent = ( ((*this_).private_temp_diagram_ids_buf)[child_idx][1] == ((*this_).private_temp_diagram_ids_buf)[probe_parent_idx][0] );
                    if ( is_parent )
                    {
                        bool parent_has_parent;
                        parent_has_parent = ( DATA_ROW_ID_VOID != ((*this_).private_temp_diagram_ids_buf)[probe_parent_idx][1] );
                        if ( ! parent_has_parent )
                        {
                            /* this diagram is child to a parent that has no parent. */
                            ((*this_).private_temp_diagram_ids_buf)[child_idx][1] = DATA_ROW_ID_VOID;
                            found_children ++;
                        }
                    }
                }
            }
        }
        TRACE_INFO_INT_INT( "invalid+circular references search: round,found_children:", tree_depth, found_children );
        finished = ( found_children == 0 );
    }

    /* add all the rest to the io_set */
    for ( uint_fast32_t diag_idx = 0; diag_idx < diagram_id_pair_count; diag_idx ++ )
    {
        bool diag_has_parent;
        diag_has_parent = ( DATA_ROW_ID_VOID != ((*this_).private_temp_diagram_ids_buf)[diag_idx][1] );
        if ( diag_has_parent )
        {
            data_row_id_t diag_id = ((*this_).private_temp_diagram_ids_buf)[diag_idx][0];
            TSLOG_ERROR_INT( "Diagram has a parent that is not linked to root:", diag_id );
            data_small_set_add_row_id( io_set, DATA_TABLE_DIAGRAM, diag_id );
        }
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_consistency_checker_private_get_diagram_ids ( data_database_consistency_checker_t *this_,
                                                                         uint32_t max_out_array_size,
                                                                         data_row_id_t (*out_diagram_id_pair)[][2],
                                                                         uint32_t *out_diagram_id_pair_count )
{
    TRACE_BEGIN();
    assert( NULL != out_diagram_id_pair );
    assert( NULL != out_diagram_id_pair_count );
    assert( max_out_array_size <= MAX_ROWS_TO_CHECK );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    (*out_diagram_id_pair_count) = 0;

    if ( ! data_database_is_open( (*this_).database ) )
    {
        result = U8_ERROR_NO_DB;
        TSLOG_WARNING( "Database not open, cannot request data." );
    }
    else
    {
        sqlite3 *native_db;
        native_db = data_database_get_database_ptr( (*this_).database );
        sqlite3_stmt *prepared_statement;

        TSLOG_EVENT_STR( "sqlite3_prepare_v2():", SELECT_DIAGRAMS_IDS );
        sqlite_err =  sqlite3_prepare_v2 ( native_db,
                                           SELECT_DIAGRAMS_IDS,
                                           AUTO_DETECT_SQL_LENGTH,
                                           &prepared_statement,
                                           NO_SQL_DEBUG_INFORMATION
                                         );

        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_prepare_v2():", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }
        else
        {
            sqlite_err = SQLITE_ROW;
            for ( uint_fast32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= MAX_ROWS_TO_CHECK) && (result == U8_ERROR_NONE); row_index ++ )
            {
                TRACE_INFO( "sqlite3_step()" );
                sqlite_err = sqlite3_step( prepared_statement );

                if ( SQLITE_DONE == sqlite_err )
                {
                    TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
                }
                else if ( SQLITE_ROW == sqlite_err )
                {
                    data_row_id_t diag_id = sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMS_DIAG_ID_COLUMN );
                    data_row_id_t diag_parent_id = sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMS_DIAG_PARENT_ID_COLUMN );
                    if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_DIAGRAMS_DIAG_PARENT_ID_COLUMN ) )
                    {
                        diag_parent_id = DATA_ROW_ID_VOID;
                        TRACE_INFO_INT( "root:", diag_id );
                    }
                    else
                    {
                        TRACE_INFO_INT_INT( "ok: diag_id, parent_id:", diag_id, diag_parent_id );
                    }

                    /* diagram found */
                    if ( row_index < max_out_array_size )
                    {
                        (*out_diagram_id_pair)[row_index][0] = diag_id;
                        (*out_diagram_id_pair)[row_index][1] = diag_parent_id;
                        (*out_diagram_id_pair_count) = row_index + 1;
                    }
                    else
                    {
                        TSLOG_ERROR_INT( "Number of diagrams in the database exceeds max_out_array_size:", max_out_array_size );
                        result |= U8_ERROR_ARRAY_BUFFER_EXCEEDED;
                    }
                }
                else /*if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))*/
                {
                    TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                    result |= U8_ERROR_AT_DB;
                }
            }
        }

        sqlite_err = sqlite3_finalize( prepared_statement );
        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_finalize():", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_consistency_checker_find_nonreferencing_diagramelements ( data_database_consistency_checker_t *this_, data_small_set_t *io_set )
{
    TRACE_BEGIN();
    assert( NULL != io_set );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;

    if ( ! data_database_is_open( (*this_).database ) )
    {
        result = U8_ERROR_NO_DB;
        TSLOG_WARNING( "Database not open, cannot request data." );
    }
    else
    {
        sqlite3 *native_db;
        native_db = data_database_get_database_ptr( (*this_).database );
        sqlite3_stmt *prepared_statement;

        TSLOG_EVENT_STR( "sqlite3_prepare_v2():", SELECT_DIAGRAMELEMENTS_AND_RELATED );
        sqlite_err =  sqlite3_prepare_v2 ( native_db,
                                           SELECT_DIAGRAMELEMENTS_AND_RELATED,
                                           AUTO_DETECT_SQL_LENGTH,
                                           &prepared_statement,
                                           NO_SQL_DEBUG_INFORMATION
        );

        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_prepare_v2():", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }
        else
        {
            sqlite_err = SQLITE_ROW;
            for ( uint_fast32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= MAX_ROWS_TO_CHECK); row_index ++ )
            {
                TRACE_INFO( "sqlite3_step()" );
                sqlite_err = sqlite3_step( prepared_statement );

                if ( SQLITE_DONE == sqlite_err )
                {
                    TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
                }
                else if ( SQLITE_ROW == sqlite_err )
                {
                    data_row_id_t diagele_id = sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENTS_DIAGELE_ID_COLUMN );
                    data_row_id_t diagele_diagram_id = sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENTS_DIAGELE_DIAGRAM_ID_COLUMN );
                    data_row_id_t diagele_classifier_id = sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENTS_DIAGELE_CLASSIFIER_ID_COLUMN );
                    /*data_row_id_t diagram_id = sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENTS_DIAGRAM_ID_COLUMN );*/
                    /*data_row_id_t classifier_id = sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENTS_CLASSIFIER_ID_COLUMN );*/
                    bool diagram_exists = ( SQLITE_INTEGER == sqlite3_column_type( prepared_statement, RESULT_DIAGRAMELEMENTS_DIAGRAM_ID_COLUMN ) );
                    bool classifier_exists = ( SQLITE_INTEGER == sqlite3_column_type( prepared_statement, RESULT_DIAGRAMELEMENTS_CLASSIFIER_ID_COLUMN ) );
                    if (( ! diagram_exists ) && ( ! classifier_exists ))
                    {
                        TSLOG_ERROR_INT( "referenced diagram and classifier not existing, diagramelement:", diagele_id );
                        TRACE_INFO_INT_INT( "referenced diagram not existing: diagramelement, diagram:", diagele_id, diagele_diagram_id );
                        TRACE_INFO_INT_INT( "referenced classifier not existing: diagramelement, classifier:", diagele_id, diagele_classifier_id );
                        data_small_set_add_row_id( io_set, DATA_TABLE_DIAGRAMELEMENT, diagele_id );
                    }
                    else if ( ! diagram_exists )
                    {
                        TSLOG_ERROR_INT( "referenced diagram not existing, diagramelement:", diagele_id );
                        TRACE_INFO_INT_INT( "referenced diagram not existing: diagramelement, diagram:", diagele_id, diagele_diagram_id );
                        data_small_set_add_row_id( io_set, DATA_TABLE_DIAGRAMELEMENT, diagele_id );
                    }
                    else if ( ! classifier_exists )
                    {
                        TSLOG_ERROR_INT( "referenced classifier not existing, diagramelement:", diagele_id );
                        TRACE_INFO_INT_INT( "referenced classifier not existing: diagramelement, classifier:", diagele_id, diagele_classifier_id );
                        data_small_set_add_row_id( io_set, DATA_TABLE_DIAGRAMELEMENT, diagele_id );
                    }
                    else
                    {
                        TRACE_INFO_INT( "ok:", diagele_id );
                    }
                }
                else /*if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))*/
                {
                    TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                    result |= U8_ERROR_AT_DB;
                }
            }
        }

        sqlite_err = sqlite3_finalize( prepared_statement );
        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_finalize():", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_consistency_checker_find_invalid_focused_features ( data_database_consistency_checker_t *this_, data_small_set_t *io_set )
{
    TRACE_BEGIN();
    assert( NULL != io_set );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;

    if ( ! data_database_is_open( (*this_).database ) )
    {
        result = U8_ERROR_NO_DB;
        TSLOG_WARNING( "Database not open, cannot request data." );
    }
    else
    {
        sqlite3 *native_db;
        native_db = data_database_get_database_ptr( (*this_).database );
        sqlite3_stmt *prepared_statement;

        TSLOG_EVENT_STR( "sqlite3_prepare_v2():", SELECT_FOCUSED_FEATURES );
        sqlite_err =  sqlite3_prepare_v2 ( native_db,
                                           SELECT_FOCUSED_FEATURES,
                                           AUTO_DETECT_SQL_LENGTH,
                                           &prepared_statement,
                                           NO_SQL_DEBUG_INFORMATION
                                         );

        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_prepare_v2():", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }
        else
        {
            sqlite_err = SQLITE_ROW;
            for ( uint_fast32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= MAX_ROWS_TO_CHECK); row_index ++ )
            {
                TRACE_INFO( "sqlite3_step()" );
                sqlite_err = sqlite3_step( prepared_statement );

                if ( SQLITE_DONE == sqlite_err )
                {
                    TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
                }
                else if ( SQLITE_ROW == sqlite_err )
                {
                    data_row_id_t diagele_id;
                    data_row_id_t diagele_classifier_id;
                    bool diagele_has_focused_feature;
                    data_row_id_t diagele_focused_feature_id;
                    bool feature_exists;
                    data_row_id_t feature_id;
                    data_row_id_t feature_classifier_id;

                    /* fetch data record */
                    diagele_id = sqlite3_column_int64( prepared_statement, RESULT_FOCUSED_FEATURES_DIAGELE_ID_COLUMN );
                    diagele_classifier_id = sqlite3_column_int64( prepared_statement, RESULT_FOCUSED_FEATURES_DIAGELE_CLASSIFIER_ID_COLUMN );
                    diagele_has_focused_feature = ( SQLITE_NULL != sqlite3_column_type( prepared_statement, RESULT_FOCUSED_FEATURES_DIAGELE_FEATURE_ID_COLUMN ) );
                    if ( diagele_has_focused_feature )
                    {
                        diagele_focused_feature_id = sqlite3_column_int64( prepared_statement, RESULT_FOCUSED_FEATURES_DIAGELE_FEATURE_ID_COLUMN );
                    }
                    else
                    {
                        diagele_focused_feature_id = DATA_ROW_ID_VOID;
                    }
                    feature_exists = ( SQLITE_NULL != sqlite3_column_type( prepared_statement, RESULT_FOCUSED_FEATURES_FEATURE_ID_COLUMN ) );
                    if ( feature_exists )
                    {
                        feature_id = sqlite3_column_int64( prepared_statement, RESULT_FOCUSED_FEATURES_FEATURE_ID_COLUMN );
                        feature_classifier_id = sqlite3_column_int64( prepared_statement,                                       RESULT_FOCUSED_FEATURES_FEATURE_CLASSIFIER_ID_COLUMN );
                    }
                    else
                    {
                        feature_id = DATA_ROW_ID_VOID;
                        feature_classifier_id = DATA_ROW_ID_VOID;
                    }

                    /* evaluate the data record */
                    if ( ! diagele_has_focused_feature )
                    {
                        TRACE_INFO_INT( "ok (no focused feature):", diagele_id );
                    }
                    else
                    {
                        if ( ! feature_exists )
                        {
                            TSLOG_ERROR_INT( "focused feature not existing, diagramelement:", diagele_id );
                            TRACE_INFO_INT_INT( "referenced classifier, classifier of feature:", diagele_classifier_id, feature_classifier_id );
                            TRACE_INFO_INT_INT( "referenced feature, feature:", diagele_focused_feature_id, feature_id );
                            data_small_set_add_row_id( io_set, DATA_TABLE_DIAGRAMELEMENT, diagele_id );
                        }
                        else if ( diagele_classifier_id != feature_classifier_id )
                        {
                            TSLOG_ERROR_INT( "referenced classifier of diagramelement and focused_feature differ, diagramelement:", diagele_id );
                            TRACE_INFO_INT_INT( "referenced classifier, classifier of feature:", diagele_classifier_id, feature_classifier_id );
                            TRACE_INFO_INT_INT( "referenced feature, feature:", diagele_focused_feature_id, feature_id );
                            data_small_set_add_row_id( io_set, DATA_TABLE_DIAGRAMELEMENT, diagele_id );
                        }
                        else
                        {
                            TRACE_INFO_INT( "ok (valid focused feature):", diagele_id );
                        }
                    }
                }
                else /*if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))*/
                {
                    TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                    result |= U8_ERROR_AT_DB;
                }
            }
        }

        sqlite_err = sqlite3_finalize( prepared_statement );
        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_finalize():", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_consistency_checker_find_unreferenced_classifiers ( data_database_consistency_checker_t *this_, data_small_set_t *io_set )
{
    TRACE_BEGIN();
    assert( NULL != io_set );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;

    if ( ! data_database_is_open( (*this_).database ) )
    {
        result = U8_ERROR_NO_DB;
        TSLOG_WARNING( "Database not open, cannot request data." );
    }
    else
    {
        sqlite3 *native_db;
        native_db = data_database_get_database_ptr( (*this_).database );
        sqlite3_stmt *prepared_statement;

        TSLOG_EVENT_STR( "sqlite3_prepare_v2():", SELECT_CLASSIFIERS_AND_DIAGRAMELEMENTS );
        sqlite_err =  sqlite3_prepare_v2 ( native_db,
                                           SELECT_CLASSIFIERS_AND_DIAGRAMELEMENTS,
                                           AUTO_DETECT_SQL_LENGTH,
                                           &prepared_statement,
                                           NO_SQL_DEBUG_INFORMATION
        );

        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_prepare_v2():", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }
        else
        {
            sqlite_err = SQLITE_ROW;
            for ( uint_fast32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= MAX_ROWS_TO_CHECK); row_index ++ )
            {
                TRACE_INFO( "sqlite3_step()" );
                sqlite_err = sqlite3_step( prepared_statement );

                if ( SQLITE_DONE == sqlite_err )
                {
                    TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
                }
                else if ( SQLITE_ROW == sqlite_err )
                {
                    data_row_id_t classifier_id = sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIERS_CLASSIFIER_ID_COLUMN );
                    /*
                    data_row_id_t diagele_classifier_id = sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIERS_DIAGELE_CLASSIFIER_ID_COLUMN );
                    */
                    data_row_id_t diagele_id = sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIERS_DIAGELE_ID_COLUMN );
                    bool diagele_exists = ( SQLITE_INTEGER == sqlite3_column_type( prepared_statement, RESULT_CLASSIFIERS_DIAGELE_CLASSIFIER_ID_COLUMN ) );
                    if ( ! diagele_exists )
                    {
                        TSLOG_ERROR_INT( "referencing diagramelement not existing, classifier :", classifier_id );
                        TRACE_INFO_INT( "referencing diagramelement not existing: classifier:", classifier_id );
                        data_small_set_add_row_id( io_set, DATA_TABLE_CLASSIFIER, classifier_id );
                    }
                    else
                    {
                        TRACE_INFO_INT_INT( "ok (classifier_id,diagele_id):", classifier_id, diagele_id );
                    }
                }
                else /*if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))*/
                {
                    TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                    result |= U8_ERROR_AT_DB;
                }
            }
        }

        sqlite_err = sqlite3_finalize( prepared_statement );
        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_finalize():", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_consistency_checker_find_unreferenced_features ( data_database_consistency_checker_t *this_, data_small_set_t *io_set )
{
    TRACE_BEGIN();
    assert( NULL != io_set );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;

    if ( ! data_database_is_open( (*this_).database ) )
    {
        result = U8_ERROR_NO_DB;
        TSLOG_WARNING( "Database not open, cannot request data." );
    }
    else
    {
        sqlite3 *native_db;
        native_db = data_database_get_database_ptr( (*this_).database );
        sqlite3_stmt *prepared_statement;

        TSLOG_EVENT_STR( "sqlite3_prepare_v2():", SELECT_FEATURES_AND_CLASSIFIERS );
        sqlite_err =  sqlite3_prepare_v2 ( native_db,
                                           SELECT_FEATURES_AND_CLASSIFIERS,
                                           AUTO_DETECT_SQL_LENGTH,
                                           &prepared_statement,
                                           NO_SQL_DEBUG_INFORMATION
        );

        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_prepare_v2():", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }
        else
        {
            sqlite_err = SQLITE_ROW;
            for ( uint_fast32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= MAX_ROWS_TO_CHECK); row_index ++ )
            {
                TRACE_INFO( "sqlite3_step()" );
                sqlite_err = sqlite3_step( prepared_statement );

                if ( SQLITE_DONE == sqlite_err )
                {
                    TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
                }
                else if ( SQLITE_ROW == sqlite_err )
                {
                    data_row_id_t feature_id = sqlite3_column_int64( prepared_statement, RESULT_FEATURES_FEATURE_ID_COLUMN );
                    data_row_id_t feature_classifier_id = sqlite3_column_int64( prepared_statement, RESULT_FEATURES_FEATURE_CLASSIFIER_ID_COLUMN );
                    /*data_row_id_t classifier_id = sqlite3_column_int64( prepared_statement, RESULT_FEATURES_CLASSIFIER_ID_COLUMN );*/
                    bool classifier_exists = ( SQLITE_INTEGER == sqlite3_column_type( prepared_statement, RESULT_FEATURES_CLASSIFIER_ID_COLUMN ) );
                    if ( ! classifier_exists )
                    {
                        TSLOG_ERROR_INT( "referenced classifier not existing, feature:", feature_id );
                        TRACE_INFO_INT_INT( "referenced classifier not existing: feature, classifier:", feature_id, feature_classifier_id );
                        data_small_set_add_row_id( io_set, DATA_TABLE_FEATURE, feature_id );
                    }
                    else
                    {
                        TRACE_INFO_INT( "ok:", feature_id );
                    }
                }
                else /*if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))*/
                {
                    TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                    result |= U8_ERROR_AT_DB;
                }
            }
        }

        sqlite_err = sqlite3_finalize( prepared_statement );
        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_finalize():", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_consistency_checker_find_unreferenced_relationships ( data_database_consistency_checker_t *this_, data_small_set_t *io_set )
{
    TRACE_BEGIN();
    assert( NULL != io_set );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;

    if ( ! data_database_is_open( (*this_).database ) )
    {
        result = U8_ERROR_NO_DB;
        TSLOG_WARNING( "Database not open, cannot request data." );
    }
    else
    {
        sqlite3 *native_db;
        native_db = data_database_get_database_ptr( (*this_).database );
        sqlite3_stmt *prepared_statement;

        TSLOG_EVENT_STR( "sqlite3_prepare_v2():", SELECT_RELATIONSHIPS_AND_CLASSIFIERS );
        sqlite_err =  sqlite3_prepare_v2 ( native_db,
                                           SELECT_RELATIONSHIPS_AND_CLASSIFIERS,
                                           AUTO_DETECT_SQL_LENGTH,
                                           &prepared_statement,
                                           NO_SQL_DEBUG_INFORMATION
        );

        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_prepare_v2():", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }
        else
        {
            sqlite_err = SQLITE_ROW;
            for ( uint_fast32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= MAX_ROWS_TO_CHECK); row_index ++ )
            {
                TRACE_INFO( "sqlite3_step()" );
                sqlite_err = sqlite3_step( prepared_statement );

                if ( SQLITE_DONE == sqlite_err )
                {
                    TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
                }
                else if ( SQLITE_ROW == sqlite_err )
                {
                    data_row_id_t relation_id = sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIPS_RELATIONSHIP_ID_COLUMN );
                    data_row_id_t relation_from_id = sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIPS_RELATIONSHIP_FROM_ID_COLUMN );
                    data_row_id_t relation_to_id = sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIPS_RELATIONSHIP_TO_ID_COLUMN );
                    /*data_row_id_t source_id = sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIPS_SOURCE_ID_COLUMN );*/
                    /*data_row_id_t dest_id = sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIPS_DEST_ID_COLUMN );*/
                    bool source_exists = ( SQLITE_INTEGER == sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIPS_SOURCE_ID_COLUMN ) );
                    bool dest_exists = ( SQLITE_INTEGER == sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIPS_DEST_ID_COLUMN ) );
                    if (( ! source_exists ) && ( ! dest_exists ))
                    {
                        TSLOG_ERROR_INT( "relationship referencing non-existing source and destiation, relationship:", relation_id );
                        TRACE_INFO_INT_INT( "relationship referencing non-existing source: relation_id, classifier:", relation_id, relation_from_id );
                        TRACE_INFO_INT_INT( "relationship referencing non-existing destination: relation_id, classifier:", relation_id, relation_to_id );
                        data_small_set_add_row_id( io_set, DATA_TABLE_RELATIONSHIP, relation_id );
                    }
                    else if ( ! source_exists )
                    {
                        TSLOG_ERROR_INT( "referenced relationship referencing non-existing source, relationship:", relation_id );
                        TRACE_INFO_INT_INT( "referenced relationship referencing non-existing source: relation_id, classifier:", relation_id, relation_from_id );
                        data_small_set_add_row_id( io_set, DATA_TABLE_RELATIONSHIP, relation_id );
                    }
                    else if ( ! dest_exists )
                    {
                        TSLOG_ERROR_INT( "referenced relationship referencing non-existing destination, relationship:", relation_id );
                        TRACE_INFO_INT_INT( "referenced relationship referencing non-existing destination: relation_id, classifier:", relation_id, relation_to_id );
                        data_small_set_add_row_id( io_set, DATA_TABLE_RELATIONSHIP, relation_id );
                    }
                    else
                    {
                        TRACE_INFO_INT( "ok:", relation_id );
                    }
                }
                else /*if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))*/
                {
                    TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                    result |= U8_ERROR_AT_DB;
                }
            }
        }

        sqlite_err = sqlite3_finalize( prepared_statement );
        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_finalize():", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_consistency_checker_find_invalid_relationship_features ( data_database_consistency_checker_t *this_, data_small_set_t *io_set )
{
    TRACE_BEGIN();
    assert( NULL != io_set );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;

    if ( ! data_database_is_open( (*this_).database ) )
    {
        result = U8_ERROR_NO_DB;
        TSLOG_WARNING( "Database not open, cannot request data." );
    }
    else
    {
        sqlite3 *native_db;
        native_db = data_database_get_database_ptr( (*this_).database );
        sqlite3_stmt *prepared_statement;

        TSLOG_EVENT_STR( "sqlite3_prepare_v2():", SELECT_FEATURE_RELATIONSHIPS );
        sqlite_err =  sqlite3_prepare_v2 ( native_db,
                                           SELECT_FEATURE_RELATIONSHIPS,
                                           AUTO_DETECT_SQL_LENGTH,
                                           &prepared_statement,
                                           NO_SQL_DEBUG_INFORMATION
                                         );

        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_prepare_v2():", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }
        else
        {
            sqlite_err = SQLITE_ROW;
            for ( uint_fast32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= MAX_ROWS_TO_CHECK); row_index ++ )
            {
                TRACE_INFO( "sqlite3_step()" );
                sqlite_err = sqlite3_step( prepared_statement );

                if ( SQLITE_DONE == sqlite_err )
                {
                    TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
                }
                else if ( SQLITE_ROW == sqlite_err )
                {
                    data_row_id_t relation_id;
                    data_row_id_t relation_from_classifier_id;
                    data_row_id_t relation_to_classifier_id;
                    bool relation_has_from_feature;
                    data_row_id_t relation_from_feature_id;
                    bool relation_has_to_feature;
                    data_row_id_t relation_to_feature_id;
                    bool source_feature_exists;
                    data_row_id_t source_feature_id;
                    data_row_id_t source_feature_classifier_id;
                    bool dest_feature_exists;
                    data_row_id_t dest_feature_id;
                    data_row_id_t dest_feature_classifier_id;

                    /* fetch data record */
                    relation_id = sqlite3_column_int64( prepared_statement, RESULT_FEATURE_RELATIONSHIPS_RELATION_ID_COLUMN );
                    relation_from_classifier_id = sqlite3_column_int64( prepared_statement, RESULT_FEATURE_RELATIONSHIPS_RELATION_FROM_CLASSIFIER_ID_COLUMN );
                    relation_to_classifier_id = sqlite3_column_int64( prepared_statement, RESULT_FEATURE_RELATIONSHIPS_RELATION_TO_CLASIFIER_ID_COLUMN );
                    relation_has_from_feature = ( SQLITE_NULL != sqlite3_column_type( prepared_statement, RESULT_FEATURE_RELATIONSHIPS_RELATION_FROM_FEATURE_ID_COLUMN ) );
                    if ( relation_has_from_feature )
                    {
                        relation_from_feature_id = sqlite3_column_int64( prepared_statement, RESULT_FEATURE_RELATIONSHIPS_RELATION_FROM_FEATURE_ID_COLUMN );
                    }
                    else
                    {
                        relation_from_feature_id = DATA_ROW_ID_VOID;
                    }
                    relation_has_to_feature = ( SQLITE_NULL != sqlite3_column_type( prepared_statement, RESULT_FEATURE_RELATIONSHIPS_RELATION_TO_FEATURE_ID_COLUMN ) );
                    if ( relation_has_to_feature )
                    {
                        relation_to_feature_id = sqlite3_column_int64( prepared_statement, RESULT_FEATURE_RELATIONSHIPS_RELATION_TO_FEATURE_ID_COLUMN );
                    }
                    else
                    {
                        relation_to_feature_id = DATA_ROW_ID_VOID;
                    }
                    source_feature_exists = ( SQLITE_NULL != sqlite3_column_type( prepared_statement, RESULT_FEATURE_RELATIONSHIPS_SOURCE_FEATURE_ID_COLUMN ) );
                    if ( source_feature_exists )
                    {
                        source_feature_id = sqlite3_column_int64( prepared_statement, RESULT_FEATURE_RELATIONSHIPS_SOURCE_FEATURE_ID_COLUMN );
                        source_feature_classifier_id = sqlite3_column_int64( prepared_statement, RESULT_FEATURE_RELATIONSHIPS_SOURCE_FEATURE_CLASSIFIER_ID_COLUMN );
                    }
                    else
                    {
                        source_feature_id = DATA_ROW_ID_VOID;
                        source_feature_classifier_id = DATA_ROW_ID_VOID;
                    }
                    dest_feature_exists = ( SQLITE_NULL != sqlite3_column_type( prepared_statement, RESULT_FEATURE_RELATIONSHIPS_DEST_FEATURE_ID_COLUMN ) );
                    if ( dest_feature_exists )
                    {
                        dest_feature_id = sqlite3_column_int64( prepared_statement, RESULT_FEATURE_RELATIONSHIPS_DEST_FEATURE_ID_COLUMN );
                        dest_feature_classifier_id = sqlite3_column_int64( prepared_statement, RESULT_FEATURE_RELATIONSHIPS_DEST_FEATURE_CLASSIFIER_ID_COLUMN );
                    }
                    else
                    {
                        dest_feature_id = DATA_ROW_ID_VOID;
                        dest_feature_classifier_id = DATA_ROW_ID_VOID;
                    }

                    /* evaluate the data record */
                    if ( ! relation_has_from_feature )
                    {
                        if ( ! relation_has_to_feature )
                        {
                            TRACE_INFO_INT( "ok (no feature relation):", relation_id );
                        }
                        else
                        {
                            if ( ! dest_feature_exists )
                            {
                                TSLOG_ERROR_INT( "relation destination feature not existing, relationship:", relation_id );
                                data_small_set_add_row_id( io_set, DATA_TABLE_RELATIONSHIP, relation_id );
                            }
                            else if ( relation_to_classifier_id != dest_feature_classifier_id )
                            {
                                TSLOG_ERROR_INT( "referenced classifier of relationship and feature differ, relationship:", relation_id );
                                TRACE_INFO_INT_INT( "referenced to classifier, dest feature classifier:", relation_to_classifier_id, dest_feature_classifier_id );
                                TRACE_INFO_INT_INT( "referenced to feature, dest feature:", relation_to_feature_id, dest_feature_id );
                                data_small_set_add_row_id( io_set, DATA_TABLE_RELATIONSHIP, relation_id );
                            }
                            else
                            {
                                TRACE_INFO_INT( "ok (valid destination feature, no source feature):", relation_id );
                            }
                        }
                    }
                    else
                    {
                        if ( ! relation_has_to_feature )
                        {
                            if ( ! source_feature_exists )
                            {
                                TSLOG_ERROR_INT( "relation source feature not existing, relationship:", relation_id );
                                data_small_set_add_row_id( io_set, DATA_TABLE_RELATIONSHIP, relation_id );
                            }
                            else if ( relation_from_classifier_id != source_feature_classifier_id )
                            {
                                TSLOG_ERROR_INT( "referenced classifier of relationship and feature differ, relationship:", relation_id );
                                TRACE_INFO_INT_INT( "referenced from classifier, src feature classifier:", relation_from_classifier_id, source_feature_classifier_id );
                                TRACE_INFO_INT_INT( "referenced from feature, src feature:", relation_from_feature_id, source_feature_id );
                                data_small_set_add_row_id( io_set, DATA_TABLE_RELATIONSHIP, relation_id );
                            }
                            else
                            {
                                TRACE_INFO_INT( "ok (valid source feature, no destination feature):", relation_id );
                            }
                        }
                        else
                        {
                            if ( ! source_feature_exists )
                            {
                                TSLOG_ERROR_INT( "relation source feature not existing, relationship:", relation_id );
                                data_small_set_add_row_id( io_set, DATA_TABLE_RELATIONSHIP, relation_id );
                            }
                            else if ( ! dest_feature_exists )
                            {
                                TSLOG_ERROR_INT( "relation destination feature not existing, relationship:", relation_id );
                                data_small_set_add_row_id( io_set, DATA_TABLE_RELATIONSHIP, relation_id );
                            }
                            else if ( relation_from_classifier_id != source_feature_classifier_id )
                            {
                                TSLOG_ERROR_INT( "referenced classifier of relationship and feature differ, relationship:", relation_id );
                                TRACE_INFO_INT_INT( "referenced from classifier, src feature classifier:", relation_from_classifier_id, source_feature_classifier_id );
                                TRACE_INFO_INT_INT( "referenced from feature, src feature:", relation_from_feature_id, source_feature_id );
                                data_small_set_add_row_id( io_set, DATA_TABLE_RELATIONSHIP, relation_id );
                            }
                            else if ( relation_to_classifier_id != dest_feature_classifier_id )
                            {
                                TSLOG_ERROR_INT( "referenced classifier of relationship and feature differ, relationship:", relation_id );
                                TRACE_INFO_INT_INT( "referenced to classifier, dest feature classifier:", relation_to_classifier_id, dest_feature_classifier_id );
                                TRACE_INFO_INT_INT( "referenced to feature, dest feature:", relation_to_feature_id, dest_feature_id );
                                data_small_set_add_row_id( io_set, DATA_TABLE_RELATIONSHIP, relation_id );
                            }
                            else
                            {
                                TRACE_INFO_INT( "ok (valid source and destination features):", relation_id );
                            }
                        }
                    }
                }
                else /*if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))*/
                {
                    TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                    result |= U8_ERROR_AT_DB;
                }
            }
        }

        sqlite_err = sqlite3_finalize( prepared_statement );
        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_finalize():", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }
    }

    TRACE_END_ERR( result );
    return result;
}

/*!
 *  \brief string constant to start a transaction
 */
static const char *DATA_DATABASE_CONSISTENCY_CHECKER_BEGIN_TRANSACTION =
    "BEGIN TRANSACTION;";

/*!
 *  \brief string constant to commit a transaction
 */
static const char *DATA_DATABASE_CONSISTENCY_CHECKER_COMMIT_TRANSACTION =
    "COMMIT TRANSACTION;";

/*!
 *  \brief prefix string constant to delete a classifier
 */
static const char *DATA_DATABASE_CONSISTENCY_CHECKER_DELETE_CLASSIFIER_PREFIX =
    "DELETE FROM classifiers WHERE (id=";

/*!
 *  \brief postfix string constant to delete a classifier
 */
static const char *DATA_DATABASE_CONSISTENCY_CHECKER_DELETE_CLASSIFIER_POSTFIX = ");";

u8_error_t data_database_consistency_checker_kill_classifier( data_database_consistency_checker_t *this_, data_row_id_t obj_id )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    char *error_msg = NULL;
    sqlite3 *db = data_database_get_database_ptr( (*this_).database );

    if ( data_database_is_open( (*this_).database ) )
    {
        TSLOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_CONSISTENCY_CHECKER_BEGIN_TRANSACTION );
        sqlite_err = sqlite3_exec( db, DATA_DATABASE_CONSISTENCY_CHECKER_BEGIN_TRANSACTION, NULL, NULL, &error_msg );
        if ( SQLITE_OK != sqlite_err )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_CONSISTENCY_CHECKER_BEGIN_TRANSACTION );
            TSLOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }
        if ( error_msg != NULL )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
            sqlite3_free( error_msg );
            error_msg = NULL;
        }

        char delete_statement_buf[56];
        utf8stringbuf_t delete_statement = UTF8STRINGBUF( delete_statement_buf );
        utf8stringbuf_copy_str( delete_statement, DATA_DATABASE_CONSISTENCY_CHECKER_DELETE_CLASSIFIER_PREFIX );
        utf8stringbuf_append_int( delete_statement, obj_id );
        utf8stringbuf_append_str( delete_statement, DATA_DATABASE_CONSISTENCY_CHECKER_DELETE_CLASSIFIER_POSTFIX );
        TSLOG_EVENT_STR( "sqlite3_exec:", utf8stringbuf_get_string(delete_statement) );
        sqlite_err = sqlite3_exec( db, utf8stringbuf_get_string(delete_statement), NULL, NULL, &error_msg );
        if ( SQLITE_OK != sqlite_err )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", utf8stringbuf_get_string(delete_statement) );
            TSLOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
            result |= (sqlite_err == SQLITE_READONLY) ? U8_ERROR_READ_ONLY_DB : U8_ERROR_AT_DB;
        }
        if ( error_msg != NULL )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
            sqlite3_free( error_msg );
            error_msg = NULL;
        }

        TSLOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_CONSISTENCY_CHECKER_COMMIT_TRANSACTION );
        sqlite_err = sqlite3_exec( db, DATA_DATABASE_CONSISTENCY_CHECKER_COMMIT_TRANSACTION, NULL, NULL, &error_msg );
        if ( SQLITE_OK != sqlite_err )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_CONSISTENCY_CHECKER_COMMIT_TRANSACTION );
            TSLOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }
        if ( error_msg != NULL )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
            sqlite3_free( error_msg );
            error_msg = NULL;
        }
    }
    else
    {
        TSLOG_WARNING( "database not open." );
        result = U8_ERROR_NO_DB;
    }

    TRACE_END_ERR( result );
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
