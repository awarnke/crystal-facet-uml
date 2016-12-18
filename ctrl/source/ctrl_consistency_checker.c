/* File: ctrl_consistency_checker.c; Copyright and License: see below */

#include "ctrl_consistency_checker.h"
#include "storage/data_database_consistency_checker.h"
#include "data_id.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

void ctrl_consistency_checker_init ( ctrl_consistency_checker_t *this_, data_database_t *database, data_database_reader_t *db_reader, data_database_writer_t *db_writer )
{
    TRACE_BEGIN();
    assert( NULL != database );
    assert( NULL != db_reader );
    assert( NULL != db_writer );

    (*this_).database = database;
    (*this_).db_reader = db_reader;
    (*this_).db_writer = db_writer;

    TRACE_END();
}

void ctrl_consistency_checker_destroy ( ctrl_consistency_checker_t *this_ )
{
    TRACE_BEGIN();

    (*this_).database = NULL;
    (*this_).db_reader = NULL;
    (*this_).db_writer = NULL;

    TRACE_END();
}

/*
 *  \brief checks and repairs the database
 *
 *  \param modify_db true if the database shall be repaired and modified
 *  \return CTRL_ERROR_NONE in case of success,
 *          CTRL_ERROR_NO_DB if database not open/loaded,
 *          CTRL_ERROR_DB_STRUCTURE if database was corrupted
 */
ctrl_error_t ctrl_consistency_checker_repair_database ( ctrl_consistency_checker_t *this_, bool modify_db )
{
    TRACE_BEGIN();
    ctrl_error_t err_result = CTRL_ERROR_NONE;
    data_error_t data_err;

    TSLOG_ERROR( "not yet implemented" );

    /* get all root diagrams */
    uint32_t out_diagram_count;
    data_err = data_database_reader_get_diagrams_by_parent_id ( (*this_).db_reader,
                                                                DATA_ID_VOID_ID,
                                                                CTRL_CONSISTENCY_CHECKER_MAX_DIAG_BUFFER,
                                                                &((*this_).temp_diagram_buffer),
                                                                & out_diagram_count
    );

    TSLOG_EVENT_INT( "ROOT DIAGRAM COUNT:", out_diagram_count );
    for ( int list_pos = 0; list_pos < out_diagram_count; list_pos ++ )
    {
        TSLOG_EVENT_STR( "ROOT DIAGRAM:", data_diagram_get_name_ptr( &((*this_).temp_diagram_buffer[list_pos]) ) );
    }

    TRACE_END_ERR( err_result );
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
