/* File: data_database_diagram_reader.c; Copyright and License: see below */

#include "storage/data_database_diagram_reader.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>

u8_error_t data_database_diagram_reader_init ( data_database_diagram_reader_t *this_, data_database_t *database )
{
    U8_TRACE_BEGIN();
    assert( NULL != database );
    u8_error_t result = U8_ERROR_NONE;

    (*this_).database = database;

    (*this_).statement_diagram_by_id = NULL;
    (*this_).statement_diagram_by_uuid = NULL;
    (*this_).statement_diagrams_by_parent_id = NULL;
    (*this_).statement_diagrams_by_parent_id_null = NULL;
#ifdef DATA_DATABASE_READER_PROVIDE_DEPRECATED_FKT
    (*this_).statement_diagrams_by_classifier_id = NULL;
#endif
    (*this_).statement_diagram_ids_by_parent_id = NULL;
    (*this_).statement_diagram_ids_by_parent_id_null = NULL;
    (*this_).statement_diagram_ids_by_classifier_id = NULL;
    (*this_).statement_diagramelement_by_id = NULL;
    (*this_).statement_diagramelement_by_uuid = NULL;
    (*this_).statement_diagramelements_by_diagram_id = NULL;
    (*this_).statement_diagramelements_by_classifier_id = NULL;

    result |= data_database_diagram_reader_private_open( this_ );

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_database_diagram_reader_destroy ( data_database_diagram_reader_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    result |= data_database_diagram_reader_private_close( this_ );

    (*this_).database = NULL;

    U8_TRACE_END_ERR(result);
    return result;
}

/* ================================ DIAGRAM ================================ */

/*!
 *  \brief predefined search statement to find a diagram by id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID[] =
    "SELECT id,parent_id,diagram_type,name,description,list_order,display_flags,uuid "
    "FROM diagrams WHERE id=?;";

/*!
 *  \brief predefined search statement to find a diagram by uuid
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAM_BY_UUID[] =
    "SELECT id,parent_id,diagram_type,name,description,list_order,display_flags,uuid "
    "FROM diagrams WHERE uuid=?;";

/*!
 *  \brief predefined search statement to find diagrams by parent-id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID[] =
    "SELECT id,parent_id,diagram_type,name,description,list_order,display_flags,uuid "
    "FROM diagrams WHERE parent_id=? ORDER BY list_order ASC;";

/*!
 *  \brief predefined search statement to find diagrams by NULL parent-id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID_NULL[] =
    "SELECT id,parent_id,diagram_type,name,description,list_order,display_flags,uuid "
    "FROM diagrams WHERE parent_id IS NULL ORDER BY list_order ASC;";

#ifdef DATA_DATABASE_READER_PROVIDE_DEPRECATED_FKT
/*!
 *  \brief predefined search statement to find diagrams by classifier-id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_CLASSIFIER_ID[] =
    "SELECT diagrams.id,diagrams.parent_id,diagrams.diagram_type,"
    "diagrams.name,diagrams.description,diagrams.list_order,diagrams.display_flags,diagrams.uuid "
    "FROM diagrams "
    "INNER JOIN diagramelements ON diagramelements.diagram_id=diagrams.id "
    "WHERE diagramelements.classifier_id=? "
    "GROUP BY diagrams.id "  /* filter duplicates if a classifier exists twice in a diagram */
    "ORDER BY diagrams.list_order ASC;";
#endif

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_DIAGRAM_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: parent_id
 */
static const int RESULT_DIAGRAM_PARENT_ID_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: type
 */
static const int RESULT_DIAGRAM_TYPE_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: name
 */
static const int RESULT_DIAGRAM_NAME_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: description
 */
static const int RESULT_DIAGRAM_DESCRIPTION_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: list_order
 */
static const int RESULT_DIAGRAM_LIST_ORDER_COLUMN = 5;

/*!
 *  \brief the column id of the result where this parameter is stored: display_flags
 */
static const int RESULT_DIAGRAM_DISPLAY_FLAGS_COLUMN = 6;

/*!
 *  \brief the column id of the result where this parameter is stored: uuid
 */
static const int RESULT_DIAGRAM_UUID_COLUMN = 7;

/*!
 *  \brief predefined search statement to find diagram ids by parent-id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_PARENT_ID[] =
    "SELECT id "
    "FROM diagrams WHERE parent_id=? ORDER BY list_order ASC;";

/*!
 *  \brief predefined search statement to find diagram ids by NULL parent-id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_PARENT_ID_NULL[] =
    "SELECT id "
    "FROM diagrams WHERE parent_id IS NULL ORDER BY list_order ASC;";

/*!
 *  \brief predefined search statement to find diagram ids by classifier-id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_CLASSIFIER_ID[] =
    "SELECT diagrams.id "
    "FROM diagrams "
    "INNER JOIN diagramelements ON diagramelements.diagram_id=diagrams.id "
    "WHERE diagramelements.classifier_id=? "
    "GROUP BY diagrams.id "  /* filter duplicates if a classifier exists twice in a diagram */
    "ORDER BY diagrams.list_order ASC;";

u8_error_t data_database_diagram_reader_get_diagram_by_id ( data_database_diagram_reader_t *this_,
                                                              data_row_id_t id,
                                                              data_diagram_t *out_diagram )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_diagram );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        prepared_statement = (*this_).statement_diagram_by_id;

        result |= data_database_diagram_reader_private_bind_id_to_statement( this_, prepared_statement, id );

        U8_TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            U8_LOG_ANOMALY( "sqlite3_step did not find a row." );
            result |= U8_ERROR_DB_STRUCTURE;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_diagram_init( out_diagram,
                                         sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_ID_COLUMN ),
                                         sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ),
                                         sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_TYPE_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_NAME_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_DESCRIPTION_COLUMN ),
                                         sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_LIST_ORDER_COLUMN ),
                                         sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_DISPLAY_FLAGS_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_UUID_COLUMN )
                                       );
            if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ) )
            {
                (*out_diagram).parent_id = DATA_ROW_ID_VOID;
            }

            data_diagram_trace( out_diagram );
        }

        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_DONE != sqlite_err )
        {
            U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
            result |= U8_ERROR_DB_STRUCTURE;
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_diagram_reader_get_diagram_by_uuid ( data_database_diagram_reader_t *this_,
                                                                const char *uuid,
                                                                data_diagram_t *out_diagram )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_diagram );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        prepared_statement = (*this_).statement_diagram_by_uuid;

        result |= data_database_diagram_reader_private_bind_text_to_statement( this_, prepared_statement, uuid );

        U8_TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            /* Do not log this incident, the caller may not expect to find a row. */
            U8_TRACE_INFO( "sqlite3_step did not find a row." );
            result |= U8_ERROR_NOT_FOUND;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_diagram_init( out_diagram,
                                         sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_ID_COLUMN ),
                                         sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ),
                                         sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_TYPE_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_NAME_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_DESCRIPTION_COLUMN ),
                                         sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_LIST_ORDER_COLUMN ),
                                         sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_DISPLAY_FLAGS_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_UUID_COLUMN )
                                       );
            if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ) )
            {
                (*out_diagram).parent_id = DATA_ROW_ID_VOID;
            }

            data_diagram_trace( out_diagram );
        }

        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_DONE != sqlite_err )
        {
            U8_LOG_ERROR_INT( "sqlite3_step not done yet:", sqlite_err );
            result |= U8_ERROR_DB_STRUCTURE;
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_diagram_reader_get_diagrams_by_parent_id ( data_database_diagram_reader_t *this_,
                                                                      data_row_id_t parent_id,
                                                                      uint32_t max_out_array_size,
                                                                      data_diagram_t (*out_diagram)[],
                                                                      uint32_t *out_diagram_count )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_diagram_count );
    assert( NULL != out_diagram );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        if ( DATA_ROW_ID_VOID == parent_id )
        {
            prepared_statement = (*this_).statement_diagrams_by_parent_id_null;
            result |= data_database_diagram_reader_private_bind_void_to_statement( this_, prepared_statement );
        }
        else
        {
            prepared_statement = (*this_).statement_diagrams_by_parent_id;
            result |= data_database_diagram_reader_private_bind_id_to_statement( this_, prepared_statement, parent_id );
        }

        *out_diagram_count = 0;
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= max_out_array_size); row_index ++ )
        {
            U8_TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= U8_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < max_out_array_size))
            {
                *out_diagram_count = row_index+1;
                data_diagram_t *current_diag = &((*out_diagram)[row_index]);

                result |= data_diagram_init( current_diag,
                                             sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_ID_COLUMN ),
                                             sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ),
                                             sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_TYPE_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_NAME_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_DESCRIPTION_COLUMN ),
                                             sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_LIST_ORDER_COLUMN ),
                                             sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_DISPLAY_FLAGS_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_UUID_COLUMN )
                                           );
                if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ) )
                {
                    (*current_diag).parent_id = DATA_ROW_ID_VOID;
                }

                data_diagram_trace( current_diag );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= max_out_array_size))
            {
                U8_LOG_ANOMALY_INT( "out_diagram[] full:", (row_index+1) );
                result |= U8_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                U8_TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

#ifdef DATA_DATABASE_READER_PROVIDE_DEPRECATED_FKT
u8_error_t data_database_diagram_reader_get_diagrams_by_classifier_id ( data_database_diagram_reader_t *this_,
                                                                          data_row_id_t classifier_id,
                                                                          uint32_t max_out_array_size,
                                                                          data_diagram_t (*out_diagram)[],
                                                                          uint32_t *out_diagram_count )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_diagram_count );
    assert( NULL != out_diagram );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        prepared_statement = (*this_).statement_diagrams_by_classifier_id;

        result |= data_database_diagram_reader_private_bind_id_to_statement( this_, prepared_statement, classifier_id );

        *out_diagram_count = 0;
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= max_out_array_size); row_index ++ )
        {
            U8_TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= U8_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < max_out_array_size))
            {
                *out_diagram_count = row_index+1;
                data_diagram_t *current_diag = &((*out_diagram)[row_index]);

                result |= data_diagram_init( current_diag,
                                             sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_ID_COLUMN ),
                                             sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ),
                                             sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_TYPE_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_NAME_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_DESCRIPTION_COLUMN ),
                                             sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_LIST_ORDER_COLUMN ),
                                             sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_DISPLAY_FLAGS_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_UUID_COLUMN )
                                           );
                if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ) )
                {
                    (*current_diag).parent_id = DATA_ROW_ID_VOID;
                }

                data_diagram_trace( current_diag );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= max_out_array_size))
            {
                U8_LOG_ANOMALY_INT( "out_diagram[] full:", (row_index+1) );
                result |= U8_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                U8_TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}
