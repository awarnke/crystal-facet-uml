/* File: data_relationship.h; Copyright and License: see below */

#ifndef DATA_RELATIONSHIP_H
#define DATA_RELATIONSHIP_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a relationship between two classifiers
 */

#include "data_relationship_type.h"
#include "data_id.h"
#include "data_row_id.h"
#include "data_uuid.h"
#include "u8/u8_error.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief constants for max string sizes
 */
enum data_relationship_max_enum {
    DATA_RELATIONSHIP_MAX_NAME_SIZE = 48,
    DATA_RELATIONSHIP_MAX_NAME_LENGTH = 47,
    DATA_RELATIONSHIP_MAX_STEREOTYPE_SIZE = 48,
    DATA_RELATIONSHIP_MAX_STEREOTYPE_LENGTH = 47,
    DATA_RELATIONSHIP_MAX_DESCRIPTION_SIZE = 1024,
    DATA_RELATIONSHIP_MAX_DESCRIPTION_LENGTH = 1023,
};

/*!
 *  \brief all data attributes needed for the relationship functions
 *
 *  \see http://www.uml-diagrams.org/uml-core.html#relationship
 */
struct data_relationship_struct {
    data_row_id_t id;
    data_row_id_t from_classifier_id;
    data_row_id_t from_feature_id;  /*!< from_classifier_id is the master, from_feature_id is an optional information; DATA_ROW_ID_VOID if not used */
    data_row_id_t to_classifier_id;
    data_row_id_t to_feature_id;  /*!< from_classifier_id is the master, from_feature_id is an optional information; DATA_ROW_ID_VOID if not used */
    data_relationship_type_t main_type;
    utf8stringbuf_t stereotype;
    char private_stereotype_buffer[DATA_RELATIONSHIP_MAX_STEREOTYPE_SIZE];
    utf8stringbuf_t name;
    char private_name_buffer[DATA_RELATIONSHIP_MAX_NAME_SIZE];
    utf8stringbuf_t description;
    char private_description_buffer[DATA_RELATIONSHIP_MAX_DESCRIPTION_SIZE];
    int32_t list_order;
    data_uuid_t uuid;  /*!< universal unique identifier, needed to merge vcs-branches */
};

typedef struct data_relationship_struct data_relationship_t;

/*!
 *  \brief initializes the data_relationship_t struct with id, from_classifier_id and to_classifier_id DATA_ROW_ID_VOID;
 *         a fresh uuid and all other values are zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_relationship_init_empty ( data_relationship_t *this_ );

/*!
 *  \brief re-initializes the data_relationship_t struct with id, from_classifier_id and to_classifier_id DATA_ROW_ID_VOID;
 *         a fresh uuid and all other values are zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_relationship_reinit_empty ( data_relationship_t *this_ );


/*!
 *  \brief initializes the data_relationship_t struct with id DATA_ROW_ID_VOID and a fresh uuid
 *
 *  \param this_ pointer to own object attributes
 *  \param relationship_main_type type of the relationship
 *  \param from_classifier_id id of the source classifier
 *  \param to_classifier_id id of the destination classifier
 *  \param stereotype stereotype of the relationship. stereotype must not be NULL.
 *  \param name name of the relationship. name must not be NULL.
 *  \param description description of the relationship. description must not be NULL.
 *  \param list_order list_order of the relationship
 *  \param from_feature_id id of the source feature if the relationship starts at a feature, DATA_ROW_ID_VOID otherwise
 *  \param to_feature_id id of the destination feature if the relationship ends at a feature, DATA_ROW_ID_VOID otherwise
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if string parameters too long, U8_ERROR_NONE otherwise.
 */
static inline u8_error_t data_relationship_init_new ( data_relationship_t *this_,
                                                      data_relationship_type_t relationship_main_type,
                                                      data_row_id_t from_classifier_id,
                                                      data_row_id_t to_classifier_id,
                                                      const char* stereotype,
                                                      const char* name,
                                                      const char* description,
                                                      int32_t list_order,
                                                      data_row_id_t from_feature_id,
                                                      data_row_id_t to_feature_id
                                                    );

