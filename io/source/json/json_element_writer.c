/* File: json_element_writer.inl; Copyright and License: see below */

#include <json/json_element_writer.h>
#include "json/json_constants.h"
#include "meta/meta_info.h"
#include "meta/meta_version.h"
#include "trace.h"
#include "tslog.h"
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
        .end_diagram = &json_element_writer_end_diagram_fake,
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

    json_writer_init( &((*this_).json_writer), output );

    (*this_).in_outer_array = false;
    (*this_).is_outer_first = false;
    (*this_).in_inner_array = false;
    (*this_).is_inner_first = false;

    (*this_).mode = JSON_WRITER_PASS_NODES;
    (*this_).export_stat = io_export_stat;

    TRACE_END();
}

void json_element_writer_destroy( json_element_writer_t *this_ )
{
    TRACE_BEGIN();

    json_writer_destroy( &((*this_).json_writer) );

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
    assert( (*this_).in_outer_array == false );
    assert( (*this_).in_inner_array == false );
    int out_err = 0;

    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_BEGIN_OBJECT_NL
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_HEAD
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_BEGIN_OBJECT_NL
                                      );

    out_err |= json_writer_write_member_string( &((*this_).json_writer), 2, "encoding", "utf-8", true );
    out_err |= json_writer_write_member_string( &((*this_).json_writer), 2, "structure_format", "rfc-8259", true );
    out_err |= json_writer_write_member_string( &((*this_).json_writer), 2, "format", "cfu-json", true );
    out_err |= json_writer_write_member_int( &((*this_).json_writer), 2, "major_version", 1, true );
    out_err |= json_writer_write_member_int( &((*this_).json_writer), 2, "minor_version", 0, true );
    out_err |= json_writer_write_member_string( &((*this_).json_writer), 2, "generator_name", META_INFO_PROGRAM_ID_STR, true );
    out_err |= json_writer_write_member_string( &((*this_).json_writer), 2, "generator_version", META_VERSION_STR, false );

    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_END_OBJECT
                                        JSON_CONSTANTS_NL
                                      );


    if ( out_err != 0 )
    {
        TSLOG_ERROR( "output buffer exceeded." );
    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_start_main( json_element_writer_t *this_, const char *document_title )
{
    TRACE_BEGIN();
    assert( document_title != NULL );
    assert( (*this_).in_outer_array == false );
    assert( (*this_).in_inner_array == false );
    int out_err = 0;

    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_NEXT_NL
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                      );


    switch( (*this_).mode )
    {
        case JSON_WRITER_PASS_VIEWS:
        {
            out_err |= json_writer_write_plain( &((*this_).json_writer), JSON_CONSTANTS_KEY_VIEWS );
        }
        break;

        case JSON_WRITER_PASS_NODES:
        {
            out_err |= json_writer_write_plain( &((*this_).json_writer), JSON_CONSTANTS_KEY_NODES );
        }
        break;

        case JSON_WRITER_PASS_EDGES:
        {
            out_err |= json_writer_write_plain( &((*this_).json_writer), JSON_CONSTANTS_KEY_EDGES );
        }
        break;

        default:
        {
            assert( false );
        }
        break;
    }

    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF_NL
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_BEGIN_ARRAY
                                      );

    if ( out_err != 0 )
    {
        TSLOG_ERROR( "output buffer exceeded." );
    }

    (*this_).in_outer_array = true;
    (*this_).is_outer_first = true;

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
        /* separate objects in array if not first */
        if ( (*this_).is_outer_first )
        {
            (*this_).is_outer_first = false;
            out_err |= json_writer_write_plain( &((*this_).json_writer), JSON_CONSTANTS_NL );
        }
        else
        {
            out_err |= json_writer_write_plain( &((*this_).json_writer), JSON_CONSTANTS_NEXT_NL );
        }

        /* begin classifier */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_BEGIN_OBJECT
                                            JSON_CONSTANTS_NL
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_CLASSIFIER
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                            JSON_CONSTANTS_BEGIN_OBJECT_NL );
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
        assert( (*this_).in_outer_array == true );
        assert( (*this_).in_inner_array == false );

        /* id */
        out_err |= json_writer_write_member_int( &((*this_).json_writer),
                                                 4,
                                                 JSON_CONSTANTS_KEY_CLASSIFIER_ID,
                                                 data_classifier_get_row_id( classifier_ptr ),
                                                 true
                                               );

        /* main type */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_CLASSIFIER_MAIN_TYPE
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                          );
        out_err |= json_writer_write_int( &((*this_).json_writer),
                                          data_classifier_get_main_type( classifier_ptr )
                                        );
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NEXT_NL
                                          );

        /* main type name */
        // TODO JSON_CONSTANTS_KEY_CLASSIFIER_MAIN_TYPE_NAME "type"

        /* stereotype */
        out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                    4,
                                                    JSON_CONSTANTS_KEY_CLASSIFIER_STEREOTYPE,
                                                    data_classifier_get_stereotype_const( classifier_ptr ),
                                                    true
                                                  );

        /* name */
        out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                    4,
                                                    JSON_CONSTANTS_KEY_CLASSIFIER_NAME,
                                                    data_classifier_get_name_const( classifier_ptr ),
                                                    true
                                                  );

        /* description */
        out_err |= json_writer_write_member_string_array( &((*this_).json_writer) ,
                                                          4,
                                                          JSON_CONSTANTS_KEY_CLASSIFIER_DESCRIPTION,
                                                          data_classifier_get_description_const( classifier_ptr ),
                                                          true
                                                        );

        /* x_order */
        out_err |= json_writer_write_member_int( &((*this_).json_writer),
                                                 4,
                                                 JSON_CONSTANTS_KEY_CLASSIFIER_X_ORDER,
                                                 data_classifier_get_x_order( classifier_ptr ),
                                                 true
                                               );

        /* y_order */
        out_err |= json_writer_write_member_int( &((*this_).json_writer),
                                                 4,
                                                 JSON_CONSTANTS_KEY_CLASSIFIER_Y_ORDER,
                                                 data_classifier_get_y_order( classifier_ptr ),
                                                 true
                                               );

        /* list_order */
        out_err |= json_writer_write_member_int( &((*this_).json_writer),
                                                 4,
                                                 JSON_CONSTANTS_KEY_CLASSIFIER_LIST_ORDER,
                                                 data_classifier_get_list_order( classifier_ptr ),
                                                 true
                                               );

        /* uuid */
        out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                    4,
                                                    JSON_CONSTANTS_KEY_UUID,
                                                    data_classifier_get_uuid_const( classifier_ptr ),
                                                    true
                                                  );

        /* array of features */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF_NL
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_BEGIN_ARRAY
                                          );

        (*this_).in_inner_array = true;
        (*this_).is_inner_first = true;

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
        assert( (*this_).in_outer_array == true );
        assert( (*this_).in_inner_array == true );
        (*this_).in_inner_array = false;
        (*this_).is_inner_first = false;

        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NL
                                          );

        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_END_ARRAY
                                            JSON_CONSTANTS_NL
                                          );

        /* end classifier */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_END_OBJECT_NL
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_END_OBJECT
                                          );

        if ( out_err != 0 )
        {
            TSLOG_ERROR( "output buffer exceeded." );
        }
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
        /* separate objects in array if not first */
        if ( (*this_).is_inner_first )
        {
            (*this_).is_inner_first = false;
            out_err |= json_writer_write_plain( &((*this_).json_writer),
                                                JSON_CONSTANTS_NL
                                              );
        }
        else
        {
            out_err |= json_writer_write_plain( &((*this_).json_writer),
                                                JSON_CONSTANTS_NEXT_NL
                                              );
        }

        /* begin feature */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_BEGIN_OBJECT_NL
                                          );
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
        assert( (*this_).in_outer_array == true );
        assert( (*this_).in_inner_array == true );

        /* id */
        out_err |= json_writer_write_member_int( &((*this_).json_writer),
                                                 6,
                                                 JSON_CONSTANTS_KEY_FEATURE_ID,
                                                 data_feature_get_row_id( feature_ptr ),
                                                 true
                                               );

        /* main_type */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_FEATURE_MAIN_TYPE
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                          );
        out_err |= json_writer_write_int( &((*this_).json_writer),
                                          data_feature_get_main_type( feature_ptr )
                                        );
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NEXT_NL
                                          );

        /* type name */
        // TODO JSON_CONSTANTS_KEY_FEATURE_MAIN_TYPE_NAME "type"

        /* key */
        out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                    6,
                                                    JSON_CONSTANTS_KEY_FEATURE_KEY,
                                                    data_feature_get_key_const( feature_ptr ),
                                                    true
                                                  );

        /* value */
        out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                    6,
                                                    JSON_CONSTANTS_KEY_FEATURE_VALUE,
                                                    data_feature_get_value_const( feature_ptr ),
                                                    true
                                                  );

        /* description */
        out_err |= json_writer_write_member_string_array( &((*this_).json_writer) ,
                                                          6,
                                                          JSON_CONSTANTS_KEY_FEATURE_DESCRIPTION,
                                                          data_feature_get_description_const( feature_ptr ),
                                                          true
                                                        );

        /* list_order */
        out_err |= json_writer_write_member_int( &((*this_).json_writer),
                                                 6,
                                                 JSON_CONSTANTS_KEY_FEATURE_LIST_ORDER,
                                                 data_feature_get_list_order( feature_ptr ),
                                                 true
                                               );

        /* uuid */
        out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                    6,
                                                    JSON_CONSTANTS_KEY_UUID,
                                                    data_feature_get_uuid_const( feature_ptr ),
                                                    false  /* LAST, no NEXT */
                                                  );

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
        /* end feature */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_END_OBJECT
                                          );

        if ( out_err != 0 )
        {
            TSLOG_ERROR( "output buffer exceeded." );
        }
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
        /* separate objects if not first */
        if ( (*this_).is_outer_first )
        {
            (*this_).is_outer_first = false;
            out_err |= json_writer_write_plain( &((*this_).json_writer),
                                                JSON_CONSTANTS_NL
                                              );
        }
        else
        {
            out_err |= json_writer_write_plain( &((*this_).json_writer),
                                                JSON_CONSTANTS_NEXT_NL
                                              );
        }

        /* begin relationship */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_BEGIN_OBJECT
                                            JSON_CONSTANTS_NL
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_RELATIONSHIP
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                            JSON_CONSTANTS_BEGIN_OBJECT_NL
                                          );
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
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_EDGES )
    {
        assert( from_c != NULL );
        assert( to_c != NULL );
        assert( (*this_).in_outer_array == true );
        assert( (*this_).in_inner_array == false );

        /* id */
        out_err |= json_writer_write_member_int( &((*this_).json_writer),
                                                 4,
                                                 JSON_CONSTANTS_KEY_RELATIONSHIP_ID,
                                                 data_relationship_get_row_id( relation_ptr ),
                                                 true
                                               );

        /* main type */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_RELATIONSHIP_MAIN_TYPE
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                          );
        out_err |= json_writer_write_int( &((*this_).json_writer),
                                          data_relationship_get_main_type( relation_ptr )
                                        );
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NEXT_NL
                                          );

        /* main type name */
        // TODO JSON_CONSTANTS_KEY_RELATIONSHIP_MAIN_TYPE_NAME "type"

        /* name */
        out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                    4,
                                                    JSON_CONSTANTS_KEY_RELATIONSHIP_NAME,
                                                    data_relationship_get_name_const( relation_ptr ),
                                                    true
                                                  );

        /* description */
        out_err |= json_writer_write_member_string_array( &((*this_).json_writer) ,
                                                          4,
                                                          JSON_CONSTANTS_KEY_RELATIONSHIP_DESCRIPTION,
                                                          data_relationship_get_description_const( relation_ptr ),
                                                          true
                                                        );

        /* list_order */
        out_err |= json_writer_write_member_int( &((*this_).json_writer),
                                                 4,
                                                 JSON_CONSTANTS_KEY_RELATIONSHIP_LIST_ORDER,
                                                 data_relationship_get_list_order( relation_ptr ),
                                                 true
                                               );

        const bool from_f_valid = ( from_f == NULL ) ? false : data_feature_is_valid( from_f );
        const bool from_c_valid = ( from_c == NULL ) ? false : data_classifier_is_valid( from_c );
        const bool to_f_valid = ( to_f == NULL ) ? false : data_feature_is_valid( to_f );
        const bool to_c_valid = ( to_c == NULL ) ? false : data_classifier_is_valid( to_c );

        /* from_classifier_id */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_CLASSIFIER_ID
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                          );
        out_err |= json_writer_write_int( &((*this_).json_writer),
                                          data_relationship_get_from_classifier_row_id( relation_ptr )
                                        );
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NEXT_NL
                                          );

        /* from_classifier_name */
        const char *const from_c_name = from_c_valid ? data_classifier_get_name_const( from_c ) : "";
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_CLASSIFIER_NAME
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                            JSON_CONSTANTS_QUOTE
                                          );
        out_err |= json_writer_write_string_enc( &((*this_).json_writer), from_c_name );
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_NEXT_NL
                                          );

        /* to_classifier_id */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_RELATIONSHIP_TO_CLASSIFIER_ID
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                          );
        out_err |= json_writer_write_int( &((*this_).json_writer),
                                          data_relationship_get_to_classifier_row_id( relation_ptr )
                                        );
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NEXT_NL
                                          );

        /* to_classifier_name */
        const char *const to_c_name = to_c_valid ? data_classifier_get_name_const( to_c ) : "";
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_RELATIONSHIP_TO_CLASSIFIER_NAME
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                            JSON_CONSTANTS_QUOTE
                                          );
        out_err |= json_writer_write_string_enc( &((*this_).json_writer), to_c_name );
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_NEXT_NL
                                          );

        /* from_feature_id */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_FEATURE_ID
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                          );
        out_err |= json_writer_write_int( &((*this_).json_writer),
                                          data_relationship_get_from_feature_row_id( relation_ptr )
                                        );
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NEXT_NL
                                          );

        /* from_feature_key */
        if ( from_f_valid )
        {
            out_err |= json_writer_write_plain( &((*this_).json_writer),
                                                JSON_CONSTANTS_TAB
                                                JSON_CONSTANTS_TAB
                                                JSON_CONSTANTS_TAB
                                                JSON_CONSTANTS_TAB
                                                JSON_CONSTANTS_QUOTE
                                                JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_FEATURE_KEY
                                                JSON_CONSTANTS_QUOTE
                                                JSON_CONSTANTS_DEF
                                                JSON_CONSTANTS_QUOTE
                                              );
            out_err |= json_writer_write_string_enc( &((*this_).json_writer),
                                                     data_feature_get_key_const( from_f )
                                                   );
            out_err |= json_writer_write_plain( &((*this_).json_writer),
                                                JSON_CONSTANTS_QUOTE
                                                JSON_CONSTANTS_NEXT_NL
                                              );
        }

        /* to_feature_id */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_RELATIONSHIP_TO_FEATURE_ID
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                          );
        out_err |= json_writer_write_int( &((*this_).json_writer),
                                          data_relationship_get_to_feature_row_id( relation_ptr )
                                        );
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NEXT_NL
                                          );

        /* to_feature_key */
        if ( to_f_valid )
        {
            out_err |= json_writer_write_plain( &((*this_).json_writer),
                                                JSON_CONSTANTS_TAB
                                                JSON_CONSTANTS_TAB
                                                JSON_CONSTANTS_TAB
                                                JSON_CONSTANTS_TAB
                                                JSON_CONSTANTS_QUOTE
                                                JSON_CONSTANTS_KEY_RELATIONSHIP_TO_FEATURE_KEY
                                                JSON_CONSTANTS_QUOTE
                                                JSON_CONSTANTS_DEF
                                                JSON_CONSTANTS_QUOTE
                                              );
            out_err |= json_writer_write_string_enc( &((*this_).json_writer),
                                                     data_feature_get_key_const( to_f )
                                                   );
            out_err |= json_writer_write_plain( &((*this_).json_writer),
                                                JSON_CONSTANTS_QUOTE
                                                JSON_CONSTANTS_NEXT_NL
                                              );
        }

        /* from node ref_uuid */
        const char *const from_node_ref
            = ( from_f_valid )
            ? data_feature_get_uuid_const( from_f )
            : ( from_c_valid )
            ? data_classifier_get_uuid_const( from_c )
            : "";
        out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                    4,
                                                    JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_NODE,
                                                    from_node_ref,
                                                    true
                                                  );

        /* to node ref_uuid */
        const char *const to_node_ref
            = ( to_f_valid )
            ? data_feature_get_uuid_const( to_f )
            : ( to_c_valid )
            ? data_classifier_get_uuid_const( to_c )
            : "";
        out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                    4,
                                                    JSON_CONSTANTS_KEY_RELATIONSHIP_TO_NODE,
                                                    to_node_ref,
                                                    true
                                                  );

        /* uuid */
        out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                    4,
                                                    JSON_CONSTANTS_KEY_UUID,
                                                    data_relationship_get_uuid_const( relation_ptr ),
                                                    false  /* LAST, no NEXT */
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
        /* end relationship */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_END_OBJECT_NL
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_END_OBJECT
                                          );

        if ( out_err != 0 )
        {
            TSLOG_ERROR( "output buffer exceeded." );
        }
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
        /* separate objects if not first */
        if ( (*this_).is_outer_first )
        {
            (*this_).is_outer_first = false;
            out_err |= json_writer_write_plain( &((*this_).json_writer),  JSON_CONSTANTS_NL );
        }
        else
        {
            /* print diagram end here because the official end requires hierarcical diagram containments */
            out_err |= json_element_writer_private_end_diagram( this_ );

            out_err |= json_writer_write_plain( &((*this_).json_writer), JSON_CONSTANTS_NEXT_NL  );
        }

        assert( (*this_).in_outer_array == true );
        assert( (*this_).in_inner_array == false );

        /* begin diagram */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_BEGIN_OBJECT
                                            JSON_CONSTANTS_NL
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_DIAGRAM
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                            JSON_CONSTANTS_BEGIN_OBJECT_NL
                                          );
    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_assemble_diagram( json_element_writer_t *this_,
                                          const data_diagram_t *parent,
                                          const data_diagram_t *diag_ptr,
                                          const char *diagram_file_base_name )
{
    TRACE_BEGIN();
    assert( diag_ptr != NULL );
    assert( diagram_file_base_name != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_VIEWS )
    {
        /* id */
        out_err |= json_writer_write_member_int( &((*this_).json_writer),
                                                 4,
                                                 JSON_CONSTANTS_KEY_DIAGRAM_ID,
                                                 data_diagram_get_row_id( diag_ptr ),
                                                 true
                                               );

        /* parent_id */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_DIAGRAM_PARENT_ID
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                          );
        out_err |= json_writer_write_int( &((*this_).json_writer),
                                          data_diagram_get_parent_row_id( diag_ptr )
                                        );
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NEXT_NL
                                          );

        const bool parent_valid = ( parent == NULL ) ? false : data_diagram_is_valid( parent );

        /* parent_name */
        if( parent_valid )
        {
            out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                        4,
                                                        JSON_CONSTANTS_KEY_DIAGRAM_PARENT_NAME,
                                                        data_diagram_get_name_const( parent ),
                                                        true
                                                    );
        }

        /* diagram type */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_DIAGRAM_DIAGRAM_TYPE
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                          );
        out_err |= json_writer_write_int( &((*this_).json_writer),
                                          data_diagram_get_diagram_type( diag_ptr )
                                        );
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NEXT_NL
                                          );

        /* diagram type name */
        // TODO JSON_CONSTANTS_KEY_DIAGRAM_DIAGRAM_TYPE_NAME "type"

        /* name */
        out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                    4,
                                                    JSON_CONSTANTS_KEY_DIAGRAM_NAME,
                                                    data_diagram_get_name_const( diag_ptr ),
                                                    true
                                                  );

        /* description */
        out_err |= json_writer_write_member_string_array( &((*this_).json_writer) ,
                                                          4,
                                                          JSON_CONSTANTS_KEY_DIAGRAM_DESCRIPTION,
                                                          data_diagram_get_description_const( diag_ptr ),
                                                          true
                                                        );

        /* list_order */
        out_err |= json_writer_write_member_int( &((*this_).json_writer),
                                                 4,
                                                 JSON_CONSTANTS_KEY_DIAGRAM_LIST_ORDER,
                                                 data_diagram_get_list_order( diag_ptr ),
                                                 true
                                               );

        /* display_flags */
        out_err |= json_writer_write_member_int( &((*this_).json_writer),
                                                 4,
                                                 JSON_CONSTANTS_KEY_DIAGRAM_DISPLAY_FLAGS,
                                                 data_diagram_get_display_flags( diag_ptr ),
                                                 true
                                               );

        /* display_tags - flag names */
        // TODO JSON_CONSTANTS_KEY_DIAGRAM_DISPLAY_FLAG_NAMES "display_tags"

        /* parent uuid */
        if ( parent_valid )
        {
            out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                        4,
                                                        JSON_CONSTANTS_KEY_DIAGRAM_PARENT,
                                                        data_diagram_get_uuid_const( parent ),
                                                        true
                                                      );
        }

        /* uuid */
        out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                    4,
                                                    JSON_CONSTANTS_KEY_UUID,
                                                    data_diagram_get_uuid_const( diag_ptr ),
                                                    true  /* NEXT, not LAST */
                                                  );

        /* array of features */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_DIAGRAM_ELEMENTS
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF_NL
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_BEGIN_ARRAY
                                          );

        (*this_).in_inner_array = true;
        (*this_).is_inner_first = true;

        /* update export statistics */
        data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_DIAGRAM, DATA_STAT_SERIES_EXPORTED );
    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_end_diagram_fake( json_element_writer_t *this_, const data_diagram_t *diag_ptr )
{
    TRACE_BEGIN();
    assert( diag_ptr != NULL );
    int out_err = 0;

    /* The JSON export does not encapsulate one diagram into another. */
    /* Therefore, diagrams are ended already when the next starts. */

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_private_end_diagram( json_element_writer_t *this_ )
{
    TRACE_BEGIN();
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_VIEWS )
    {
        (*this_).in_inner_array = false;
        (*this_).is_inner_first = false;

        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NL
                                          );

        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_END_ARRAY
                                            JSON_CONSTANTS_NL
                                          );

        /* end diagram */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_END_OBJECT_NL
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_END_OBJECT
                                          );
    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_start_diagramelement( json_element_writer_t *this_,
                                              const data_diagram_t *parent,
                                              const data_diagramelement_t *diagramelement_ptr )
{
    TRACE_BEGIN();
    assert( diagramelement_ptr != NULL );
    assert( parent != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_VIEWS )
    {
        /* separate objects in array if not first */
        if ( (*this_).is_inner_first )
        {
            (*this_).is_inner_first = false;
            out_err |= json_writer_write_plain( &((*this_).json_writer),
                                                JSON_CONSTANTS_NL
                                              );
        }
        else
        {
            out_err |= json_writer_write_plain( &((*this_).json_writer),
                                                JSON_CONSTANTS_NEXT_NL
                                              );
        }

        /* begin diagramelement */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_BEGIN_OBJECT_NL
                                          );

        if ( out_err != 0 )
        {
            TSLOG_ERROR( "output buffer exceeded." );
        }
    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_assemble_diagramelement( json_element_writer_t *this_,
                                                 const data_diagram_t *parent,
                                                 const data_diagramelement_t *diagramelement_ptr,
                                                 const data_classifier_t *occurrence,
                                                 const data_feature_t *feat_occur )
{
    TRACE_BEGIN();
    assert( parent != NULL );
    assert( diagramelement_ptr != NULL );
    assert( occurrence != NULL );
    /* feat_occur may be NULL */
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_VIEWS )
    {
        assert( (*this_).in_outer_array == true );
        assert( (*this_).in_inner_array == true );

        /* id */
        out_err |= json_writer_write_member_int( &((*this_).json_writer),
                                                 6,
                                                 JSON_CONSTANTS_KEY_DIAGRAMELEMENT_ID,
                                                 data_diagramelement_get_row_id( diagramelement_ptr ),
                                                 true
                                               );

        /* display_flags */
        out_err |= json_writer_write_member_int( &((*this_).json_writer),
                                                 6,
                                                 JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DISPLAY_FLAGS,
                                                 data_diagramelement_get_display_flags( diagramelement_ptr ),
                                                 true
                                               );

        /* display_tags -- flag names */
        // TODO JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DISPLAY_FLAG_NAMES "display_tags"

        /* classifier_id */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_DIAGRAMELEMENT_CLASSIFIER_ID
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                          );
        out_err |= json_writer_write_int( &((*this_).json_writer),
                                          data_diagramelement_get_classifier_row_id( diagramelement_ptr )
                                        );
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NEXT_NL
                                          );

        /* classifier_name */
        out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                    6,
                                                    JSON_CONSTANTS_KEY_DIAGRAMELEMENT_CLASSIFIER_NAME,
                                                    data_classifier_get_name_const( occurrence ),
                                                    true
                                                  );

        /* focused_feature_id */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_DIAGRAMELEMENT_FOCUSED_FEATURE_ID
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                          );
        out_err |= json_writer_write_int( &((*this_).json_writer),
                                          data_diagramelement_get_focused_feature_row_id( diagramelement_ptr )
                                        );
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NEXT_NL
                                          );

        /* focused_feature_name */
        /* note that focused features are always lifelines and these never have names */
