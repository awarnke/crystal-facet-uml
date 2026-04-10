/* File: io_import_step.h; Copyright and License: see below */

#ifndef IO_IMPORT_STEP_H
#define IO_IMPORT_STEP_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines the import step for json import streams
 */

/*!
 *  \brief step/pass of the json import
 */
enum io_import_step_enum {
    IO_IMPORT_STEP_CHECK = 0,  /*!< just evaluate the input stream for errors */
    IO_IMPORT_STEP_CREATE = 1,  /*!< after this first pass, all diagrams and classifiers shall exist, also lifelines */
    IO_IMPORT_STEP_LINK_VIEWS = 2,  /*!< after this second pass, all diagramelements and diagram_parents shall be linked */
    IO_IMPORT_STEP_ADD_FEATURES = 3,  /*!< after this pass, all features shall exist */
    IO_IMPORT_STEP_RELATE_NODES = 4,  /*!< after this pass, all relationships shall exist */
};

typedef enum io_import_step_enum io_import_step_t;

#endif  /* IO_IMPORT_STEP_H */


/*
Copyright 2016-2026 Andreas Warnke

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