/*!
 *  \brief initializes the data_relationship_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param relationship_id id of the relationship
 *  \param relationship_main_type type of the relationship
 *  \param from_classifier_id id of the source classifier
 *  \param to_classifier_id id of the destination classifier
 *  \param stereotype stereotype of the relationship. stereotype must not be NULL.
 *  \param name name of the relationship. name must not be NULL.
 *  \param description description of the relationship. description must not be NULL.
 *  \param list_order list_order of the relationship
 *  \param from_feature_id id of the source feature if the relationship starts at a feature, DATA_ROW_ID_VOID otherwise
 *  \param to_feature_id id of the destination feature if the relationship ends at a feature, DATA_ROW_ID_VOID otherwise
 *  \param uuid a universal unique identifier according to rfc4122
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if string parameters too long,
 *          U8_ERROR_VALUE_OUT_OF_RANGE if uuid malformed, U8_ERROR_NONE otherwise.
 */
static inline u8_error_t data_relationship_init ( data_relationship_t *this_,
                                                  data_row_id_t relationship_id,
                                                  data_relationship_type_t relationship_main_type,
                                                  data_row_id_t from_classifier_id,
                                                  data_row_id_t to_classifier_id,
                                                  const char* stereotype,
                                                  const char* name,
                                                  const char* description,
                                                  int32_t list_order,
                                                  data_row_id_t from_feature_id,
                                                  data_row_id_t to_feature_id,
                                                  const char* uuid
                                                );

/*!
 *  \brief initializes the data_relationship_t struct with a copy
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes to copy
 */
static inline void data_relationship_copy ( data_relationship_t *this_, const data_relationship_t *original );

/*!
 *  \brief overwrites the data_relationship_t struct with a copy
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to object attributes to copy
 */
static inline void data_relationship_replace ( data_relationship_t *this_, const data_relationship_t *that );

/*!
 *  \brief destroys the data_relationship_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_relationship_destroy ( data_relationship_t *this_ );

/*!
 *  \brief gets the attribute id
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline data_row_id_t data_relationship_get_row_id ( const data_relationship_t *this_ );

/*!
 *  \brief sets the attribute id
 *
 *  \param this_ pointer to own object attributes
 *  \param id new id of this object
 */
static inline void data_relationship_set_row_id ( data_relationship_t *this_, data_row_id_t id );

/*!
 *  \brief gets the data_id derived from the id attribute
 *
 *  \param this_ pointer to own object attributes
 *  \return data_id denoting this object
 */
static inline data_id_t data_relationship_get_data_id ( const data_relationship_t *this_ );

/*!
 *  \brief gets the attribute from_classifier_id
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline data_row_id_t data_relationship_get_from_classifier_row_id ( const data_relationship_t *this_ );

/*!
 *  \brief sets the attribute from_classifier_id
 *
 *  \param this_ pointer to own object attributes
 *  \param from_classifier_id new from_classifier_id of this object
 */
static inline void data_relationship_set_from_classifier_row_id ( data_relationship_t *this_, data_row_id_t from_classifier_id );

/*!
 *  \brief gets the data_id derived from the from_classifier_id attribute
 *
 *  \param this_ pointer to own object attributes
 *  \return data_id denoting the from_classifier_id
 */
static inline data_id_t data_relationship_get_from_classifier_data_id ( const data_relationship_t *this_ );

/*!
 *  \brief gets the attribute from_feature_id
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline data_row_id_t data_relationship_get_from_feature_row_id ( const data_relationship_t *this_ );

/*!
 *  \brief sets the attribute from_feature_id
 *
 *  \param this_ pointer to own object attributes
 *  \param from_feature_id new from_feature_id of this object
 */
static inline void data_relationship_set_from_feature_row_id ( data_relationship_t *this_, data_row_id_t from_feature_id );

/*!
 *  \brief gets the data_id derived from the from_feature_id attribute
 *
 *  \param this_ pointer to own object attributes
 *  \return data_id denoting the from_feature_id
 */
static inline data_id_t data_relationship_get_from_feature_data_id ( const data_relationship_t *this_ );

/*!
 *  \brief gets the attribute to_classifier_id
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline data_row_id_t data_relationship_get_to_classifier_row_id ( const data_relationship_t *this_ );

/*!
 *  \brief sets the attribute to_classifier_id
 *
 *  \param this_ pointer to own object attributes
 *  \param to_classifier_id new to_classifier_id of this object
 */
static inline void data_relationship_set_to_classifier_row_id ( data_relationship_t *this_, data_row_id_t to_classifier_id );

/*!
 *  \brief gets the data_id derived from the to_classifier_id attribute
 *
 *  \param this_ pointer to own object attributes
 *  \return data_id denoting the to_classifier_id
 */
