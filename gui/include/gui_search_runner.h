/* File: gui_search_runner.h; Copyright and License: see below */

#ifndef GUI_SEARCH_RUNNER_H
#define GUI_SEARCH_RUNNER_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Gets a search query string, performs the search and provides the search result.
 */

#include "pos/pos_scroll_page.h"
#include "pos/pos_search_result_page.h"
#include "observer/observer.h"
#include "storage/data_database_reader.h"
#include "storage/data_database_text_search.h"
#include "set/data_small_set.h"
#include "set/data_search_result.h"
#include "set/data_search_result_list.h"
#include "entity/data_diagramelement.h"
#include "entity/data_feature.h"
#include "entity/data_relationship.h"
#include "gui_simple_message_to_user.h"
#include "utf8stream/utf8stream_writemem.h"

/*!
 *  \brief constants for maximum values of gui_search_runner_t
 */
enum gui_search_runner_max_enum {
    GUI_SEARCH_RUNNER_MAX_RESULTS = POS_SEARCH_RESULT_PAGE_MAX_PAGE_SIZE,  /*!< maximum number of search results */
};

/*!
 *  \brief attributes of the gui_search_runner
 */
struct gui_search_runner_struct {
    /* external entities */
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    data_database_text_search_t db_searcher;  /*!< own instance of a database freetext searcher */
    observer_t result_consumer;  /*!< observer_t which is informed on search results */
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external message-displayer */

    /* request data */
    char search_string_buf[256];  /*!< the current search string, truncated to 255 bytes, 0-terminated */
    utf8stream_writemem_t search_string_writer;  /*!< a writer to the current search string */
    pos_scroll_page_t page_request;  /*!< description of the requested search page */

    /* result data */
    uint32_t result_buffer_start;  /*!< offset of start element in result_list_buffer (relative to absolute start of result list) */
    /* uint32_t result_buffer_length; */  /*!< number of elements in result_list_buffer list page */
    data_search_result_t result_buffer[GUI_SEARCH_RUNNER_MAX_RESULTS];  /*!< memory for the result list page */
    data_search_result_list_t result_list;  /*!< a list of search results; storage for search results is: result_buffer */

    /* temporary buffers */
    data_diagram_t temp_diagram;  /*!< memory to read a diagram */
    data_diagramelement_t temp_diagramelement;  /*!< memory to read a diagram element */
    data_feature_t temp_feature;  /*!< memory to read a feature */
    data_relationship_t temp_relationship;  /*!< memory to read a relationship */
    data_classifier_t temp_classifier;  /*!< memory to read a classifier */
};

typedef struct gui_search_runner_struct gui_search_runner_t;

/*!
 *  \brief initializes the gui_search_runner_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param message_to_user pointer to external message-displayer
 *  \param db_reader pointer to external database reader
 *  \param database pointer to external database - used to create a data_database_text_search_t
 *  \param result_consumer pointer to external observer_t which is informed on search results
 */
void gui_search_runner_init ( gui_search_runner_t *this_,
                              gui_simple_message_to_user_t *message_to_user,
                              data_database_reader_t *db_reader,
                              data_database_t *database,
                              observer_t result_consumer
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
 *  \param page the page to be loaded
 */
void gui_search_runner_run ( gui_search_runner_t *this_, const char* search_string, pos_scroll_page_t page );

/*!
 *  \brief performs a search on the same text again and informs the result_consumer of result set
 *
 *  \param this_ pointer to own object attributes
 *  \param page the page to be loaded
 */
void gui_search_runner_rerun ( gui_search_runner_t *this_, pos_scroll_page_t page );

/*!
 *  \brief searches diagrams in which a given classifier is visible and adds their ids to a result set
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_template a search result template that is already half-filled, only the diagram information is missing
 *  \param io_list list to which to add an entry for each found diagram
 */
void gui_search_runner_private_add_diagrams_of_classifier ( gui_search_runner_t *this_,
                                                            data_search_result_t *classifier_template,
                                                            data_search_result_list_t *io_list
                                                          );

/*!
 *  \brief gets the page request that was guiding the search
 *
 *  \param this_ pointer to own object attributes
 *  \return page request
 */
const pos_scroll_page_t* gui_search_runner_get_page_request( gui_search_runner_t *this_ );

/*!
 *  \brief gets the result_list
 *
 *  \param this_ pointer to own object attributes
 *  \return result_list
 */
const data_search_result_list_t* gui_search_runner_get_result_list( gui_search_runner_t *this_ );

/*!
 *  \brief gets the result_buffer_start
 *
 *  \param this_ pointer to own object attributes
 *  \return result_buffer_start indicates how many search results are skipped before result_list
 */
uint32_t gui_search_runner_get_result_buffer_start( gui_search_runner_t *this_ );

#endif  /* GUI_SEARCH_RUNNER_H */


/*
Copyright 2020-2025 Andreas Warnke

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
