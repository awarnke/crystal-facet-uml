/* File: json_importer.h; Copyright and License: see below */

#ifndef JSON_IMPORTER_H
#define JSON_IMPORTER_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Deserializes a set of objects from the clipboard
 */

#include "json/json_element_reader.h"
#include "io_import_elements.h"
#include "data_rules.h"
#include "set/data_stat.h"
#include "stream/universal_input_stream.h"
#include "universal_utf8_writer.h"
#include "util/string/utf8stringbuf.h"

/*!
 *  \brief attributes of the json import object
 *
 *  Lifecycle: A json importer shall perform a single import operation only.
 *  It may be initialized before one import operation and be destroyed afterwards.
 */
struct json_importer_struct {
    data_rules_t data_rules;  /*!< own instance of uml and sysml consistency rules */

    json_element_reader_t temp_element_reader;  /*!< own instance of a json element deserializer */
    io_import_elements_t *elements_importer;  /*!< pointer to external db-element sync to database */

    data_stat_t *stat;  /*!< pointer to import statistics */

    data_diagram_t temp_diagram;  /*!< memory buffer to store a diagram temporarily when reading a json object */
    data_diagramelement_t temp_diagramelement;  /*!< memory buffer to store a diagramelement temporarily when reading a json object */
    data_classifier_t temp_classifier;  /*!< memory buffer to store a classifier temporarily when reading a json object */
    data_feature_t temp_feature;  /*!< memory buffer to store a feature temporarily when reading a json object */
    data_relationship_t temp_relationship;  /*!< memory buffer to store a relationship temporarily when reading a json object */
};

typedef struct json_importer_struct json_importer_t;

/*!
 *  \brief initializes the json_importer_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param elements_importer pointer to an object that synchronizes the json-object with the database
 *  \param io_stat undefined in case of an error in the return value,
 *                 otherwise statistics on DATA_STAT_SERIES_CREATED,
 *                 DATA_STAT_SERIES_MODIFIED (e.g. in future) and
 *                 DATA_STAT_SERIES_IGNORED (e.g. at import of lifelines
 *                 or if classifier and its features already exist) and
 *                 DATA_STAT_SERIES_ERROR (e.g. if a relation has no source
 *                 or no destination).
 *                 Statistics are only added, *io_stat shall be initialized by caller.
 */
void json_importer_init( json_importer_t *this_,
                         io_import_elements_t *elements_importer,
                         data_stat_t *io_stat
                       );

/*!
 *  \brief destroys the json_importer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void json_importer_destroy( json_importer_t *this_ );

/*!
 *  \brief copies the clipboard contents to the focused diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param json_text stream in json format, not NULL
 *  \param out_read_line read position in the stream, in case of an error, this may help finding the cause
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_DB_STRUCTURE if diagram_id does not exist,
 *          U8_ERROR_PARSER_STRUCTURE if unexpected order of tokens, other error code otherwise
 */
u8_error_t json_importer_import_stream( json_importer_t *this_,
                                        universal_input_stream_t *json_text,
                                        uint32_t *out_read_line
                                      );

/*!
 *  \brief imports views to the focused diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_DB_STRUCTURE if diagram_id does not exist, other error code otherwise
 */
u8_error_t json_importer_private_import_views( json_importer_t *this_ );

/*!
 *  \brief imports nodes to the focused diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_DB_STRUCTURE if diagram_id does not exist, other error code otherwise
 */
u8_error_t json_importer_private_import_nodes( json_importer_t *this_ );

/*!
 *  \brief imports edges to the focused diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_DB_STRUCTURE if diagram_id does not exist, other error code otherwise
 */
u8_error_t json_importer_private_import_edges( json_importer_t *this_ );

/*!
 *  \brief parses the next diagramelement array
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_uuid uuid of surrounding diagram, empty string if none available, never NULL
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if strings do not fit into the out_object,
 *          U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_importer_private_import_diagramelement_array ( json_importer_t *this_, const char *diagram_uuid );


/*!
 *  \brief parses the next feature array
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_uuid uuid of surrounding classifier, empty string if none available, never NULL
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if strings do not fit into the out_object,
 *          U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_importer_private_import_feature_array ( json_importer_t *this_, const char *classifier_uuid );


#endif  /* JSON_IMPORTER_H */


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