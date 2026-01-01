/* File: data_feature.h; Copyright and License: see below */

#ifndef DATA_FEATURE_H
#define DATA_FEATURE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a feature like a class-attribute or a class-method
 */

#include "entity/data_feature_type.h"
#include "entity/data_id.h"
#include "entity/data_row.h"
#include "entity/data_uuid.h"
#include "u8/u8_error.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief constants for max string sizes
 */
enum data_feature_max_enum {
    DATA_FEATURE_MAX_KEY_SIZE = 48,  /* name */
    DATA_FEATURE_MAX_KEY_LENGTH = 47,
    DATA_FEATURE_MAX_VALUE_SIZE = 256,  /* type */
    DATA_FEATURE_MAX_VALUE_LENGTH = 255,
    DATA_FEATURE_MAX_DESCRIPTION_SIZE = 1024,
    DATA_FEATURE_MAX_DESCRIPTION_LENGTH = 1023,
};

/*!
 *  \brief all data attributes needed for the feature (property or operation) functions
 *
 *  \see http://www.uml-diagrams.org/uml-core.html#feature
 */
struct data_feature_struct {
    data_row_t id;
    data_row_t classifier_id;
    data_feature_type_t main_type;
    utf8stringbuf_t key;  /*!< name of the feature */
    char private_key_buffer[DATA_FEATURE_MAX_KEY_SIZE];
    utf8stringbuf_t value;  /*!< type of the feature */
    char private_value_buffer[DATA_FEATURE_MAX_VALUE_SIZE];
    utf8stringbuf_t description;
    char private_description_buffer[DATA_FEATURE_MAX_DESCRIPTION_SIZE];
    int32_t list_order;  /*!< For feature lists, lower values are on top.
                              For ports and interfaces, INT32_MIN/MAX is bottom-right, INT32_MIN/2 is top-right,
                              0 is top-left, INT32_MAX/2 is bottom-left */
    data_uuid_t uuid;  /*!< universal unique identifier, needed to merge vcs-branches */
};

typedef struct data_feature_struct data_feature_t;

/*!
 *  \brief initializes the data_feature_t struct with id and classifier_id DATA_ROW_VOID;
 *         a fresh uuid and all other values are zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_feature_init_empty ( data_feature_t *this_ );

/*!
 *  \brief re-initializes the data_feature_t struct with id and classifier_id DATA_ROW_VOID;
 *         a fresh uuid and all other values are zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_feature_reinit_empty ( data_feature_t *this_ );

/*!
 *  \brief initializes the data_feature_t struct with id DATA_ROW_VOID and a fresh uuid
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_main_type main_type of the feature
 *  \param classifier_id classifier_id of the feature's parent classifier
 *  \param feature_key key/name of the feature. feature_key must not be NULL.
 *  \param feature_value value/type of the feature. feature_value must not be NULL.
 *  \param feature_description description of the feature. feature_description must not be NULL.
 *  \param list_order list_order of the feature
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if string parameters too long, U8_ERROR_NONE otherwise.
 */
static inline u8_error_t data_feature_init_new ( data_feature_t *this_,
                                                 data_feature_type_t feature_main_type,
                                                 data_row_t classifier_id,
                                                 const char* feature_key,
                                                 const char* feature_value,
                                                 const char* feature_description,
                                                 int32_t list_order
                                                );

/*!
 *  \brief initializes the data_feature_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_id id of the feature
 *  \param feature_main_type main_type of the feature
 *  \param classifier_id classifier_id of the feature's parent classifier
 *  \param feature_key key/name of the feature. feature_key must not be NULL.
 *  \param feature_value value/type of the feature. feature_value must not be NULL.
 *  \param feature_description description of the feature. feature_description must not be NULL.
 *  \param list_order list_order of the feature
 *  \param uuid a universal unique identifier according to rfc4122
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if string parameters too long,
 *          U8_ERROR_VALUE_OUT_OF_RANGE if uuid malformed, U8_ERROR_NONE otherwise.
 */
static inline u8_error_t data_feature_init ( data_feature_t *this_,
                                             data_row_t feature_id,
                                             data_feature_type_t feature_main_type,
                                             data_row_t classifier_id,
                                             const char* feature_key,
                                             const char* feature_value,
                                             const char* feature_description,
                                             int32_t list_order,
                                             const char* uuid
                                           );

/*!
 *  \brief initializes the data_feature_t struct with a copy
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes to copy
 */
static inline void data_feature_copy ( data_feature_t *this_, const data_feature_t *original );

/*!
 *  \brief overwrites the data_feature_t struct with a copy
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to object attributes to copy
 */
