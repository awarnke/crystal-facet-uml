/* File: test_vector_db.c; Copyright and License: see below */

#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "test_expect.h"
#include <glib.h>
#include <assert.h>


static inline void test_vector_db_init( test_vector_db_t *this_, data_database_writer_t *db_writer )
{
    assert( db_writer != NULL );
    (*this_).db_writer = db_writer;
}

static inline void test_vector_db_destroy( test_vector_db_t *this_ )
{
    (*this_).db_writer = NULL;
}

static data_row_id_t test_vector_db_create_diagram( test_vector_db_t *this_,
                                                    data_row_id_t parent_diagram_id,
                                                    const char* name,
                                                    const char* stereotype )
{
    U8_TRACE_BEGIN();
    assert( name != NULL );
    assert( stereotype != NULL );
    u8_error_t data_err;

    /* create a diagram */
    data_row_id_t root_diag_id;
    {
        static data_diagram_t new_diagram;  /* static ok for a single-threaded test case and preserves stack space, which is important for 32bit systems */
        data_err = data_diagram_init( &new_diagram,
                                      DATA_ROW_ID_VOID, /* diagram_id is ignored */
                                      parent_diagram_id,
                                      DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,  /* default is an interaction diagram */
                                      stereotype, /* stereotype */
                                      name,
                                      "diagram_description-root",
                                      -13, /* list_order */
                                      DATA_DIAGRAM_FLAG_NONE,
                                      "d017a037-efb8-454e-b62a-a33628e90ece"
                                    );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

        root_diag_id = DATA_ROW_ID_VOID;
        data_err = data_database_writer_create_diagram ( (*this_).db_writer,
                                                         &new_diagram,
                                                         &root_diag_id
                                                       );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );
        data_diagram_destroy ( &new_diagram );
    }
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != root_diag_id );

    U8_TRACE_END();
    return root_diag_id;
}

static data_row_id_t test_vector_db_create_classifier( test_vector_db_t *this_,
                                                       const char* name,
                                                       data_classifier_type_t classifier_type,
                                                       const char* stereotype )
{
    U8_TRACE_BEGIN();
    assert( name != NULL );
    assert( stereotype != NULL );
    u8_error_t data_err;

    /* create a classifier */
    data_row_id_t classifier_id;
    {
        static data_classifier_t new_classifier;  /* static ok for a single-threaded test case and preserves stack space, which is important for 32bit systems */
        data_err = data_classifier_init( &new_classifier,
                                         DATA_ROW_ID_VOID /* classifier_id is ignored */,
                                         classifier_type,
                                         stereotype,  /* stereotype */
                                         name,
                                         "",  /* description */
                                         45,
                                         4500,
                                         450000,
                                         "7958d381-1859-49fc-b6c5-49fbc2bfebe8"
                                       );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );
        data_err = data_database_writer_create_classifier ( (*this_).db_writer,
                                                            &new_classifier,
                                                            &classifier_id
                                                          );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );
        data_classifier_destroy ( &new_classifier );
    }
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != classifier_id );

    U8_TRACE_END();
    return classifier_id;
}

static data_row_id_t test_vector_db_create_diagramelement( test_vector_db_t *this_,
                                                           data_row_id_t diagram_id,
                                                           data_row_id_t classifier_id )
{
    U8_TRACE_BEGIN();
    u8_error_t data_err;

    /* create a diagramelement */
    data_row_id_t diagele_id;
    {
        static data_diagramelement_t new_diagele;  /* static ok for a single-threaded test case and preserves stack space, which is important for 32bit systems */
        data_err = data_diagramelement_init( &new_diagele,
                                             DATA_ROW_ID_VOID /* diagramelement_id is ignored */,
                                             diagram_id,
                                             classifier_id,
                                             DATA_DIAGRAMELEMENT_FLAG_NONE,
                                             DATA_ROW_ID_VOID,
                                             "0fea7d08-3888-4186-9ba1-7af85edf383e"
                                           );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );
        data_err = data_database_writer_create_diagramelement ( (*this_).db_writer,
                                                                &new_diagele,
                                                                &diagele_id
                                                              );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );
        data_diagramelement_destroy ( &new_diagele );
    }
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != diagele_id );

    U8_TRACE_END();
    return diagele_id;
}


/*
 * Copyright 2023-2023 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
