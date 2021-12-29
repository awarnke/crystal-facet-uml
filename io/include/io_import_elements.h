/* File: io_import_elements.h; Copyright and License: see below */

#ifndef IO_IMPORT_ELEMENTS_H
#define IO_IMPORT_ELEMENTS_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a pair of a) a pointer to a concrete instance of an interface
 *  and b) a pointer to objectdata that implements the interface.
 */

#include "io_import_mode.h"
#include "data_classifier.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "data_diagram.h"
#include "data_diagramelement.h"
#include "data_row_id.h"
#include "data_rules.h"
#include "storage/data_database_reader.h"
#include "set/data_stat.h"
#include "set/data_visible_set.h"
#include "ctrl_controller.h"
#include "ctrl_multi_step_changer.h"
#include "u8/u8_error.h"

/*!
 *  \brief object data of a io_import_elements_t.
 *
 *  Lifecycle: An element importer shall perform a single import operation only.
 *  It may be initialized before one import operation and be destroyed afterwards.
 */
struct io_import_elements_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    ctrl_controller_t *controller;  /*!< pointer to external controller */
    data_rules_t data_rules;  /*!< own instance of uml and sysml consistency rules */

    io_import_mode_t mode;  /*!< the import mode */
    data_row_id_t paste_to_diagram;  /*!< the diagram where pasted objects shall be attached to if IO_IMPORT_MODE_PASTE */
    ctrl_multi_step_changer_t multi_step_changer;  /*!< own instance of a controller */
                                                   /*!< that can handle preferred ids and proposed names */

    data_stat_t *stat;  /*!< pointer to import statistics */

    data_diagram_t temp_diagram;  /*!< memory buffer to load a diagram temporarily from the database */
    data_diagramelement_t temp_diagramelement;  /*!< memory buffer to load a diagramelement temporarily from the database */
    data_classifier_t temp_classifier;  /*!< memory buffer to load a classifier temporarily from the database */
    data_feature_t temp_feature;  /*!< memory buffer to load a feature temporarily from the database */
    data_relationship_t temp_relationship;  /*!< memory buffer to load a relationship temporarily from the database */
};

typedef struct io_import_elements_struct io_import_elements_t;

/*!
 *  \brief initializes the io_import_elements_t
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader
 *  \param controller pointer to a controller object which can modify the database
 *  \param io_stat Statistics are only added, *io_stat shall be initialized by caller.
 *                 In case the element can be created but id or name have to be adapted to be unique,
 *                 a warning is added.
 */
void io_import_elements_init( io_import_elements_t *this_,
                              data_database_reader_t *db_reader,
                              ctrl_controller_t *controller,
                              data_stat_t *io_stat
                            );

/*!
 *  \brief initializes the io_import_elements_t
 *
 *  \param this_ pointer to own object attributes
 *  \param paste_to_diagram the parent diagram where pasted objects shall be attached to
 *  \param db_reader pointer to a database reader
 *  \param controller pointer to a controller object which can modify the database
 *  \param io_stat Statistics are only added, *io_stat shall be initialized by caller.
 *                 In case the element can be created but id or name have to be adapted to be unique,
 *                 a warning is added.
 */
void io_import_elements_init_for_paste( io_import_elements_t *this_,
                                         data_row_id_t paste_to_diagram,
                                         data_database_reader_t *db_reader,
                                         ctrl_controller_t *controller,
                                         data_stat_t *io_stat
                                       );

/*!
 *  \brief destroys the io_import_elements_t.
 *
 *  \param this_ pointer to own object attributes
 */
void io_import_elements_destroy( io_import_elements_t *this_ );

/*!
 *  \brief destroys the io_import_elements_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param mode import mode to set
 */
void io_import_elements_set_mode( io_import_elements_t *this_, io_import_mode_t mode );

/*!
 *  \brief synchronizes a diagram with the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_ptr pointer to diagram that shall be written, not NULL, (diagram may be modified)
 *  \param parent_uuid uuid of the parent diagram, NULL if root diagram
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_FOCUS_EMPTY if no parent diagram provided
 */
u8_error_t io_import_elements_sync_diagram( io_import_elements_t *this_,
                                            const data_diagram_t *diagram_ptr,
                                            const char *parent_uuid
                                          );

/*!
 *  \brief synchronizes a diagramelement with the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_ptr pointer to diagramelement that shall be written, not NULL, (diagramelement may be modified)
 *  \param diagram_uuid uuid of the parent diagram
 *  \param node_uuid uuid of the referenced focused feature (lifeline) if there is one,
 *                   uuid of the classifier otherwise.
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_import_elements_sync_diagramelement( io_import_elements_t *this_,
                                                   const data_diagramelement_t *diagramelement_ptr,
                                                   const char *diagram_uuid,
                                                   const char *node_uuid
                                                 );

/*!
 *  \brief invents a diagramelement to attach a new classifier to a diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier for which a diagramelement shall be created
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_import_elements_private_create_diagramelement( io_import_elements_t *this_, data_row_id_t classifier_id );

/*!
 *  \brief synchronizes a classifier with the database
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL, (classifier may be modified)
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_FOCUS_EMPTY if no parent diagram provided
 */
u8_error_t io_import_elements_sync_classifier( io_import_elements_t *this_,
                                               data_classifier_t *classifier_ptr
                                             );

/*!
 *  \brief synchronizes a feature with the database
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_ptr pointer to feature that shall be written, not NULL, (feature may be modified)
 *  \param classifier_uuid uuid of the parent classifier
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_import_elements_sync_feature( io_import_elements_t *this_,
                                            const data_feature_t *feature_ptr,
                                            const char *classifier_uuid
                                          );

/*!
 *  \brief synchronizes a relationship with the database
 *
 *  \param this_ pointer to own object attributes
 *  \param relation_ptr pointer to relationship that shall be written, not NULL, (relationship may be modified)
 *  \param from_node_uuid uuid of the source feature if there is one,
 *                        uuid of the source classifier otherwise.
 *  \param to_node_uuid uuid of the destination feature if there is one,
 *                      uuid of the destination classifier otherwise.
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_import_elements_sync_relationship( io_import_elements_t *this_,
                                                 const data_relationship_t *relation_ptr,
                                                 const char *from_node_uuid,
                                                 const char *to_node_uuid
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
