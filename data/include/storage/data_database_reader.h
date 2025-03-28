/* File: data_database_reader.h; Copyright and License: see below */

#ifndef DATA_DATABASE_READER_H
#define DATA_DATABASE_READER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Searches and reads records from the database
 */

#include "storage/data_database_listener.h"
#include "storage/data_database_listener_signal.h"
#include "storage/data_classifier_iterator.h"
#include "storage/data_feature_iterator.h"
#include "storage/data_relationship_iterator.h"
#include "storage/data_diagram_iterator.h"
#include "storage/data_diagramelement_iterator.h"
#include "storage/data_visible_classifier_iterator.h"
#include "storage/data_database.h"
#include "storage/data_database_classifier_reader.h"
#include "storage/data_database_diagram_reader.h"
#include "entity/data_diagram.h"
#include "u8/u8_error.h"
#include "entity/data_classifier.h"
#include "set/data_visible_classifier.h"
#include "set/data_small_set.h"
#include "entity/data_feature.h"
#include "entity/data_relationship.h"
#include "entity/data_row.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for the database functions
 */
struct data_database_reader_struct {
    data_database_t *database;  /*!< pointer to external database */
    data_database_classifier_reader_t temp_classifier_reader;  /*!< own instance of a classifier reader, initialized while open */
    data_database_diagram_reader_t temp_diagram_reader;  /*!< own instance of a diagram reader, initialized while open */

    bool is_open;  /*!< the temp_classifier_reader and temp_diagram_reader with their prepared statements */
                   /*!< are only initialized if the database is open */

    data_database_listener_t me_as_listener;  /*!< own instance of data_database_listener_t */
                                              /*!< which wraps data_database_reader_db_change_callback */
};

typedef struct data_database_reader_struct data_database_reader_t;

/*!
 *  \brief initializes the data_database_reader_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param database database which this reader uses
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_reader_init ( data_database_reader_t *this_, data_database_t *database );

/*!
 *  \brief destroys the data_database_reader_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_reader_destroy ( data_database_reader_t *this_ );

/*!
 *  \brief prepares a database change and re-initializes afterwards
 *
 *  \param this_ pointer to own object attributes
 *  \param signal_id state of database change
 */
void data_database_reader_db_change_callback ( data_database_reader_t *this_, data_database_listener_signal_t signal_id );

/*!
 *  \brief checks if the database reader is open
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the database reader is open
 */
static inline bool data_database_reader_is_open( data_database_reader_t *this_ );

/* ================================ DIAGRAM ================================ */

/*!
 *  \brief reads a diagram from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the diagram to be read from the database
 *  \param[out] out_diagram the diagram read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_DB_STRUCTURE if id does not exist or U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_diagram_by_id ( data_database_reader_t *this_,
                                                                  data_row_t id,
                                                                  data_diagram_t *out_diagram
                                                                );

/*!
 *  \brief reads a diagram from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param uuid the diagram to be read from the database
 *  \param[out] out_diagram the diagram read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_DB_STRUCTURE if uuid does not exist or U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_diagram_by_uuid ( data_database_reader_t *this_,
                                                                    const char *uuid,
                                                                    data_diagram_t *out_diagram
                                                                  );

/*!
 *  \brief reads all child-diagrams from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_id id of the parent diagram, DATA_ROW_VOID to get all root diagrams
 *  \param[in,out] io_diagram_iterator iterator over diagrams of selected parent diagram. The caller is responsible
 *                                     for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_diagrams_by_parent_id ( data_database_reader_t *this_,
                                                                          data_row_t parent_id,
                                                                          data_diagram_iterator_t *io_diagram_iterator
                                                                        );

/*!
 *  \brief reads all classifier-displaying diagrams from the database
 *
 *  If a diagram shows the classifier multiple times, the diagram is returned just once (DISTINCT).
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier
 *  \param[in,out] io_diagram_iterator iterator over diagrams of selected classifier. The caller is responsible
 *                                     for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_diagrams_by_classifier_id ( data_database_reader_t *this_,
                                                                              data_row_t classifier_id,
                                                                              data_diagram_iterator_t *io_diagram_iterator
                                                                            );

/*!
 *  \brief reads all child-diagram ids from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_id id of the parent diagram, DATA_ROW_VOID to get all root diagram ids
 *  \param[out] out_diagram_ids set of diagram ids read from the database (in case of success). The provided set shall be initialized.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_diagram_ids_by_parent_id ( data_database_reader_t *this_,
                                                                             data_row_t parent_id,
                                                                             data_small_set_t *out_diagram_ids
                                                                           );

/*!
 *  \brief reads all classifier-displaying diagrams ids from the database
 *
 *  If a diagram shows the classifier multiple times, the diagram is returned just once (DISTINCT).
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier
 *  \param[out] out_diagram_ids set of diagram ids read from the database (in case of success). The provided set shall be initialized.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_diagram_ids_by_classifier_id ( data_database_reader_t *this_,
                                                                                 data_row_t classifier_id,
                                                                                 data_small_set_t *out_diagram_ids
                                                                               );

/* ================================ CLASSIFIER ================================ */

