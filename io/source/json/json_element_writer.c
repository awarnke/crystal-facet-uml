/* File: json_element_writer.inl; Copyright and License: see below */

#include <json/json_element_writer.h>
#include <assert.h>

/* define a struct where the function pointers have the exact right signatures to avoid typecasts */
#define io_element_writer_impl_t json_element_writer_t
struct json_element_writer_io_element_writer_if_struct {
#include "io_element_writer_if.inl"
};
#undef io_element_writer_impl_t

/* the vmt implementing the interface */
static const struct json_element_writer_io_element_writer_if_struct json_element_writer_private_io_element_writer_if
    = {
        .write_header = &json_element_writer_write_header,
        .start_main = &json_element_writer_start_main,
        .can_classifier_nest_classifier = &json_element_writer_can_classifier_nest_classifier,
        .can_classifier_nest_relationship = &json_element_writer_can_classifier_nest_relationship,
        .start_classifier = &json_element_writer_start_classifier,
        .assemble_classifier = &json_element_writer_assemble_classifier,
        .end_classifier = &json_element_writer_end_classifier,
        .start_feature = &json_element_writer_start_feature,
        .assemble_feature = &json_element_writer_assemble_feature,
        .end_feature = &json_element_writer_end_feature,
        .start_relationship = &json_element_writer_start_relationship,
        .assemble_relationship = &json_element_writer_assemble_relationship,
        .end_relationship = &json_element_writer_end_relationship,
        .start_diagram = &json_element_writer_start_diagram,
        .assemble_diagram = &json_element_writer_assemble_diagram,
        .end_diagram = &json_element_writer_end_diagram,
        .start_diagramelement = &json_element_writer_start_diagramelement,
        .assemble_diagramelement = &json_element_writer_assemble_diagramelement,
        .end_diagramelement = &json_element_writer_end_diagramelement,
        .end_main = &json_element_writer_end_main,
        .write_footer = &json_element_writer_write_footer
    };

void json_element_writer_init( json_element_writer_t *this_,
                               data_stat_t *io_export_stat,
                               universal_output_stream_t *output)
{
    TRACE_BEGIN();
    assert( io_export_stat != NULL );
    assert( output != NULL );

    io_element_writer_private_init( &((*this_).element_writer),
                                    (io_element_writer_if_t*) &json_element_writer_private_io_element_writer_if,
                                    this_
                                  );
    json_serializer_init( &((*this_).serializer), output );
    (*this_).mode = JSON_WRITER_PASS_NODES;
    (*this_).export_stat = io_export_stat;

    TRACE_END();
}

void json_element_writer_destroy( json_element_writer_t *this_ )
{
    TRACE_BEGIN();

    json_serializer_destroy( &((*this_).serializer) );
    io_element_writer_private_destroy( &((*this_).element_writer) );
    (*this_).export_stat = NULL;

    TRACE_END();
}

io_element_writer_t * json_element_writer_get_element_writer( json_element_writer_t *this_ )
{
    TRACE_BEGIN();

    io_element_writer_t * base = &((*this_).element_writer);

    TRACE_END();
    return base;
}

void json_element_writer_set_mode( json_element_writer_t *this_, json_writer_pass_t mode )
{
    TRACE_BEGIN();

    (*this_).mode = mode;

    TRACE_END();
}