#if 0
        const bool feature_valid = ( feat_occur == NULL ) ? false : data_feature_is_valid( feat_occur );
        if ( feature_valid )
        {
            out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                        6,
                                                        JSON_CONSTANTS_KEY_DIAGRAMELEMENT_FOCUSED_FEATURE_NAME,
                                                        data_feature_get_key_const( feat_occur ),
                                                        true
                                                      );
        }
#endif

        /* ref_uuid */
        const bool feat_valid = ( feat_occur == NULL ) ? false : data_feature_is_valid( feat_occur );
        const bool clas_valid = ( occurrence == NULL ) ? false : data_classifier_is_valid( occurrence );
        const char *const node_ref
            = ( feat_valid )
            ? data_feature_get_uuid_const( feat_occur )
            : ( clas_valid )
            ? data_classifier_get_uuid_const( occurrence )
            : "";
        out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                    6,
                                                    JSON_CONSTANTS_KEY_DIAGRAMELEMENT_NODE,
                                                    node_ref,
                                                    true
                                                  );

        /* uuid */
        out_err |= json_writer_write_member_string( &((*this_).json_writer),
                                                    6,
                                                    JSON_CONSTANTS_KEY_UUID,
                                                    data_diagramelement_get_uuid_const( diagramelement_ptr ),
                                                    false  /* LAST, no NEXT */
                                                  );

        /* update export statistics */
        data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_EXPORTED );
    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_end_diagramelement( json_element_writer_t *this_,
                                            const data_diagram_t *parent,
                                            const data_diagramelement_t *diagramelement_ptr )
{
    TRACE_BEGIN();
    assert( diagramelement_ptr != NULL );
    assert( parent != NULL );
    int out_err = 0;

    if ( (*this_).mode == JSON_WRITER_PASS_VIEWS )
    {
        /* end diagramelement */
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_END_OBJECT
                                          );

        if ( out_err != 0 )
        {
            TSLOG_ERROR( "output buffer exceeded." );
        }
    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_end_main( json_element_writer_t *this_ )
{
    TRACE_BEGIN();
    int out_err = 0;

    /* print diagram end here because the official end requires hierarcical diagram containments */
    if ( (*this_).mode == JSON_WRITER_PASS_VIEWS )
    {
        if ( ! (*this_).is_outer_first )
        {
            out_err |= json_element_writer_private_end_diagram( this_ );
        }
    }

    assert( (*this_).in_outer_array == true );
    assert( (*this_).in_inner_array == false );
    (*this_).in_outer_array = false;
    (*this_).is_outer_first = false;

    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NL
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_END_ARRAY
                                        JSON_CONSTANTS_NL
                                      );

    if ( out_err != 0 )
    {
        TSLOG_ERROR( "output buffer exceeded." );
    }

    TRACE_END_ERR(out_err);
    return out_err;
}

int json_element_writer_write_footer( json_element_writer_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).in_outer_array == false );
    assert( (*this_).in_inner_array == false );
    int out_err = 0;

    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_END_OBJECT_NL
                                      );

    if ( out_err != 0 )
    {
        TSLOG_ERROR( "output buffer exceeded." );
    }

    TRACE_END_ERR(out_err);
    return out_err;
}


/*
Copyright 2016-2022 Andreas Warnke

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
