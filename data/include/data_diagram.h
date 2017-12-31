/* File: data_diagram.h; Copyright and License: see below */

#ifndef DATA_DIAGRAM_H
#define DATA_DIAGRAM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a diagram
 */

#include "data_diagram_type.h"
#include "data_error.h"
#include "data_id.h"
#include "util/string/utf8stringbuf.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief constants for max string sizes
 */
enum data_diagram_max_enum {
    DATA_DIAGRAM_MAX_NAME_SIZE = 48,
    DATA_DIAGRAM_MAX_NAME_LENGTH = 47,
    DATA_DIAGRAM_MAX_DESCRIPTION_SIZE = 8192,
    DATA_DIAGRAM_MAX_DESCRIPTION_LENGTH = 8191,
};

/*!
 *  \brief all data attributes needed for the diagram functions
 */
struct data_diagram_struct {
    int64_t id;
    int64_t parent_id;
    data_diagram_type_t diagram_type;
    utf8stringbuf_t name;
    char private_name_buffer[DATA_DIAGRAM_MAX_NAME_SIZE];
    utf8stringbuf_t description;
    char private_description_buffer[DATA_DIAGRAM_MAX_DESCRIPTION_SIZE];
    int32_t list_order;
};

typedef struct data_diagram_struct data_diagram_t;

/*!
 *  \brief initializes the data_diagram_t struct with id DATA_ID_VOID_ID
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_diagram_id id of the parent diagram
 *  \param diagram_type type of the diagram
 *  \param diagram_name name of the diagram. diagram_name must not be NULL.
 *  \param diagram_description description of the diagram. diagram_description must not be NULL.
 *  \param list_order list_order of the diagram
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if string parameters too long, DATA_ERROR_NONE otherwise.
 */
static inline data_error_t data_diagram_init_new ( data_diagram_t *this_,
                                                   int64_t parent_diagram_id,
                                                   data_diagram_type_t diagram_type,
                                                   const char* diagram_name,
                                                   const char* diagram_description,
                                                   int32_t list_order
                                                 );

/*!
 *  \brief initializes the data_diagram_t struct with id and parent_id DATA_ID_VOID_ID; all other values are zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_diagram_init_empty ( data_diagram_t *this_ );

/*!
 *  \brief re-initializes the data_diagram_t struct with id and parent_id DATA_ID_VOID_ID; all other values are zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_diagram_reinit_empty ( data_diagram_t *this_ );

/*!
 *  \brief initializes the data_diagram_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram
 *  \param parent_diagram_id id of the parent diagram
 *  \param diagram_type type of the diagram
 *  \param diagram_name name of the diagram. diagram_name must not be NULL.
 *  \param diagram_description description of the diagram. diagram_description must not be NULL.
 *  \param list_order list_order of the diagram
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if string parameters too long, DATA_ERROR_NONE otherwise.
 */
static inline data_error_t data_diagram_init ( data_diagram_t *this_,
                                               int64_t diagram_id,
                                               int64_t parent_diagram_id,
                                               data_diagram_type_t diagram_type,
                                               const char* diagram_name,
                                               const char* diagram_description,
                                               int32_t list_order
                                             );

/*!
 *  \brief initializes the data_diagram_t struct with a copy
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes to copy
 */
static inline void data_diagram_copy ( data_diagram_t *this_, const data_diagram_t *original );

/*!
 *  \brief overwrites the data_diagram_t struct with a copy
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to object attributes to copy
 */
static inline void data_diagram_replace ( data_diagram_t *this_, const data_diagram_t *that );

/*!
 *  \brief destroys the data_diagram_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_diagram_destroy ( data_diagram_t *this_ );

/*!
 *  \brief prints the data_diagram_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_diagram_trace ( data_diagram_t *this_ );

/*!
 *  \brief gets the attribute id
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline int64_t data_diagram_get_id ( data_diagram_t *this_ );

/*!
 *  \brief sets the attribute id
 *
 *  \param this_ pointer to own object attributes
 *  \param id new id of this object
 */
static inline void data_diagram_set_id ( data_diagram_t *this_, int64_t id );

/*!
 *  \brief gets the attribute parent_id
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline int64_t data_diagram_get_parent_id ( data_diagram_t *this_ );

/*!
 *  \brief sets the attribute parent_id
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_id new parent_id of this object
 */
static inline void data_diagram_set_parent_id ( data_diagram_t *this_, int64_t parent_id );

/*!
 *  \brief gets the attribute diagram_type
 *
 *  \param this_ pointer to own object attributes
 */
static inline data_diagram_type_t data_diagram_get_diagram_type ( data_diagram_t *this_ );

/*!
 *  \brief sets the attribute diagram_type
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_type new diagram_type of this object
 */
static inline void data_diagram_set_diagram_type ( data_diagram_t *this_, data_diagram_type_t diagram_type );

/*!
 *  \brief gets the attribute name
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline const char *data_diagram_get_name_ptr ( data_diagram_t *this_ );

/*!
 *  \brief gets the attribute name as utf8stringbuf_t
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline utf8stringbuf_t data_diagram_get_name_buf_ptr ( data_diagram_t *this_ );

/*!
 *  \brief sets the attribute name
 *
 *  \param this_ pointer to own object attributes
 *  \param name new name of this object
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if new string too long
 */
static inline data_error_t data_diagram_set_name ( data_diagram_t *this_, const char *name );

/*!
 *  \brief gets the attribute description
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline const char *data_diagram_get_description_ptr ( data_diagram_t *this_ );

/*!
 *  \brief gets the attribute description as utf8stringbuf_t
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline utf8stringbuf_t data_diagram_get_description_buf_ptr ( data_diagram_t *this_ );

/*!
 *  \brief sets the attribute description
 *
 *  \param this_ pointer to own object attributes
 *  \param description new description of this object
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if new string too long
 */
static inline data_error_t data_diagram_set_description ( data_diagram_t *this_, const char *description );

/*!
 *  \brief gets the attribute list_order
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline int32_t data_diagram_get_list_order ( data_diagram_t *this_ );

/*!
 *  \brief sets the attribute list_order
 *
 *  \param this_ pointer to own object attributes
 *  \param list_order new list_order of this object
 */
static inline void data_diagram_set_list_order ( data_diagram_t *this_, int32_t list_order );

/*!
 *  \brief checks if attribute id is not DATA_ID_VOID_ID
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the id of the diagram is not DATA_ID_VOID_ID
 */
static inline bool data_diagram_is_valid ( data_diagram_t *this_ );

#include "data_diagram.inl"

#endif  /* DATA_DIAGRAM_H */


/*
Copyright 2016-2018 Andreas Warnke

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
