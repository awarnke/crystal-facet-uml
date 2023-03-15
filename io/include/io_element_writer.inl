/* File: io_element_writer.inl; Copyright and License: see below */

#include <assert.h>

static inline void io_element_writer_private_init( io_element_writer_t *this_,
                                                   const io_element_writer_if_t *interface,
                                                   io_element_writer_impl_t* objectdata )
{
    assert( interface != NULL );
    assert( objectdata != NULL );
    (*this_).interface = interface;
    (*this_).objectdata = objectdata;
}

static inline u8_error_t io_element_writer_private_destroy( io_element_writer_t *this_ )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    /*u8_error_t result = (*(  (*(  (*this_).interface  )).destroy  )) ( (*this_).objectdata );*/
    (*this_).interface = NULL;
    (*this_).objectdata = NULL;
    /*return result;*/
    return 0;
}

static inline const io_element_writer_if_t* io_element_writer_get_interface ( io_element_writer_t *this_ )
{
    assert( (*this_).interface != NULL );
    return (*this_).interface;
}

static inline io_element_writer_impl_t* io_element_writer_get_objectdata ( io_element_writer_t *this_ )
{
    assert( (*this_).objectdata != NULL );
    return (*this_).objectdata;
}

static inline u8_error_t io_element_writer_write_header( io_element_writer_t *this_, const char *document_title )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).write_header != NULL );
    return (*(  (*((*this_).interface)).write_header  )) ( (*this_).objectdata, document_title );
}

static inline u8_error_t io_element_writer_start_main( io_element_writer_t *this_, const char *document_title )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).start_main != NULL );
    return (*(  (*((*this_).interface)).start_main  )) ( (*this_).objectdata, document_title );
}

static inline bool io_element_writer_can_classifier_nest_classifier( io_element_writer_t *this_,
                                                                     data_classifier_type_t host_type,
                                                                     data_classifier_type_t child_type )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).can_classifier_nest_classifier != NULL );
    return (*(  (*((*this_).interface)).can_classifier_nest_classifier  )) ( (*this_).objectdata, host_type, child_type );
}

static inline bool io_element_writer_can_classifier_nest_relationship( io_element_writer_t *this_,
                                                                       data_classifier_type_t host_type,
                                                                       data_relationship_type_t child_type )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).can_classifier_nest_relationship != NULL );
    return (*(  (*((*this_).interface)).can_classifier_nest_relationship  )) ( (*this_).objectdata, host_type, child_type );
}

static inline u8_error_t io_element_writer_start_classifier( io_element_writer_t *this_,
                                                             data_classifier_type_t host_type,
                                                             const data_classifier_t *classifier_ptr
                                                           )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).start_classifier != NULL );
    return (*(  (*((*this_).interface)).start_classifier  )) ( (*this_).objectdata, host_type, classifier_ptr );
}

static inline u8_error_t io_element_writer_assemble_classifier( io_element_writer_t *this_,
                                                                data_classifier_type_t host_type,
                                                                const data_classifier_t *classifier_ptr )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).assemble_classifier != NULL );
    return (*(  (*((*this_).interface)).assemble_classifier  )) ( (*this_).objectdata, host_type, classifier_ptr );
}

static inline u8_error_t io_element_writer_end_classifier( io_element_writer_t *this_,
                                                           data_classifier_type_t host_type,
                                                           const data_classifier_t *classifier_ptr )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).end_classifier != NULL );
    return (*(  (*((*this_).interface)).end_classifier  )) ( (*this_).objectdata, host_type, classifier_ptr );
}

static inline u8_error_t io_element_writer_start_feature( io_element_writer_t *this_,
                                                          data_classifier_type_t parent_type,
                                                          const data_feature_t *feature_ptr )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).start_feature != NULL );
    return (*(  (*((*this_).interface)).start_feature  )) ( (*this_).objectdata, parent_type, feature_ptr );
}

static inline u8_error_t io_element_writer_assemble_feature( io_element_writer_t *this_,
                                                             const data_classifier_t *parent,
                                                             const data_feature_t *feature_ptr )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).assemble_feature != NULL );
    return (*(  (*((*this_).interface)).assemble_feature  )) ( (*this_).objectdata, parent, feature_ptr );
}

