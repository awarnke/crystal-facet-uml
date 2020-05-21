/* File: json_export_from_database.h; Copyright and License: see below */

#ifndef JSON_EXPORT_FROM_DATABASE_H
#define JSON_EXPORT_FROM_DATABASE_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Serializes and deserializes a set of objects to/from the clipboard
 */

#include "data_feature.h"
#include "storage/data_database_reader.h"
#include "set/data_stat.h"
#include "util/string/utf8stringbuf.h"

/*!
 *  \brief constants for maximum values of json_export_from_database_t
 */
enum json_export_from_database_max_enum {
    JSON_EXPORT_FROM_DATABASE_MAX_FEATURES = 64,  /*!< maximum number of features per classifier */
};

/*!
 *  \brief attributes of the json export object
 */
struct json_export_from_database_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external data_database_reader */

    data_feature_t temp_features[JSON_EXPORT_FROM_DATABASE_MAX_FEATURES];  /*!< temporary memory for feature list */
};

typedef struct json_export_from_database_struct json_export_from_database_t;

/*!
 *  \brief initializes the json_export_from_database_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader
 */
void json_export_from_database_init ( json_export_from_database_t *this_,
                                      data_database_reader_t *db_reader
                                    );

/*!
 *  \brief destroys the json_export_from_database_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void json_export_from_database_destroy ( json_export_from_database_t *this_ );

/*!
 *  \brief copies a set of objects to a string buffer, can be forwarded e.g. to the clipboard
 *
 *  \param this_ pointer to own object attributes
 *  \param set_to_be_copied ids of the objects to be exported
 *  \param io_stat undefined in case of an error in the return value,
 *                 otherwise statistics on DATA_STAT_SERIES_CREATED,
 *                 DATA_STAT_SERIES_IGNORED (e.g. at export of t.b.d.) and
 *                 DATA_STAT_SERIES_ERROR (e.g. if t.b.d.).
 *                 Statistics are only added, *io_stat shall be initialized by caller.
 *  \param out_buf string buffer to which to write the json formatted data to
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t json_export_from_database_export_set_to_buf( json_export_from_database_t *this_,
                                                          const data_small_set_t *set_to_be_copied,
                                                          data_stat_t *io_stat,
                                                          utf8stringbuf_t out_buf
                                                        );

#endif  /* JSON_EXPORT_FROM_DATABASE_H */


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
