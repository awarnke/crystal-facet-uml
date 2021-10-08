/* File: json_element_writer.h; Copyright and License: see below */

#ifndef JSON_ELEMENT_WRITER_H
#define JSON_ELEMENT_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a pair of a) a pointer to a concrete instance of an interface
 *  and b) a pointer to objectdata that implements the interface.
 */

#include "io_element_writer.h"
#include "data_classifier.h"
#include "data_classifier_type.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "data_relationship_type.h"
#include "data_diagram.h"
#include "data_diagram_type.h"
#include "data_diagramelement.h"
#include "set/data_stat.h"
#include "stream/universal_output_stream.h"

/*!
 *  \brief object (vmt+data) of a json_element_writer_t.
 *
 */
struct json_element_writer_struct {
    io_element_writer_t element_writer;  /*!< instance of implemented interface \c io_element_writer_t */

    data_stat_t *export_stat;  /*!< pointer to external statistics object where export statistics are collected */
    universal_output_stream_t *output;  /*!< json stream sink */
};

typedef struct json_element_writer_struct json_element_writer_t;

/*!
 *  \brief initializes the json_element_writer_t
 *
 *  \param this_ pointer to own object attributes
 *  \param io_export_stat pointer to statistics object where export statistics are collected
 *  \param output output stream where to write the generated output to
 */
void json_element_writer_init( json_element_writer_t *this_,
                               data_stat_t *io_export_stat,
                               universal_output_stream_t *output
                             );

/*!
 *  \brief destroys the json_element_writer_t.
 *
 *  \param this_ pointer to own object attributes
 */
void json_element_writer_destroy( json_element_writer_t *this_ );

/*!
 *  \brief writes the header of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param document_title title of the document
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_write_header( json_element_writer_t *this_, const char *document_title );

/*!
 *  \brief writes the start of the main section
 *
 *  This starts a section that contains the main part of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param document_title title of the document
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_start_main( json_element_writer_t *this_, const char *document_title );

/*!
 *  \brief checks if a hosting parent classifier may nest a child classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type data_classifier_type_t of the hosting parent of which the nesting-ability shall be determined
 *  \param child_type data_classifier_type_t of the nested child of which the nesting-ability shall be determined
 *  \return true if nesting is allowed
 */
bool json_element_writer_can_classifier_nest_classifier( json_element_writer_t *this_,
                                                         data_classifier_type_t host_type,
                                                         data_classifier_type_t child_type
                                                       );

/*!
 *  \brief checks if a hosting parent classifier may nest relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type data_classifier_type_t of the hosting parent of which the nesting-ability shall be determined
 *  \param child_type data_relationship_type_t of the nested child of which the nesting-ability shall be determined
 *  \return true if nesting or any relationship is allowed
 */
bool json_element_writer_can_classifier_nest_relationship( json_element_writer_t *this_,
                                                           data_classifier_type_t host_type,
                                                           data_relationship_type_t child_type
                                                         );

