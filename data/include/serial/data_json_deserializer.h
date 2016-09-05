/* File: data_json_deserializer.h; Copyright and License: see below */

#ifndef DATA_JSON_DESERIALIZER_H
#define DATA_JSON_DESERIALIZER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Deserializes data objects from json format.
 *
 *  This object is a json parser, using the data_json_tokenizer as lexer.
 */

#include "serial/data_json_tokenizer.h"
#include "data_error.h"
#include "util/string/utf8stringbuf.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief constants for max stack size
 */
enum data_json_deserializer_max_enum {
    DATA_JSON_DESERIALIZER_MAX_STACK_SIZE = 16,  /*!< maxmum number of stack size, objects and arrays that contain other objects and arrays. */
};

/*!
 *  \brief constants for parser state
 */
enum data_json_deserializer_state_enum {
    DATA_JSON_DESERIALIZER_STATE_START,  /*!< no token has been parsed yet */
    DATA_JSON_DESERIALIZER_STATE_BEGIN_OBJECT, /*!< last parsed token is a begin-object */
    DATA_JSON_DESERIALIZER_STATE_BEGIN_ARRAY, /*!< last parsed token is a begin-object */
    DATA_JSON_DESERIALIZER_STATE_VALUE, /*!< last parsed token is a complete value, e.g. 17 or an array */
    DATA_JSON_DESERIALIZER_STATE_VALUE_SEPARATOR, /*!< last parsed token is the value separator ',' */
    DATA_JSON_DESERIALIZER_STATE_NAME_SEPARATOR, /*!< last parsed token is the name separator ':' */
    DATA_JSON_DESERIALIZER_STATE_NAME, /*!< last parsed token is a member name */
};

/*!
 *  \brief all data attributes needed for deserializing data objects
 *
 *  The attribute values keep track on the structure of the input tokens
 *  to check if these appear in the right order.
 */
struct data_json_deserializer_struct {
    data_json_tokenizer_t tokenizer;  /*!< own tokenizer instance to consecutively fetch tokens from the json input file */

    unsigned int container_stack_size;  /*!< current stack size: objects and arrays that contain other objects and arrays. */
    data_json_value_type_t container_stack[ DATA_JSON_DESERIALIZER_MAX_STACK_SIZE ];
    unsigned int root_object_count;  /*!< number of objects at top-level; shall be 1 */
    enum data_json_deserializer_state_enum last_token;  /*!< type of last parsed token */
};

typedef struct data_json_deserializer_struct data_json_deserializer_t;

/*!
 *  \brief initializes the data_json_deserializer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_json_deserializer_init ( data_json_deserializer_t *this_ );

/*!
 *  \brief destroys the data_json_deserializer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_json_deserializer_destroy ( data_json_deserializer_t *this_ );

#endif  /* DATA_JSON_DESERIALIZER_H */


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
