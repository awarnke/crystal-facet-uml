/* File: json_writer_pass.h; Copyright and License: see below */

#ifndef JSON_WRITER_PASS_H
#define JSON_WRITER_PASS_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines the semantics of multiple model-traversal-passes
 */

/*!
 *  \brief constants for output mode: classifiers+features, relationships, diagrams
 */
enum json_writer_pass_enum {
    JSON_WRITER_PASS_VIEWS = 0,  /*!< mode diagrams and diagramelements output */
    JSON_WRITER_PASS_NODES = 1,  /*!< mode for classifiers and features output */
    JSON_WRITER_PASS_EDGES = 2,  /*!< mode for relationships output */
};

typedef enum json_writer_pass_enum json_writer_pass_t;

#endif  /* JSON_WRITER_PASS_H */


/*
Copyright 2021-2024 Andreas Warnke

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
