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
#include "xmi/xmi_type_converter.h"
#include "xmi/xmi_interaction_writer.h"
#include "xmi/xmi_writer_pass.h"
#include "format/io_xml_writer.h"
#include "io_file_format.h"
#include "io_element_writer_if.h"
#include "io_element_writer.h"
#include "data_diagram.h"
#include "data_classifier.h"
#include "data_classifier_type.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "data_relationship_type.h"
#include "set/data_visible_set.h"
#include "set/data_stat.h"
#include "storage/data_database_reader.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "u8stream/universal_output_stream.h"
#include "u8/u8_error.h"

/*!
 *  \brief attributes of the xmi_element_writer_t
 */
struct xmi_element_writer_struct {
    io_element_writer_t element_writer;  /*!< instance of implemented interface \c io_element_writer_t */
    xmi_writer_pass_t mode;  /*!< depending on the mode, conversion from a data object to the output format differs */

    xmi_interaction_writer_t interaction_writer;  /*!< instance of own xmi_interaction_writer */
                                                  /*!< which is the interaction output sink, */
                                                  /*!< replaces this xmi_element_writer_t in case of interactions */

    data_stat_t *export_stat;  /*!< pointer to external statistics object where export statistics are collected */
    io_xml_writer_t xml_writer;  /*!< own instance of an xml writer */
    xmi_atom_writer_t atom_writer;  /*!< own instance of an atom writer */

    xmi_type_converter_t xmi_types;  /*!< own instance of an xmi_type_converter_t */
};

typedef struct xmi_element_writer_struct xmi_element_writer_t;

/*!
 *  \brief initializes the xmi_element_writer_t
 *
 *  \param this_ pointer to own object attributes
 *  \param io_export_stat pointer to statistics object where export statistics are collected
 *  \param output output stream where to write the generated output to
 */
