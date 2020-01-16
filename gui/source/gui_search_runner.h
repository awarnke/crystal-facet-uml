/* File: gui_search_runner.h; Copyright and License: see below */

#ifndef GUI_SEARCH_RUNNER_H
#define GUI_SEARCH_RUNNER_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Gets a search query string, performs the search and provides the search result.
 */

#include "sketch_area/gui_sketch_area.h"
#include "storage/data_database_reader.h"
#include "set/data_small_set.h"
#include "data_diagramelement.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "gui_simple_message_to_user.h"

/*!
 *  \brief constants for maximum values of gui_search_runner_t
 */
enum gui_search_runner_max_enum {
    GUI_SEARCH_RUNNER_MAX_DIAGRAMS = 64,  /*!< maximum number of diagrams which may contain a classifier */
};

/*!
 *  \brief attributes of the gui_search_runner
 */
struct gui_search_runner_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    gui_sketch_area_t *result_consumer;  /*!< pointer to external gui_sketch_area_t which is informed on search results */
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external message-displayer */

    data_small_set_t temp_result_set;  /*!< memory for the result set */
    data_diagramelement_t temp_diagramelement[GUI_SEARCH_RUNNER_MAX_DIAGRAMS];  /*!< memory to read a set of diagram elements */
    data_feature_t temp_feature;  /*!< memory to read a feature */
    data_relationship_t temp_relationship;  /*!< memory to read a relationship */
};

typedef struct gui_search_runner_struct gui_search_runner_t;

/*!
 *  \brief initializes the gui_search_runner_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param message_to_user pointer to external message-displayer
 *  \param db_reader pointer to external database reader
 *  \param result_consumer pointer to external gui_sketch_area_t which is informed on search results
 */
void gui_search_runner_init ( gui_search_runner_t *this_,
                              gui_simple_message_to_user_t *message_to_user,
                              data_database_reader_t *db_reader,
                              gui_sketch_area_t *result_consumer
                            );

/*!
 *  \brief destroys the gui_search_runner_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_search_runner_destroy ( gui_search_runner_t *this_ );

/*!
 *  \brief performs a search and informs the result_consumer of result set
 *
 *  \param this_ pointer to own object attributes
 *  \param search_string search query, 0-terminated
 */
void gui_search_runner_run ( gui_search_runner_t *this_, const char* search_string );

#endif  /* GUI_SEARCH_RUNNER_H */


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
