/* File: data_profile_part.h; Copyright and License: see below */

#ifndef DATA_PROFILE_PART_H
#define DATA_PROFILE_PART_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Loads the stereotypes needed by the pencil_diagram_maker to render a data_visible_set_t from the database
 */

#include "entity/data_diagram.h"
#include "set/data_visible_set.h"
#include "storage/data_database_reader.h"
#include "entity/data_relationship.h"
#include "entity/data_feature.h"
#include "entity/data_id.h"
#include "entity/data_row.h"
#include "utf8stringbuf/utf8stringview.h"
#include <cairo.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief constants for maximum values of data_profile_part_t
 */
enum data_profile_part_max_enum {
    DATA_PROFILE_PART_MAX_STEREOTYPES = 48,  /*!< maximum number of stereotypes to be used in one single diagram. */
                                             /*!< Note that a single profile diagram may contain all custom stereotypes, */
                                             /*!< therefore 48 is a size that may be suitable. */
};

/*!
 *  \brief attributes of the data_profile_part_t
 */
struct data_profile_part_struct {
    uint32_t stereotype_count;  /*!< number of all contained visible classifier records */
    data_classifier_t stereotypes[DATA_PROFILE_PART_MAX_STEREOTYPES];  /*!< all contained stereotype records */
};

typedef struct data_profile_part_struct data_profile_part_t;

/*!
 *  \brief initializes the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
void data_profile_part_init ( data_profile_part_t *this_ );

/*!
 *  \brief re-initializes the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
void data_profile_part_reinit ( data_profile_part_t *this_ );

/*!
 *  \brief destroys the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
void data_profile_part_destroy ( data_profile_part_t *this_ );

/*!
 *  \brief fetches the painter input data from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_elements set of elements in the diagram for which the stereotypes shall be loaded
 *  \param db_reader database reader to use when acquiring data
 *  \return U8_ERROR_NONE in case of success, in case of error 
 *          e.g. U8_ERROR_DB_STRUCTURE if id does not exist or U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_profile_part_load ( data_profile_part_t *this_,
                                    const data_visible_set_t * diagram_elements,
                                    data_database_reader_t *db_reader
                                  );

/*!
 *  \brief gets the number of stereotype classifiers within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param stereotype_name name of the stereotype to load
 *  \param db_reader database reader to use when acquiring data
 *  \return bitmask of unhandled errors
 */
static inline u8_error_t data_profile_part_private_load_stereotype ( data_profile_part_t *this_,
                                                                     const char *stereotype_name,
                                                                     data_database_reader_t *db_reader
                                                                   );

/*!
 *  \brief gets the number of stereotype classifiers within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \return number of loaded stereotype classifiers
 */
static inline uint32_t data_profile_part_get_stereotype_count ( const data_profile_part_t *this_ );

/*!
 *  \brief gets a stereotype classifier within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the stereotype classifier to retrieve; 0 &lt;= index &lt; data_profile_part_get_stereotype_count().
 *  \return NULL if index &gt;= data_profile_part_get_stereotype_count(); pointer to data_classifier_t otherwise.
 */
static inline const data_classifier_t *data_profile_part_get_stereotype_const ( const data_profile_part_t *this_,
                                                                                uint32_t index
                                                                              );

/*!
 *  \brief gets a stereotype classifier within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param stereotype_name name of the stereotype for which to retrieve the classifier record
 *  \return NULL if id not existant; pointer to data_classifier_t otherwise.
 */
static inline const data_classifier_t *data_profile_part_get_stereotype_by_name_const ( const data_profile_part_t *this_,
                                                                                        const utf8stringview_t *stereotype_name
                                                                                      );

/*!
 *  \brief traces the stereotype names
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_profile_part_trace ( const data_profile_part_t *this_ );

#include "data_profile_part.inl"

#endif  /* DATA_PROFILE_PART_H */


/*
Copyright 2023-2026 Andreas Warnke

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
