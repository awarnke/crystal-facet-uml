/* File: data_classifier.h; Copyright and License: see below */

#ifndef DATA_CLASSIFIER_H
#define DATA_CLASSIFIER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a classifier like a class, a state, an activity, an interface
 */

#include "data_classifier_type.h"
#include "data_id.h"
#include "data_row_id.h"
#include "data_error.h"
#include "util/string/utf8stringbuf.h"
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief constants for max string sizes
 */
enum data_classifier_max_enum {
    DATA_CLASSIFIER_MAX_NAME_SIZE = 48,
    DATA_CLASSIFIER_MAX_NAME_LENGTH = 47,
    DATA_CLASSIFIER_MAX_STEREOTYPE_SIZE = 48,
    DATA_CLASSIFIER_MAX_STEREOTYPE_LENGTH = 47,
    DATA_CLASSIFIER_MAX_DESCRIPTION_SIZE = 4096,
    DATA_CLASSIFIER_MAX_DESCRIPTION_LENGTH = 4095,
};

/*!
 *  \brief all data attributes needed for the classifier functions
 *
 *  \see https://en.wikipedia.org/wiki/Classifier_%28UML%29
 *  \see http://www.uml-diagrams.org/classifier.html
 */
struct data_classifier_struct {
    data_row_id_t id;

    data_classifier_type_t main_type;
    utf8stringbuf_t stereotype;
    char private_stereotype_buffer[DATA_CLASSIFIER_MAX_STEREOTYPE_SIZE];
    utf8stringbuf_t name;
    char private_name_buffer[DATA_CLASSIFIER_MAX_NAME_SIZE];
    utf8stringbuf_t description;
    char private_description_buffer[DATA_CLASSIFIER_MAX_DESCRIPTION_SIZE];
    int32_t x_order;
    int32_t y_order;
    int32_t list_order;
};

typedef struct data_classifier_struct data_classifier_t;

/*!
 *  \brief initializes the data_classifier_t struct with id DATA_ROW_ID_VOID
 *
 *  \param this_ pointer to own object attributes
 *  \param main_type type of the classifier.
 *  \param stereotype name of the classifier. classifier_name must not be NULL.
 *  \param name name of the classifier. classifier_name must not be NULL.
 *  \param description description of the classifier. classifier_description must not be NULL.
 *  \param x_order sequence order of objects in x-direction
 *  \param y_order sequence order of objects in y direction
 *  \param list_order sequence order of objects in lists
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if string parameters too long, DATA_ERROR_NONE otherwise.
 */
static inline data_error_t data_classifier_init_new ( data_classifier_t *this_,
                                                      data_classifier_type_t main_type,
                                                      const char* stereotype,
                                                      const char* name,
                                                      const char* description,
                                                      int32_t x_order,
                                                      int32_t y_order,
                                                      int32_t list_order
                                                    );

/*!
 *  \brief initializes the data_classifier_t struct with id DATA_ROW_ID_VOID; all other values are zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_classifier_init_empty ( data_classifier_t *this_ );

/*!
 *  \brief re-initializes the data_classifier_t struct with id DATA_ROW_ID_VOID; all other values are zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_classifier_reinit_empty ( data_classifier_t *this_ );

/*!
 *  \brief initializes the data_classifier_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param id id of the classifier
 *  \param main_type type of the classifier.
 *  \param stereotype name of the classifier. classifier_name must not be NULL.
 *  \param name name of the classifier. classifier_name must not be NULL.
 *  \param description description of the classifier. classifier_description must not be NULL.
 *  \param x_order sequence order of objects in x-direction
 *  \param y_order sequence order of objects in y direction
 *  \param list_order sequence order of objects in lists
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if string parameters too long, DATA_ERROR_NONE otherwise.
 */
static inline data_error_t data_classifier_init ( data_classifier_t *this_,
                                                  data_row_id_t id,
                                                  data_classifier_type_t main_type,
                                                  const char* stereotype,
                                                  const char* name,
                                                  const char* description,
                                                  int32_t x_order,
                                                  int32_t y_order,
                                                  int32_t list_order
                                                );

/*!
 *  \brief ire-nitializes the data_classifier_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param id id of the classifier
 *  \param main_type type of the classifier.
 *  \param stereotype name of the classifier. classifier_name must not be NULL.
 *  \param name name of the classifier. classifier_name must not be NULL.
 *  \param description description of the classifier. classifier_description must not be NULL.
 *  \param x_order sequence order of objects in x-direction
 *  \param y_order sequence order of objects in y direction
 *  \param list_order sequence order of objects in lists
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if string parameters too long, DATA_ERROR_NONE otherwise.
 */
static inline data_error_t data_classifier_reinit ( data_classifier_t *this_,
                                                    data_row_id_t id,
                                                    data_classifier_type_t main_type,
                                                    const char* stereotype,
                                                    const char* name,
                                                    const char* description,
                                                    int32_t x_order,
                                                    int32_t y_order,
                                                    int32_t list_order
                                                  );

/*!
 *  \brief initializes the data_classifier_t struct with a copy
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes to copy
 */
static inline void data_classifier_copy ( data_classifier_t *this_, const data_classifier_t *original );

