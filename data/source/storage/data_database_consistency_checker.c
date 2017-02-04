/* File: data_database_consistency_checker.c; Copyright and License: see below */

#include "storage/data_database_consistency_checker.h"
#include "trace.h"
#include "tslog.h"
#include "util/string/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>

data_error_t data_database_consistency_checker_init ( data_database_consistency_checker_t *this_, data_database_t *database )
{
    TRACE_BEGIN();
    assert( NULL != database );
    data_error_t result = DATA_ERROR_NONE;

    (*this_).database = database;

    TRACE_END_ERR(result);
    return result;
}


data_error_t data_database_consistency_checker_destroy ( data_database_consistency_checker_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

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
 *  \brief search statement to find orphaned diagrams
 */
static const char SELECT_DIAGRAMS_AND_PARENTS[] =
    "SELECT child.id,child.parent_id,parent.id "
    "FROM diagrams AS child "
    "LEFT JOIN diagrams AS parent ON child.parent_id=parent.id;";

/*!
 *  \brief the column id of the result where this parameter is stored: child.id
 */
static const int RESULT_DIAGRAMS_CHILD_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: child.parent_id
 */
static const int RESULT_DIAGRAMS_CHILD_PARENT_ID_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: parent.id
 */
static const int RESULT_DIAGRAMS_PARENT_ID_COLUMN = 2;

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
 *  \brief search statement to find relationships that contain invalid references
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

data_error_t data_database_consistency_checker_find_unreferenced_diagrams ( data_database_consistency_checker_t *this_,
                                                                            uint32_t *out_total_count,
                                                                            data_small_set_t *io_set )
{
    TRACE_BEGIN();
    assert( NULL != io_set );
    assert( NULL != out_total_count );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    (*out_total_count) = 0;

    if ( ! data_database_is_open( (*this_).database ) )
    {
        result = DATA_ERROR_NO_DB;
        TSLOG_WARNING( "Database not open, cannot request data." );
    }
    else
    {
        sqlite3 *native_db;
        native_db = data_database_get_database_ptr( (*this_).database );
        sqlite3_stmt *prepared_statement;

        TSLOG_EVENT_STR( "sqlite3_prepare_v2():", SELECT_DIAGRAMS_AND_PARENTS );
        sqlite_err =  sqlite3_prepare_v2 ( native_db,
                                           SELECT_DIAGRAMS_AND_PARENTS,
                                           AUTO_DETECT_SQL_LENGTH,
                                           &prepared_statement,
                                           NO_SQL_DEBUG_INFORMATION
                                         );

        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_prepare_v2():", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }
        else
        {
            sqlite_err = SQLITE_ROW;
            for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= MAX_ROWS_TO_CHECK); row_index ++ )
            {
                TRACE_INFO( "sqlite3_step()" );
                sqlite_err = sqlite3_step( prepared_statement );

                if ( SQLITE_DONE == sqlite_err )
                {
                    TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
                }
                else if ( SQLITE_ROW == sqlite_err )
                {
                    int64_t child_id = sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMS_CHILD_ID_COLUMN );
                    int64_t child_parent_id = sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMS_CHILD_PARENT_ID_COLUMN );
                    /*int64_t parent_id = sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMS_PARENT_ID_COLUMN );*/
                    bool parent_exists = ( SQLITE_INTEGER == sqlite3_column_type( prepared_statement, RESULT_DIAGRAMS_PARENT_ID_COLUMN ) );
                    if ( child_parent_id == DATA_ID_VOID_ID )
                    {
                        TRACE_INFO_INT( "root:", child_id );
                    }
                    else if ( ! parent_exists )
                    {
                        TSLOG_ERROR_INT( "referenced parent not existing, child:", child_id );
                        TRACE_INFO_INT_INT( "referenced parent not existing: child, parent:", child_id, child_parent_id );
                        data_small_set_add_row_id( io_set, DATA_TABLE_DIAGRAM, child_id );
                    }
                    else
                    {
                        TRACE_INFO_INT( "ok:", child_id );
                    }

                    /* diagram found */
                    (*out_total_count)++;
                }
                else /*if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))*/
                {
                    TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                    result |= DATA_ERROR_AT_DB;
                }
            }
        }

        sqlite_err = sqlite3_finalize( prepared_statement );
        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_finalize():", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_consistency_checker_find_nonreferencing_diagramelements ( data_database_consistency_checker_t *this_, data_small_set_t *io_set )
{
    TRACE_BEGIN();
    assert( NULL != io_set );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;

    if ( ! data_database_is_open( (*this_).database ) )
    {
        result = DATA_ERROR_NO_DB;
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
            result |= DATA_ERROR_AT_DB;
        }
        else
        {
            sqlite_err = SQLITE_ROW;
            for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= MAX_ROWS_TO_CHECK); row_index ++ )
            {
                TRACE_INFO( "sqlite3_step()" );
                sqlite_err = sqlite3_step( prepared_statement );

                if ( SQLITE_DONE == sqlite_err )
                {
                    TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
                }
                else if ( SQLITE_ROW == sqlite_err )
                {
                    int64_t diagele_id = sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENTS_DIAGELE_ID_COLUMN );
                    int64_t diagele_diagram_id = sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENTS_DIAGELE_DIAGRAM_ID_COLUMN );
                    int64_t diagele_classifier_id = sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENTS_DIAGELE_CLASSIFIER_ID_COLUMN );
                    /*int64_t diagram_id = sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENTS_DIAGRAM_ID_COLUMN );*/
                    /*int64_t classifier_id = sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENTS_CLASSIFIER_ID_COLUMN );*/
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
                    result |= DATA_ERROR_AT_DB;
                }
            }
        }

        sqlite_err = sqlite3_finalize( prepared_statement );
        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_finalize():", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_consistency_checker_find_unreferenced_classifiers ( data_database_consistency_checker_t *this_, data_small_set_t *io_set )
{
    TRACE_BEGIN();
    assert( NULL != io_set );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;

    if ( ! data_database_is_open( (*this_).database ) )
    {
        result = DATA_ERROR_NO_DB;
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
            result |= DATA_ERROR_AT_DB;
        }
        else
        {
            sqlite_err = SQLITE_ROW;
            for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= MAX_ROWS_TO_CHECK); row_index ++ )
            {
                TRACE_INFO( "sqlite3_step()" );
                sqlite_err = sqlite3_step( prepared_statement );

                if ( SQLITE_DONE == sqlite_err )
                {
                    TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
                }
                else if ( SQLITE_ROW == sqlite_err )
                {
                    int64_t classifier_id = sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIERS_CLASSIFIER_ID_COLUMN );
                    /*int64_t diagele_classifier_parent_id = sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIERS_DIAGELE_CLASSIFIER_ID_COLUMN );*/
                    /*int64_t diagele_id = sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIERS_DIAGELE_ID_COLUMN );*/
                    bool diagele_exists = ( SQLITE_INTEGER == sqlite3_column_type( prepared_statement, RESULT_CLASSIFIERS_DIAGELE_CLASSIFIER_ID_COLUMN ) );
                    if ( ! diagele_exists )
                    {
                        TSLOG_ERROR_INT( "referencing diagramelement not existing, classifier :", classifier_id );
                        TRACE_INFO_INT( "referencing diagramelement not existing: classifier:", classifier_id );
                        data_small_set_add_row_id( io_set, DATA_TABLE_CLASSIFIER, classifier_id );
                    }
                    else
                    {
                        TRACE_INFO_INT( "ok:", classifier_id );
                    }
                }
                else /*if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))*/
                {
                    TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                    result |= DATA_ERROR_AT_DB;
                }
            }
        }

        sqlite_err = sqlite3_finalize( prepared_statement );
        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_finalize():", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_consistency_checker_find_unreferenced_features ( data_database_consistency_checker_t *this_, data_small_set_t *io_set )
{
    TRACE_BEGIN();
    assert( NULL != io_set );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;

    if ( ! data_database_is_open( (*this_).database ) )
    {
        result = DATA_ERROR_NO_DB;
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
            result |= DATA_ERROR_AT_DB;
        }
        else
        {
            sqlite_err = SQLITE_ROW;
            for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= MAX_ROWS_TO_CHECK); row_index ++ )
            {
                TRACE_INFO( "sqlite3_step()" );
                sqlite_err = sqlite3_step( prepared_statement );

                if ( SQLITE_DONE == sqlite_err )
                {
                    TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
                }
                else if ( SQLITE_ROW == sqlite_err )
                {
                    int64_t feature_id = sqlite3_column_int64( prepared_statement, RESULT_FEATURES_FEATURE_ID_COLUMN );
                    int64_t feature_classifier_id = sqlite3_column_int64( prepared_statement, RESULT_FEATURES_FEATURE_CLASSIFIER_ID_COLUMN );
                    /*int64_t classifier_id = sqlite3_column_int64( prepared_statement, RESULT_FEATURES_CLASSIFIER_ID_COLUMN );*/
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
                    result |= DATA_ERROR_AT_DB;
                }
            }
        }

        sqlite_err = sqlite3_finalize( prepared_statement );
        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_finalize():", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_consistency_checker_find_unreferenced_relationships ( data_database_consistency_checker_t *this_, data_small_set_t *io_set )
{
    TRACE_BEGIN();
    assert( NULL != io_set );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;

    if ( ! data_database_is_open( (*this_).database ) )
    {
        result = DATA_ERROR_NO_DB;
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
            result |= DATA_ERROR_AT_DB;
        }
        else
        {
            sqlite_err = SQLITE_ROW;
            for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= MAX_ROWS_TO_CHECK); row_index ++ )
            {
                TRACE_INFO( "sqlite3_step()" );
                sqlite_err = sqlite3_step( prepared_statement );

                if ( SQLITE_DONE == sqlite_err )
                {
                    TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
                }
                else if ( SQLITE_ROW == sqlite_err )
                {
                    int64_t relation_id = sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIPS_RELATIONSHIP_ID_COLUMN );
                    int64_t relation_from_id = sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIPS_RELATIONSHIP_FROM_ID_COLUMN );
                    int64_t relation_to_id = sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIPS_RELATIONSHIP_TO_ID_COLUMN );
                    /*int64_t source_id = sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIPS_SOURCE_ID_COLUMN );*/
                    /*int64_t dest_id = sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIPS_DEST_ID_COLUMN );*/
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
                    result |= DATA_ERROR_AT_DB;
                }
            }
        }

        sqlite_err = sqlite3_finalize( prepared_statement );
        if ( 0 != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_finalize():", sqlite_err );
            result |= DATA_ERROR_AT_DB;
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

data_error_t data_database_consistency_checker_kill_classifier( data_database_consistency_checker_t *this_, int64_t obj_id )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;
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
            result |= DATA_ERROR_AT_DB;
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
            result |= DATA_ERROR_AT_DB;
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
            result |= DATA_ERROR_AT_DB;
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
        result = DATA_ERROR_NO_DB;
    }

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2016-2017 Andreas Warnke

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
