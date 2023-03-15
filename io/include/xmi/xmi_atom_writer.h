/* File: xmi_atom_writer.h; Copyright and License: see below */

#ifndef XMI_ATOM_WRITER_H
#define XMI_ATOM_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Writes xml fragments which represent parts of xmi elements to one file output stream.
 *
 *  Source: io_exporter and io_export_model_traversal_t;
 *  Task: convert model-elements to an output stream (using an own xml writer);
 *  Sink: universal_output_stream_t
 */

#include "xml/xml_writer.h"
#include "md/md_filter.h"
#include "storage/data_database_reader.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "u8stream/universal_output_stream.h"
#include "u8/u8_error.h"

/*!
 *  \brief attributes of the xmi_atom_writer_t
 */
struct xmi_atom_writer_struct {
    xml_writer_t *xml_writer;  /*!< pointer to external xml writer */
};

typedef struct xmi_atom_writer_struct xmi_atom_writer_t;

/*!
 *  \brief initializes the xmi_atom_writer_t
 *
 *  \param this_ pointer to own object attributes
 *  \param out_writer output stream where to write the generated output to
 */
void xmi_atom_writer_init( xmi_atom_writer_t *this_,
                           xml_writer_t *out_writer
                         );

/*!
 *  \brief destroys the xmi_atom_writer_t
 *
 *  \param this_ pointer to own object attributes
 */
void xmi_atom_writer_destroy( xmi_atom_writer_t *this_ );

