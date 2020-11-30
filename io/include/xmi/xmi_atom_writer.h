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
#include "util/string/utf8stringbuf.h"
#include "stream/universal_output_stream.h"

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
 *  \param db_reader pointer to a database reader object (to resolve markdown links)
 *  \param out_writer output stream where to write the generated output to
 */
void xmi_atom_writer_init( xmi_atom_writer_t *this_,
                           data_database_reader_t *db_reader,
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
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_atom_writer_write_xmi_comment( xmi_atom_writer_t *this_,
                                       data_id_t element_id,
                                       const char *comment_type,
                                       const char *comment
                                     );

/*!
 *  \brief encodes a data_id to an xml number (as required for xmi base_Class attribute)
 *
 *  \param this_ pointer to own object attributes
 *  \param element_id id which to encode for xmi
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_atom_writer_encode_xmi_id( xmi_atom_writer_t *this_,
                                   data_id_t element_id
                                 );

/*!
 *  \brief reports an issue as comment in xmi output
 * 
 *  The report consists of three sentences:
 *  A fact that is observed,
 *  a rule that is not adhered,
 *  a solution that is proposed.
 *
 *  \param this_ pointer to own object attributes
 *  \param fact_subject_id the id of actor/parent/container/sender/active object
 *  \param fact_subject_type type of the subject as string
 *  \param fact_relation action/relation of the subject to the object
 *  \param fact_object_id the id of used/related/child/contained/receiving/passive object
 *  \param fact_object_type type of the object as string
 *  \param problem_description english sentence, what is wrong
 *  \param solution_proposal english sentence what to modify, may contain alternatives
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_atom_writer_report_issue( xmi_atom_writer_t *this_,
                                  data_id_t fact_subject_id,
                                  const char* fact_subject_type,
                                  const char* fact_relation,
                                  data_id_t fact_object_id,
                                  const char* fact_object_type,
                                  const char* problem_description,
                                  const char *solution_proposal
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
 *  \param fact_parent_type type of the object as string
 *  \result 0 in case of success, -1 otherwise
 */
int xmi_atom_writer_report_illegal_container( xmi_atom_writer_t *this_,
                                              data_id_t fact_classifier_id,
                                              data_classifier_type_t fact_classifier_type,
                                              data_classifier_type_t fact_parent_type
                                            );

#endif  /* XMI_ATOM_WRITER_H */


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