static inline u8_error_t io_element_writer_end_feature( io_element_writer_t *this_,
                                                        data_classifier_type_t parent_type,
                                                        const data_feature_t *feature_ptr )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).end_feature != NULL );
    return (*(  (*((*this_).interface)).end_feature  )) ( (*this_).objectdata, parent_type, feature_ptr );
}

static inline u8_error_t io_element_writer_start_relationship( io_element_writer_t *this_,
                                                               data_classifier_type_t host_type,
                                                               const data_relationship_t *relation_ptr )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).start_relationship != NULL );
    return (*(  (*((*this_).interface)).start_relationship  )) ( (*this_).objectdata, host_type, relation_ptr );
}

static inline u8_error_t io_element_writer_assemble_relationship( io_element_writer_t *this_,
                                                                  const data_classifier_t *host,
                                                                  const data_relationship_t *relation_ptr,
                                                                  const data_classifier_t *from_c,
                                                                  const data_feature_t *from_f,
                                                                  const data_classifier_t *to_c,
                                                                  const data_feature_t *to_f )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).assemble_relationship != NULL );
    return (*(  (*((*this_).interface)).assemble_relationship  )) ( (*this_).objectdata,
                                                                    host,
                                                                    relation_ptr,
                                                                    from_c,
                                                                    from_f,
                                                                    to_c,
                                                                    to_f
                                                                  );
}

static inline u8_error_t io_element_writer_end_relationship( io_element_writer_t *this_,
                                                             data_classifier_type_t host_type,
                                                             const data_relationship_t *relation_ptr )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).end_relationship != NULL );
    return (*(  (*((*this_).interface)).end_relationship  )) ( (*this_).objectdata, host_type, relation_ptr );
}

static inline u8_error_t io_element_writer_start_diagram( io_element_writer_t *this_, const data_diagram_t *diag_ptr )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).start_diagram != NULL );
    return (*(  (*((*this_).interface)).start_diagram  )) ( (*this_).objectdata, diag_ptr );
}

static inline u8_error_t io_element_writer_assemble_diagram( io_element_writer_t *this_,
                                                             const data_diagram_t *parent,
                                                             const data_diagram_t *diag_ptr,
                                                             const char *diagram_file_base_name )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).assemble_diagram != NULL );
    return (*(  (*((*this_).interface)).assemble_diagram  )) ( (*this_).objectdata, parent, diag_ptr, diagram_file_base_name );
}

static inline u8_error_t io_element_writer_end_diagram( io_element_writer_t *this_, const data_diagram_t *diag_ptr )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).end_diagram != NULL );
    return (*(  (*((*this_).interface)).end_diagram  )) ( (*this_).objectdata, diag_ptr );
}

static inline u8_error_t io_element_writer_start_diagramelement( io_element_writer_t *this_,
                                                                 const data_diagram_t *parent,
                                                                 const data_diagramelement_t *diagramelement_ptr )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).start_diagramelement != NULL );
    return (*(  (*((*this_).interface)).start_diagramelement  )) ( (*this_).objectdata, parent, diagramelement_ptr );
}

static inline u8_error_t io_element_writer_assemble_diagramelement( io_element_writer_t *this_,
                                                                    const data_diagram_t *parent,
                                                                    const data_diagramelement_t *diagramelement_ptr,
                                                                    const data_classifier_t *occurrence,
                                                                    const data_feature_t *feat_occur )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).assemble_diagramelement != NULL );
    return (*(  (*((*this_).interface)).assemble_diagramelement  )) ( (*this_).objectdata,
                                                                      parent,
                                                                      diagramelement_ptr,
                                                                      occurrence,
                                                                      feat_occur
                                                                    );
}

static inline u8_error_t io_element_writer_end_diagramelement( io_element_writer_t *this_,
                                                               const data_diagram_t *parent,
                                                               const data_diagramelement_t *diagramelement_ptr )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).end_diagramelement != NULL );
    return (*(  (*((*this_).interface)).end_diagramelement  )) ( (*this_).objectdata, parent, diagramelement_ptr );
}

static inline u8_error_t io_element_writer_end_main( io_element_writer_t *this_ )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).end_main != NULL );
    return (*(  (*((*this_).interface)).end_main  )) ( (*this_).objectdata );
}

static inline u8_error_t io_element_writer_write_footer( io_element_writer_t *this_ )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).write_footer != NULL );
    return (*(  (*((*this_).interface)).write_footer  )) ( (*this_).objectdata );
}


/*
Copyright 2021-2023 Andreas Warnke

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
