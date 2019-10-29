/* File: json_serializer.h; Copyright and License: see below */

#ifndef JSON_SERIALIZER_H
#define JSON_SERIALIZER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Serializes data objects to json format.
 */

#include "data_error.h"
#include "data_classifier.h"
#include "data_diagram.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "util/string/utf8stringbuf.h"
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for serializing data objects
 */
struct json_serializer_struct {
    bool in_array;  /*!< true if begin_array() was called and end_array() is not yet called. */
    bool is_first;  /*!< true if after begin_array(), no object was inserted yet. */
};

typedef struct json_serializer_struct json_serializer_t;

/*!
 *  \brief initializes the json_serializer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void json_serializer_init ( json_serializer_t *this_ );

/*!
 *  \brief destroys the json_serializer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void json_serializer_destroy ( json_serializer_t *this_ );

/*!
 *  \brief begins a json object containing an array of objects
 *
 *  \param this_ pointer to own object attributes
 *  \param out stringbuffer where to write the result to
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_STRING_BUFFER_EXCEEDED if stringbuffer exceeded,
 *          DATA_ERROR_INVALID_REQUEST if json_serializer_begin_array() was already called.
 */
data_error_t json_serializer_begin_set ( json_serializer_t *this_, utf8stringbuf_t out );

/*!
 *  \brief ends a json object containing an array of objects
 *
 *  \param this_ pointer to own object attributes
 *  \param out stringbuffer where to write the result to
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_STRING_BUFFER_EXCEEDED if stringbuffer exceeded,
 *          DATA_ERROR_INVALID_REQUEST if json_serializer_end_array() was already called.
 */
data_error_t json_serializer_end_set ( json_serializer_t *this_, utf8stringbuf_t out );

/*!
 *  \brief appends a classifier to the stringbuffer
 *
 *  \param this_ pointer to own object attributes
 *  \param object a pointer to the classifier to serialize
 *  \param features array of features of the classifier
 *  \param feature_count number of feature records stored in features
 *  \param out stringbuffer where to write the result to
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_STRING_BUFFER_EXCEEDED if stringbuffer exceeded,
 *          DATA_ERROR_INVALID_REQUEST if json_serializer_begin_array() was not yet called.
 */
data_error_t json_serializer_append_classifier ( json_serializer_t *this_,
                                                 data_classifier_t *object,
                                                 data_feature_t (*features)[],
                                                 uint32_t feature_count,
                                                 utf8stringbuf_t out
                                               );

/*!
 *  \brief appends a diagram to the stringbuffer
 *
 *  \param this_ pointer to own object attributes
 *  \param object a pointer to the diagram to serialize
 *  \param out stringbuffer where to write the result to
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_STRING_BUFFER_EXCEEDED if stringbuffer exceeded,
 *          DATA_ERROR_INVALID_REQUEST if json_serializer_begin_array() was not yet called.
 */
data_error_t json_serializer_append_diagram ( json_serializer_t *this_, data_diagram_t *object, utf8stringbuf_t out );

/*!
 *  \brief appends a relationship to the stringbuffer
 *
 *  \param this_ pointer to own object attributes
 *  \param object a pointer to the relationship to serialize
 *  \param from_clas a pointer to the source classifier of the relationship to serialize
 *  \param from_feat a pointer to the source feature of the relationship to serialize, pointer to DATA_ID_VOID_ID if not existant
 *  \param to_clas a pointer to the destination classifier of the relationship to serialize
 *  \param to_feat a pointer to the destination feature of the relationship to serialize, pointer to DATA_ID_VOID_ID if not existant
 *  \param out stringbuffer where to write the result to
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_STRING_BUFFER_EXCEEDED if stringbuffer exceeded,
 *          DATA_ERROR_INVALID_REQUEST if json_serializer_begin_array() was not yet called.
 */
data_error_t json_serializer_append_relationship ( json_serializer_t *this_,
                                                   data_relationship_t *object,
                                                   data_classifier_t *from_clas,
                                                   data_feature_t *from_feat,
                                                   data_classifier_t *to_clas,
                                                   data_feature_t *to_feat,
                                                   utf8stringbuf_t out
                                                 );

/*!
 *  \brief appends a feature to the stringbuffer, intended for use within a classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param object a pointer to the feature to serialize
 *  \param out stringbuffer where to write the result to
 *  \return UTF8ERROR_SUCCESS in case of success,
 *          negative value if stringbuffer exceeded
 */
utf8error_t json_serializer_private_append_feature ( json_serializer_t *this_, data_feature_t *object, utf8stringbuf_t out );

#endif  /* JSON_SERIALIZER_H */


/*
Copyright 2016-2019 Andreas Warnke

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