static inline void data_feature_replace ( data_feature_t *this_, const data_feature_t *that );

/*!
 *  \brief destroys the data_feature_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_feature_destroy ( data_feature_t *this_ );

/*!
 *  \brief gets the attribute id
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline data_row_t data_feature_get_row_id ( const data_feature_t *this_ );

/*!
 *  \brief sets the attribute id
 *
 *  \param this_ pointer to own object attributes
 *  \param id new id of this object
 */
static inline void data_feature_set_row_id ( data_feature_t *this_, data_row_t id );

/*!
 *  \brief gets the data_id derived from the id attribute
 *
 *  \param this_ pointer to own object attributes
 *  \return data_id denoting this object
 */
static inline data_id_t data_feature_get_data_id ( const data_feature_t *this_ );

/*!
 *  \brief gets the attribute classifier_id
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline data_row_t data_feature_get_classifier_row_id ( const data_feature_t *this_ );

/*!
 *  \brief sets the attribute classifier_id
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id new classifier_id of this object
 */
static inline void data_feature_set_classifier_row_id ( data_feature_t *this_, data_row_t classifier_id );

/*!
 *  \brief gets the data_id derived from the classifier_id attribute
 *
 *  \param this_ pointer to own object attributes
 *  \return data_id denoting the classifier_id
 */
static inline data_id_t data_feature_get_classifier_data_id ( const data_feature_t *this_ );

/*!
 *  \brief gets the attribute main_type
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline data_feature_type_t data_feature_get_main_type ( const data_feature_t *this_ );

/*!
 *  \brief sets the attribute main_type
 *
 *  \param this_ pointer to own object attributes
 *  \param main_type new main_type of this object
 */
static inline void data_feature_set_main_type ( data_feature_t *this_, data_feature_type_t main_type );

/*!
 *  \brief gets the attribute key
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline const char *data_feature_get_key_const ( const data_feature_t *this_ );

/*!
 *  \brief sets the attribute key
 *
 *  \param this_ pointer to own object attributes
 *  \param key new key of this object
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if new string too long
 */
static inline u8_error_t data_feature_set_key ( data_feature_t *this_, const char *key );

/*!
 *  \brief gets the attribute value
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline const char *data_feature_get_value_const ( const data_feature_t *this_ );

/*!
 *  \brief checks if the value attribute is empty
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the value is not empty
 */
static inline bool data_feature_has_value ( const data_feature_t *this_ );

/*!
 *  \brief sets the attribute value
 *
 *  \param this_ pointer to own object attributes
 *  \param value new value of this object
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if new string too long
 */
static inline u8_error_t data_feature_set_value ( data_feature_t *this_, const char *value );

/*!
 *  \brief gets the attribute description
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline const char *data_feature_get_description_const ( const data_feature_t *this_ );

/*!
 *  \brief sets the attribute description
 *
 *  \param this_ pointer to own object attributes
 *  \param description new description of this object
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if new string too long
 */
static inline u8_error_t data_feature_set_description ( data_feature_t *this_, const char *description );

/*!
 *  \brief appends a string to the attribute description
 *
 *  \param this_ pointer to own object attributes
 *  \param description additional description of this object
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if concatenated string too long
 */
static inline u8_error_t data_feature_append_description ( data_feature_t *this_, const char *description );

/*!
 *  \brief gets the attribute list_order
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline int32_t data_feature_get_list_order ( const data_feature_t *this_ );

/*!
 *  \brief sets the attribute list_order
 *
 *  \param this_ pointer to own object attributes
 *  \param list_order new list_order of this object
 */
static inline void data_feature_set_list_order ( data_feature_t *this_, int32_t list_order );

/*!
 *  \brief gets the universal unique identifier of this data_feature_t
 *
 *  \param this_ pointer to own object attributes
 *  \return uuid of this object
 */
static inline const char *data_feature_get_uuid_const ( const data_feature_t *this_ );

/*!
 *  \brief sets the attribute uuid
 *
 *  \param this_ pointer to own object attributes
 *  \param uuid new uuid of this object
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if new string too long
 */
static inline u8_error_t data_feature_set_uuid ( data_feature_t *this_, const char *uuid );

/*!
 *  \brief checks if attribute id is not DATA_ROW_VOID
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the id of the feature is not DATA_ROW_VOID
 */
static inline bool data_feature_is_valid ( const data_feature_t *this_ );

/*!
 *  \brief prints the data_feature_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_feature_trace ( const data_feature_t *this_ );

#include "entity/data_feature.inl"

#endif  /* DATA_FEATURE_H */


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
