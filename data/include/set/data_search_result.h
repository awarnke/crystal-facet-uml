/* File: data_search_result.h; Copyright and License: see below */

#ifndef DATA_SEARCH_RESULT_H
#define DATA_SEARCH_RESULT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines ids of a diagram, classifier, feature and relationship needed to describe a search result
 */

#include "entity/data_id.h"
#include "entity/data_row.h"
#include "entity/data_type.h"

/*!
 *  \brief constants for max string sizes
 */
enum data_search_result_max_enum {
    DATA_SEARCH_RESULT_MAX_NAME_SIZE = 48,
    DATA_SEARCH_RESULT_MAX_NAME_LENGTH = 47,
};

/*!
 *  \brief the ids needed to describe a search result
 */
struct data_search_result_struct {
    data_id_t match_object_id;  /*!< the object id of the found object */
    data_type_t match_object_type;  /*!< the type of the found object */
    utf8stringbuf_t match_object_name;  /*!< name of the found object */
    char private_match_name_buffer[DATA_SEARCH_RESULT_MAX_NAME_SIZE];
    data_id_t src_classifier_id;  /*!< in case of feature or relationship, the source classifier id */
    data_id_t dst_classifier_id;  /*!< in case of relationship, the destination classifier id */
    data_id_t diagram_id;  /*!< the diagram id */
};

typedef struct data_search_result_struct data_search_result_t;

/*!
 *  \brief initializes the data_search_result_t struct to reference a diagram.
 *
 *  \param this_ pointer to own object attributes
 *  \param match_id matching diagram id
 *  \param match_type matching objects type, data_diagram_type_t
 *  \param match_name matching object name, must not be NULL
 */
static inline void data_search_result_init_diagram ( data_search_result_t *this_,
                                                     data_row_t match_id,
                                                     data_diagram_type_t match_type,
                                                     const char* match_name
                                                   );

/*!
 *  \brief initializes the data_search_result_t struct to reference a classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param match_id matching classifier id
 *  \param match_type matching objects type, data_classifier_type_t 
 *  \param match_name matching object name, must not be NULL
 *  \param diagram_id diagram id where the matching object is visible
 */
static inline void data_search_result_init_classifier ( data_search_result_t *this_,
                                                        data_row_t match_id,
                                                        data_classifier_type_t match_type,
                                                        const char* match_name,
                                                        data_row_t diagram_id
                                                      );

/*!
 *  \brief initializes the data_search_result_t struct to reference a feature.
 *
 *  \param this_ pointer to own object attributes
 *  \param match_id matching feature id
 *  \param match_type matching objects type, data_feature_type_t
 *  \param match_name matching object name, must not be NULL
 *  \param classifier_id classifier of matching feature
 *  \param diagram_id diagram id where the matching feature is visible
 */
static inline void data_search_result_init_feature ( data_search_result_t *this_,
                                                     data_row_t match_id,
                                                     data_feature_type_t match_type,
                                                     const char* match_name,
                                                     data_row_t classifier_id,
                                                     data_row_t diagram_id
                                                   );

/*!
 *  \brief initializes the data_search_result_t struct to reference a relationship.
 *
 *  \param this_ pointer to own object attributes
 *  \param match_id matching relationship id
 *  \param match_type matching objects type, data_relationship_type_t
 *  \param match_name matching object name, must not be NULL
 *  \param from_classifier_id from classifier of matching relationship
 *  \param to_classifier_id to classifier of matching relationship
 *  \param diagram_id diagram id where the matching relationship is visible
 */
static inline void data_search_result_init_relationship ( data_search_result_t *this_,
                                                          data_row_t match_id,
                                                          data_relationship_type_t match_type,
                                                          const char* match_name,
                                                          data_row_t from_classifier_id,
                                                          data_row_t to_classifier_id,
                                                          data_row_t diagram_id
                                                        );

/*!
 *  \brief initializes the data_search_result_t struct with a copy
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes to copy
 */
static inline void data_search_result_copy ( data_search_result_t *this_, const data_search_result_t *original );

/*!
 *  \brief destroys the data_search_result_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_search_result_destroy ( data_search_result_t *this_ );

/*!
 *  \brief gets the match object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the match object
 */
static inline data_id_t data_search_result_get_match_id ( const data_search_result_t *this_ );

/*!
 *  \brief gets a pointer on the match object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the match object
 */
static inline data_id_t *data_search_result_get_match_id_ptr ( data_search_result_t *this_ );

/*!
 *  \brief gets a pointer on the match object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the match object
 */
static inline const data_id_t *data_search_result_get_match_id_const ( const data_search_result_t *this_ );

/*!
 *  \brief gets the match object type
 *
 *  \param this_ pointer to own object attributes
 *  \return the type of the match object
 */
static inline data_type_t data_search_result_get_match_type ( const data_search_result_t *this_ );

/*!
 *  \brief gets the match object name
 *
 *  \param this_ pointer to own object attributes
 *  \return the name of the match object, possibly empty but not NULL
 */
static inline const char* data_search_result_get_match_name_const ( const data_search_result_t *this_ );

/*!
 *  \brief gets the src classifier id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the src classifier object
 */
static inline data_id_t data_search_result_get_src_classifier_id ( const data_search_result_t *this_ );

/*!
 *  \brief gets a pointer on the src classifier object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the src classifier object
 */
static inline data_id_t *data_search_result_get_src_classifier_id_ptr ( data_search_result_t *this_ );

/*!
 *  \brief gets a pointer on the src classifier object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the src classifier object
 */
static inline const data_id_t *data_search_result_get_src_classifier_id_const ( const data_search_result_t *this_ );

/*!
 *  \brief gets the dst classifier id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the dst classifier object
 */
static inline data_id_t data_search_result_get_dst_classifier_id ( const data_search_result_t *this_ );

/*!
 *  \brief gets a pointer on the dst classifier object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the dst classifier object
 */
static inline data_id_t *data_search_result_get_dst_classifier_id_ptr ( data_search_result_t *this_ );

/*!
 *  \brief gets a pointer on the dst classifier object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the dst classifier object
 */
static inline const data_id_t *data_search_result_get_dst_classifier_id_const ( const data_search_result_t *this_ );

/*!
 *  \brief gets the diagram id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the diagram
 */
static inline data_id_t data_search_result_get_diagram_id ( const data_search_result_t *this_ );

/*!
 *  \brief gets a pointer on the diagram id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the diagram
 */
static inline data_id_t *data_search_result_get_diagram_id_ptr ( data_search_result_t *this_ );

/*!
 *  \brief gets a pointer on the diagram id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the diagram
 */
static inline const data_id_t *data_search_result_get_diagram_id_const ( const data_search_result_t *this_ );

/*!
 *  \brief traces match_id and diagram_id and if set, the classifier ids
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_search_result_trace ( const data_search_result_t *this_ );

#include "set/data_search_result.inl"

#endif  /* DATA_SEARCH_RESULT_H */


/*
Copyright 2020-2024 Andreas Warnke

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
