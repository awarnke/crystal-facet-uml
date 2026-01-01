/* File: document_element_writer.h; Copyright and License: see below */

#ifndef DOCUMENT_ELEMENT_WRITER_H
#define DOCUMENT_ELEMENT_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Writes several document pieces to one file output stream.
 *
 *  Source: io_exporter and io_export_diagram_traversal;
 *  Task: convert model-elements to an output stream (using an own txt/xml/... writer);
 *  Sink: universal_output_stream_t
 */

#include "document_link_provider.h"
#include "io_element_writer.h"
#include "io_file_format.h"
#include "xmi/xmi_writer_pass.h"
#include "xmi/xmi_type_converter.h"
#include "format/io_xml_writer.h"
#include "format/io_txt_writer.h"
#include "format/io_txt_icon.h"
#include "format/io_md_writer.h"
#include "json/json_type_name_map.h"
#include "entity/data_diagram.h"
#include "entity/data_classifier.h"
#include "data_rules.h"
#include "set/data_visible_set.h"
#include "set/data_stat.h"
#include "storage/data_database_reader.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "u8stream/universal_output_stream.h"
#include "u8/u8_error.h"

/*!
 *  \brief attributes of the element writer
 */
struct document_element_writer_struct {
    io_element_writer_t element_writer;  /*!< instance of implemented interface \c io_element_writer_t */

    data_stat_t *export_stat;  /*!< pointer to external statistics object where export statistics are collected */

    io_file_format_t export_type;  /*!< format of output document */
    uint32_t current_tree_depth;  /*!< tree depth in diagram tree, starts at 0, */
                                  /*!< increases with every call to document_element_writer_start_diagram() */

    json_type_name_map_t type_map;  /*!< own instance of a mapping from type ids to type names */
    data_rules_t data_rules;  /*!< own instance of uml and sysml consistency rules */
    document_link_provider_t link_provider;  /*!< own instance of a document_link_provider_t */
    io_txt_icon_t txt_icon;  /*!< own instance of a io_txt_icon provider for relationship types */

    io_txt_writer_t txt_writer;  /*!< own instance of a txt writer */
    io_xml_writer_t xml_writer;  /*!< own instance of an xml writer */
    io_md_writer_t md_writer;  /*!< own instance of an md filter */
};

typedef struct document_element_writer_struct document_element_writer_t;

/*!
 *  \brief initializes the element writer
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 *  \param export_type format of output document
 *  \param io_export_stat pointer to statistics object where export statistics are collected
 *  \param output output stream where to write the generated output to
 */
