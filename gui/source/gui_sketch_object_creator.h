/* File: gui_sketch_object_creator.h; Copyright and License: see below */

#ifndef GUI_SKETCH_OBJECT_CREATOR_H
#define GUI_SKETCH_OBJECT_CREATOR_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Creates new classifiers, diagrams, relationships and features
 */

#include "storage/data_database_reader.h"
#include "data_diagram.h"
#include "data_table.h"
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
    data_rules_t data_rules;  /*!< own instance of uml and sysml consistency rules */
};

typedef struct gui_sketch_object_creator_struct gui_sketch_object_creator_t;

/*!
 *  \brief initializes the gui_sketch_object_creator_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param controller pointer to a controller object which can modify the database
 *  \param db_reader pointer to a database reader object
 */
void gui_sketch_object_creator_init ( gui_sketch_object_creator_t *this_,
                                     ctrl_controller_t *controller,
                                     data_database_reader_t *db_reader
                                   );

/*!
 *  \brief destroys the gui_sketch_object_creator_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_object_creator_destroy ( gui_sketch_object_creator_t *this_ );

/*!
 *  \brief destroys the gui_sketch_object_creator_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram where the new classifier shall appear
 *  \param x_order x order where the new classifier shall be located
 *  \param y_order y order where the new classifier shall be located
 *  \param out_diagramelement_id row_id of the newly created diagramelement
 *  \param out_classifier_id row_id of the newly created classifier
 *  \return CTRL_ERROR_NONE in case of success, CTRL_ERROR_DUPLICATE_NAME in case of error
 */
ctrl_error_t gui_sketch_object_creator_create_classifier ( gui_sketch_object_creator_t *this_,
                                                           int64_t diagram_id,
                                                           int32_t x_order,
                                                           int32_t y_order,
                                                           int64_t *out_diagramelement_id,
                                                           int64_t *out_classifier_id
                                                         );

#endif  /* GUI_SKETCH_OBJECT_CREATOR_H */


/*
Copyright 2017-2017 Andreas Warnke

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