/*!
 *  \brief overwrites the data_classifier_t struct with a copy
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to object attributes to copy
 */
static inline void data_classifier_replace ( data_classifier_t *this_, const data_classifier_t *that );

/*!
 *  \brief destroys the data_classifier_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_classifier_destroy ( data_classifier_t *this_ );

/*!
 *  \brief prints the data_classifier_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_classifier_trace ( const data_classifier_t *this_ );

/*!
 *  \brief gets the attribute id
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline data_row_id_t data_classifier_get_row_id ( const data_classifier_t *this_ );

/*!
 *  \brief sets the attribute id
 *
 *  \param this_ pointer to own object attributes
 *  \param id new id of this object
 */
static inline void data_classifier_set_row_id ( data_classifier_t *this_, data_row_id_t id );

/*!
 *  \brief gets the data_id derived from the id attribute
 *
 *  \param this_ pointer to own object attributes
 *  \return data_id denoting this object
 */
static inline data_id_t data_classifier_get_data_id ( const data_classifier_t *this_ );

/*!
 *  \brief gets the attribute main_type
 *
 *  The main_type is the type of this element.
 *  The name main_type is chosen to
 *  distinguish this type from the type-name of a possible generalization or the class-name which this possible instance possibly realizes.
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline data_classifier_type_t data_classifier_get_main_type ( const data_classifier_t *this_ );

/*!
 *  \brief sets the attribute main_type
 *
 *  \param this_ pointer to own object attributes
 *  \param main_type new main_type of this object
 */
static inline void data_classifier_set_main_type ( data_classifier_t *this_, data_classifier_type_t main_type );

/*!
 *  \brief gets the attribute stereotype
 *
 *  A stereotype is a marker-attribute which can be used to assign the classifier to a group
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline const char *data_classifier_get_stereotype_ptr ( const data_classifier_t *this_ );

/*!
 *  \brief gets the attribute stereotype as utf8stringbuf_t
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline utf8stringbuf_t data_classifier_get_stereotype_buf_ptr ( data_classifier_t *this_ );

/*!
 *  \brief checks if the attribute stereotype is empty
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the stereotype string is not empty
 */
static inline bool data_classifier_has_stereotype ( data_classifier_t *this_ );

/*!
 *  \brief sets the attribute stereotype
 *
 *  \param this_ pointer to own object attributes
 *  \param stereotype new main_type of this object
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if new string too long
 */
static inline data_error_t data_classifier_set_stereotype ( data_classifier_t *this_, const char *stereotype );

/*!
 *  \brief gets the attribute name
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline const char *data_classifier_get_name_ptr ( const data_classifier_t *this_ );

/*!
 *  \brief gets the attribute name as utf8stringbuf_t
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline utf8stringbuf_t data_classifier_get_name_buf_ptr ( data_classifier_t *this_ );

/*!
 *  \brief sets the attribute name
 *
 *  \param this_ pointer to own object attributes
 *  \param name new name of this object
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if new string too long
 */
static inline data_error_t data_classifier_set_name ( data_classifier_t *this_, const char *name );

/*!
 *  \brief gets the attribute description
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline const char *data_classifier_get_description_ptr ( const data_classifier_t *this_ );

/*!
 *  \brief gets the attribute description as utf8stringbuf_t
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline utf8stringbuf_t data_classifier_get_description_buf_ptr ( data_classifier_t *this_ );

/*!
 *  \brief sets the attribute description
 *
 *  \param this_ pointer to own object attributes
 *  \param description new description of this object
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if new string too long
 */
static inline data_error_t data_classifier_set_description ( data_classifier_t *this_, const char *description );

/*!
 *  \brief gets the attribute x_order
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline int32_t data_classifier_get_x_order ( const data_classifier_t *this_ );

/*!
 *  \brief sets the attribute x_order
 *
 *  \param this_ pointer to own object attributes
 *  \param x_order new x_order of this object
 */
static inline void data_classifier_set_x_order ( data_classifier_t *this_, int32_t x_order );

/*!
 *  \brief gets the attribute y_order
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline int32_t data_classifier_get_y_order ( const data_classifier_t *this_ );

/*!
 *  \brief sets the attribute y_order
 *
 *  \param this_ pointer to own object attributes
 *  \param y_order new y_order of this object
 */
static inline void data_classifier_set_y_order ( data_classifier_t *this_, int32_t y_order );

/*!
 *  \brief gets the attribute list_order
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline int32_t data_classifier_get_list_order ( const data_classifier_t *this_ );

/*!
 *  \brief sets the attribute list_order
 *
 *  \param this_ pointer to own object attributes
 *  \param list_order new list_order of this object
 */
static inline void data_classifier_set_list_order ( data_classifier_t *this_, int32_t list_order );

/*!
 *  \brief checks if attribute id is not DATA_ROW_ID_VOID
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the id of the classifier is not DATA_ROW_ID_VOID
 */
static inline bool data_classifier_is_valid ( const data_classifier_t *this_ );

#include "data_classifier.inl"

#endif  /* DATA_CLASSIFIER_H */


/*
Copyright 2016-2021 Andreas Warnke

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
