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

#include "io_file_format.h"
#include "io_writer_pass.h"
#include "xml/xml_writer.h"
#include "txt/txt_writer.h"
#include "xmi/xmi_type_converter.h"
#include "md/md_filter.h"
#include "data_diagram.h"
#include "data_classifier.h"
#include "set/data_visible_set.h"
#include "storage/data_database_reader.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/string/utf8stringbuf.h"
#include "stream/universal_output_stream.h"

/*!
 *  \brief attributes of the format writer
 */
struct xmi_element_writer_struct {
    io_file_format_t export_type;  /*!< format of output document */
    io_writer_pass_t mode;  /*!< depending on the mode, conversion from a data object to the output format differs */

    xml_writer_t xml_writer;  /*!< own instance of an xml writer */
    xmi_type_converter_t xmi_types;  /*!< own instance of an xmi_type_converter_t */
};

typedef struct xmi_element_writer_struct xmi_element_writer_t;

/*!
 *  \brief initializes the format writer
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 *  \param export_type image file format
 *  \param output output stream where to write the generated output to
 */
void xmi_element_writer_init( xmi_element_writer_t *this_,
                            data_database_reader_t *db_reader,
                            io_file_format_t export_type,
                            universal_output_stream_t *output
                          );

/*!
 *  \brief destroys the format writer
 *
 *  \param this_ pointer to own object attributes
 */
void xmi_element_writer_destroy( xmi_element_writer_t *this_ );

/*!
 *  \brief sets the conversion mode of the format writer
 *
 *  \param this_ pointer to own object attributes
 *  \param mode mode how to convert a data object to the output format, e.g. uml-basic or profile-extension
 */
void xmi_element_writer_set_mode( xmi_element_writer_t *this_, io_writer_pass_t mode );

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
 *  \brief writes a classifier start
 *
 *  This starts a division that contains a classifier and a list of features and relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classifier, needed for xmi export
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_start_nested_classifier( xmi_element_writer_t *this_,
                                              data_classifier_type_t parent_type,
                                              const data_classifier_t *classifier_ptr
                                            );

/*!
 *  \brief writes a classifier of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_write_classifier( xmi_element_writer_t *this_, const data_classifier_t *classifier_ptr );

/*!
 *  \brief writes a feature of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_write_feature( xmi_element_writer_t *this_, const data_feature_t *feature_ptr );

/*!
 *  \brief writes a relationship of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param relation_ptr pointer to relationship that shall be written, not NULL
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_write_relationship( xmi_element_writer_t *this_,
                                           const data_relationship_t *relation_ptr
                                         );

/*!
 *  \brief writes a classifier end
 *
 *  This ends a division that contains a classifier and a list of features and relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classiier, needed for xmi export
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_end_nested_classifier( xmi_element_writer_t *this_,
                                            data_classifier_type_t parent_type,
                                            const data_classifier_t *classifier_ptr
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
 *  \brief writes a comment in xmi format
 *
 *  \param this_ pointer to own object attributes
 *  \param element_id id of the element which to write a comment for
 *  \param comment_type type is typically spec - but maybe there are other types in future
 *  \param comment the comment to encode and write
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_private_write_xmi_comment( xmi_element_writer_t *this_,
                                                  data_id_t element_id,
                                                  const char *comment_type,
                                                  const char *comment
                                                );

/*!
 *  \brief encodes a data_id to an xml number as required for xmi base_Class attribute
 *
 *  \param this_ pointer to own object attributes
 *  \param element_id id which to encode for xmi
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_element_writer_private_encode_xmi_id( xmi_element_writer_t *this_,
                                              data_id_t element_id
                                            );

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
