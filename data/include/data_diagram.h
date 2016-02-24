/* File: data_diagram.h; Copyright and License: see below */

#ifndef DATA_DIAGRAM_H
#define DATA_DIAGRAM_H

/* public file for the doxygen documentation: */
/*! \file */

#include "data_diagram_type.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for the diagram functions
 */
struct data_diagram_struct {
    int32_t id;
    int32_t parent_id;
    data_diagram_type_t diagram_type;
};

typedef struct data_diagram_struct data_diagram_t;

/*!
 *  \brief constants for new diagram id and uninitialized diagram ids
 */
enum data_diagram_id_enum {
    DATA_DIAGRAM_ID_ROOT_ID = 0,
    DATA_DIAGRAM_ID_UNINITIALIZED_ID = -1,
    DATA_DIAGRAM_ID_NEW_ID = -2,
};

/*!
 *  \brief initializes the data_diagram_t struct with id DATA_DIAGRAM_ID_NEW_ID
 */
static inline void data_diagram_init_new ( data_diagram_t *this_, int32_t parent_diagram_id, data_diagram_type_t diagram_type, const char* diagram_name );

/*!
 *  \brief initializes the data_diagram_t struct
 */
static inline void data_diagram_init ( data_diagram_t *this_, int32_t diagram_id, int32_t parent_diagram_id, data_diagram_type_t diagram_type, const char* diagram_name );

/*!
 *  \brief destroys the data_diagram_t struct
 */
static inline void data_diagram_destroy ( data_diagram_t *this_ );

#include "data_diagram.inl"

#endif  /* DATA_DIAGRAM_H */


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