/*!
 *  \brief writes a comment in xmi format
 *
 *  \param this_ pointer to own object attributes
 *  \param element_id id of the element which to write a comment for
 *  \param comment_type type is typically spec - but maybe there are other types in future
 *  \param comment the comment to encode and write
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_atom_writer_write_xmi_comment( xmi_atom_writer_t *this_,
                                              data_id_t element_id,
                                              const char *comment_type,
                                              const char *comment
                                            );

/*!
 *  \brief encodes a data_id to an xml number (as required for xmi base_Class attribute)
 *
 *  \param this_ pointer to own object attributes
 *  \param element_id id which to encode for xmi
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_atom_writer_encode_xmi_id( xmi_atom_writer_t *this_,
                                          data_id_t element_id
                                        );

/*!
 *  \brief reports an issue as comment in xmi output: a classifier has an unknown type
 *
 *  This can happen if the database was created with a newer version of this program.
 *
 *  \param this_ pointer to own object attributes
 *  \param fact_classifier_id the id of classifier object
 *  \param fact_classifier_type type of the classifier object
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_atom_writer_report_unknown_classifier( xmi_atom_writer_t *this_,
                                                      data_id_t fact_classifier_id,
                                                      data_classifier_type_t fact_classifier_type
                                                    );

/*!
 *  \brief reports an issue as comment in xmi output: a feature has an unknown type
 *
 *  This can happen if the database was created with a newer version of this program.
 *
 *  \param this_ pointer to own object attributes
 *  \param fact_feature_id the id of feature object
 *  \param fact_feature_type type of the feature object
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_atom_writer_report_unknown_feature( xmi_atom_writer_t *this_,
                                                   data_id_t fact_feature_id,
                                                   data_feature_type_t fact_feature_type
                                                 );

/*!
 *  \brief reports an issue as comment in xmi output: a relationship has an unknown type
 *
 *  This can happen if the database was created with a newer version of this program.
 *
 *  \param this_ pointer to own object attributes
 *  \param fact_relationship_id the id of relationship object
 *  \param fact_relationship_type type of the relationship object
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_atom_writer_report_unknown_relationship( xmi_atom_writer_t *this_,
                                                        data_id_t fact_relationship_id,
                                                        data_relationship_type_t fact_relationship_type
                                                      );

/*!
 *  \brief reports an issue as comment in xmi output: a container shall not contain the classifier of given type
 *
 *  The report consists of three sentences:
 *  A fact that is observed,
 *  a rule that is not adhered,
 *  a solution that is proposed.
 *
 *  \param this_ pointer to own object attributes
 *  \param fact_classifier_id the id of contained/nested classifier object
 *  \param fact_classifier_type type of the container/host classifier object
 *  \param fact_parent_type type of the container/host object
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_atom_writer_report_illegal_container( xmi_atom_writer_t *this_,
                                                     data_id_t fact_classifier_id,
                                                     data_classifier_type_t fact_classifier_type,
                                                     data_classifier_type_t fact_parent_type
                                                   );

/*!
 *  \brief reports an issue as comment in xmi output: a stereotype has no valid tag-name characters
 *
 *  The report consists of three sentences:
 *  A fact that is observed,
 *  a rule that is not adhered,
 *  a solution that is proposed.
 *
 *  \param this_ pointer to own object attributes
 *  \param element_id the id of the element
 *  \param stereotype stereotype of the element
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_atom_writer_report_illegal_stereotype( xmi_atom_writer_t *this_,
                                                      data_id_t element_id,
                                                      utf8stringview_t stereotype
                                                    );

/*!
 *  \brief reports an issue as comment in xmi output: an element has a datatype/valuetype but is not a typed element
 *
 *  The report consists of three sentences:
 *  A fact that is observed,
 *  a rule that is not adhered,
 *  a solution that is proposed.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_id the id of the feature
 *  \param datatype datatype/valuetype of the feature
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_atom_writer_report_illegal_datatype( xmi_atom_writer_t *this_,
                                                    data_id_t feature_id,
                                                    const char * datatype
                                                  );

/*!
 *  \brief reports an issue as comment in xmi output: a parent shall not contain the feature of given type
 *
 *  The report consists of three sentences:
 *  A fact that is observed,
 *  a rule that is not adhered,
 *  a solution that is proposed.
 *
 *  \param this_ pointer to own object attributes
 *  \param fact_feature_id the id of child feature object
 *  \param fact_feature_type type of the child feature object
 *  \param fact_parent_type type of the parent object
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_atom_writer_report_illegal_parent( xmi_atom_writer_t *this_,
                                                  data_id_t fact_feature_id,
                                                  data_feature_type_t fact_feature_type,
                                                  data_classifier_type_t fact_parent_type
                                                );

/*!
 *  \brief reports an issue as comment in xmi output: the relationship of given type cannot be put to a location
 *
 *  The report consists of three sentences:
 *  A fact that is observed,
 *  a rule that is not adhered,
 *  a solution that is proposed.
 *
 *  \param this_ pointer to own object attributes
 *  \param fact_relationship_id the id of child feature object
 *  \param fact_relationship_type type of the child feature object
 *  \param fact_hosting_type type of the location/host object
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_atom_writer_report_illegal_location( xmi_atom_writer_t *this_,
                                                    data_id_t fact_relationship_id,
                                                    data_relationship_type_t fact_relationship_type,
                                                    data_classifier_type_t fact_hosting_type
                                                  );

/*!
 *  \brief reports an issue as comment in xmi output: the relationship of given type cannot end at given type
 *
 *  The report consists of three sentences:
 *  A fact that is observed,
 *  a rule that is not adhered,
 *  a solution that is proposed.
 *
 *  \param this_ pointer to own object attributes
 *  \param fact_relationship_id the id of child feature object
 *  \param fact_relationship_type type of the child feature object
 *  \param fact_hosting_type type of the location/host object
 *  \param fact_from_end true if the source(from) end, is requested, false if the target(to) end is requested.
 *  \param fact_end_classifier_type the type of classifier at relationship-end
 *  \param fact_end_feature_type the type of feature at relationship-end; DATA_FEATURE_TYPE_VOID if no feature specified
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xmi_atom_writer_report_illegal_relationship_end ( xmi_atom_writer_t *this_,
                                                             data_id_t fact_relationship_id,
                                                             data_relationship_type_t fact_relationship_type,
                                                             data_classifier_type_t fact_hosting_type,
                                                             bool fact_from_end,
                                                             data_classifier_type_t fact_end_classifier_type,
                                                             data_feature_type_t fact_end_feature_type
                                                           );

#endif  /* XMI_ATOM_WRITER_H */


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
