/* File: data_database_classifier_reader.c; Copyright and License: see below */

#include "storage/data_database_classifier_reader.h"
#include "storage/data_database_borrowed_stmt.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>

u8_error_t data_database_classifier_reader_init ( data_database_classifier_reader_t *this_, data_database_t *database )
{
    U8_TRACE_BEGIN();
    assert( NULL != database );
    u8_error_t result = U8_ERROR_NONE;

    (*this_).database = database;

    (*this_).statement_classifier_by_id = NULL;
    (*this_).statement_classifier_by_name = NULL;
    (*this_).statement_classifier_by_uuid = NULL;
    (*this_).statement_classifiers_all = NULL;
    (*this_).statement_classifiers_all_hierarchical = NULL;

    (*this_).statement_feature_by_id = NULL;
    (*this_).statement_feature_by_uuid = NULL;
    (*this_).statement_features_by_classifier_id = NULL;
    (*this_).statement_features_by_diagram_id = NULL;

    (*this_).statement_relationship_by_id = NULL;
    (*this_).statement_relationship_by_uuid = NULL;
    (*this_).statement_relationships_by_classifier_id = NULL;
    (*this_).statement_relationships_by_feature_id = NULL;
    (*this_).statement_relationships_by_diagram_id = NULL;

    result |= data_database_classifier_reader_private_open( this_ );

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_database_classifier_reader_destroy ( data_database_classifier_reader_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    result |= data_database_classifier_reader_private_close( this_ );

    (*this_).database = NULL;

    U8_TRACE_END_ERR(result);
    return result;
}

/* ================================ CLASSIFIER ================================ */

/*!
 *  \brief predefined search statement to find a classifier by id
 */
static const char DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_ID[] =
    "SELECT id,main_type,stereotype,name,description,x_order,y_order,list_order,uuid "
    "FROM classifiers WHERE id=?;";

/*!
 *  \brief predefined search statement to find a classifier by uuid
 */
static const char DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_UUID[] =
    "SELECT id,main_type,stereotype,name,description,x_order,y_order,list_order,uuid "
    "FROM classifiers WHERE uuid=?;";

/*!
 *  \brief predefined search statement to find a classifier by name
 */
static const char DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_NAME[] =
    "SELECT id,main_type,stereotype,name,description,x_order,y_order,list_order,uuid "
    "FROM classifiers WHERE name=?;";

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_CLASSIFIER_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: main_type
 */
static const int RESULT_CLASSIFIER_MAIN_TYPE_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: stereotype
 */
static const int RESULT_CLASSIFIER_STEREOTYPE_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: name
 */
static const int RESULT_CLASSIFIER_NAME_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: description
 */
static const int RESULT_CLASSIFIER_DESCRIPTION_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: x_order
 */
static const int RESULT_CLASSIFIER_X_ORDER_COLUMN = 5;

/*!
 *  \brief the column id of the result where this parameter is stored: y_order
 */
static const int RESULT_CLASSIFIER_Y_ORDER_COLUMN = 6;

/*!
 *  \brief the column id of the result where this parameter is stored: list_order
 */
static const int RESULT_CLASSIFIER_LIST_ORDER_COLUMN = 7;

/*!
 *  \brief the column id of the result where this parameter is stored: uuid
 */
static const int RESULT_CLASSIFIER_UUID_COLUMN = 8;

u8_error_t data_database_classifier_reader_get_classifier_by_id( data_database_classifier_reader_t *this_,
                                                                 data_row_id_t id,
                                                                 data_classifier_t *out_classifier )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_classifier );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        prepared_statement = (*this_).statement_classifier_by_id;

        result |= data_database_classifier_reader_private_bind_id_to_statement( this_, prepared_statement, id );

        U8_TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            /* this situation may happen if a classifier is deleted that is shown in the attributes editor */
            U8_TRACE_INFO_INT( "sqlite3_step did not find a row for id", id );
            result |= U8_ERROR_DB_STRUCTURE;  /* non-existing ids are worse than U8_ERROR_NOT_FOUND */
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_classifier_init( out_classifier,
                                            sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIER_ID_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_MAIN_TYPE_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_STEREOTYPE_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_NAME_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_DESCRIPTION_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_X_ORDER_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_Y_ORDER_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_LIST_ORDER_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_UUID_COLUMN )
                                          );

            data_classifier_trace( out_classifier );

            sqlite_err = sqlite3_step( prepared_statement );
            if ( SQLITE_DONE != sqlite_err )
            {
                U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= U8_ERROR_DB_STRUCTURE;
            }
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_classifier_reader_get_classifier_by_name( data_database_classifier_reader_t *this_,
                                                                   const char *name,
                                                                   data_classifier_t *out_classifier )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_classifier );
    assert( NULL != name );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        prepared_statement = (*this_).statement_classifier_by_name;

        result |= data_database_classifier_reader_private_bind_text_to_statement( this_, prepared_statement, name );

        U8_TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            U8_TRACE_INFO_STR( "sqlite3_step did not find a row for name", name );
            result |= U8_ERROR_NOT_FOUND;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_classifier_init( out_classifier,
                                            sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIER_ID_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_MAIN_TYPE_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_STEREOTYPE_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_NAME_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_DESCRIPTION_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_X_ORDER_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_Y_ORDER_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_LIST_ORDER_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_UUID_COLUMN )
                                          );

            data_classifier_trace( out_classifier );

            sqlite_err = sqlite3_step( prepared_statement );
            if ( SQLITE_DONE != sqlite_err )
            {
                U8_LOG_ERROR_INT( "sqlite3_step not done yet:", sqlite_err );
                result |= U8_ERROR_DB_STRUCTURE;
            }
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_classifier_reader_get_classifier_by_uuid ( data_database_classifier_reader_t *this_,
                                                                    const char *uuid,
                                                                    data_classifier_t *out_classifier )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_classifier );
    assert( NULL != uuid );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        prepared_statement = (*this_).statement_classifier_by_uuid;

        result |= data_database_classifier_reader_private_bind_text_to_statement( this_, prepared_statement, uuid );

        U8_TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            /* Do not log this incident, the caller may not expect to find a row. */
            U8_TRACE_INFO_STR( "sqlite3_step did not find a row for uuid", uuid );
            result |= U8_ERROR_NOT_FOUND;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_classifier_init( out_classifier,
                                            sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIER_ID_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_MAIN_TYPE_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_STEREOTYPE_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_NAME_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_DESCRIPTION_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_X_ORDER_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_Y_ORDER_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_LIST_ORDER_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_UUID_COLUMN )
                                          );

            data_classifier_trace( out_classifier );

            sqlite_err = sqlite3_step( prepared_statement );
            if ( SQLITE_DONE != sqlite_err )
            {
                U8_LOG_ERROR_INT( "sqlite3_step not done yet:", sqlite_err );
                result |= U8_ERROR_DB_STRUCTURE;
            }
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_classifier_reader_get_all_classifiers( data_database_classifier_reader_t *this_,
                                                                bool hierarchical,
                                                                data_classifier_iterator_t *io_classifier_iterator
                                                              )
{
    U8_TRACE_BEGIN();
    assert( NULL != io_classifier_iterator );
    u8_error_t result = U8_ERROR_NONE;

    sqlite3_stmt *const db_statement
        = hierarchical
        ? (*this_).statement_classifiers_all_hierarchical
        : (*this_).statement_classifiers_all;
    bool *const borrow_flag
        = hierarchical
        ? &((*this_).statement_classifiers_all_hierarchical_borrowed)
        : &((*this_).statement_classifiers_all_borrowed);
    data_database_borrowed_stmt_t sql_statement;
    data_database_borrowed_stmt_init( &sql_statement, (*this_).database, db_statement, borrow_flag );

    result |= data_classifier_iterator_reinit( io_classifier_iterator, sql_statement );
    /* do not destroy sql_statement; the object is transferred to the iterator and consumed there. */

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ FEATURE ================================ */

/*!
 *  \brief predefined search statement to find a feature by id
 */
static const char DATA_DATABASE_READER_SELECT_FEATURE_BY_ID[] =
    "SELECT id,main_type,classifier_id,key,value,description,list_order,uuid "
    "FROM features WHERE id=?;";

/*!
 *  \brief predefined search statement to find a feature by uuid
 */
static const char DATA_DATABASE_READER_SELECT_FEATURE_BY_UUID[] =
    "SELECT id,main_type,classifier_id,key,value,description,list_order,uuid "
    "FROM features WHERE uuid=?;";

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_FEATURE_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: main_type
 */
static const int RESULT_FEATURE_MAIN_TYPE_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: classifier_id
 */
static const int RESULT_FEATURE_CLASSIFIER_ID_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: key
 */
static const int RESULT_FEATURE_KEY_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: value
 */
static const int RESULT_FEATURE_VALUE_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: description
 */
static const int RESULT_FEATURE_DESCRIPTION_COLUMN = 5;

/*!
 *  \brief the column id of the result where this parameter is stored: list_order
 */
static const int RESULT_FEATURE_LIST_ORDER_COLUMN = 6;

/*!
 *  \brief the column id of the result where this parameter is stored: uuid
 */
static const int RESULT_FEATURE_LIST_UUID_COLUMN = 7;

u8_error_t data_database_classifier_reader_get_feature_by_id ( data_database_classifier_reader_t *this_,
                                                               data_row_id_t id,
                                                               data_feature_t *out_feature )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_feature );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        prepared_statement = (*this_).statement_feature_by_id;

        result |= data_database_classifier_reader_private_bind_id_to_statement( this_, prepared_statement, id );

        U8_TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            U8_TRACE_INFO_INT( "sqlite3_step did not find a row for id", id );
            result |= U8_ERROR_DB_STRUCTURE;  /* non-existing ids are worse than U8_ERROR_NOT_FOUND */
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_feature_init( out_feature,
                                         sqlite3_column_int64( prepared_statement, RESULT_FEATURE_ID_COLUMN ),
                                         sqlite3_column_int( prepared_statement, RESULT_FEATURE_MAIN_TYPE_COLUMN ),
                                         sqlite3_column_int64( prepared_statement, RESULT_FEATURE_CLASSIFIER_ID_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_KEY_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_VALUE_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_DESCRIPTION_COLUMN ),
                                         sqlite3_column_int( prepared_statement, RESULT_FEATURE_LIST_ORDER_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_LIST_UUID_COLUMN )
                                       );

            data_feature_trace( out_feature );

            sqlite_err = sqlite3_step( prepared_statement );
            if ( SQLITE_DONE != sqlite_err )
            {
                U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= U8_ERROR_DB_STRUCTURE;
            }
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_classifier_reader_get_feature_by_uuid ( data_database_classifier_reader_t *this_,
                                                                 const char *uuid,
                                                                 data_feature_t *out_feature )
{
    U8_TRACE_BEGIN();
    assert( NULL != uuid );
    assert( NULL != out_feature );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        prepared_statement = (*this_).statement_feature_by_uuid;

        result |= data_database_classifier_reader_private_bind_text_to_statement( this_, prepared_statement, uuid );

        U8_TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            /* Do not log this incident, the caller may not expect to find a row. */
            U8_TRACE_INFO_STR( "sqlite3_step did not find a row for uuid", uuid );
            result |= U8_ERROR_NOT_FOUND;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_feature_init( out_feature,
                                         sqlite3_column_int64( prepared_statement, RESULT_FEATURE_ID_COLUMN ),
                                         sqlite3_column_int( prepared_statement, RESULT_FEATURE_MAIN_TYPE_COLUMN ),
                                         sqlite3_column_int64( prepared_statement, RESULT_FEATURE_CLASSIFIER_ID_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_KEY_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_VALUE_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_DESCRIPTION_COLUMN ),
                                         sqlite3_column_int( prepared_statement, RESULT_FEATURE_LIST_ORDER_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_LIST_UUID_COLUMN )
                                       );

            data_feature_trace( out_feature );

            sqlite_err = sqlite3_step( prepared_statement );
            if ( SQLITE_DONE != sqlite_err )
            {
                U8_LOG_ERROR_INT( "sqlite3_step not done yet:", sqlite_err );
                result |= U8_ERROR_DB_STRUCTURE;
            }
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_classifier_reader_get_features_by_classifier_id ( data_database_classifier_reader_t *this_,
                                                                           data_row_id_t classifier_id,
                                                                           data_feature_iterator_t *io_feature_iterator )
{
    U8_TRACE_BEGIN();
    assert( NULL != io_feature_iterator );
    u8_error_t result = U8_ERROR_NONE;

    sqlite3_stmt *const prepared_statement = (*this_).statement_features_by_classifier_id;
    result |= data_database_classifier_reader_private_bind_id_to_statement( this_, prepared_statement, classifier_id );

    data_database_borrowed_stmt_t sql_statement;
    data_database_borrowed_stmt_init( &sql_statement,
                                      (*this_).database,
                                      prepared_statement,
                                      &((*this_).statement_features_by_classifier_id_borrowed)
                                    );
    result |= data_feature_iterator_reinit( io_feature_iterator, sql_statement );
    /* do not destroy sql_statement; the object is transferred to the iterator and consumed there. */

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_classifier_reader_get_features_by_diagram_id ( data_database_classifier_reader_t *this_,
                                                                        data_row_id_t diagram_id,
                                                                        data_feature_iterator_t *io_feature_iterator )
{
    U8_TRACE_BEGIN();
    assert( NULL != io_feature_iterator );
    u8_error_t result = U8_ERROR_NONE;

    sqlite3_stmt *const prepared_statement = (*this_).statement_features_by_diagram_id;
    result |= data_database_classifier_reader_private_bind_id_to_statement( this_, prepared_statement, diagram_id );

    data_database_borrowed_stmt_t sql_statement;
    data_database_borrowed_stmt_init( &sql_statement,
                                      (*this_).database,
                                      prepared_statement,
                                      &((*this_).statement_features_by_diagram_id_borrowed)
                                    );
    result |= data_feature_iterator_reinit( io_feature_iterator, sql_statement );
    /* do not destroy sql_statement; the object is transferred to the iterator and consumed there. */

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ RELATIONSHIP ================================ */

/*!
 *  \brief predefined search statement to find a relationship by id
 */
static const char DATA_DATABASE_READER_SELECT_RELATIONSHIP_BY_ID[] =
    "SELECT id,main_type,from_classifier_id,to_classifier_id,stereotype,name,description,list_order,"
    "from_feature_id,to_feature_id,uuid "
    "FROM relationships WHERE id=?;";

/*!
 *  \brief predefined search statement to find a relationship by uuid
 */
static const char DATA_DATABASE_READER_SELECT_RELATIONSHIP_BY_UUID[] =
    "SELECT id,main_type,from_classifier_id,to_classifier_id,stereotype,name,description,list_order,"
    "from_feature_id,to_feature_id,uuid "
    "FROM relationships WHERE uuid=?;";

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_RELATIONSHIP_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: main_type
 */
static const int RESULT_RELATIONSHIP_MAIN_TYPE_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: from_classifier_id
 */
static const int RESULT_RELATIONSHIP_FROM_CLASSIFIER_ID_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: to_classifier_id
 */
static const int RESULT_RELATIONSHIP_TO_CLASSIFIER_ID_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: name
 */
static const int RESULT_RELATIONSHIP_STEREOTYPE_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: name
 */
static const int RESULT_RELATIONSHIP_NAME_COLUMN = 5;

/*!
 *  \brief the column id of the result where this parameter is stored: description
 */
static const int RESULT_RELATIONSHIP_DESCRIPTION_COLUMN = 6;

/*!
 *  \brief the column id of the result where this parameter is stored: list_order
 */
static const int RESULT_RELATIONSHIP_LIST_ORDER_COLUMN = 7;

/*!
 *  \brief the column id of the result where this parameter is stored: from_feature_id
 */
static const int RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN = 8;

/*!
 *  \brief the column id of the result where this parameter is stored: to_feature_id
 */
static const int RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN = 9;

/*!
 *  \brief the column id of the result where this parameter is stored: uuid
 */
static const int RESULT_RELATIONSHIP_UUID_COLUMN = 10;

u8_error_t data_database_classifier_reader_get_relationship_by_id ( data_database_classifier_reader_t *this_,
                                                                    data_row_id_t id,
                                                                    data_relationship_t *out_relationship )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_relationship );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        prepared_statement = (*this_).statement_relationship_by_id;

        result |= data_database_classifier_reader_private_bind_id_to_statement( this_, prepared_statement, id );

        U8_TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            U8_TRACE_INFO_INT( "sqlite3_step did not find a row for id", id );
            result |= U8_ERROR_DB_STRUCTURE;  /* non-existing ids are worse than U8_ERROR_NOT_FOUND */
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_relationship_init( out_relationship,
                                              sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_ID_COLUMN ),
                                              sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_FROM_CLASSIFIER_ID_COLUMN ),
                                              sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN ),
                                              sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_TO_CLASSIFIER_ID_COLUMN ),
                                              sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN ),
                                              sqlite3_column_int( prepared_statement, RESULT_RELATIONSHIP_MAIN_TYPE_COLUMN ),
                                              (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_STEREOTYPE_COLUMN ),
                                              (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_NAME_COLUMN ),
                                              (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_DESCRIPTION_COLUMN ),
                                              sqlite3_column_int( prepared_statement, RESULT_RELATIONSHIP_LIST_ORDER_COLUMN ),
                                              (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_UUID_COLUMN )
                                            );
            if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN ) )
            {
                data_relationship_set_from_feature_row_id ( out_relationship, DATA_ROW_ID_VOID );
            }
            if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN ) )
            {
                data_relationship_set_to_feature_row_id ( out_relationship, DATA_ROW_ID_VOID );
            }

            data_relationship_trace( out_relationship );

            sqlite_err = sqlite3_step( prepared_statement );
            if ( SQLITE_DONE != sqlite_err )
            {
                U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= U8_ERROR_DB_STRUCTURE;
            }
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_classifier_reader_get_relationship_by_uuid ( data_database_classifier_reader_t *this_,
                                                                      const char *uuid,
                                                                      data_relationship_t *out_relationship )
{
    U8_TRACE_BEGIN();
    assert( NULL != uuid );
    assert( NULL != out_relationship );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        prepared_statement = (*this_).statement_relationship_by_uuid;

        result |= data_database_classifier_reader_private_bind_text_to_statement( this_, prepared_statement, uuid );

        U8_TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            /* Do not log this incident, the caller may not expect to find a row. */
            U8_TRACE_INFO_STR( "sqlite3_step did not find a row for uuid", uuid );
            result |= U8_ERROR_NOT_FOUND;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_relationship_init( out_relationship,
                                              sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_ID_COLUMN ),
                                              sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_FROM_CLASSIFIER_ID_COLUMN ),
                                              sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN ),
                                              sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_TO_CLASSIFIER_ID_COLUMN ),
                                              sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN ),
                                              sqlite3_column_int( prepared_statement, RESULT_RELATIONSHIP_MAIN_TYPE_COLUMN ),
                                              (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_STEREOTYPE_COLUMN ),
                                              (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_NAME_COLUMN ),
                                              (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_DESCRIPTION_COLUMN ),
                                              sqlite3_column_int( prepared_statement, RESULT_RELATIONSHIP_LIST_ORDER_COLUMN ),
                                              (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_UUID_COLUMN )
                                            );
            if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN ) )
            {
                data_relationship_set_from_feature_row_id ( out_relationship, DATA_ROW_ID_VOID );
            }
            if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN ) )
            {
                data_relationship_set_to_feature_row_id ( out_relationship, DATA_ROW_ID_VOID );
            }

            data_relationship_trace( out_relationship );

            sqlite_err = sqlite3_step( prepared_statement );
            if ( SQLITE_DONE != sqlite_err )
            {
                U8_LOG_ERROR_INT( "sqlite3_step not done yet:", sqlite_err );
                result |= U8_ERROR_DB_STRUCTURE;
            }
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_classifier_reader_get_relationships_by_classifier_id ( data_database_classifier_reader_t *this_,
                                                                                data_row_id_t classifier_id,
                                                                                data_relationship_iterator_t *io_relationship_iterator )
{
    U8_TRACE_BEGIN();
    assert( NULL != io_relationship_iterator );
    u8_error_t result = U8_ERROR_NONE;

    sqlite3_stmt *const prepared_statement = (*this_).statement_relationships_by_classifier_id;
    result |= data_database_classifier_reader_private_bind_two_ids_to_statement( this_, prepared_statement, classifier_id, classifier_id );

    data_database_borrowed_stmt_t sql_statement;
    data_database_borrowed_stmt_init( &sql_statement,
                                      (*this_).database,
                                      prepared_statement,
                                      &((*this_).statement_relationships_by_classifier_id_borrowed)
                                    );
    result |= data_relationship_iterator_reinit( io_relationship_iterator, sql_statement );
    /* do not destroy sql_statement; the object is transferred to the iterator and consumed there. */

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_classifier_reader_get_relationships_by_feature_id ( data_database_classifier_reader_t *this_,
                                                                             data_row_id_t feature_id,
                                                                             data_relationship_iterator_t *io_relationship_iterator )
{
    U8_TRACE_BEGIN();
    assert( NULL != io_relationship_iterator );
    u8_error_t result = U8_ERROR_NONE;

    sqlite3_stmt *const prepared_statement = (*this_).statement_relationships_by_feature_id;
    result |= data_database_classifier_reader_private_bind_two_ids_to_statement( this_, prepared_statement, feature_id, feature_id );

    data_database_borrowed_stmt_t sql_statement;
    data_database_borrowed_stmt_init( &sql_statement,
                                      (*this_).database,
                                      prepared_statement,
                                      &((*this_).statement_relationships_by_feature_id_borrowed)
                                    );
    result |= data_relationship_iterator_reinit( io_relationship_iterator, sql_statement );
    /* do not destroy sql_statement; the object is transferred to the iterator and consumed there. */

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_classifier_reader_get_relationships_by_diagram_id ( data_database_classifier_reader_t *this_,
                                                                             data_row_id_t diagram_id,
                                                                             data_relationship_iterator_t *io_relationship_iterator )
{
    U8_TRACE_BEGIN();
    assert( NULL != io_relationship_iterator );
    u8_error_t result = U8_ERROR_NONE;

    sqlite3_stmt *const prepared_statement = (*this_).statement_relationships_by_diagram_id;
    result |= data_database_classifier_reader_private_bind_id_to_statement( this_, prepared_statement, diagram_id );

    data_database_borrowed_stmt_t sql_statement;
    data_database_borrowed_stmt_init( &sql_statement,
                                      (*this_).database,
                                      prepared_statement,
                                      &((*this_).statement_relationships_by_diagram_id_borrowed)
                                    );
    result |= data_relationship_iterator_reinit( io_relationship_iterator, sql_statement );
    /* do not destroy sql_statement; the object is transferred to the iterator and consumed there. */

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ private ================================ */

u8_error_t data_database_classifier_reader_private_open ( data_database_classifier_reader_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    {
        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_ID,
                                                   sizeof( DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_ID ),
                                                   &((*this_).statement_classifier_by_id)
                                                 );

        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_UUID,
                                                   sizeof( DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_UUID ),
                                                   &((*this_).statement_classifier_by_uuid)
                                                 );

        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_NAME,
                                                   sizeof( DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_NAME ),
                                                   &((*this_).statement_classifier_by_name)
                                                 );

        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_CLASSIFIER_ITERATOR_SELECT_ALL,
                                                   DATA_DATABASE_SQL_LENGTH_AUTO_DETECT,
                                                   &((*this_).statement_classifiers_all)
                                                 );
        (*this_).statement_classifiers_all_borrowed = false;

        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_CLASSIFIER_ITERATOR_SELECT_ALL_HIERARCHICAL,
                                                   DATA_DATABASE_SQL_LENGTH_AUTO_DETECT,
                                                   &((*this_).statement_classifiers_all_hierarchical)
                                                 );
        (*this_).statement_classifiers_all_hierarchical_borrowed = false;

        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_DATABASE_READER_SELECT_FEATURE_BY_ID,
                                                   sizeof( DATA_DATABASE_READER_SELECT_FEATURE_BY_ID ),
                                                   &((*this_).statement_feature_by_id)
                                                 );

        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_DATABASE_READER_SELECT_FEATURE_BY_UUID,
                                                   sizeof( DATA_DATABASE_READER_SELECT_FEATURE_BY_UUID ),
                                                   &((*this_).statement_feature_by_uuid)
                                                 );

        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_FEATURE_ITERATOR_SELECT_FEATURES_BY_CLASSIFIER_ID,
                                                   DATA_DATABASE_SQL_LENGTH_AUTO_DETECT,
                                                   &((*this_).statement_features_by_classifier_id)
                                                 );
        (*this_).statement_features_by_classifier_id_borrowed = false;

        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_FEATURE_ITERATOR_SELECT_FEATURES_BY_DIAGRAM_ID,
                                                   DATA_DATABASE_SQL_LENGTH_AUTO_DETECT,
                                                   &((*this_).statement_features_by_diagram_id)
                                                 );
        (*this_).statement_features_by_diagram_id_borrowed = false;

        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_DATABASE_READER_SELECT_RELATIONSHIP_BY_ID,
                                                   sizeof( DATA_DATABASE_READER_SELECT_RELATIONSHIP_BY_ID ),
                                                   &((*this_).statement_relationship_by_id)
                                                 );

        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_DATABASE_READER_SELECT_RELATIONSHIP_BY_UUID,
                                                   sizeof( DATA_DATABASE_READER_SELECT_RELATIONSHIP_BY_UUID ),
                                                   &((*this_).statement_relationship_by_uuid)
                                                 );

        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_RELATIONSHIP_ITERATOR_SELECT_RELATIONSHIPS_BY_CLASSIFIER_ID,
                                                   DATA_DATABASE_SQL_LENGTH_AUTO_DETECT,
                                                   &((*this_).statement_relationships_by_classifier_id)
                                                 );
        (*this_).statement_relationships_by_classifier_id_borrowed = false;

        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_RELATIONSHIP_ITERATOR_SELECT_RELATIONSHIPS_BY_FEATURE_ID,
                                                   DATA_DATABASE_SQL_LENGTH_AUTO_DETECT,
                                                   &((*this_).statement_relationships_by_feature_id)
                                                 );
        (*this_).statement_relationships_by_feature_id_borrowed = false;

        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_RELATIONSHIP_ITERATOR_SELECT_RELATIONSHIPS_BY_DIAGRAM_ID,
                                                   DATA_DATABASE_SQL_LENGTH_AUTO_DETECT,
                                                   &((*this_).statement_relationships_by_diagram_id)
                                                 );
        (*this_).statement_relationships_by_diagram_id_borrowed = false;

        if ( result != U8_ERROR_NONE )
        {
            U8_LOG_ERROR( "A prepared statement could not be prepared." );
        }
    }

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_database_classifier_reader_private_close ( data_database_classifier_reader_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    {
        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_classifier_by_id );
        (*this_).statement_classifier_by_id = NULL;

        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_classifier_by_name );
        (*this_).statement_classifier_by_name = NULL;

        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_classifier_by_uuid );
        (*this_).statement_classifier_by_uuid = NULL;

        assert( (*this_).statement_classifiers_all_borrowed == false );
        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_classifiers_all );
        (*this_).statement_classifiers_all = NULL;

        assert( (*this_).statement_classifiers_all_hierarchical_borrowed == false );
        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_classifiers_all_hierarchical );
        (*this_).statement_classifiers_all_hierarchical = NULL;

        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_feature_by_id );
        (*this_).statement_feature_by_id = NULL;

        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_feature_by_uuid );
        (*this_).statement_feature_by_uuid = NULL;

        assert( (*this_).statement_features_by_classifier_id_borrowed == false );
        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_features_by_classifier_id );
        (*this_).statement_features_by_classifier_id = NULL;

        assert( (*this_).statement_features_by_diagram_id_borrowed == false );
        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_features_by_diagram_id );
        (*this_).statement_features_by_diagram_id = NULL;

        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_relationship_by_id );
        (*this_).statement_relationship_by_id = NULL;

        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_relationship_by_uuid );
        (*this_).statement_relationship_by_uuid = NULL;

        assert( (*this_).statement_relationships_by_classifier_id_borrowed == false );
        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_relationships_by_classifier_id );
        (*this_).statement_relationships_by_classifier_id = NULL;

        assert( (*this_).statement_relationships_by_feature_id_borrowed == false );
        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_relationships_by_feature_id );
        (*this_).statement_relationships_by_feature_id = NULL;

        assert( (*this_).statement_relationships_by_diagram_id_borrowed == false );
        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_relationships_by_diagram_id );
        (*this_).statement_relationships_by_diagram_id = NULL;
    }

    U8_TRACE_END_ERR(result);
    return result;
}


/*
Copyright 2016-2024 Andreas Warnke

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
