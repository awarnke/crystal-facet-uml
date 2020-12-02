/* File: xmi_interaction_writer.h; Copyright and License: see below */

#ifndef XMI_INTERACTION_WRITER_H
#define XMI_INTERACTION_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Writes several document pieces to one file output stream.
 *
 *  Source: io_exporter and io_export_model_traversal_t;
 *  Task: convert model-elements to an output stream (using an own xml writer);
 *  Sink: universal_output_stream_t
 */

#include "xmi/xmi_atom_writer.h"
#include "xml/xml_writer.h"
#include "xmi/xmi_type_converter.h"
#include "io_file_format.h"
#include "io_writer_pass.h"
#include "data_diagram.h"
#include "data_classifier.h"
#include "set/data_visible_set.h"
#include "set/data_stat.h"
#include "storage/data_database_reader.h"
#include "util/string/utf8stringbuf.h"

/*!
 *  \brief attributes of the format writer
 */
struct xmi_interaction_writer_struct {
    io_writer_pass_t mode;  /*!< depending on the mode, conversion from a data object to the output format differs */
    
    data_stat_t *export_stat;  /*!< pointer to external statistics object where export statistics are collected */
    xml_writer_t *xml_writer;  /*!< pointer to external xml writer */
    xmi_atom_writer_t atom_writer;  /*!< own instance of an atom writer */

    xmi_type_converter_t xmi_types;  /*!< own instance of an xmi_type_converter_t */
};

typedef struct xmi_interaction_writer_struct xmi_interaction_writer_t;

/*!
 *  \brief initializes the format writer
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object (to resolve markdown links)
 *  \param io_export_stat pointer to statistics object where export statistics are collected
 *  \param out_writer output stream where to write the generated output to
 */
void xmi_interaction_writer_init( xmi_interaction_writer_t *this_,
                                  data_database_reader_t *db_reader,
                                  data_stat_t *io_export_stat,
                                  xml_writer_t *out_writer 
                                );

/*!
 *  \brief destroys the format writer
 *
 *  \param this_ pointer to own object attributes
 */
void xmi_interaction_writer_destroy( xmi_interaction_writer_t *this_ );

/*!
 *  \brief writes the start of the main section
 *
 *  This starts a section that contains the main part of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classifier
 *  \param diagram_ptr diagram which shall be written as XMI interaction
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_interaction_writer_start_diagram( xmi_interaction_writer_t *this_, 
                                          data_classifier_type_t parent_type,
                                          const data_diagram_t *diagram_ptr 
                                        );

/*!
 *  \brief writes the ending of the main section
 *
 *  This ends a section that contains the main part of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classifier
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_interaction_writer_end_diagram( xmi_interaction_writer_t *this_,
                                        data_classifier_type_t parent_type 
                                      );

/*!
 *  \brief writes constents of a lifeline-type feature
 *
 *  \param this_ pointer to own object attributes
 *  \param reference_id id of the object which this lifeline proxies
 *  \param parent_type type of the owning parent classifier
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_interaction_writer_assemble_feature( xmi_interaction_writer_t *this_,
                                             data_id_t reference_id,
                                             data_classifier_type_t parent_type,
                                             const data_feature_t *feature_ptr
                                           );

/*!
 *  \brief writes the contents of a message-type relationship 
 *
 *  \param this_ pointer to own object attributes
 *  \param interaction_id id of the interaction to which the message (relationship) belongs
 *  \param parent_type type of the nesting-parent classifier, needed for xmi export
 *  \param relation_ptr pointer to relationship that shall be written, not NULL
 *  \param from_c_type the type of classifier at source end
 *  \param from_f_type the type of feature at source end; DATA_FEATURE_TYPE_VOID if no feature specified
 *  \param to_c_type the type of classifier at target end
 *  \param to_f_type the type of feature at target end; DATA_FEATURE_TYPE_VOID if no feature specified
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_interaction_writer_assemble_relationship( xmi_interaction_writer_t *this_,
                                                  data_id_t interaction_id,
                                                  data_classifier_type_t parent_type,
                                                  const data_relationship_t *relation_ptr,
                                                  data_classifier_type_t from_c_type,
                                                  data_feature_type_t from_f_type,
                                                  data_classifier_type_t to_c_type,
                                                  data_feature_type_t to_f_type 
                                                );


#endif  /* XMI_INTERACTION_WRITER_H */


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