int json_element_writer_write_header( json_element_writer_t *this_, const char *document_title )
{
    TRACE_BEGIN();
    assert( document_title != NULL );
    int out_err = 0;

    out_err |= json_serializer_write_header( &((*this_).serializer) );

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_start_main( json_element_writer_t *this_, const char *document_title )
{
    TRACE_BEGIN();
    assert( document_title != NULL );
    int out_err = 0;

    switch( (*this_).mode )
    {
        case JSON_WRITER_PASS_VIEWS:
        {
            out_err |= json_serializer_begin_section( &((*this_).serializer), JSON_CONSTANTS_KEY_VIEWS );
        }
        break;

        case JSON_WRITER_PASS_NODES:
        {
            out_err |= json_serializer_begin_section( &((*this_).serializer), JSON_CONSTANTS_KEY_NODES );
        }
        break;

        case JSON_WRITER_PASS_EDGES:
        {
            out_err |= json_serializer_begin_section( &((*this_).serializer), JSON_CONSTANTS_KEY_EDGES );
        }
        break;

        default:
        {
            assert( false );
        }
        break;
    }

    TRACE_END_ERR(out_err);
    return out_err;
}

bool json_element_writer_can_classifier_nest_classifier( json_element_writer_t *this_,
                                                         data_classifier_type_t host_type,
                                                         data_classifier_type_t child_type )
{
    TRACE_BEGIN();
    const bool can_nest = false;
    TRACE_END();
    return can_nest;
}

bool json_element_writer_can_classifier_nest_relationship( json_element_writer_t *this_,
                                                           data_classifier_type_t host_type,
                                                           data_relationship_type_t child_type )
{
    TRACE_BEGIN();
    const bool can_nest = true;
    TRACE_END();
    return can_nest;
}

int json_element_writer_start_classifier( json_element_writer_t *this_,
                                          data_classifier_type_t host_type,
                                          const data_classifier_t *classifier_ptr )
{
    TRACE_BEGIN();
    assert( classifier_ptr != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_NODES )
    {
    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_assemble_classifier( json_element_writer_t *this_,
                                             data_classifier_type_t host_type,
                                             const data_classifier_t *classifier_ptr )
{
    TRACE_BEGIN();
    assert( classifier_ptr != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_NODES )
    {
        out_err |= json_serializer_begin_classifier( &((*this_).serializer), classifier_ptr );

        /* update export statistics */
        data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_EXPORTED );
    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_end_classifier( json_element_writer_t *this_,
                                        data_classifier_type_t host_type,
                                        const data_classifier_t *classifier_ptr )
{
    TRACE_BEGIN();
    assert( classifier_ptr != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_NODES )
    {
        out_err |= json_serializer_end_classifier( &((*this_).serializer), classifier_ptr );
    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_start_feature( json_element_writer_t *this_,
                                       data_classifier_type_t parent_type,
                                       const data_feature_t *feature_ptr )
{
    TRACE_BEGIN();
    assert( feature_ptr != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_NODES )
    {

    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_assemble_feature( json_element_writer_t *this_,
                                          data_classifier_type_t parent_type,
                                          const data_feature_t *feature_ptr )
{
    TRACE_BEGIN();
    assert( feature_ptr != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_NODES )
    {
        out_err |= json_serializer_append_feature( &((*this_).serializer), feature_ptr );

        /* update export statistics */
        data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_EXPORTED );
    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_end_feature( json_element_writer_t *this_,
                                     data_classifier_type_t parent_type,
                                     const data_feature_t *feature_ptr )
{
    TRACE_BEGIN();
    assert( feature_ptr != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_NODES )
    {

    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_start_relationship( json_element_writer_t *this_,
                                            data_classifier_type_t host_type,
                                            const data_relationship_t *relation_ptr )
{
    TRACE_BEGIN();
    assert( relation_ptr != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_EDGES )
    {

    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_assemble_relationship( json_element_writer_t *this_,
                                               const data_classifier_t *host,
                                               const data_relationship_t *relation_ptr,
                                               const data_classifier_t *from_c,
                                               const data_feature_t *from_f,
                                               const data_classifier_t *to_c,
                                               const data_feature_t *to_f )
{
    TRACE_BEGIN();
    assert( relation_ptr != NULL );
    assert( from_c != NULL );
    assert( to_c != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_EDGES )
    {
        out_err |= json_serializer_append_relationship( &((*this_).serializer),
                                                            relation_ptr,
                                                            from_c,
                                                            from_f,
                                                            to_c,
                                                            to_f
                                                          );

        /* update export statistics */
        data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_EXPORTED );
    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_end_relationship( json_element_writer_t *this_,
                                          data_classifier_type_t host_type,
                                          const data_relationship_t *relation_ptr )
{
    TRACE_BEGIN();
    assert( relation_ptr != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_EDGES )
    {

    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_start_diagram( json_element_writer_t *this_, const data_diagram_t *diag_ptr )
{
    TRACE_BEGIN();
    assert( diag_ptr != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_VIEWS )
    {

    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_assemble_diagram( json_element_writer_t *this_,
                                          const data_diagram_t *diag_ptr,
                                          const char *diagram_file_base_name )
{
    TRACE_BEGIN();
    assert( diag_ptr != NULL );
    assert( diagram_file_base_name != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_VIEWS )
    {
        out_err |= json_serializer_append_diagram( &((*this_).serializer), diag_ptr );

        /* update export statistics */
        data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_DIAGRAM, DATA_STAT_SERIES_EXPORTED );
    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_end_diagram( json_element_writer_t *this_, const data_diagram_t *diag_ptr )
{
    TRACE_BEGIN();
    assert( diag_ptr != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_VIEWS )
    {

    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_start_diagramelement( json_element_writer_t *this_,
                                              const data_diagram_t *parent,
                                              const data_diagramelement_t *diagramelement_ptr,
                                              const data_classifier_t *occurrence )
{
    TRACE_BEGIN();
    assert( diagramelement_ptr != NULL );
    assert( parent != NULL );
    assert( occurrence != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_VIEWS )
    {

    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_assemble_diagramelement( json_element_writer_t *this_,
                                                 const data_diagram_t *parent,
                                                 const data_diagramelement_t *diagramelement_ptr,
                                                 const data_classifier_t *occurrence )
{
    TRACE_BEGIN();
    assert( diagramelement_ptr != NULL );
    assert( parent != NULL );
    assert( occurrence != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_VIEWS )
    {
        out_err |= json_serializer_append_diagramelement( &((*this_).serializer), diagramelement_ptr );

        /* update export statistics */
        data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_EXPORTED );
    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_end_diagramelement( json_element_writer_t *this_,
                                            const data_diagram_t *parent,
                                            const data_diagramelement_t *diagramelement_ptr,
                                            const data_classifier_t *occurrence )
{
    TRACE_BEGIN();
    assert( diagramelement_ptr != NULL );
    assert( parent != NULL );
    assert( occurrence != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_VIEWS )
    {

    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_end_main( json_element_writer_t *this_ )
{
    TRACE_BEGIN();
    int out_err = 0;

    out_err |= json_serializer_end_section( &((*this_).serializer) );

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_write_footer( json_element_writer_t *this_ )
{
    TRACE_BEGIN();
    int out_err = 0;

    out_err |= json_serializer_write_footer( &((*this_).serializer) );

    TRACE_END_ERR(out_err);
    return out_err;
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