/*!
 *  \brief writes a classifier start-element
 *
 *  This starts a division that contains a classifier and a list of features and relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the hosting parent classifier
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_start_classifier( json_element_writer_t *this_,
                                          data_classifier_type_t host_type,
                                          const data_classifier_t *classifier_ptr
                                        );

/*!
 *  \brief writes contents of a classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the hosting parent classifier
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_assemble_classifier( json_element_writer_t *this_,
                                             data_classifier_type_t host_type,
                                             const data_classifier_t *classifier_ptr
                                           );

/*!
 *  \brief writes a classifier end-element
 *
 *  This ends a division that contains a classifier and a list of features and relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the hosting parent classifier, needed for xmi export
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_end_classifier( json_element_writer_t *this_,
                                        data_classifier_type_t host_type,
                                        const data_classifier_t *classifier_ptr
                                      );

/*!
 *  \brief writes a feature start-element
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the owning parent classifier
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_start_feature( json_element_writer_t *this_,
                                       data_classifier_type_t parent_type,
                                       const data_feature_t *feature_ptr
                                     );

/*!
 *  \brief writes constents of a a feature
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the owning parent classifier
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_assemble_feature( json_element_writer_t *this_,
                                          data_classifier_type_t parent_type,
                                          const data_feature_t *feature_ptr
                                        );

/*!
 *  \brief writes a feature end-element
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the owning parent classifier
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_end_feature( json_element_writer_t *this_,
                                     data_classifier_type_t parent_type,
                                     const data_feature_t *feature_ptr
                                   );

/*!
 *  \brief starts a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the hosting parent classifier, needed for xmi export
 *  \param relation_ptr pointer to relationship that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_start_relationship( json_element_writer_t *this_,
                                            data_classifier_type_t host_type,
                                            const data_relationship_t *relation_ptr
                                          );

/*!
 *  \brief writes the contents of a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param host the hosting parent classifier, needed for xmi export; is NULL on top-level of document
 *  \param relation_ptr pointer to relationship that shall be written, not NULL
 *  \param from_c the classifier at source end
 *  \param from_f the feature at source end; NULL or !is_valid() if no feature specified
 *  \param to_c the classifier at target end
 *  \param to_f the feature at target end; NULL or !is_valid() if no feature specified
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_assemble_relationship( json_element_writer_t *this_,
                                               const data_classifier_t *host,
                                               const data_relationship_t *relation_ptr,
                                               const data_classifier_t *from_c,
                                               const data_feature_t *from_f,
                                               const data_classifier_t *to_c,
                                               const data_feature_t *to_f
                                             );

/*!
 *  \brief ends a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the hosting parent classifier, needed for xmi export
 *  \param relation_ptr pointer to relationship that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_end_relationship( json_element_writer_t *this_,
                                          data_classifier_type_t host_type,
                                          const data_relationship_t *relation_ptr
                                        );

/*!
 *  \brief writes a diagram start
 *
 *  This starts a section that contains a diagram and a list of diagramelements (classifier-occurrences)
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_ptr pointer to diagram that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_start_diagram( json_element_writer_t *this_, const data_diagram_t *diag_ptr );

/*!
 *  \brief writes a diagram of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_ptr pointer to diagram that shall be written
 *  \param diagram_file_base_name filename of the diagram without extension
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_assemble_diagram( json_element_writer_t *this_,
                                          const data_diagram_t *diag_ptr,
                                          const char *diagram_file_base_name
                                        );

/*!
 *  \brief ends a diagram
 *
 *  This ends a section that contains a diagram and a list of diagramelements (classifier-occurrences)
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_ptr pointer to diagram that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_end_diagram( json_element_writer_t *this_, const data_diagram_t *diag_ptr );

/*!
 *  \brief writes a diagramelement start-element
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_ptr pointer to diagramelement that shall be written, not NULL
 *  \param parent the hosting parent diagram
 *  \param occurrence the occurring classifier
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_start_diagramelement( json_element_writer_t *this_,
                                              const data_diagramelement_t *diagramelement_ptr,
                                              const data_diagram_t *parent,
                                              const data_classifier_t *occurrence
                                            );

/*!
 *  \brief writes constents of a a diagramelement
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_ptr pointer to diagramelement that shall be written, not NULL
 *  \param parent the hosting parent diagram
 *  \param occurrence the occurring classifier
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_assemble_diagramelement( json_element_writer_t *this_,
                                                 const data_diagramelement_t *diagramelement_ptr,
                                                 const data_diagram_t *parent,
                                                 const data_classifier_t *occurrence
                                               );

/*!
 *  \brief writes a diagramelement end-element
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_ptr pointer to diagramelement that shall be written, not NULL
 *  \param parent the hosting parent diagram
 *  \param occurrence the occurring classifier
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_end_diagramelement( json_element_writer_t *this_,
                                            const data_diagramelement_t *diagramelement_ptr,
                                            const data_diagram_t *parent,
                                            const data_classifier_t *occurrence
                                          );

/*!
 *  \brief writes the ending of the main section
 *
 *  This ends a section that contains the main part of the document
 *
 *  \param this_ pointer to own object attributes
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_end_main( json_element_writer_t *this_ );

/*!
 *  \brief writes the footer of the document
 *
 *  \param this_ pointer to own object attributes
 *  \return 0 in case of success, -1 otherwise
 */
int json_element_writer_write_footer( json_element_writer_t *this_ );

/*!
 *  \brief gets the io element writer interface of this json_element_writer_t
 *
 *  \param this_ pointer to own object attributes
 *  \return the abstract base class of this_
 */
io_element_writer_t * json_element_writer_get_element_writer( json_element_writer_t *this_ );

#endif  /* JSON_ELEMENT_WRITER_H */


/*
Copyright 2021-2021 Andreas Warnke

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
