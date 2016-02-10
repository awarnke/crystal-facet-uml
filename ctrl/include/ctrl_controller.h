/* File: ctrl_controller.h; Copyright and License: see below */

#ifndef CTRL_CONTROLLER_H
#define CTRL_CONTROLLER_H

/* public file for the doxygen documentation: */
/*! \file */

#include "ctrl_classifier.h"
#include "ctrl_diagram.h"
#include "data_database.h"

/*!
 *  \brief all data attributes needed for the diagram functions
 */
struct ctrl_controller_struct {
    ctrl_classifier_t classifiers;
    ctrl_diagram_t diagrams;
    data_database_t *database;
};

typedef struct ctrl_controller_struct ctrl_controller_t;

/*!
 *  \brief initializes the ctrl_controller_t struct
 */
void ctrl_controller_init ( ctrl_controller_t *this_, data_database_t *database );

/*!
 *  \brief destroys the ctrl_controller_t struct
 */
void ctrl_controller_destroy ( ctrl_controller_t *this_ );

/*!
 *  \brief gets the classifier controller
 */
static inline ctrl_classifier_t *ctrl_controller_get_classifier_control ( ctrl_controller_t *this_ );

/*!
 *  \brief gets the diagram controller
 */
static inline ctrl_diagram_t *ctrl_controller_get_diagram_control ( ctrl_controller_t *this_ );

#include "ctrl_controller.inl"

#endif  /* CTRL_CONTROLLER_H */


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