#endif

u8_error_t data_database_diagram_reader_get_diagram_ids_by_parent_id ( data_database_diagram_reader_t *this_,
                                                                         data_row_id_t parent_id,
                                                                         data_small_set_t *out_diagram_ids )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_diagram_ids );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        if ( DATA_ROW_ID_VOID == parent_id )
        {
            prepared_statement = (*this_).statement_diagram_ids_by_parent_id_null;
            result |= data_database_diagram_reader_private_bind_void_to_statement( this_, prepared_statement );
        }
        else
        {
            prepared_statement = (*this_).statement_diagram_ids_by_parent_id;
            result |= data_database_diagram_reader_private_bind_id_to_statement( this_, prepared_statement, parent_id );
        }

        data_small_set_clear( out_diagram_ids );
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= DATA_SMALL_SET_MAX_SET_SIZE); row_index ++ )
        {
            U8_TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= U8_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < DATA_SMALL_SET_MAX_SET_SIZE))
            {
                data_id_t current_diag_id;
                data_id_init( &current_diag_id, DATA_TABLE_DIAGRAM, sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_ID_COLUMN ) );
                result |= data_small_set_add_obj( out_diagram_ids, current_diag_id );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= DATA_SMALL_SET_MAX_SET_SIZE))
            {
                U8_LOG_ANOMALY_INT( "out_diagram_ids[] full:", (row_index+1) );
                result |= U8_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                U8_TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
        data_small_set_trace( out_diagram_ids );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_diagram_reader_get_diagram_ids_by_classifier_id ( data_database_diagram_reader_t *this_,
                                                                             data_row_id_t classifier_id,
                                                                             data_small_set_t *out_diagram_ids )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_diagram_ids );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        prepared_statement = (*this_).statement_diagram_ids_by_classifier_id;

        result |= data_database_diagram_reader_private_bind_id_to_statement( this_, prepared_statement, classifier_id );

        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= DATA_SMALL_SET_MAX_SET_SIZE); row_index ++ )
        {
            U8_TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= U8_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < DATA_SMALL_SET_MAX_SET_SIZE))
            {
                data_id_t current_diag_id;
                data_id_init( &current_diag_id, DATA_TABLE_DIAGRAM, sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_ID_COLUMN ) );
                result |= data_small_set_add_obj( out_diagram_ids, current_diag_id );
                data_id_trace( &current_diag_id );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= DATA_SMALL_SET_MAX_SET_SIZE))
            {
                U8_LOG_ANOMALY_INT( "out_diagram_ids[] full:", (row_index+1) );
                result |= U8_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                U8_TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ DIAGRAMELEMENT ================================ */

/*!
 *  \brief predefined search statement to find a diagramelement by id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAMELEMENT_BY_ID[] =
    "SELECT id,diagram_id,classifier_id,display_flags,focused_feature_id,uuid FROM diagramelements WHERE id=?;";

/*!
 *  \brief predefined search statement to find a diagramelement by uuid
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAMELEMENT_BY_UUID[] =
    "SELECT id,diagram_id,classifier_id,display_flags,focused_feature_id,uuid FROM diagramelements WHERE uuid=?;";

/*!
 *  \brief predefined search statement to find diagramelements by diagram id
 *
 *  The "order by id" is important to get reproducable results, e.g. for json export
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAMELEMENTS_BY_DIAGRAM_ID[] =
    "SELECT id,diagram_id,classifier_id,display_flags,focused_feature_id,uuid FROM diagramelements "
    "WHERE diagram_id=? ORDER BY id ASC;";

/*!
 *  \brief predefined search statement to find diagramelements by classifier id
 *
 *  The "order by id" is important to get reproducable results, e.g. for json export
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAMELEMENTS_BY_CLASSIFIER_ID[] =
    "SELECT id,diagram_id,classifier_id,display_flags,focused_feature_id,uuid FROM diagramelements "
    "WHERE classifier_id=? ORDER BY id ASC;";

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_DIAGRAMELEMENT_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: diagram_id
 */
static const int RESULT_DIAGRAMELEMENT_DIAGRAM_ID_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: classifier_id
 */
static const int RESULT_DIAGRAMELEMENT_CLASSIFIER_ID_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: display_flags
 */
static const int RESULT_DIAGRAMELEMENT_DISPLAY_FLAGS_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: focused_feature_id
 */
static const int RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: uuid
 */
static const int RESULT_DIAGRAMELEMENT_UUID_COLUMN = 5;

u8_error_t data_database_diagram_reader_get_diagramelement_by_id ( data_database_diagram_reader_t *this_,
                                                                     data_row_id_t id,
                                                                     data_diagramelement_t *out_diagramelement )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_diagramelement );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        prepared_statement = (*this_).statement_diagramelement_by_id;

        result |= data_database_diagram_reader_private_bind_id_to_statement( this_, prepared_statement, id );

        U8_TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            U8_LOG_ANOMALY( "sqlite3_step did not find a row." );
            result |= U8_ERROR_DB_STRUCTURE;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_diagramelement_init( out_diagramelement,
                                                sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_ID_COLUMN ),
                                                sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_DIAGRAM_ID_COLUMN ),
                                                sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_CLASSIFIER_ID_COLUMN ),
                                                sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_DISPLAY_FLAGS_COLUMN ),
                                                sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN ),
                                                (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAMELEMENT_UUID_COLUMN )
                                              );
            if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN ) )
            {
                data_diagramelement_set_focused_feature_row_id ( out_diagramelement, DATA_ROW_ID_VOID );
            }

            data_diagramelement_trace( out_diagramelement );
        }

        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_DONE != sqlite_err )
        {
            U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
            result |= U8_ERROR_DB_STRUCTURE;
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_diagram_reader_get_diagramelement_by_uuid ( data_database_diagram_reader_t *this_,
                                                                       const char *uuid,
                                                                       data_diagramelement_t *out_diagramelement )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_diagramelement );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        prepared_statement = (*this_).statement_diagramelement_by_uuid;

        result |= data_database_diagram_reader_private_bind_text_to_statement( this_, prepared_statement, uuid );

        U8_TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            /* Do not log this incident, the caller may not expect to find a row. */
            U8_TRACE_INFO( "sqlite3_step did not find a row." );
            result |= U8_ERROR_NOT_FOUND;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_diagramelement_init( out_diagramelement,
                                                sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_ID_COLUMN ),
                                                sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_DIAGRAM_ID_COLUMN ),
                                                sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_CLASSIFIER_ID_COLUMN ),
                                                sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_DISPLAY_FLAGS_COLUMN ),
                                                sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN ),
                                                (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAMELEMENT_UUID_COLUMN )
                                              );
            if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN ) )
            {
                data_diagramelement_set_focused_feature_row_id ( out_diagramelement, DATA_ROW_ID_VOID );
            }

            data_diagramelement_trace( out_diagramelement );
        }

        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_DONE != sqlite_err )
        {
            U8_LOG_ERROR_INT( "sqlite3_step not done yet:", sqlite_err );
            result |= U8_ERROR_DB_STRUCTURE;
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_diagram_reader_get_diagramelements_by_diagram_id ( data_database_diagram_reader_t *this_,
                                                                              data_row_id_t diagram_id,
                                                                              uint32_t max_out_array_size,
                                                                              data_diagramelement_t (*out_diagramelement)[],
                                                                              uint32_t *out_diagramelement_count )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_diagramelement_count );
    assert( NULL != out_diagramelement );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        prepared_statement = (*this_).statement_diagramelements_by_diagram_id;

        result |= data_database_diagram_reader_private_bind_id_to_statement( this_, prepared_statement, diagram_id );

        *out_diagramelement_count = 0;
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= max_out_array_size); row_index ++ )
        {
            U8_TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= U8_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < max_out_array_size))
            {
                *out_diagramelement_count = row_index+1;
                data_diagramelement_t *current_diagele = &((*out_diagramelement)[row_index]);

                result |= data_diagramelement_init( current_diagele,
                                                    sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_ID_COLUMN ),
                                                    sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_DIAGRAM_ID_COLUMN ),
                                                    sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_CLASSIFIER_ID_COLUMN ),
                                                    sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_DISPLAY_FLAGS_COLUMN ),
                                                    sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN ),
                                                    (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAMELEMENT_UUID_COLUMN )
                                                  );
                if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN ) )
                {
                    data_diagramelement_set_focused_feature_row_id ( current_diagele, DATA_ROW_ID_VOID );
                }

                data_diagramelement_trace( current_diagele );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= max_out_array_size))
            {
                U8_LOG_ANOMALY_INT( "out_diagramelement[] full:", (row_index+1) );
                result |= U8_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                U8_TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_diagram_reader_get_diagramelements_by_classifier_id( data_database_diagram_reader_t *this_,
                                                                                data_row_id_t classifier_id,
                                                                                uint32_t max_out_array_size,
                                                                                data_diagramelement_t (*out_diagramelement)[],
                                                                                uint32_t *out_diagramelement_count )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_diagramelement_count );
    assert( NULL != out_diagramelement );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        prepared_statement = (*this_).statement_diagramelements_by_classifier_id;

        result |= data_database_diagram_reader_private_bind_id_to_statement( this_, prepared_statement, classifier_id );

        *out_diagramelement_count = 0;
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= max_out_array_size); row_index ++ )
        {
            U8_TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= U8_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < max_out_array_size))
            {
                *out_diagramelement_count = row_index+1;
                data_diagramelement_t *current_diagele = &((*out_diagramelement)[row_index]);

                result |= data_diagramelement_init( current_diagele,
                                                    sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_ID_COLUMN ),
                                                    sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_DIAGRAM_ID_COLUMN ),
                                                    sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_CLASSIFIER_ID_COLUMN ),
                                                    sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_DISPLAY_FLAGS_COLUMN ),
                                                    sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN ),
                                                    (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAMELEMENT_UUID_COLUMN )
                                                  );
                if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN ) )
                {
                    data_diagramelement_set_focused_feature_row_id ( current_diagele, DATA_ROW_ID_VOID );
                }

                data_diagramelement_trace( current_diagele );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= max_out_array_size))
            {
                U8_LOG_ANOMALY_INT( "out_diagramelement[] full:", (row_index+1) );
                result |= U8_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                U8_TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ private ================================ */

u8_error_t data_database_diagram_reader_private_open ( data_database_diagram_reader_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    {
        result |= data_database_diagram_reader_private_prepare_statement ( this_,
                                                                           DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID,
                                                                           sizeof( DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID ),
                                                                           &((*this_).statement_diagram_by_id)
                                                                         );

        result |= data_database_diagram_reader_private_prepare_statement ( this_,
                                                                           DATA_DATABASE_READER_SELECT_DIAGRAM_BY_UUID,
                                                                           sizeof( DATA_DATABASE_READER_SELECT_DIAGRAM_BY_UUID ),
                                                                           &((*this_).statement_diagram_by_uuid)
                                                                         );

        result |= data_database_diagram_reader_private_prepare_statement ( this_,
                                                                           DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID,
                                                                           sizeof( DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID ),
                                                                           &((*this_).statement_diagrams_by_parent_id)
                                                                         );

        result |= data_database_diagram_reader_private_prepare_statement ( this_,
                                                                           DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID_NULL,
                                                                           sizeof( DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID_NULL ),
                                                                           &((*this_).statement_diagrams_by_parent_id_null)
                                                                         );

#ifdef DATA_DATABASE_READER_PROVIDE_DEPRECATED_FKT
        result |= data_database_diagram_reader_private_prepare_statement ( this_,
                                                                           DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_CLASSIFIER_ID,
                                                                           sizeof( DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_CLASSIFIER_ID ),
                                                                           &((*this_).statement_diagrams_by_classifier_id)
                                                                         );
#endif

        result |= data_database_diagram_reader_private_prepare_statement ( this_,
                                                                           DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_PARENT_ID,
                                                                           sizeof( DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_PARENT_ID ),
                                                                           &((*this_).statement_diagram_ids_by_parent_id)
                                                                         );

        result |= data_database_diagram_reader_private_prepare_statement ( this_,
                                                                           DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_PARENT_ID_NULL,
                                                                           sizeof( DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_PARENT_ID_NULL ),
                                                                           &((*this_).statement_diagram_ids_by_parent_id_null)
                                                                         );

        result |= data_database_diagram_reader_private_prepare_statement ( this_,
                                                                           DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_CLASSIFIER_ID,
                                                                           sizeof( DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_CLASSIFIER_ID ),
                                                                           &((*this_).statement_diagram_ids_by_classifier_id)
                                                                         );

        result |= data_database_diagram_reader_private_prepare_statement ( this_,
                                                                           DATA_DATABASE_READER_SELECT_DIAGRAMELEMENT_BY_ID,
                                                                           sizeof( DATA_DATABASE_READER_SELECT_DIAGRAMELEMENT_BY_ID ),
                                                                           &((*this_).statement_diagramelement_by_id)
                                                                         );

        result |= data_database_diagram_reader_private_prepare_statement ( this_,
                                                                           DATA_DATABASE_READER_SELECT_DIAGRAMELEMENT_BY_UUID,
                                                                           sizeof( DATA_DATABASE_READER_SELECT_DIAGRAMELEMENT_BY_UUID ),
                                                                           &((*this_).statement_diagramelement_by_uuid)
                                                                         );

        result |= data_database_diagram_reader_private_prepare_statement ( this_,
                                                                           DATA_DATABASE_READER_SELECT_DIAGRAMELEMENTS_BY_DIAGRAM_ID,
                                                                           sizeof( DATA_DATABASE_READER_SELECT_DIAGRAMELEMENTS_BY_DIAGRAM_ID ),
                                                                           &((*this_).statement_diagramelements_by_diagram_id)
                                                                         );

        result |= data_database_diagram_reader_private_prepare_statement ( this_,
                                                                           DATA_DATABASE_READER_SELECT_DIAGRAMELEMENTS_BY_CLASSIFIER_ID,
                                                                           sizeof( DATA_DATABASE_READER_SELECT_DIAGRAMELEMENTS_BY_CLASSIFIER_ID ),
                                                                           &((*this_).statement_diagramelements_by_classifier_id)
                                                                         );

        if ( result != U8_ERROR_NONE )
        {
            U8_LOG_ERROR( "A prepared statement could not be prepared." );
        }
    }

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_database_diagram_reader_private_close ( data_database_diagram_reader_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    {
        result |= data_database_diagram_reader_private_finalize_statement( this_, (*this_).statement_diagram_by_id );
        (*this_).statement_diagram_by_id = NULL;

        result |= data_database_diagram_reader_private_finalize_statement( this_, (*this_).statement_diagram_by_uuid );
        (*this_).statement_diagram_by_uuid = NULL;

        result |= data_database_diagram_reader_private_finalize_statement( this_, (*this_).statement_diagrams_by_parent_id );
        (*this_).statement_diagrams_by_parent_id = NULL;

        result |= data_database_diagram_reader_private_finalize_statement( this_, (*this_).statement_diagrams_by_parent_id_null );
        (*this_).statement_diagrams_by_parent_id_null = NULL;

#ifdef DATA_DATABASE_READER_PROVIDE_DEPRECATED_FKT
        result |= data_database_diagram_reader_private_finalize_statement( this_, (*this_).statement_diagrams_by_classifier_id );
        (*this_).statement_diagrams_by_classifier_id = NULL;
#endif

        result |= data_database_diagram_reader_private_finalize_statement( this_, (*this_).statement_diagram_ids_by_parent_id );
        (*this_).statement_diagram_ids_by_parent_id = NULL;

        result |= data_database_diagram_reader_private_finalize_statement( this_, (*this_).statement_diagram_ids_by_parent_id_null );
        (*this_).statement_diagram_ids_by_parent_id_null = NULL;

        result |= data_database_diagram_reader_private_finalize_statement( this_, (*this_).statement_diagram_ids_by_classifier_id );
        (*this_).statement_diagram_ids_by_classifier_id = NULL;

        result |= data_database_diagram_reader_private_finalize_statement( this_, (*this_).statement_diagramelement_by_id );
        (*this_).statement_diagramelement_by_id = NULL;

        result |= data_database_diagram_reader_private_finalize_statement( this_, (*this_).statement_diagramelement_by_uuid );
        (*this_).statement_diagramelement_by_uuid = NULL;

        result |= data_database_diagram_reader_private_finalize_statement( this_, (*this_).statement_diagramelements_by_diagram_id );
        (*this_).statement_diagramelements_by_diagram_id = NULL;

        result |= data_database_diagram_reader_private_finalize_statement( this_, (*this_).statement_diagramelements_by_classifier_id );
        (*this_).statement_diagramelements_by_classifier_id = NULL;

    }

    U8_TRACE_END_ERR(result);
    return result;
}


/*
Copyright 2016-2023 Andreas Warnke

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
