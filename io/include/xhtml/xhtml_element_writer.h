/* File: xhtml_element_writer.h; Copyright and License: see below */

#ifndef XHTML_ELEMENT_WRITER_H
#define XHTML_ELEMENT_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Writes several document pieces to one file output stream.
 *
 *  Source: io_exporter and io_export_diagram_traversal;
 *  Task: convert model-elements to an output stream (using an own txt/xml/... writer);
 *  Sink: universal_output_stream_t
 */

#include "io_element_writer.h"
#include "io_file_format.h"
#include "xmi/xmi_writer_pass.h"
#include "xml/xml_writer.h"
#include "txt/txt_writer.h"
#include "xmi/xmi_type_converter.h"
#include "md/md_filter.h"
#include "data_diagram.h"
#include "data_classifier.h"
#include "set/data_visible_set.h"
#include "set/data_stat.h"
#include "storage/data_database_reader.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "stream/universal_output_stream.h"

/*!
 *  \brief attributes of the element writer
 */
struct xhtml_element_writer_struct {
    io_element_writer_t element_writer;  /*!< instance of implemented interface \c io_element_writer_t */

    data_stat_t *export_stat;  /*!< pointer to external statistics object where export statistics are collected */

    io_file_format_t export_type;  /*!< format of output document */
    uint32_t current_tree_depth;  /*!< tree depth in diagram tree, starts at 0, increases with every call to xhtml_element_writer_start_diagram */

    txt_writer_t txt_writer;  /*!< own instance of a txt writer */
    xml_writer_t xml_writer;  /*!< own instance of an xml writer */
    md_filter_t md_filter;  /*!< own instance of an md filter */
};

typedef struct xhtml_element_writer_struct xhtml_element_writer_t;

/*!
 *  \brief initializes the element writer
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 *  \param export_type image file format
 *  \param io_export_stat pointer to statistics object where export statistics are collected
 *  \param output output stream where to write the generated output to
 */
void xhtml_element_writer_init( xhtml_element_writer_t *this_,
                                data_database_reader_t *db_reader,
                                io_file_format_t export_type,
                                data_stat_t *io_export_stat,
                                universal_output_stream_t *output
                              );

/*!
 *  \brief destroys the element writer
 *
 *  \param this_ pointer to own object attributes
 */
void xhtml_element_writer_destroy( xhtml_element_writer_t *this_ );

/*!
 *  \brief gets the io element writer interface of this xhtml_element_writer_t
 *
 *  \param this_ pointer to own object attributes
 *  \return the abstract base class of this_
 */
io_element_writer_t * xhtml_element_writer_get_element_writer( xhtml_element_writer_t *this_ );

/*!
 *  \brief writes the header of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param document_title title of the document
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_write_header( xhtml_element_writer_t *this_, const char *document_title );

/*!
 *  \brief writes the start of the main section
 *
 *  This starts a section that contains the main part of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param document_title title of the document
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_start_main( xhtml_element_writer_t *this_, const char *document_title );

/*!
 *  \brief checks if a hosting parent classifier may nest a child classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type data_classifier_type_t of the hosting parent of which the nesting-ability shall be determined
 *  \param child_type data_classifier_type_t of the nested child of which the nesting-ability shall be determined
 *  \return true if nesting is allowed
 */