void document_element_writer_init( document_element_writer_t *this_,
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
void document_element_writer_destroy( document_element_writer_t *this_ );

/*!
 *  \brief gets the io element writer interface of this document_element_writer_t
 *
 *  \param this_ pointer to own object attributes
 *  \return the abstract base class of this_
 */
io_element_writer_t * document_element_writer_get_element_writer( document_element_writer_t *this_ );

/*!
 *  \brief writes the header of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param document_title title of the document
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_write_header( document_element_writer_t *this_, const char *document_title );

/*!
 *  \brief writes the start of the main section
 *
 *  This starts a section that contains the main part of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param document_title title of the document
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_start_main( document_element_writer_t *this_, const char *document_title );

/*!
 *  \brief checks if a hosting parent classifier may nest a child classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type data_classifier_type_t of the hosting parent of which the nesting-ability shall be determined
 *  \param child_type data_classifier_type_t of the nested child of which the nesting-ability shall be determined
 *  \return true if nesting is allowed
 */
bool document_element_writer_can_classifier_nest_classifier( document_element_writer_t *this_,
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
bool document_element_writer_can_classifier_nest_relationship( document_element_writer_t *this_,
                                                               data_classifier_type_t host_type,
                                                               data_relationship_type_t child_type
                                                             );

/*!
 *  \brief starts a table-of-contents section
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_start_toc ( document_element_writer_t *this_ );

/*!
 *  \brief starts a table-of-contents sublist
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_start_toc_sublist ( document_element_writer_t *this_ );

/*!
 *  \brief starts a table-of-contents entry, consisting of an entry and an optional sublist
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_start_toc_entry ( document_element_writer_t *this_ );

/*!
 *  \brief writes a table-of-contents entry
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_ptr pointer to diagram that shall be written
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_write_toc_entry ( document_element_writer_t *this_, const data_diagram_t *diag_ptr );

/*!
 *  \brief ends a table-of-contents entry, consisting of an entry and an optional sublist
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_end_toc_entry ( document_element_writer_t *this_ );

/*!
 *  \brief ends a table-of-contents sublist
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_end_toc_sublist ( document_element_writer_t *this_ );

/*!
 *  \brief ends a table-of-contents section
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_end_toc ( document_element_writer_t *this_ );

/*!
 *  \brief writes a classifier start
 *
 *  This starts a division that contains a classifier and a list of features and relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the hosting parent classifier, needed for xmi export, DATA_CLASSIFIER_TYPE_VOID if toplevel
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_start_classifier( document_element_writer_t *this_,
                                                     data_classifier_type_t host_type,
                                                     const data_classifier_t *classifier_ptr
                                                   );

/*!
 *  \brief writes a classifier of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the hosting parent classifier, needed for xmi export, DATA_CLASSIFIER_TYPE_VOID if toplevel
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_assemble_classifier( document_element_writer_t *this_,
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
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_end_classifier( document_element_writer_t *this_,
                                                   data_classifier_type_t host_type,
                                                   const data_classifier_t *classifier_ptr
                                                 );

/*!
 *  \brief writes a feature start-element
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the owning parent classifier
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_start_feature( document_element_writer_t *this_,
                                                  data_classifier_type_t parent_type,
                                                  const data_feature_t *feature_ptr
                                                );

/*!
 *  \brief writes a feature of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param parent the owning parent classifier
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_assemble_feature( document_element_writer_t *this_,
                                                     const data_classifier_t *parent,
                                                     const data_feature_t *feature_ptr
                                                   );

/*!
 *  \brief writes a feature end-element
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the owning parent classifier
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_end_feature( document_element_writer_t *this_,
                                                data_classifier_type_t parent_type,
                                                const data_feature_t *feature_ptr
                                              );

/*!
 *  \brief starts a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the hosting parent classifier, needed for xmi export
 *  \param relation_ptr pointer to relationship that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_start_relationship( document_element_writer_t *this_,
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
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_assemble_relationship( document_element_writer_t *this_,
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
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_end_relationship( document_element_writer_t *this_,
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
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_start_diagram( document_element_writer_t *this_, const data_diagram_t *diag_ptr );

/*!
 *  \brief writes a diagram of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param parent pointer to parent diagram or NULL in case of root
 *  \param diag_ptr pointer to diagram that shall be written
 *  \param diagram_file_base_name filename of the diagram without extension
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_assemble_diagram( document_element_writer_t *this_,
                                                     const data_diagram_t *parent,
                                                     const data_diagram_t *diag_ptr,
                                                     const char *diagram_file_base_name
                                                   );

/*!
 *  \brief writes the local end of a diagram and the start to descend
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_ptr pointer to diagram that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_descend_diagram( document_element_writer_t *this_, const data_diagram_t *diag_ptr );

/*!
 *  \brief writes a diagram end
 *
 *  This ends a section that contains a diagram and a list of classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_ptr pointer to diagram that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_end_diagram( document_element_writer_t *this_, const data_diagram_t *diag_ptr );

/*!
 *  \brief writes a diagramelement start-element
 *
 *  \param this_ pointer to own object attributes
 *  \param parent the hosting parent diagram
 *  \param diagramelement_ptr pointer to diagramelement that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_start_diagramelement( document_element_writer_t *this_,
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
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_assemble_diagramelement( document_element_writer_t *this_,
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
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_end_diagramelement( document_element_writer_t *this_,
                                                       const data_diagram_t *parent,
                                                       const data_diagramelement_t *diagramelement_ptr
                                                     );

/*!
 *  \brief writes the ending of the main section
 *
 *  This ends a section that contains the main part of the document
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_end_main( document_element_writer_t *this_ );

/*!
 *  \brief writes the footer of the document
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_element_writer_write_footer( document_element_writer_t *this_ );

#endif  /* DOCUMENT_ELEMENT_WRITER_H */


/*
Copyright 2019-2026 Andreas Warnke

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