static inline data_id_t data_relationship_get_to_classifier_data_id ( const data_relationship_t *this_ );

/*!
 *  \brief gets the attribute to_feature_id
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline data_row_id_t data_relationship_get_to_feature_row_id ( const data_relationship_t *this_ );

/*!
 *  \brief sets the attribute to_feature_id
 *
 *  \param this_ pointer to own object attributes
 *  \param to_feature_id new to_feature_id of this object
 */
static inline void data_relationship_set_to_feature_row_id ( data_relationship_t *this_, data_row_id_t to_feature_id );

/*!
 *  \brief gets the data_id derived from the to_feature_id attribute
 *
 *  \param this_ pointer to own object attributes
 *  \return data_id denoting the to_feature_id
 */
static inline data_id_t data_relationship_get_to_feature_data_id ( const data_relationship_t *this_ );

/*!
 *  \brief gets the attribute main_type
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline data_relationship_type_t data_relationship_get_main_type ( const data_relationship_t *this_ );

/*!
 *  \brief sets the attribute main_type
 *
 *  \param this_ pointer to own object attributes
 *  \param main_type new main_type of this object
 */
static inline void data_relationship_set_main_type ( data_relationship_t *this_, data_relationship_type_t main_type );

/*!
 *  \brief gets the attribute stereotype
 *
 *  A stereotype is a marker-attribute which can be used to assign the relationship to a group
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline const char *data_relationship_get_stereotype_const ( const data_relationship_t *this_ );

/*!
 *  \brief checks if the attribute stereotype is empty
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the stereotype string is not empty
 */
static inline bool data_relationship_has_stereotype ( const data_relationship_t *this_ );

/*!
 *  \brief sets the attribute stereotype
 *
 *  \param this_ pointer to own object attributes
 *  \param stereotype new main_type of this object
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if new string too long
 */
static inline u8_error_t data_relationship_set_stereotype ( data_relationship_t *this_, const char *stereotype );

/*!
 *  \brief gets the attribute name
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline const char *data_relationship_get_name_const ( const data_relationship_t *this_ );

/*!
 *  \brief sets the attribute name
 *
 *  \param this_ pointer to own object attributes
 *  \param name new name of this object
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if new string too long
 */
static inline u8_error_t data_relationship_set_name ( data_relationship_t *this_, const char *name );

/*!
 *  \brief gets the attribute description
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline const char *data_relationship_get_description_const ( const data_relationship_t *this_ );

/*!
 *  \brief sets the attribute description
 *
 *  \param this_ pointer to own object attributes
 *  \param description new description of this object
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if new string too long
 */
static inline u8_error_t data_relationship_set_description ( data_relationship_t *this_, const char *description );

/*!
 *  \brief appends a string to the attribute description
 *
 *  \param this_ pointer to own object attributes
 *  \param description additional description of this object
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if concatenated string too long
 */
static inline u8_error_t data_relationship_append_description ( data_relationship_t *this_, const char *description );

/*!
 *  \brief gets the attribute list_order
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline int32_t data_relationship_get_list_order ( const data_relationship_t *this_ );

/*!
 *  \brief sets the attribute list_order
 *
 *  \param this_ pointer to own object attributes
 *  \param list_order new list_order of this object
 */
static inline void data_relationship_set_list_order ( data_relationship_t *this_, int32_t list_order );

/*!
 *  \brief gets the universal unique identifier of this data_relationship_t
 *
 *  \param this_ pointer to own object attributes
 *  \return uuid of this object
 */
static inline const char *data_relationship_get_uuid_const ( const data_relationship_t *this_ );

/*!
 *  \brief sets the attribute uuid
 *
 *  \param this_ pointer to own object attributes
 *  \param uuid new uuid of this object
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if new string too long
 */
static inline u8_error_t data_relationship_set_uuid ( data_relationship_t *this_, const char *uuid );

/*!
 *  \brief checks if attribute id is not DATA_ROW_ID_VOID
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the id of the relationship is not DATA_ROW_ID_VOID
 */
static inline bool data_relationship_is_valid ( const data_relationship_t *this_ );

/*!
 *  \brief prints the data_relationship_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_relationship_trace ( const data_relationship_t *this_ );

#include "data_relationship.inl"

#endif  /* DATA_RELATIONSHIP_H */


/*
Copyright 2016-2024 Andreas Warnke

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
