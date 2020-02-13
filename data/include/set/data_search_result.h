/* File: data_search_result.h; Copyright and License: see below */

#ifndef DATA_ID_SEARCH_RESULT_H
#define DATA_ID_SEARCH_RESULT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines ids of a diagram, classifier, feature and relationship needed to describe a search result
 */

#include "data_id.h"

/*!
 *  \brief the ids needed to describe a search result
 */
struct data_search_result_struct {
    data_id_t match_object_id;  /*!< the object id of the found object */
    data_id_t src_classifier_id;  /*!< in case of feature or relationship, the source classifier id */
    data_id_t dst_classifier_id;  /*!< in case of relationship, the destination classifier id */
    data_id_t diagram_id;  /*!< the diagram id */
};

typedef struct data_search_result_struct data_search_result_t;

/*!
 *  \brief initializes the data_search_result_t struct.
 *
 *  \param this_ pointer to own object attributes
 *  \param match_id matching object id
 *  \param diagram_id diagram id where the matching object is visible
 */
static inline void data_search_result_init ( data_search_result_t *this_, data_id_t match_id, data_id_t diagram_id );

/*!
 *  \brief initializes the data_search_result_t struct.
 *
 *  \param this_ pointer to own object attributes
 *  \param match_id matching feature id
 *  \param classifier_id classifier of matching feature
 *  \param diagram_id diagram id where the matching feature is visible
 */
static inline void data_search_result_init_feature ( data_search_result_t *this_,
                                                     data_id_t match_id,
                                                     data_id_t classifier_id,
                                                     data_id_t diagram_id
                                                   );

/*!
 *  \brief initializes the data_search_result_t struct.
 *
 *  \param this_ pointer to own object attributes
 *  \param match_id matching relationship id
 *  \param from_classifier_id from classifier of matching relationship
 *  \param to_classifier_id to classifier of matching relationship
 *  \param diagram_id diagram id where the matching relationship is visible
 */
static inline void data_search_result_init_relationship ( data_search_result_t *this_,
                                                          data_id_t match_id,
                                                          data_id_t from_classifier_id,
                                                          data_id_t to_classifier_id,
                                                          data_id_t diagram_id
                                                        );

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
 *  \brief traces match_id and diagram_id and if set, the classifier ids
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_search_result_trace ( const data_search_result_t *this_ );

#include "set/data_search_result.inl"

#endif  /* DATA_ID_SEARCH_RESULT_H */


/*
Copyright 2020-2020 Andreas Warnke

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