/*!
 *  \brief reads a classifier from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the classifier to be read from the database
 *  \param[out] out_classifier the classifier read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_DB_STRUCTURE if id does not exist or U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_classifier_by_id ( data_database_reader_t *this_,
                                                                     data_row_t id,
                                                                     data_classifier_t *out_classifier
                                                                   );

/*!
 *  \brief reads a classifier from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param name the classifier to be read from the database
 *  \param[out] out_classifier the classifier read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NOT_FOUND if name does not exist or U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_classifier_by_name ( data_database_reader_t *this_,
                                                                       const char *name,
                                                                       data_classifier_t *out_classifier
                                                                     );

/*!
 *  \brief reads a classifier from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param uuid the classifier to be read from the database
 *  \param[out] out_classifier the classifier read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_DB_STRUCTURE if uuid does not exist or U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_classifier_by_uuid ( data_database_reader_t *this_,
                                                                       const char *uuid,
                                                                       data_classifier_t *out_classifier
                                                                     );

/*!
 *  \brief iterates over all classifiers from the database.
 *
 *  classifiers are sorted by number of containment-parents, ascending.
 *
 *  \param this_ pointer to own object attributes
 *  \param hierarchical true if the iterator shall start with classifiers without parent
 *  \param[in,out] io_classifier_iterator iterator over all classifiers. The caller is responsible
 *                                        for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_all_classifiers ( data_database_reader_t *this_,
                                                                    bool hierarchical,
                                                                    data_classifier_iterator_t *io_classifier_iterator
                                                                  );

/* ================================ VISIBLE_CLASSIFIER ================================ */

/*!
 *  \brief reads all visible classifiers of a diagram from the database.
 *
 *  If a classifier is contained multiple times in a diagram, it is returned multiple times in the io_visible_classifier_iterator.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the containing diagram
 *  \param[in,out] io_visible_classifier_iterator iterator over visible classifiers of selected diagram. The caller is responsible
 *                                                for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_visible_classifiers_by_diagram_id ( data_database_reader_t *this_,
                                                                                      data_row_t diagram_id,
                                                                                      data_visible_classifier_iterator_t *io_visible_classifier_iterator
                                                                                    );

/* ================================ DIAGRAMELEMENT ================================ */

/*!
 *  \brief reads a diagramelement from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the diagramelement to be read from the database
 *  \param[out] out_diagramelement the diagramelement read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_DB_STRUCTURE if id does not exist or U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_diagramelement_by_id ( data_database_reader_t *this_,
                                                                         data_row_t id,
                                                                         data_diagramelement_t *out_diagramelement
                                                                       );

/*!
 *  \brief reads a diagramelement from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param uuid the diagramelement to be read from the database
 *  \param[out] out_diagramelement the diagramelement read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_DB_STRUCTURE if uuid does not exist or U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_diagramelement_by_uuid ( data_database_reader_t *this_,
                                                                           const char *uuid,
                                                                           data_diagramelement_t *out_diagramelement
                                                                         );

/*!
 *  \brief reads all diagramelements of a diagram from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram
 *  \param[in,out] io_diagramelement_iterator iterator over diagramelements of selected diagram. The caller is responsible
 *                                            for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_diagramelements_by_diagram_id ( data_database_reader_t *this_,
                                                                                  data_row_t diagram_id,
                                                                                  data_diagramelement_iterator_t *io_diagramelement_iterator
                                                                                );

/*!
 *  \brief reads all diagramelements of a classifier from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the diagram
 *  \param[in,out] io_diagramelement_iterator iterator over diagramelements of selected classifier. The caller is responsible
 *                                            for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_diagramelements_by_classifier_id ( data_database_reader_t *this_,
                                                                                     data_row_t classifier_id,
                                                                                     data_diagramelement_iterator_t *io_diagramelement_iterator
                                                                                   );

/* ================================ FEATURE ================================ */

