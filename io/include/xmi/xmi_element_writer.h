/* File: xmi_element_writer.h; Copyright and License: see below */

#ifndef XMI_ELEMENT_WRITER_H
#define XMI_ELEMENT_WRITER_H

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
#include "util/geometry/geometry_rectangle.h"
#include "util/string/utf8stringbuf.h"
#include "stream/universal_output_stream.h"

/*!
 *  \brief attributes of the xmi_element_writer_t
 */
struct xmi_element_writer_struct {
    io_writer_pass_t mode;  /*!< depending on the mode, conversion from a data object to the output format differs */

    data_stat_t *export_stat;  /*!< pointer to external statistics object where export statistics are collected */
    xml_writer_t xml_writer;  /*!< own instance of an xml writer */
    xmi_atom_writer_t atom_writer;  /*!< own instance of an atom writer */

    xmi_type_converter_t xmi_types;  /*!< own instance of an xmi_type_converter_t */
};

typedef struct xmi_element_writer_struct xmi_element_writer_t;

/*!
 *  \brief initializes the xmi_element_writer_t
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object (to resolve markdown links)
 *  \param io_export_stat pointer to statistics object where export statistics are collected
 *  \param output output stream where to write the generated output to
 */
void xmi_element_writer_init( xmi_element_writer_t *this_,
                              data_database_reader_t *db_reader,
                              data_stat_t *io_export_stat,
                              universal_output_stream_t *output
                            );

/*!
 *  \brief destroys the xmi_element_writer_t
 *
 *  \param this_ pointer to own object attributes
 */
void xmi_element_writer_destroy( xmi_element_writer_t *this_ );

/*!
 *  \brief gets the conversion mode of the xmi_element_writer_t
 *
 *  \param this_ pointer to own object attributes
 *  \result mode how to convert a data object to the output format, e.g. uml-basic or profile-extension
 */
static inline io_writer_pass_t xmi_element_writer_get_mode( xmi_element_writer_t *this_ );

/*!
 *  \brief sets the conversion mode of the xmi_element_writer_t
 *
 *  \param this_ pointer to own object attributes
 *  \param mode mode how to convert a data object to the output format, e.g. uml-basic or profile-extension
 */
static inline void xmi_element_writer_set_mode( xmi_element_writer_t *this_, io_writer_pass_t mode );

/*!
 *  \brief gets a pointer to the xml_writer
 *
 *  \param this_ pointer to own object attributes
 *  \result pointer to the internal xml_writer
 */
static inline xml_writer_t *xmi_element_writer_get_xml_writer_ptr( xmi_element_writer_t *this_ );

/*!
 *  \brief writes the header of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param document_title title of the document
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_write_header( xmi_element_writer_t *this_, const char *document_title );

/*!
 *  \brief writes the start of the main section
 *
 *  This starts a section that contains the main part of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param document_title title of the document
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_start_main( xmi_element_writer_t *this_, const char *document_title );

/*!
 *  \brief checks if a parent classifier may nest a child classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type data_classifier_type_t of the parent of which the nesting-ability shall be determined
 *  \param child_type data_classifier_type_t of the nested child of which the nesting-ability shall be determined
 *  \result true if nesting is allowed
 */
static inline bool xmi_element_writer_can_classifier_nest_classifier ( xmi_element_writer_t *this_,
                                                                       data_classifier_type_t parent_type,
                                                                       data_classifier_type_t child_type
                                                                     );

/*!
 *  \brief writes a classifier start-element
 *
 *  This starts a division that contains a classifier and a list of features and relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classifier
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_start_classifier( xmi_element_writer_t *this_,
                                         data_classifier_type_t parent_type,
                                         const data_classifier_t *classifier_ptr
                                       );

/*!
 *  \brief writes contents of a classifier 
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classifier
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_assemble_classifier( xmi_element_writer_t *this_,
                                            data_classifier_type_t parent_type,
                                            const data_classifier_t *classifier_ptr
                                          );

/*!
 *  \brief writes a classifier end-element
 *
 *  This ends a division that contains a classifier and a list of features and relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classifier, needed for xmi export
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_end_classifier( xmi_element_writer_t *this_,
                                       data_classifier_type_t parent_type,
                                       const data_classifier_t *classifier_ptr
                                     );

/*!
 *  \brief writes a feature start-element
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the owning parent classifier
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_start_feature( xmi_element_writer_t *this_,
                                      data_classifier_type_t parent_type,
                                      const data_feature_t *feature_ptr
                                    );

/*!
 *  \brief writes constents of a a feature
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the owning parent classifier
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_assemble_feature( xmi_element_writer_t *this_,
                                         data_classifier_type_t parent_type,
                                         const data_feature_t *feature_ptr
                                       );

/*!
 *  \brief writes a feature end-element
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the owning parent classifier
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_end_feature( xmi_element_writer_t *this_,
                                    data_classifier_type_t parent_type,
                                    const data_feature_t *feature_ptr
                                  );

/*!
 *  \brief checks if a parent classifier may nest relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type data_classifier_type_t of the parent of which the nesting-ability shall be determined
 *  \param child_type data_relationship_type_t of the nested child of which the nesting-ability shall be determined
 *  \result true if nesting or any relationship is allowed
 */
static inline bool xmi_element_writer_can_classifier_nest_relationship ( xmi_element_writer_t *this_,
                                                                         data_classifier_type_t parent_type,
                                                                         data_relationship_type_t child_type
                                                                       );

/*!
 *  \brief starts a relationship 
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classifier, needed for xmi export
 *  \param relation_ptr pointer to relationship that shall be written, not NULL
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_start_relationship( xmi_element_writer_t *this_,
                                           data_classifier_type_t parent_type,
                                           const data_relationship_t *relation_ptr
                                         );

/*!
 *  \brief writes the contents of a relationship 
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classifier, needed for xmi export
 *  \param parent_is_source indicates if the parent/hosting classifier is the source end of the relationship
 *  \param relation_ptr pointer to relationship that shall be written, not NULL
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_assemble_relationship( xmi_element_writer_t *this_,
                                              data_classifier_type_t parent_type,
                                              bool parent_is_source,
                                              const data_relationship_t *relation_ptr
                                            );

/*!
 *  \brief ends a relationship 
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classifier, needed for xmi export
 *  \param relation_ptr pointer to relationship that shall be written, not NULL
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_end_relationship( xmi_element_writer_t *this_,
                                         data_classifier_type_t parent_type,
                                         const data_relationship_t *relation_ptr
                                       );

/*!
 *  \brief writes the ending of the main section
 *
 *  This ends a section that contains the main part of the document
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_end_main( xmi_element_writer_t *this_ );

/*!
 *  \brief writes the footer of the document
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_write_footer( xmi_element_writer_t *this_ );

/*!
 *  \brief writes a member end property of a relationship of uml-type association
 *
 *  \param this_ pointer to own object attributes
 *  \param relationship_id id of the relationship
 *  \param relationship_type type of relation, used to determine aggregation kind; none, shared or composite
 *  \param classifier_id id of the classifier at the relationship-end
 *  \param is_target_end true if the target member-end shall be written, 
 *                       false in case of the source member-end
 *  \result 0 in case of success, -1 otherwise
 */
static inline int xmi_element_writer_private_fake_memberend ( xmi_element_writer_t *this_,
                                                              data_id_t relationship_id,
                                                              data_relationship_type_t relationship_type,
                                                              data_id_t classifier_id,
                                                              bool is_target_end
                                                           );

#include "xmi_element_writer.inl"

#endif  /* XMI_ELEMENT_WRITER_H */


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
