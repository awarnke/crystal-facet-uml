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
 *  \brief constants for maximum values of json_importer_t
 */
enum json_importer_max_enum {
    JSON_IMPORT_TO_DATABASE_MAX_FEATURES = 64,  /*!< maximum number of features per classifier */
};

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

    data_feature_t temp_features[JSON_IMPORT_TO_DATABASE_MAX_FEATURES];  /*!< temporary memory for feature list */
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
 *  \brief prescans which object ids are to be imported and which already exist in the current db
 *
 *  \param this_ pointer to own object attributes
 *  \param in_stream input stream where to import the data from
 *  \param out_english_report non translated report on errors in input file and warnings from importing
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t json_importer_prescan( json_importer_t *this_,
                                  universal_input_stream_t *in_stream,
                                  universal_utf8_writer_t *out_english_report
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
