/* File: io_element_writer.inl; Copyright and License: see below */

static inline void io_element_writer_init( io_element_writer_t *this_,
                                           const io_element_writer_if_t *interface,
                                           void* objectdata )
{
    (*this_).interface = interface;
    (*this_).objectdata = objectdata;
}

static inline int io_element_writer_destroy( io_element_writer_t *this_ )
{
    /*int result = (*(  (*(  (*this_).interface  )).destroy  )) ( (*this_).objectdata );*/
    (*this_).interface = NULL;
    (*this_).objectdata = NULL;
    /*return result;*/
    return 0;
}

static inline const io_element_writer_if_t* io_element_writer_get_interface ( io_element_writer_t *this_ )
{
    return (*this_).interface;
}

static inline void* io_element_writer_get_objectdata ( io_element_writer_t *this_ )
{
    return (*this_).objectdata;
}

static inline int io_element_writer_write_header( io_element_writer_t *this_, const char *document_title )
{
    return 0;
}

static inline int io_element_writer_start_main( io_element_writer_t *this_, const char *document_title )
{
    return 0;
}

static inline bool io_element_writer_can_classifier_nest_classifier( io_element_writer_t *this_,
                                                                     data_classifier_type_t parent_type,
                                                                     data_classifier_type_t child_type )
{
    return false;
}

static inline bool io_element_writer_can_classifier_nest_relationship( io_element_writer_t *this_,
                                                                       data_classifier_type_t parent_type,
                                                                       data_relationship_type_t child_type )
{
    return false;
}

static inline int io_element_writer_start_classifier( io_element_writer_t *this_,
                                                      data_classifier_type_t parent_type,
                                                      const data_classifier_t *classifier_ptr
                                                    )
{
    return 0;
}

static inline int io_element_writer_assemble_classifier( io_element_writer_t *this_,
                                                         data_classifier_type_t parent_type,
                                                         const data_classifier_t *classifier_ptr )
{
    return 0;
}

static inline int io_element_writer_end_classifier( io_element_writer_t *this_,
                                                    data_classifier_type_t parent_type,
                                                    const data_classifier_t *classifier_ptr )
{
    return 0;
}

static inline int io_element_writer_start_feature( io_element_writer_t *this_,
                                                   data_classifier_type_t parent_type,
                                                   const data_feature_t *feature_ptr )
{
    return 0;
}

static inline int io_element_writer_assemble_feature( io_element_writer_t *this_,
                                                      data_classifier_type_t parent_type,
                                                      const data_feature_t *feature_ptr )
{
    return 0;
}

static inline int io_element_writer_end_feature( io_element_writer_t *this_,
                                                 data_classifier_type_t parent_type,
                                                 const data_feature_t *feature_ptr )
{
    return 0;
}

static inline int io_element_writer_start_relationship( io_element_writer_t *this_,
                                                        data_classifier_type_t parent_type,
                                                        const data_relationship_t *relation_ptr )
{
    return 0;
}

static inline int io_element_writer_assemble_relationship( io_element_writer_t *this_,
                                                           data_classifier_type_t parent_type,
                                                           bool parent_is_source,
                                                           const data_relationship_t *relation_ptr,
                                                           data_classifier_type_t from_c_type,
                                                           data_feature_type_t from_f_type,
                                                           data_classifier_type_t to_c_type,
                                                           data_feature_type_t to_f_type )
{
    return 0;
}

static inline int io_element_writer_end_relationship( io_element_writer_t *this_,
                                                      data_classifier_type_t parent_type,
                                                      const data_relationship_t *relation_ptr )
{
    return 0;
}

static inline int io_element_writer_start_diagram( io_element_writer_t *this_, const data_diagram_t *diag_ptr )
{
    return 0;
}

static inline int io_element_writer_assemble_diagram( io_element_writer_t *this_,
                                       const data_diagram_t *diag_ptr,
                                       const char *diagram_file_base_name )
{
    return 0;
}

static inline int io_element_writer_end_diagram( io_element_writer_t *this_, const data_diagram_t *diag_ptr )
{
    return 0;
}

static inline int io_element_writer_start_diagramelement( io_element_writer_t *this_,
                                                          data_diagram_type_t parent_type,
                                                          const data_diagramelement_t *diagramelement_ptr )
{
    return 0;
}

static inline int io_element_writer_assemble_diagramelement( io_element_writer_t *this_,
                                                             data_diagram_type_t parent_type,
                                                             const data_diagramelement_t *diagramelement_ptr )
{
    return 0;
}

static inline int io_element_writer_end_diagramelement( io_element_writer_t *this_,
                                                        data_diagram_type_t parent_type,
                                                        const data_diagramelement_t *diagramelement_ptr )
{
    return 0;
}

static inline int io_element_writer_end_main( io_element_writer_t *this_ )
{
    return 0;
}

static inline int io_element_writer_write_footer( io_element_writer_t *this_ )
{
    return 0;
}


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