/*!
 *  \brief reads a feature from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the feature to be read from the database
 *  \param[out] out_feature the feature read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_DB_STRUCTURE if id does not exist or U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_feature_by_id ( data_database_reader_t *this_,
                                                                  data_row_t id,
                                                                  data_feature_t *out_feature
                                                                );

/*!
 *  \brief reads a feature from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param uuid the feature to be read from the database
 *  \param[out] out_feature the feature read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_DB_STRUCTURE if uuid does not exist or U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_feature_by_uuid ( data_database_reader_t *this_,
                                                                    const char *uuid,
                                                                    data_feature_t *out_feature
                                                                  );

/*!
 *  \brief reads all features of a classifier from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the parent classifier
 *  \param[in,out] io_feature_iterator iterator over features of selected classifier. The caller is responsible
 *                                     for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_features_by_classifier_id ( data_database_reader_t *this_,
                                                                              data_row_t classifier_id,
                                                                              data_feature_iterator_t *io_feature_iterator
                                                                            );

/*!
 *  \brief reads all features of a diagram from the database
 *
 *  If a classifier is contained multiple times in a diagram, the classifiers associated features are returned only once nonetheless (DISTINCT).
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the containing diagram
 *  \param[in,out] io_feature_iterator iterator over features of selected diagrams. The caller is responsible
 *                                     for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_features_by_diagram_id ( data_database_reader_t *this_,
                                                                           data_row_t diagram_id,
                                                                           data_feature_iterator_t *io_feature_iterator
                                                                         );

/* ================================ RELATIONSHIP ================================ */

/*!
 *  \brief reads a relationship from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the relationship to be read from the database
 *  \param[out] out_relationship the relationship read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_DB_STRUCTURE if id does not exist or U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_relationship_by_id ( data_database_reader_t *this_,
                                                                       data_row_t id,
                                                                       data_relationship_t *out_relationship
                                                                     );

/*!
 *  \brief reads a relationship from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param uuid the relationship to be read from the database
 *  \param[out] out_relationship the relationship read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_DB_STRUCTURE if uuid does not exist or U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_relationship_by_uuid ( data_database_reader_t *this_,
                                                                         const char *uuid,
                                                                         data_relationship_t *out_relationship
                                                                       );

/*!
 *  \brief reads all relationships of a classifier from the database
 *
 *  This includes relationships where the classifier is source-only, destination-only or both.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the source(from) or destination(to) classifier
 *  \param[in,out] io_relationship_iterator iterator over relationships of selected classifier. The caller is responsible
 *                                          for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_relationships_by_classifier_id ( data_database_reader_t *this_,
                                                                                   data_row_t classifier_id,
                                                                                   data_relationship_iterator_t *io_relationship_iterator
                                                                                 );

/*!
 *  \brief reads all relationships of a feature from the database
 *
 *  This includes relationships where the feature is source-only, destination-only or both.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_id id of the source(from) or destination(to) feature; must not be DATA_ROW_VOID.
 *  \param[in,out] io_relationship_iterator iterator over relationships of selected feature. The caller is responsible
 *                                          for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_relationships_by_feature_id ( data_database_reader_t *this_,
                                                                                data_row_t feature_id,
                                                                                data_relationship_iterator_t *io_relationship_iterator
                                                                              );

/*!
 *  \brief reads all relationships of a diagram from the database
 *
 *  If classifiers are contained multiple times in a diagram, each relationship is returned only once nonetheless (DISTINCT).
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the containing diagram
 *  \param[in,out] io_relationship_iterator iterator over relationships of selected diagram. The caller is responsible
 *                                          for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_reader_get_relationships_by_diagram_id ( data_database_reader_t *this_,
                                                                                data_row_t diagram_id,
                                                                                data_relationship_iterator_t *io_relationship_iterator
                                                                              );

/* ================================ private ================================ */

/*!
 *  \brief initializes the data_database_reader_t struct and allows access to the database after the database is opened
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_reader_private_open ( data_database_reader_t *this_ );

/*!
 *  \brief closes the data_database_reader_t struct (prohibits access to the database) before the database is closed
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_reader_private_close ( data_database_reader_t *this_ );

#include "storage/data_database_reader.inl"

#endif  /* DATA_DATABASE_READER_H */


/*
Copyright 2016-2025 Andreas Warnke

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