void xmi_element_writer_init( xmi_element_writer_t *this_,
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
 *  \brief gets the io element writer interface of this xmi_element_writer_t
 *
 *  \param this_ pointer to own object attributes
 *  \return the abstract base class of this_
 */
io_element_writer_t * xmi_element_writer_get_element_writer( xmi_element_writer_t *this_ );

/*!
 *  \brief sets the conversion mode of the xmi_element_writer_t
 *
 *  \param this_ pointer to own object attributes
 *  \param mode mode how to convert a data object to the output format, e.g. uml-basic or profile-extension
 */
void xmi_element_writer_set_mode( xmi_element_writer_t *this_, xmi_writer_pass_t mode );

/*!
 *  \brief writes the header of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param document_title title of the document
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_element_writer_write_header( xmi_element_writer_t *this_, const char *document_title );

/*!
 *  \brief writes the start of the main section
 *
 *  This starts a section that contains the main part of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param document_title title of the document
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_element_writer_start_main( xmi_element_writer_t *this_, const char *document_title );

/*!
 *  \brief checks if a host classifier may nest a child classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type data_classifier_type_t of the host of which the nesting-ability shall be determined
 *  \param child_type data_classifier_type_t of the nested child of which the nesting-ability shall be determined
 *  \return true if nesting is allowed
 */
bool xmi_element_writer_can_classifier_nest_classifier ( xmi_element_writer_t *this_,
                                                         data_classifier_type_t host_type,
                                                         data_classifier_type_t child_type
                                                       );

/*!
 *  \brief checks if a host classifier may nest relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type data_classifier_type_t of the host of which the nesting-ability shall be determined
 *  \param child_type data_relationship_type_t of the nested child of which the nesting-ability shall be determined
 *  \return true if nesting or any relationship is allowed
 */
bool xmi_element_writer_can_classifier_nest_relationship ( xmi_element_writer_t *this_,
                                                           data_classifier_type_t host_type,
                                                           data_relationship_type_t child_type
                                                         );

/*!
 *  \brief writes a classifier start-element
 *
 *  This starts a division that contains a classifier and a list of features and relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the host classifier
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_element_writer_start_classifier( xmi_element_writer_t *this_,
                                                data_classifier_type_t host_type,
                                                const data_classifier_t *classifier_ptr
                                              );

/*!
 *  \brief writes contents of a classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the host classifier
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_element_writer_assemble_classifier( xmi_element_writer_t *this_,
                                                   data_classifier_type_t host_type,
                                                   const data_classifier_t *classifier_ptr
                                                 );

/*!
 *  \brief writes a classifier end-element
 *
 *  This ends a division that contains a classifier and a list of features and relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the host classifier, needed for xmi export
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_element_writer_end_classifier( xmi_element_writer_t *this_,
                                              data_classifier_type_t host_type,
                                              const data_classifier_t *classifier_ptr
                                            );

/*!
 *  \brief writes a feature start-element
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the owning host classifier
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_element_writer_start_feature( xmi_element_writer_t *this_,
                                             data_classifier_type_t parent_type,
                                             const data_feature_t *feature_ptr
                                           );

/*!
 *  \brief writes constents of a a feature
 *
 *  \param this_ pointer to own object attributes
 *  \param parent the owning host classifier
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_element_writer_assemble_feature( xmi_element_writer_t *this_,
                                                const data_classifier_t *parent,
                                                const data_feature_t *feature_ptr
                                              );

/*!
 *  \brief writes a feature end-element
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the owning host classifier
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_element_writer_end_feature( xmi_element_writer_t *this_,
                                           data_classifier_type_t parent_type,
                                           const data_feature_t *feature_ptr
                                         );

/*!
 *  \brief starts a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the host classifier, needed for xmi export
 *  \param relation_ptr pointer to relationship that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_element_writer_start_relationship( xmi_element_writer_t *this_,
                                                  data_classifier_type_t host_type,
                                                  const data_relationship_t *relation_ptr
                                                );

/*!
 *  \brief writes the contents of a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param host the host classifier, needed for xmi export
 *  \param relation_ptr pointer to relationship that shall be written, not NULL
 *  \param from_c the classifier at source end
 *  \param from_f the feature at source end; NULL if no feature specified
 *  \param to_c the classifier at target end
 *  \param to_f the feature at target end; NULL if no feature specified
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_element_writer_assemble_relationship( xmi_element_writer_t *this_,
                                                     const data_classifier_t *host,
                                                     const data_relationship_t *relation_ptr,
                                                     const data_classifier_t *from_c,
                                                     const data_feature_t *from_f,
                                                     const data_classifier_t *to_c,
                                                     const data_feature_t *to_f
                                                   );

/*!
 *  \brief writes the contents of a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the host classifier, needed for xmi export
 *  \param host_is_source indicates if the host classifier is the source end of the relationship
 *  \param host_id id of the host classifier, needed for xmi-interaction export
 *  \param relation_ptr pointer to relationship that shall be written, not NULL
 *  \param from_c_type the type of classifier at source end
 *  \param from_f_type the type of feature at source end; DATA_FEATURE_TYPE_VOID if no feature specified
 *  \param to_c_type the type of classifier at target end
 *  \param to_f_type the type of feature at target end; DATA_FEATURE_TYPE_VOID if no feature specified
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_element_writer_private_assemble_relationship( xmi_element_writer_t *this_,
                                                             data_classifier_type_t host_type,
                                                             bool host_is_source,
                                                             data_id_t host_id,
                                                             const data_relationship_t *relation_ptr,
                                                             data_classifier_type_t from_c_type,
                                                             data_feature_type_t from_f_type,
                                                             data_classifier_type_t to_c_type,
                                                             data_feature_type_t to_f_type
                                                           );

/*!
 *  \brief ends a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the host classifier, needed for xmi export
 *  \param relation_ptr pointer to relationship that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_element_writer_end_relationship( xmi_element_writer_t *this_,
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
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_NOT_YET_IMPLEMENTED if not implemented.
 */
u8_error_t xmi_element_writer_start_diagram( xmi_element_writer_t *this_, const data_diagram_t *diag_ptr );

/*!
 *  \brief writes a diagram of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param parent pointer to parent diagram or NULL in case of root
 *  \param diag_ptr pointer to diagram that shall be written
 *  \param diagram_file_base_name filename of the diagram without extension
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_NOT_YET_IMPLEMENTED if not implemented.
 */
u8_error_t xmi_element_writer_assemble_diagram( xmi_element_writer_t *this_,
                                                const data_diagram_t *parent,
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
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_NOT_YET_IMPLEMENTED if not implemented.
 */
u8_error_t xmi_element_writer_end_diagram( xmi_element_writer_t *this_, const data_diagram_t *diag_ptr );

/*!
 *  \brief writes a diagramelement start-element
 *
 *  \param this_ pointer to own object attributes
 *  \param parent the hosting diagram
 *  \param diagramelement_ptr pointer to diagramelement that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_NOT_YET_IMPLEMENTED if not implemented.
 */
u8_error_t xmi_element_writer_start_diagramelement( xmi_element_writer_t *this_,
                                                    const data_diagram_t *parent,
                                                    const data_diagramelement_t *diagramelement_ptr
                                                  );

/*!
 *  \brief writes constents of a a diagramelement
 *
 *  \param this_ pointer to own object attributes
 *  \param parent the hosting diagram
 *  \param diagramelement_ptr pointer to diagramelement that shall be written, not NULL
 *  \param occurrence the occurring classifier
 *  \param feat_occur the focused feature of the occurring classifier
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_NOT_YET_IMPLEMENTED if not implemented.
 */
u8_error_t xmi_element_writer_assemble_diagramelement( xmi_element_writer_t *this_,
                                                       const data_diagram_t *parent,
                                                       const data_diagramelement_t *diagramelement_ptr,
                                                       const data_classifier_t *occurrence,
                                                       const data_feature_t *feat_occur
                                                     );

/*!
 *  \brief writes a diagramelement end-element
 *
 *  \param this_ pointer to own object attributes
 *  \param parent the hosting diagram
 *  \param diagramelement_ptr pointer to diagramelement that shall be written, not NULL
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_NOT_YET_IMPLEMENTED if not implemented.
 */
u8_error_t xmi_element_writer_end_diagramelement( xmi_element_writer_t *this_,
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
u8_error_t xmi_element_writer_end_main( xmi_element_writer_t *this_ );

/*!
 *  \brief writes the footer of the document
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_element_writer_write_footer( xmi_element_writer_t *this_ );

/*!
 *  \brief writes a member end property of a relationship of uml-type association
 *
 *  \param this_ pointer to own object attributes
 *  \param relationship_id id of the relationship
 *  \param relationship_type type of relation, used to determine aggregation kind; none, shared or composite
 *  \param end_object_id id of the classifier or feature at the relationship-end
 *  \param end_classifier_type the type of classifier at relationship end
 *  \param end_feature_type the type of feature at relationship end; DATA_FEATURE_TYPE_VOID if no feature specified
 *  \param is_target_end true if the target member-end shall be written,
 *                       false in case of the source member-end
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_element_writer_private_fake_memberend ( xmi_element_writer_t *this_,
                                                       data_id_t relationship_id,
                                                       data_relationship_type_t relationship_type,
                                                       data_id_t end_object_id,
                                                       data_classifier_type_t end_classifier_type,
                                                       data_feature_type_t end_feature_type,
                                                       bool is_target_end
                                                     );

/*!
 *  \brief writes a stereotype start-element
 *
 *  This starts a division that represents a stereotype instance
 *
 *  \param this_ pointer to own object attributes
 *  \param profile_ns namespace of the stereotype, e.g. XMI_XML_NS_SYSML
 *  \param profile_type type name of the stereotype
 *  \param base_type type name of the UML base type (which is extended by this stereotype)
 *  \param element_id the id of the element for which a stereotype is written
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_element_writer_private_start_stereotype( xmi_element_writer_t *this_,
                                                        const char* profile_ns,
                                                        const utf8stringview_t profile_type,
                                                        const char* base_type,
                                                        data_id_t element_id
                                                      );

/*!
 *  \brief writes a stereotype end-element
 *
 *  This ends a division that represents a stereotype instance
 *
 *  \param this_ pointer to own object attributes
 *  \param profile_ns namespace of the stereotype, e.g. XMI_XML_NS_SYSML
 *  \param profile_type type of the stereotype
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_element_writer_private_end_stereotype( xmi_element_writer_t *this_,
                                                      const char* profile_ns,
                                                      const utf8stringview_t profile_type
                                                    );

#endif  /* XMI_ELEMENT_WRITER_H */


/*
Copyright 2020-2023 Andreas Warnke

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
