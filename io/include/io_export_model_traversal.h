/* File: io_export_model_traversal.h; Copyright and License: see below */

#ifndef IO_EXPORT_MODEL_TRAVERSAL_H
#define IO_EXPORT_MODEL_TRAVERSAL_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Traverses the model or a model-part(diagram) and writes the elements to an format writer
 * 
 *  Source: input_data(data_visible_set_t) or db_reader(data_database_reader_t);
 *  Task: traverse the source model(-part);
 *  Sink: io_format_writer_t
 */

#include "io_format_writer.h"
#include "io_filter_flag.h"
#include "set/data_visible_set.h"
#include "data_table.h"
#include "data_rules.h"
#include "universal_array_list.h"
#include <stdio.h>

/*!
 *  \brief attributes of the description writer
 */
struct io_export_model_traversal_struct {
    /*data_database_reader_t *db_reader;*/  /* !< pointer to external database reader */
    const data_visible_set_t *input_data;  /*!< pointer to an external data cache */
    data_rules_t filter_rules;  /*!< own instance of uml and sysml consistency rules */
    io_filter_flag_t filter_flags;  /*!< flags indicating which elements shall be exported */
    
    data_id_t written_id_set_buf[50000];  /*!< buffer for list of already exported element ids */
    universal_array_list_t written_id_set;  /*!< list of already exported element ids */
};

typedef struct io_export_model_traversal_struct io_export_model_traversal_t;

/*!
 *  \brief initializes the io_export_model_traversal_t
 *
 *  \param this_ pointer to own object attributes
 *  \param filter_flags flags indicating which elements shall be exported
 *  \param input_data pointer to the (cached) data to be rendered
 */
void io_export_model_traversal_init( io_export_model_traversal_t *this_, 
                                     io_filter_flag_t filter_flags, 
                                     const data_visible_set_t *input_data
                                   );

/*!
 *  \brief re-initializes the io_export_model_traversal_t
 *
 *  \param this_ pointer to own object attributes
 *  \param filter_flags flags indicating which elements shall be exported
 *  \param input_data pointer to the (cached) data to be rendered
 */
void io_export_model_traversal_reinit( io_export_model_traversal_t *this_, 
                                       io_filter_flag_t filter_flags, 
                                       const data_visible_set_t *input_data 
                                     );

/*!
 *  \brief destroys the io_export_model_traversal_t
 *
 *  \param this_ pointer to own object attributes
 */
void io_export_model_traversal_destroy( io_export_model_traversal_t *this_ );

/*!
 *  \brief prints names and descriptions of the diagram and contained classifiers to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param format_writer writer to format the data and stream it out to a file
 *  \return -1 in case of error, 0 in case of success
 */
int io_export_model_traversal_walk_diagram ( io_export_model_traversal_t *this_, io_format_writer_t *format_writer );

/*!
 *  \brief prints names and descriptions of the classifiers to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param format_writer writer to format the data and stream it out to a file
 *  \return -1 in case of error, 0 in case of success
 */
int io_export_model_traversal_write_classifiers ( io_export_model_traversal_t *this_, io_format_writer_t *format_writer );

/*!
 *  \brief prints names and descriptions of the classifier to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier of which the features are written
 *  \param format_writer writer to format the data and stream it out to a file
 *  \return -1 in case of error, 0 in case of success
 */
int io_export_model_traversal_private_write_features_of_classifier ( io_export_model_traversal_t *this_,
                                                                     data_id_t classifier_id,
                                                                     io_format_writer_t *format_writer
                                                                   );

/*!
 *  \brief prints names and descriptions of the relationships to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param from_classifier_id id of the classifier of which the relationships are written
 *  \param format_writer writer to format the data and stream it out to a file
 *  \return -1 in case of error, 0 in case of success
 */
int io_export_model_traversal_private_write_relations_of_classifier ( io_export_model_traversal_t *this_,
                                                                      data_id_t from_classifier_id,
                                                                      io_format_writer_t *format_writer
                                                                    );

#endif  /* IO_EXPORT_MODEL_TRAVERSAL_H */


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
