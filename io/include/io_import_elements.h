/* File: io_import_elements.h; Copyright and License: see below */

#ifndef IO_IMPORT_ELEMENTS_H
#define IO_IMPORT_ELEMENTS_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a pair of a) a pointer to a concrete instance of an interface
 *  and b) a pointer to objectdata that implements the interface.
 */

#include "data_classifier.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "data_diagram.h"
#include "data_diagramelement.h"
#include "ctrl_controller.h"
#include "storage/data_database_reader.h"

/*!
 *  \brief object data of a io_import_elements_t.
 *
 */
struct io_import_elements_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    ctrl_controller_t *controller;  /*!< pointer to external controller */
};

typedef struct io_import_elements_struct io_import_elements_t;

/*!
 *  \brief initializes the io_import_elements_t
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader
 *  \param controller pointer to a controller object which can modify the database
 */
void io_import_elements_init( io_import_elements_t *this_,
                              data_database_reader_t *db_reader,
                              ctrl_controller_t *controller
                            );

/*!
 *  \brief destroys the io_import_elements_t.
 *
 *  \param this_ pointer to own object attributes
 */
void io_import_elements_destroy( io_import_elements_t *this_ );

/*!
 *  \brief synchronizes a classifier with the database
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int io_import_elements_sync_classifier( io_import_elements_t *this_,
                                        const data_classifier_t *classifier_ptr
                                      );

/*!
 *  \brief synchronizes a feature with the database
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int io_import_elements_sync_feature( io_import_elements_t *this_,
                                     const data_feature_t *feature_ptr
                                   );

/*!
 *  \brief synchronizes a relationship with the database
 *
 *  \param this_ pointer to own object attributes
 *  \param relation_ptr pointer to relationship that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int io_import_elements_sync_relationship( io_import_elements_t *this_,
                                          const data_relationship_t *relation_ptr
                                        );

/*!
 *  \brief synchronizes a diagram with the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_ptr pointer to diagram that shall be written
 *  \return 0 in case of success, -1 otherwise
 */
int io_import_elements_sync_diagram( io_import_elements_t *this_,
                                     const data_diagram_t *diag_ptr
                                   );

/*!
 *  \brief synchronizes a diagramelement with the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_ptr pointer to diagramelement that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int io_import_elements_sync_diagramelement( io_import_elements_t *this_,
                                            const data_diagramelement_t *diagramelement_ptr
                                          );

#endif  /* IO_IMPORT_ELEMENTS_H */


/*
Copyright 2021-2021 Andreas Warnke

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
