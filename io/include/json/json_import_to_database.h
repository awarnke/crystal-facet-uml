/* File: json_import_to_database.h; Copyright and License: see below */

#ifndef JSON_IMPORT_TO_DATABASE_H
#define JSON_IMPORT_TO_DATABASE_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Serializes and deserializes a set of objects to/from the clipboard
 */

#include "ctrl_controller.h"
#include "data_rules.h"
#include "set/data_visible_set.h"
#include "set/data_stat.h"
#include "storage/data_database_reader.h"
#include "util/string/utf8stringbuf.h"

/*!
 *  \brief constants for maximum values of json_import_to_database_t
 */
enum json_import_to_database_max_enum {
    JSON_IMPORT_TO_DATABASE_MAX_FEATURES = 64,  /*!< maximum number of features per classifier */
    JSON_IMPORT_TO_DATABASE_MAX_DIAGELES = DATA_VISIBLE_SET_MAX_CLASSIFIERS,  /*!< maximum number of diagramelements per diagram */
};

/*!
 *  \brief attributes of the json import object
 */
struct json_import_to_database_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external data_database_reader */
    ctrl_controller_t *controller;  /*!< pointer to external controller */
    data_rules_t data_rules;  /*!< own instance of uml and sysml consistency rules */

    data_feature_t temp_features[JSON_IMPORT_TO_DATABASE_MAX_FEATURES];  /*!< temporary memory for feature list */
    data_diagramelement_t temp_diageles[JSON_IMPORT_TO_DATABASE_MAX_DIAGELES];  /*!< temporary memory for diagramelement list */
};

typedef struct json_import_to_database_struct json_import_to_database_t;

/*!
 *  \brief initializes the json_import_to_database_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader
 *  \param controller pointer to a controller object which can modify the database
 */
void json_import_to_database_init ( json_import_to_database_t *this_,
                                    data_database_reader_t *db_reader,
                                    ctrl_controller_t *controller
                                  );

/*!
 *  \brief destroys the json_import_to_database_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void json_import_to_database_destroy ( json_import_to_database_t *this_ );

/*!
 *  \brief copies the clipboard contents to the focused diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param json_text null-terminated string in json format, not NULL
 *  \param diagram_id id of the diagram to which to attach the imported data
 *  \param io_stat undefined in case of an error in the return value,
 *                 otherwise statistics on DATA_STAT_SERIES_CREATED,
 *                 DATA_STAT_SERIES_MODIFIED (e.g. in future) and
 *                 DATA_STAT_SERIES_IGNORED (e.g. at import of lifelines
 *                 or if classifier and its features already exist) and
 *                 DATA_STAT_SERIES_ERROR (e.g. if a relation has no source
 *                 or no destination)
 *                 Statistics are only added, *io_stat shall be initialized by caller.
 *  \param out_read_pos read position in the stream, in case of an error, this may help finding the cause
 *  \return DATA_ERROR_NONE in case of success, DATA_ERROR_DB_STRUCTURE if diagram_id does not exist, other error code otherwise
 */
data_error_t json_import_to_database_import_buf_to_db( json_import_to_database_t *this_,
                                                       const char *json_text,
                                                       data_row_id_t diagram_id,
                                                       data_stat_t *io_stat,
                                                       uint32_t *out_read_pos
                                                     );

/*!
 *  \brief checks if a given lifeline (feature) is visible (focused) in the current diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to which to attach the imported data
 *  \param feature_id id of the feature that shall be the focused_feature of the diagramelement (for a lifeline, this means being visible)
 *  \return true if a matching diagramelement is found
 */
bool json_import_to_database_private_is_feature_focused_in_diagram( json_import_to_database_t *this_, data_row_id_t diagram_id, data_row_id_t feature_id );

#endif  /* JSON_IMPORT_TO_DATABASE_H */


/*
Copyright 2016-2020 Andreas Warnke

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
