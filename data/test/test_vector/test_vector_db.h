/* File: test_vector_db.h; Copyright and License: see below */

#ifndef TEST_VECTOR_DB_H
#define TEST_VECTOR_DB_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides functions to initialize the database before starting a test case
 */

#include "storage/data_database_writer.h"
#include "data_row_id.h"

/*!
 *  \brief attributes of the test_vector_db_t
 */
struct test_vector_db_struct {
    data_database_writer_t *db_writer;  /*!< pointer to external data_database_writer_t */
};

typedef struct test_vector_db_struct test_vector_db_t;

/*!
 *  \brief initializes the test_vector_db_t
 *
 *  \param this_ pointer to own object attributes
 *  \param db_writer the db_writer pointer
 */
static inline void test_vector_db_init( test_vector_db_t *this_, data_database_writer_t *db_writer );

/*!
 *  \brief destroys the test_vector_db_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void test_vector_db_destroy( test_vector_db_t *this_ );

/*!
 *  \brief helper function to initialize the database
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_diagram_id id of the parent diagram or DATA_ROW_ID_VOID to create a root diagram
 *  \param name name of the newly created element
 *  \param stereotype stereotype name of the new element
 *  \return id of newly created diagram
 */
static data_row_id_t test_vector_db_create_diagram( test_vector_db_t *this_,
                                                    data_row_id_t parent_diagram_id,
                                                    const char* name,
                                                    const char* stereotype
                                                  );

/*!
 *  \brief helper function to initialize the database
 *
 *  \param this_ pointer to own object attributes
 *  \param name name of the newly created element
 *  \param stereotype stereotype name of the new element
 *  \return id of newly created classifier
 */
static data_row_id_t test_vector_db_create_classifier( test_vector_db_t *this_,
                                                       const char* name,
                                                       const char* stereotype
                                                     );

/*!
 *  \brief helper function to initialize the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the parent diagram to which to attach the classifier
 *  \param classifier_id id of the classifier which to attach to the diagram
 *  \return id of newly created diagramelement
 */
static data_row_id_t test_vector_db_create_diagramelement( test_vector_db_t *this_,
                                                           data_row_id_t diagram_id,
                                                           data_row_id_t classifier_id
                                                         );

#include "test_vector_db.inl"

#endif  /* TEST_VECTOR_DB_H */


/*
Copyright 2023-2023 Andreas Warnke

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
