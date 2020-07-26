/* File: io_format_writer.h; Copyright and License: see below */

#ifndef IO_FORMAT_WRITER_H
#define IO_FORMAT_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Writes several document pieces to one file output stream.
 * 
 *  Source: io_export_model_traversal_t;
 *  Task: convert model-elements to an output stream (using an own txt/xml/... writer);
 *  Sink: universal_output_stream_t
 */

#include "io_file_format.h"
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
 *  \brief attributes of the document exporter
 */
struct io_format_writer_struct {
    io_file_format_t export_type;  /*!< format of output document */
    uint32_t current_tree_depth;  /*!< tree depth in diagram tree, starts at 0, increases with every call to io_format_writer_start_diagram */

    txt_writer_t txt_writer;  /*!< own instance of a txt writer */
    xml_writer_t xml_writer;  /*!< own instance of an xml writer */
    xmi_type_converter_t xmi_types;  /*!< own instance of an xmi_type_converter_t */
    md_filter_t md_filter;  /*!< own instance of an md filter */
};

typedef struct io_format_writer_struct io_format_writer_t;

/*!
 *  \brief initializes the document exporter
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 *  \param export_type image file format
 *  \param output output stream where to write the generated output to
 */
void io_format_writer_init( io_format_writer_t *this_,
                            data_database_reader_t *db_reader,
                            io_file_format_t export_type,
                            universal_output_stream_t *output
                          );

/*!
 *  \brief destroys the document exporter
 *
 *  \param this_ pointer to own object attributes
 */
void io_format_writer_destroy( io_format_writer_t *this_ );

/*!
 *  \brief writes the header of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param document_title title of the document
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_write_header( io_format_writer_t *this_, const char *document_title );

/*!
 *  \brief starts a table-of-contents sublist
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_start_toc_sublist ( io_format_writer_t *this_ );

/*!
 *  \brief starts a table-of-contents entry, consisting of an entry and an optional sublist
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_start_toc_entry ( io_format_writer_t *this_ );

/*!
 *  \brief writes a table-of-contents entry
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_ptr pointer to diagram that shall be written
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_write_toc_entry ( io_format_writer_t *this_, const data_diagram_t *diag_ptr );

/*!
 *  \brief end a table-of-contents entry, consisting of an entry and an optional sublist
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_end_toc_entry ( io_format_writer_t *this_ );

/*!
 *  \brief end a table-of-contents sublist
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_end_toc_sublist ( io_format_writer_t *this_ );

/*!
 *  \brief writes a diagram start
 *
 *  This starts a section that contains a diagram and a list of classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_id identifier of the diagram
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_start_diagram( io_format_writer_t *this_, data_id_t diag_id );

/*!
 *  \brief writes a diagram of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_ptr pointer to diagram that shall be written
 *  \param diagram_file_base_name filename of the diagram without extension
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_write_diagram( io_format_writer_t *this_,
                                    const data_diagram_t *diag_ptr,
                                    const char *diagram_file_base_name
                                  );

/*!
 *  \brief writes a classifier start
 *
 *  This starts a division that contains a classifier and a list of features and relationships
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_start_classifier( io_format_writer_t *this_ );

/*!
 *  \brief writes a classifier of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_ptr pointer to classifier that shall be written
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_write_classifier( io_format_writer_t *this_, const data_classifier_t *classifier_ptr );

/*!
 *  \brief writes a feature of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_ptr pointer to feature that shall be written
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_write_feature( io_format_writer_t *this_, const data_feature_t *feature_ptr );

/*!
 *  \brief writes a relationship of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param relation_ptr pointer to relationship that shall be written
 *  \param dest_classifier_ptr pointer to destination classifier
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_write_relationship( io_format_writer_t *this_,
                                         const data_relationship_t *relation_ptr,
                                         const data_classifier_t *dest_classifier_ptr
                                       );

/*!
 *  \brief writes a classifier end
 *
 *  This ends a division that contains a classifier and a list of features and relationships
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_end_classifier( io_format_writer_t *this_ );

/*!
 *  \brief writes a diagram end
 *
 *  This ends a section that contains a diagram and a list of classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_end_diagram( io_format_writer_t *this_ );

/*!
 *  \brief writes a diagram of the document
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_write_footer( io_format_writer_t *this_ );

/*!
 *  \brief writes a css stylesheet file
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_write_stylesheet( io_format_writer_t *this_ );

/*!
 *  \brief writes a comment in xmi format
 *
 *  \param this_ pointer to own object attributes
 *  \param element_id id of the element which to write a comment for
 *  \param comment_type type is typically spec - but maybe there are other types in future
 *  \param comment the comment to encode and write
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_private_write_xmi_comment( io_format_writer_t *this_,
                                                data_id_t element_id,
                                                const char *comment_type,
                                                const char *comment
                                              );

#endif  /* IO_FORMAT_WRITER_H */


/*
Copyright 2019-2020 Andreas Warnke

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