bool xhtml_element_writer_can_classifier_nest_classifier( xhtml_element_writer_t *this_,
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
bool xhtml_element_writer_can_classifier_nest_relationship( xhtml_element_writer_t *this_,
                                                            data_classifier_type_t host_type,
                                                            data_relationship_type_t child_type
                                                          );

/*!
 *  \brief starts a table-of-contents sublist
 *
 *  \param this_ pointer to own object attributes
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_start_toc_sublist ( xhtml_element_writer_t *this_ );

/*!
 *  \brief starts a table-of-contents entry, consisting of an entry and an optional sublist
 *
 *  \param this_ pointer to own object attributes
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_start_toc_entry ( xhtml_element_writer_t *this_ );

/*!
 *  \brief writes a table-of-contents entry
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_ptr pointer to diagram that shall be written
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_write_toc_entry ( xhtml_element_writer_t *this_, const data_diagram_t *diag_ptr );

/*!
 *  \brief end a table-of-contents entry, consisting of an entry and an optional sublist
 *
 *  \param this_ pointer to own object attributes
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_end_toc_entry ( xhtml_element_writer_t *this_ );

/*!
 *  \brief end a table-of-contents sublist
 *
 *  \param this_ pointer to own object attributes
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_end_toc_sublist ( xhtml_element_writer_t *this_ );

/*!
 *  \brief writes a classifier start
 *
 *  This starts a division that contains a classifier and a list of features and relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the hosting parent classifier, needed for xmi export, DATA_CLASSIFIER_TYPE_VOID if toplevel
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_start_classifier( xhtml_element_writer_t *this_,
                                           data_classifier_type_t host_type,
                                           const data_classifier_t *classifier_ptr
                                         );

/*!
 *  \brief writes a classifier of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the hosting parent classifier, needed for xmi export, DATA_CLASSIFIER_TYPE_VOID if toplevel
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_assemble_classifier( xhtml_element_writer_t *this_,
                                              data_classifier_type_t host_type,
                                              const data_classifier_t *classifier_ptr
                                            );

/*!
 *  \brief writes a classifier end
 *
 *  This ends a division that contains a classifier and a list of features and relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the hosting parent classifier, needed for xmi export, DATA_CLASSIFIER_TYPE_VOID if toplevel
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_end_classifier( xhtml_element_writer_t *this_,
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
int xhtml_element_writer_start_feature( xhtml_element_writer_t *this_,
                                        data_classifier_type_t parent_type,
                                        const data_feature_t *feature_ptr
                                      );

/*!
 *  \brief writes a feature of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the owning parent classifier
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_assemble_feature( xhtml_element_writer_t *this_,
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
int xhtml_element_writer_end_feature( xhtml_element_writer_t *this_,
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
int xhtml_element_writer_start_relationship( xhtml_element_writer_t *this_,
                                             data_classifier_type_t host_type,
                                             const data_relationship_t *relation_ptr
                                           );

/*!
 *  \brief writes a relationship of the document
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
int xhtml_element_writer_assemble_relationship( xhtml_element_writer_t *this_,
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
int xhtml_element_writer_end_relationship( xhtml_element_writer_t *this_,
                                           data_classifier_type_t host_type,
                                           const data_relationship_t *relation_ptr
                                         );

/*!
 *  \brief writes a diagram start
 *
 *  This starts a section that contains a diagram and a list of classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_ptr pointer to diagram that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_start_diagram( xhtml_element_writer_t *this_, const data_diagram_t *diag_ptr );

/*!
 *  \brief writes a diagram of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param parent pointer to parent diagram or NULL in case of root
 *  \param diag_ptr pointer to diagram that shall be written
 *  \param diagram_file_base_name filename of the diagram without extension
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_assemble_diagram( xhtml_element_writer_t *this_,
                                           const data_diagram_t *parent,
                                           const data_diagram_t *diag_ptr,
                                           const char *diagram_file_base_name
                                         );

/*!
 *  \brief writes a diagram end
 *
 *  This ends a section that contains a diagram and a list of classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_ptr pointer to diagram that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_end_diagram( xhtml_element_writer_t *this_, const data_diagram_t *diag_ptr );

/*!
 *  \brief writes a diagramelement start-element
 *
 *  \param this_ pointer to own object attributes
 *  \param parent the hosting parent diagram
 *  \param diagramelement_ptr pointer to diagramelement that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_start_diagramelement( xhtml_element_writer_t *this_,
                                               const data_diagram_t *parent,
                                               const data_diagramelement_t *diagramelement_ptr
                                             );

/*!
 *  \brief writes constents of a a diagramelement
 *
 *  \param this_ pointer to own object attributes
 *  \param parent the hosting parent diagram
 *  \param diagramelement_ptr pointer to diagramelement that shall be written, not NULL
 *  \param occurrence the occurring classifier
 *  \param feat_occur the focused feature of the occurring classifier
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_assemble_diagramelement( xhtml_element_writer_t *this_,
                                                  const data_diagram_t *parent,
                                                  const data_diagramelement_t *diagramelement_ptr,
                                                  const data_classifier_t *occurrence,
                                                  const data_feature_t *feat_occur
                                                );

/*!
 *  \brief writes a diagramelement end-element
 *
 *  \param this_ pointer to own object attributes
 *  \param parent the hosting parent diagram
 *  \param diagramelement_ptr pointer to diagramelement that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_end_diagramelement( xhtml_element_writer_t *this_,
                                             const data_diagram_t *parent,
                                             const data_diagramelement_t *diagramelement_ptr
                                           );

/*!
 *  \brief writes the ending of the main section
 *
 *  This ends a section that contains the main part of the document
 *
 *  \param this_ pointer to own object attributes
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_end_main( xhtml_element_writer_t *this_ );

/*!
 *  \brief writes the footer of the document
 *
 *  \param this_ pointer to own object attributes
 *  \return 0 in case of success, -1 otherwise
 */
int xhtml_element_writer_write_footer( xhtml_element_writer_t *this_ );

#endif  /* XHTML_ELEMENT_WRITER_H */


/*
Copyright 2019-2022 Andreas Warnke

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
