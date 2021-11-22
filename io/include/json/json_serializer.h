/* File: json_serializer.h; Copyright and License: see below */

#ifndef JSON_SERIALIZER_H
#define JSON_SERIALIZER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Serializes data objects to json format.
 */

#include "json/json_writer.h"
#include "data_classifier.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "data_diagram.h"
#include "data_diagramelement.h"
#include "stream/universal_output_stream.h"
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for serializing data objects
 */
struct json_serializer_struct {
    json_writer_t json_writer;  /*!< own instance of the json writer */

    bool in_outer_array;  /*!< true if begin_array() was called and end_array() is not yet called. */
    bool is_outer_first;  /*!< true if after begin_array(), no object was inserted yet. */
    bool in_inner_array;  /*!< true if an inner array was begun and not yet ended. */
    bool is_inner_first;  /*!< true if in the innner array, no object was inserted yet. */
};

typedef struct json_serializer_struct json_serializer_t;

/*!
 *  \brief initializes the json_serializer_t struct
 *
 *  \param output output stream where to write json utf8 bytes to
 *  \param this_ pointer to own object attributes
 */
void json_serializer_init ( json_serializer_t *this_, universal_output_stream_t * output );

/*!
 *  \brief destroys the json_serializer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void json_serializer_destroy ( json_serializer_t *this_ );

/*!
 *  \brief writes a header for the json stream
 *
 *  \param this_ pointer to own object attributes
 *  \return 0 in case of success, -1 if output buffer exceeded
 */
int json_serializer_write_header ( json_serializer_t *this_ );

/*!
 *  \brief writes a footer/ending for the json stream
 *
 *  \param this_ pointer to own object attributes
 *  \return 0 in case of success, -1 if output buffer exceeded
 */
int json_serializer_write_footer ( json_serializer_t *this_ );

/*!
 *  \brief begins an array of objects as named json member
 *
 *  \param this_ pointer to own object attributes
 *  \param section_name json encoded name of the section to start
 *  \return 0 in case of success, -1 if output buffer exceeded
 */
int json_serializer_begin_section ( json_serializer_t *this_, const char* section_name );

/*!
 *  \brief ends an array of objects
 *
 *  \param this_ pointer to own object attributes
 *  \return 0 in case of success, -1 if output buffer exceeded
 */
int json_serializer_end_section ( json_serializer_t *this_ );

/*!
 *  \brief appends a classifier to the output
 *
 *  \param this_ pointer to own object attributes
 *  \param object a pointer to the classifier to serialize
 *  \return 0 in case of success, -1 if output buffer exceeded
 */
int json_serializer_begin_classifier ( json_serializer_t *this_, const data_classifier_t *object );

/*!
 *  \brief appends a classifier to the output
 *
 *  \param this_ pointer to own object attributes
 *  \param object a pointer to the classifier to serialize
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_STRING_BUFFER_EXCEEDED if output buffer exceeded,
 *          DATA_ERROR_INVALID_REQUEST if json_serializer_begin_array() was not yet called.
 */
int json_serializer_end_classifier ( json_serializer_t *this_, const data_classifier_t *object );

/*!
 *  \brief appends a feature to the output, intended for use within a classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param object a pointer to the feature to serialize
 *  \return 0 in case of success, -1 if output buffer exceeded
 */
int json_serializer_append_feature ( json_serializer_t *this_, const data_feature_t *object );

/*!
 *  \brief appends a relationship to the output
 *
 *  \param this_ pointer to own object attributes
 *  \param object a pointer to the relationship to serialize
 *  \param from_clas a pointer to the source classifier of the relationship to serialize
 *  \param from_feat a pointer to the source feature of the relationship to serialize, pointer to DATA_ROW_ID_VOID if not existant
 *  \param to_clas a pointer to the destination classifier of the relationship to serialize
 *  \param to_feat a pointer to the destination feature of the relationship to serialize, pointer to DATA_ROW_ID_VOID if not existant
 *  \return 0 in case of success, -1 if output buffer exceeded
 */
int json_serializer_append_relationship ( json_serializer_t *this_,
                                          const data_relationship_t *object,
                                          const data_classifier_t *from_clas,
                                          const data_feature_t *from_feat,
                                          const data_classifier_t *to_clas,
                                          const data_feature_t *to_feat
                                        );

/*!
 *  \brief appends a diagram to the output
 *
 *  \param this_ pointer to own object attributes
 *  \param object a pointer to the diagram to serialize
 *  \return 0 in case of success, -1 if output buffer exceeded
 */
int json_serializer_append_diagram ( json_serializer_t *this_, const data_diagram_t *object );

/*!
 *  \brief appends a diagramelement to the output
 *
 *  \param this_ pointer to own object attributes
 *  \param object a pointer to the diagramelement to serialize
 *  \return 0 in case of success, -1 if output buffer exceeded
 */
int json_serializer_append_diagramelement ( json_serializer_t *this_, const data_diagramelement_t *object );

#endif  /* JSON_SERIALIZER_H */


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
