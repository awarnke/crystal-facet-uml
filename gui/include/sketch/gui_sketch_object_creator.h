/* File: gui_sketch_object_creator.h; Copyright and License: see below */

#ifndef GUI_SKETCH_OBJECT_CREATOR_H
#define GUI_SKETCH_OBJECT_CREATOR_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Creates new classifiers, diagrams, relationships and features
 */

#include "gui_simple_message_to_user.h"
#include "gui_sketch_defaults.h"
#include "storage/data_database_reader.h"
#include "entity/data_diagram.h"
#include "entity/data_table.h"
#include "data_rules.h"
#include "ctrl_controller.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief attributes of the sketch area widget
 */
struct gui_sketch_object_creator_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    ctrl_controller_t *controller;  /*!< pointer to external controller */
    gui_sketch_defaults_t defaults;  /*!< own instance of uml and sysml model default types */
    data_rules_t data_rules;  /*!< own instance of uml and sysml consistency rules */
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external gui_simple_message_to_user_t */

    data_diagram_t private_temp_diagram;  /*!< instance for temporary object data, exists here to avoid medium-sized objects on the stack */
    data_classifier_t private_temp_classifier;  /*!< instance for temporary object data, exists here to avoid medium-sized objects on the stack */
    data_relationship_t private_temp_relationship;  /*!< instance for temporary object data, exists here to avoid medium-sized objects on the stack */
    data_feature_t private_temp_feature;  /*!< instance for temporary object data, exists here to avoid medium-sized objects on the stack */
};

typedef struct gui_sketch_object_creator_struct gui_sketch_object_creator_t;

/*!
 *  \brief initializes the gui_sketch_object_creator_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param controller pointer to a controller object which can modify the database
 *  \param db_reader pointer to a database reader object
 *  \param message_to_user pointer to the message_to_user object to use
 */
void gui_sketch_object_creator_init ( gui_sketch_object_creator_t *this_,
                                      ctrl_controller_t *controller,
                                      data_database_reader_t *db_reader,
                                      gui_simple_message_to_user_t *message_to_user
                                    );

/*!
 *  \brief destroys the gui_sketch_object_creator_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_object_creator_destroy ( gui_sketch_object_creator_t *this_ );

/*!
 *  \brief creates a new classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram where the new classifier shall appear
 *  \param x_order x order where the new classifier shall be located
 *  \param y_order y order where the new classifier shall be located
 *  \param out_diagramelement_id row_id of the newly created diagramelement
 *  \param out_classifier_id row_id of the newly created classifier
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_DUPLICATE_NAME in case of error
 */
u8_error_t gui_sketch_object_creator_create_classifier ( gui_sketch_object_creator_t *this_,
                                                           data_row_t diagram_id,
                                                           int32_t x_order,
                                                           int32_t y_order,
                                                           data_row_t *out_diagramelement_id,
                                                           data_row_t *out_classifier_id
                                                         );

/*!
 *  \brief creates a new classifier as child of another one
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram where the new classifier shall appear
 *  \param parent_classifier_id id of the parent classifier of the new classifier
 *  \param x_order x order where the new classifier shall be located
 *  \param y_order y order where the new classifier shall be located
 *  \param out_diagramelement_id row_id of the newly created diagramelement
 *  \param out_classifier_id row_id of the newly created classifier
 *  \param out_relationship_id row_id of the newly created relationship
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_DUPLICATE_NAME in case of error
 */
u8_error_t gui_sketch_object_creator_create_classifier_as_child ( gui_sketch_object_creator_t *this_,
                                                                    data_row_t diagram_id,
                                                                    data_row_t parent_classifier_id,
                                                                    int32_t x_order,
                                                                    int32_t y_order,
                                                                    data_row_t *out_diagramelement_id,
                                                                    data_row_t *out_classifier_id,
                                                                    data_row_t *out_relationship_id
                                                                  );

/*!
 *  \brief creates a new diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_diagram_id id of the parent diagram where the new diagram shall appear. DATA_ROW_VOID if a root diagram shall be created.
 *  \param list_order order of the new diagram
 *  \param out_diagram_id row_id of the newly created diagram
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t gui_sketch_object_creator_create_diagram ( gui_sketch_object_creator_t *this_,
                                                        data_row_t parent_diagram_id,
                                                        int32_t list_order,
                                                        data_row_t *out_diagram_id
                                                      );

/*!
 *  \brief creates a new relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_type diagram type where the relationship was constructed and shall be visible
 *  \param from_classifier_id id of the classifier where the new relationship shall start from
 *  \param from_feature_id optional id of the feature where the new relationship shall start from; alternatively DATA_ROW_VOID
 *  \param to_classifier_id id of the classifier where the new relationship shall end at
 *  \param to_feature_id optional id of the feature where the new relationship shall end at; alternatively DATA_ROW_VOID
 *  \param list_order order where the new relationship shall be located
 *  \param out_relationship_id row_id of the newly created relationship
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t gui_sketch_object_creator_create_relationship ( gui_sketch_object_creator_t *this_,
                                                             data_diagram_type_t diag_type,
                                                             data_row_t from_classifier_id,
                                                             data_row_t from_feature_id,
                                                             data_row_t to_classifier_id,
                                                             data_row_t to_feature_id,
                                                             int32_t list_order,
                                                             data_row_t *out_relationship_id
                                                           );

/*!
 *  \brief creates a new feature
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_type diagram type where the relationship was constructed and shall be visible
 *  \param parent_classifier_id id of the parent classifier where the new feature shall appear
 *  \param std_list_order order where the new feature shall be located, if not a port
 *  \param port_list_order order where the new feature shall be located, if a port
 *  \param out_feature_id row_id of the newly created feature
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_DUPLICATE_NAME in case of error
 */
u8_error_t gui_sketch_object_creator_create_feature ( gui_sketch_object_creator_t *this_,
                                                        data_diagram_type_t diag_type,
                                                        data_row_t parent_classifier_id,
                                                        int32_t std_list_order,
                                                        int32_t port_list_order,
                                                        data_row_t *out_feature_id
                                                      );

/*!
 *  \brief proposes a default name for diagrams
 *
 *  \param this_ pointer to own object attributes
 *  \return a name for a diagram
 */
const char* gui_sketch_object_creator_private_propose_diagram_name ( gui_sketch_object_creator_t *this_ );

/*!
 *  \brief proposes a default name for classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param c_type type of the classifier
 *  \return a name for a classifier
 */
const char* gui_sketch_object_creator_private_propose_classifier_name ( gui_sketch_object_creator_t *this_,
                                                                        data_classifier_type_t c_type
                                                                      );

/*!
 *  \brief proposes a default name for features
 *
 *  \param this_ pointer to own object attributes
 *  \param f_type type of the feature
 *  \return a name for a feature
 */
const char* gui_sketch_object_creator_private_propose_feature_name ( gui_sketch_object_creator_t *this_,
                                                                     data_feature_type_t f_type
                                                                   );

#endif  /* GUI_SKETCH_OBJECT_CREATOR_H */


/*
Copyright 2017-2024 Andreas Warnke

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
