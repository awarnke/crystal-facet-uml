/* File: data_table.h; Copyright and License: see below */

#ifndef DATA_TABLE_H
#define DATA_TABLE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Specifies the table in which an object is stored
 */

/*!
 *  \brief database table in which an object is stored
 *
 *  A data_table_t and an id allow to uniquely specify any object in the database
 */
enum data_table_enum {
    DATA_TABLE_VOID = 0,  /*!< no table selected */
    DATA_TABLE_CLASSIFIER = 1,  /*!< \see data_classifier_t */
    DATA_TABLE_FEATURE = 2,  /*!< \see data_feature_t */
    DATA_TABLE_RELATIONSHIP = 3,  /*!< \see data_relationship_t */
    DATA_TABLE_DIAGRAMELEMENT = 4,  /*!< \see data_diagramelement_t */
    DATA_TABLE_DIAGRAM = 5,  /*!< \see data_diagram_t */
};

typedef enum data_table_enum data_table_t;

#endif  /* DATA_TABLE_H */


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
