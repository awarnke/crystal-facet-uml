/* File: io_element_writer_if.h; Copyright and License: see below */

    u8_error_t (*write_header)( io_element_writer_impl_t *this_, const char *document_title );  /*!< \see io_element_writer_t */
    u8_error_t (*start_main)( io_element_writer_impl_t *this_, const char *document_title );  /*!< \see io_element_writer_t */
    bool (*can_classifier_nest_classifier)( io_element_writer_impl_t *this_,
                                            data_classifier_type_t host_type,
                                            data_classifier_type_t child_type
                                          );  /*!< \see io_element_writer_t */
    bool (*can_classifier_nest_relationship)( io_element_writer_impl_t *this_,
                                              data_classifier_type_t host_type,
                                              data_relationship_type_t child_type
                                            );  /*!< \see io_element_writer_t */
    u8_error_t (*start_classifier)( io_element_writer_impl_t *this_,
                                    data_classifier_type_t host_type,
                                    const data_classifier_t *classifier_ptr
                                  );  /*!< \see io_element_writer_t */
    u8_error_t (*assemble_classifier)( io_element_writer_impl_t *this_,
                                       data_classifier_type_t host_type,
                                       const data_classifier_t *classifier_ptr
                                     );  /*!< \see io_element_writer_t */
    u8_error_t (*end_classifier)( io_element_writer_impl_t *this_,
                                  data_classifier_type_t host_type,
                                  const data_classifier_t *classifier_ptr
                                );  /*!< \see io_element_writer_t */
    u8_error_t (*start_feature)( io_element_writer_impl_t *this_,
                                 data_classifier_type_t parent_type,
                                 const data_feature_t *feature_ptr
                               );  /*!< \see io_element_writer_t */
    u8_error_t (*assemble_feature)( io_element_writer_impl_t *this_,
                                     const data_classifier_t *parent,
                                     const data_feature_t *feature_ptr
                                  );  /*!< \see io_element_writer_t */
    u8_error_t (*end_feature)( io_element_writer_impl_t *this_,
                               data_classifier_type_t parent_type,
                               const data_feature_t *feature_ptr
                             );  /*!< \see io_element_writer_t */
    u8_error_t (*start_relationship)( io_element_writer_impl_t *this_,
                                      data_classifier_type_t host_type,
                                      const data_relationship_t *relation_ptr
                                    );  /*!< \see io_element_writer_t */
    u8_error_t (*assemble_relationship)( io_element_writer_impl_t *this_,
                                         const data_classifier_t *host,
                                         const data_relationship_t *relation_ptr,
                                         const data_classifier_t *from_c,
                                         const data_feature_t *from_f,
                                         const data_classifier_t *to_c,
                                         const data_feature_t *to_f
                                       );  /*!< \see io_element_writer_t */
    u8_error_t (*end_relationship)( io_element_writer_impl_t *this_,
                                    data_classifier_type_t host_type,
                                    const data_relationship_t *relation_ptr
                                  );  /*!< \see io_element_writer_t */
    u8_error_t (*start_diagram)( io_element_writer_impl_t *this_, const data_diagram_t *diag_ptr );  /*!< \see io_element_writer_t */
    u8_error_t (*assemble_diagram)( io_element_writer_impl_t *this_,
                                    const data_diagram_t *parent,
                                    const data_diagram_t *diag_ptr,
                                    const char *diagram_file_base_name
                                  );  /*!< \see io_element_writer_t */
    u8_error_t (*end_diagram)( io_element_writer_impl_t *this_, const data_diagram_t *diag_ptr );  /*!< \see io_element_writer_t */
    u8_error_t (*start_diagramelement)( io_element_writer_impl_t *this_,
                                        const data_diagram_t *host,
                                        const data_diagramelement_t *diagramelement_ptr
                                      );  /*!< \see io_element_writer_t */
    u8_error_t (*assemble_diagramelement)( io_element_writer_impl_t *this_,
                                           const data_diagram_t *host,
                                           const data_diagramelement_t *diagramelement_ptr,
                                           const data_classifier_t *occurrence,
                                           const data_feature_t *feat_occur
                                         );  /*!< \see io_element_writer_t */
    u8_error_t (*end_diagramelement)( io_element_writer_impl_t *this_,
                                      const data_diagram_t *host,
                                      const data_diagramelement_t *diagramelement_ptr
                                    );  /*!< \see io_element_writer_t */
    u8_error_t (*end_main)( io_element_writer_impl_t *this_ );  /*!< \see io_element_writer_t */
    u8_error_t (*write_footer)( io_element_writer_impl_t *this_ );  /*!< \see io_element_writer_t */


/*
Copyright 2021-2025 Andreas Warnke

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
