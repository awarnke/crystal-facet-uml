/* File: ctrl_classifier_controller.h; Copyright and License: see below */

#ifndef CTRL_CLASSIFIER_CONTROLLER_H
#define CTRL_CLASSIFIER_CONTROLLER_H

/* public file for the doxygen documentation: */
/*! \file */

#include "data_database.h"
#include "data_classifier.h"
#include "data_classifier_type.h"
#include "data_diagram.h"
#include <stdio.h>
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the classifier functions
 */
struct ctrl_classifier_controller_struct {
    data_database_t *database;
};

typedef struct ctrl_classifier_controller_struct ctrl_classifier_controller_t;

/*!
 *  \brief initializes the ctrl_classifier_controller_t struct
 */
void ctrl_classifier_controller_init ( ctrl_classifier_controller_t *this_, data_database_t *database );

/*!
 *  \brief destroys the ctrl_classifier_controller_t struct
 */
void ctrl_classifier_controller_destroy ( ctrl_classifier_controller_t *this_ );

/*!
 *  \brief creates a new classifier or package or other object and attaches it to a diagram
 */
int32_t ctrl_classifier_controller_create_object_in_diagram ( ctrl_classifier_controller_t *this_, int32_t parent_diagram_id, data_classifier_type_t object_type, const char* object_name );


#endif  /* CTRL_CLASSIFIER_CONTROLLER_H */


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
