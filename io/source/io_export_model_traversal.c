/* File: io_export_model_traversal.c; Copyright and License: see below */

#include "io_export_model_traversal.h"
#include "u8/u8_trace.h"
#include "entity/data_diagram.h"
#include "entity/data_classifier.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void io_export_model_traversal_init( io_export_model_traversal_t *this_,
                                     data_database_reader_t *db_reader,
                                     data_visible_set_t *input_data,
                                     data_stat_t *io_export_stat,
                                     io_element_writer_t *out_element_writer )
{
    U8_TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != io_export_stat );
    assert( NULL != out_element_writer );

    (*this_).db_reader = db_reader;
    (*this_).export_stat = io_export_stat;
    (*this_).element_writer = out_element_writer;

    universal_array_list_init ( &((*this_).written_id_set),
                                sizeof((*this_).written_id_set_buf)/sizeof(data_id_t),
                                &((*this_).written_id_set_buf),
                                sizeof(data_id_t),
                                ((char*)(&((*this_).written_id_set_buf[1])))-((char*)(&((*this_).written_id_set_buf[0]))),
                                (void (*)(void *, const void *)) &data_id_copy,
                                (bool (*)(const void *, const void *)) &data_id_equals,
                                (void (*)(void *)) &data_id_destroy
                              );

    io_export_interaction_traversal_init( &((*this_).interaction_helper),
                                          db_reader,
                                          input_data,
                                          &((*this_).written_id_set),
                                          io_export_stat,
                                          out_element_writer
                                        );
    U8_TRACE_END();
}

void io_export_model_traversal_destroy( io_export_model_traversal_t *this_ )
{
    U8_TRACE_BEGIN();

    io_export_interaction_traversal_destroy ( &((*this_).interaction_helper) );

    universal_array_list_destroy ( &((*this_).written_id_set) );

    (*this_).db_reader = NULL;
    (*this_).export_stat = NULL;
    (*this_).element_writer = NULL;

    U8_TRACE_END();
}

u8_error_t io_export_model_traversal_walk_model_nodes ( io_export_model_traversal_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t write_err = U8_ERROR_NONE;

    {
        u8_error_t data_err;
        data_classifier_iterator_t classifier_iterator;
        universal_array_list_clear( &((*this_).written_id_set) );

        /* init the iterator */
        data_classifier_iterator_init_empty( &classifier_iterator );
        data_err = data_database_reader_get_all_classifiers ( (*this_).db_reader, true, &classifier_iterator );
        if ( data_err != U8_ERROR_NONE )
        {
            write_err |= data_err;
        }
        else
        {
            while( data_classifier_iterator_has_next( &classifier_iterator ) && ( write_err==0 ) )
            {
                data_err = data_classifier_iterator_next( &classifier_iterator, &((*this_).temp_classifier) );
                if ( data_err != U8_ERROR_NONE )
                {
                    write_err |= data_err;
                }
                else
                {
                    const data_id_t classifier_id = data_classifier_get_data_id( &((*this_).temp_classifier) );
                    data_classifier_destroy( &((*this_).temp_classifier) );

                    write_err |= io_export_model_traversal_private_walk_node( this_,
                                                                              DATA_ID_VOID,
                                                                              DATA_ID_VOID,
                                                                              classifier_id,
                                                                              0 /* initial recursion_depth */
                                                                            );
                }
            }
        }
        data_err = data_classifier_iterator_destroy( &classifier_iterator );
        if ( data_err != U8_ERROR_NONE )
        {
            write_err |= data_err;
        }
    }

    U8_TRACE_END_ERR( write_err );
    return write_err;
}

u8_error_t io_export_model_traversal_private_walk_node( io_export_model_traversal_t *this_,
                                                        data_id_t host_id,
                                                        data_id_t containment_relationship_id,
                                                        data_id_t classifier_id,
                                                        unsigned int recursion_depth )
{
    U8_TRACE_BEGIN();
    assert( recursion_depth <= IO_EXPORT_MODEL_TRAVERSAL_MAX_TREE_DEPTH );
    assert( data_id_is_valid( &classifier_id ) );
    data_id_trace( &classifier_id );
    u8_error_t write_err = U8_ERROR_NONE;

    /* initially define flags and attributes */
    bool duplicate_classifier
        = ( -1 != universal_array_list_get_index_of( &((*this_).written_id_set), &classifier_id ) );
    bool is_classifier_compliant_here = false;  /* a default value */

    /* the id-sets needed on the stack during recursion */
    data_small_set_t contained_classifiers;
    data_small_set_init( &contained_classifiers );
    data_small_set_t containment_relations;
    data_small_set_init( &containment_relations );

    /* tasks before recursion */
    if ( ! duplicate_classifier )
    {
        const u8_error_t data_err_1
            = ( ! data_id_is_valid( &host_id ) )
            ? U8_ERROR_NONE
            : data_database_reader_get_classifier_by_id( (*this_).db_reader,
                                                         data_id_get_row_id( &host_id ),
                                                         &((*this_).temp_host)
                                                       );
        const u8_error_t data_err_2
            = data_database_reader_get_classifier_by_id( (*this_).db_reader,
                                                         data_id_get_row_id( &classifier_id ),
                                                         &((*this_).temp_classifier)
                                                       );

        if (( data_err_1 != U8_ERROR_NONE )||( data_err_2 != U8_ERROR_NONE ))
        {
            write_err |= data_err_1 | data_err_2;
        }
        else
        {
            /* update flags and attributes */
            const data_classifier_type_t host_type
                = ( ! data_id_is_valid( &host_id ) )
                ? DATA_CLASSIFIER_TYPE_PACKAGE   /* a uml:Model (the out-most model element) is a uml:Package*/
                : data_classifier_get_main_type( &((*this_).temp_host) );
            const data_classifier_type_t classifier_type = data_classifier_get_main_type( &((*this_).temp_classifier) );

            /* fake and export interactions in which the current node is participant first */
            /* recursion trick: If classifier is nested, this block is evaluated
             * 1x for the nested classifier and 1x for same classifier at the toplevel model package
             */
            {
                /* write the relationships that can be stated after the classifier or in the toplevel package */
                write_err |= io_export_model_traversal_private_fake_interactions_of_node( this_,
                                                                                          host_type,
                                                                                          &((*this_).temp_classifier)
                                                                                        );
                /* check if the classifier is already written */
                duplicate_classifier
                    =( -1 != universal_array_list_get_index_of( &((*this_).written_id_set), &classifier_id ) );
            }

            is_classifier_compliant_here = io_element_writer_can_classifier_nest_classifier( (*this_).element_writer,
                                                                                             host_type,
                                                                                             classifier_type
                                                                                           );
            if (( recursion_depth == 0 )&&( !is_classifier_compliant_here ))
            {
                /* fallback, there is no other place to put the classifier */
                is_classifier_compliant_here = true;
            }

            if (( ! duplicate_classifier )&&( is_classifier_compliant_here ))
            {
                /* add this classifier to the already written elements */
                write_err |= universal_array_list_append( &((*this_).written_id_set), &classifier_id );
                /* if this classifier was found via a containment_relationship, */
                /* add this containment_relationship to the already written elements */
                if ( data_id_is_valid( &containment_relationship_id ) )
                {
                    write_err |= universal_array_list_append( &((*this_).written_id_set), &containment_relationship_id );
                }

                /* walk node which is not a duplicate */
                write_err |= io_export_model_traversal_private_begin_node( this_,
                                                                           host_type,
                                                                           &((*this_).temp_classifier)
                                                                         );

                write_err |= io_export_model_traversal_private_iterate_node_features( this_, &((*this_).temp_classifier) );

                write_err |= io_export_model_traversal_private_get_containments( this_,
                                                                                 &((*this_).temp_classifier),
                                                                                 &contained_classifiers,
                                                                                 &containment_relations
                                                                               );
            }
        }

        if ( data_id_is_valid( &host_id ) )
        {
            data_classifier_destroy( &((*this_).temp_host) );
        }
        data_classifier_destroy( &((*this_).temp_classifier) );
    }

    /* do recursion, all required data is stored on the stack now */
    if (( ! duplicate_classifier )&&( is_classifier_compliant_here ))
    {
        write_err |= io_export_model_traversal_private_walk_containments( this_,
                                                                          classifier_id,
                                                                          &contained_classifiers,
                                                                          &containment_relations,
                                                                          recursion_depth
                                                                        );
    }

    /* tasks after recursion */
    {
        const u8_error_t data_err_3
            = ( ! data_id_is_valid( &host_id ) )
            ? U8_ERROR_NONE
            : data_database_reader_get_classifier_by_id( (*this_).db_reader,
                                                         data_id_get_row_id( &host_id ),
                                                         &((*this_).temp_host)
                                                       );
        const u8_error_t data_err_4
            = data_database_reader_get_classifier_by_id( (*this_).db_reader,
                                                         data_id_get_row_id( &classifier_id ),
                                                         &((*this_).temp_classifier)
                                                       );

        if (( data_err_3 != U8_ERROR_NONE )||( data_err_4 != U8_ERROR_NONE ))
        {
            write_err |= data_err_3 | data_err_4;
        }
        else
        {
            const data_classifier_type_t host_type_2
                = ( ! data_id_is_valid( &host_id ) )
                ? DATA_CLASSIFIER_TYPE_PACKAGE   /* a uml:Model is a uml:Package*/
                : data_classifier_get_main_type( &((*this_).temp_host) );
            const data_classifier_t *host_2
                = ( ! data_id_is_valid( &host_id ) )
                ? NULL
                : &((*this_).temp_host);

            if (( ! duplicate_classifier )&&( is_classifier_compliant_here ))
            {
                /* write the relationships that can be nested within the classifier */
                write_err |= io_export_model_traversal_private_iterate_node_relationships( this_,
                                                                                           false,
                                                                                           &((*this_).temp_classifier),  /* inside here */
                                                                                           &((*this_).temp_classifier)
                                                                                         );

                write_err |= io_export_model_traversal_private_end_node( this_,
                                                                         host_type_2,
                                                                         &((*this_).temp_classifier)
                                                                       );
            }

            /* recursion trick: If from- and to- classifiers are nested, this block is evaluated
             * 1x for the nested from-/to- classifier and 1x for same classifier at the toplevel model package
             */
            {
                /* write the relationships that can be stated after the classifier or in the toplevel package */
                write_err |= io_export_model_traversal_private_iterate_node_relationships( this_,
                                                                                           true,
                                                                                           host_2,  /* outside now */
                                                                                           &((*this_).temp_classifier)
                                                                                         );
            }
        }

        if ( data_id_is_valid( &host_id ) )
        {
            data_classifier_destroy( &((*this_).temp_host) );
        }
        data_classifier_destroy( &((*this_).temp_classifier) );
    }

    data_small_set_destroy( &contained_classifiers );
    data_small_set_destroy( &containment_relations );

    U8_TRACE_END_ERR( write_err );
    return write_err;
}

u8_error_t io_export_model_traversal_private_begin_node ( io_export_model_traversal_t *this_,
                                                          data_classifier_type_t host_type,
                                                          const data_classifier_t *classifier )
{
    U8_TRACE_BEGIN();
    assert( classifier != NULL );
    u8_error_t write_err = U8_ERROR_NONE;

    write_err |= io_element_writer_start_classifier( (*this_).element_writer, host_type, classifier );
    write_err |= io_element_writer_assemble_classifier( (*this_).element_writer, host_type, classifier );

    U8_TRACE_END_ERR( write_err );
    return write_err;
}

u8_error_t io_export_model_traversal_private_get_containments( io_export_model_traversal_t *this_,
                                                               const data_classifier_t *classifier,
                                                               data_small_set_t *io_contained_classifiers,
                                                               data_small_set_t *io_containment_relations )
{
    U8_TRACE_BEGIN();
    assert( classifier != NULL );
    assert( io_contained_classifiers != NULL );
    assert( io_containment_relations != NULL );
    u8_error_t result = U8_ERROR_NONE;

    const data_id_t classifier_id = data_classifier_get_data_id( classifier );
    data_id_trace( &classifier_id );

    /* search containments in relationships */
    data_relationship_iterator_t rel_iterator;
    result |= data_relationship_iterator_init_empty( &rel_iterator );
    result |= data_database_reader_get_relationships_by_classifier_id( (*this_).db_reader,
                                                                       data_classifier_get_row_id( classifier ),
                                                                       &rel_iterator
                                                                     );

    while ( data_relationship_iterator_has_next( &rel_iterator ) )
    {
        /* get relationship */
        result |= data_relationship_iterator_next( &rel_iterator, &((*this_).temp_relationship) );
        if ( data_relationship_is_valid( &((*this_).temp_relationship) ) )
        {
            const data_id_t from_classifier_id = data_relationship_get_from_classifier_data_id( &((*this_).temp_relationship) );
            const bool container_is_this = data_id_equals( &from_classifier_id, &classifier_id );
            const bool is_containment
                = ( DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT == data_relationship_get_main_type( &((*this_).temp_relationship) ) );
            if ( container_is_this && is_containment )
            {
                const data_id_t relationship_id = data_relationship_get_data_id( &((*this_).temp_relationship) );
                const data_id_t from_feature_id = data_relationship_get_from_feature_data_id( &((*this_).temp_relationship) );
                const data_id_t to_feature_id = data_relationship_get_to_feature_data_id( &((*this_).temp_relationship) );
                const data_id_t to_classifier_id = data_relationship_get_to_classifier_data_id( &((*this_).temp_relationship) );
                if ( data_id_is_valid( &from_feature_id ) )
                {
                    U8_TRACE_INFO("Anomaly: A feature of this classifier contains something");
                }
                else if ( data_id_is_valid( &to_feature_id ) )
                {
                    U8_TRACE_INFO("Anomaly: This classifier contains a feature");
                }
                else if ( data_id_equals( &to_classifier_id, &classifier_id ) )
                {
                    U8_TRACE_INFO("Anomaly: This classifier contains itself");
                }
                else if ( data_small_set_contains( io_contained_classifiers, to_classifier_id ) )
                {
                    U8_TRACE_INFO("Anomaly: This classifier contains a child twice");
                }
                else
                {
                    data_small_set_add_obj ( io_contained_classifiers, to_classifier_id );
                    data_small_set_add_obj ( io_containment_relations, relationship_id );
                }
            }
        }
        else
        {
            assert( false );
        }
    }
    result |= data_relationship_iterator_destroy( &rel_iterator );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t io_export_model_traversal_private_walk_containments ( io_export_model_traversal_t *this_,
                                                                 data_id_t host_id,
                                                                 const data_small_set_t *contained_classifiers,
                                                                 const data_small_set_t *containment_relations,
                                                                 unsigned int recursion_depth )
{
    U8_TRACE_BEGIN();
    assert( contained_classifiers != NULL );
    assert( containment_relations != NULL );
    data_id_trace( &host_id );
    u8_error_t write_err = U8_ERROR_NONE;

    /* do recursion */
    if ( recursion_depth < IO_EXPORT_MODEL_TRAVERSAL_MAX_TREE_DEPTH )
    {
        const uint32_t children = data_small_set_get_count ( contained_classifiers );
        U8_TRACE_INFO_INT( "Children: ", children );
        assert( children == data_small_set_get_count ( containment_relations ) );
        for ( uint32_t index = 0; index < children; index ++ )
        {
            const data_id_t child = data_small_set_get_id ( contained_classifiers, index );
            const data_id_t c_rel = data_small_set_get_id ( containment_relations, index );
            write_err |= io_export_model_traversal_private_walk_node( this_, host_id, c_rel, child, recursion_depth+1 );
        }
    }

    U8_TRACE_END_ERR( write_err );
    return write_err;
}

u8_error_t io_export_model_traversal_private_end_node ( io_export_model_traversal_t *this_,
                                                        data_classifier_type_t host_type,
                                                        const data_classifier_t *classifier )
{
    U8_TRACE_BEGIN();
    assert( classifier != NULL );
    u8_error_t write_err = U8_ERROR_NONE;

    write_err |= io_element_writer_end_classifier( (*this_).element_writer, host_type, classifier );

    U8_TRACE_END_ERR( write_err );
    return write_err;
}

u8_error_t io_export_model_traversal_private_iterate_node_features ( io_export_model_traversal_t *this_,
                                                                     const data_classifier_t *classifier )
{
    U8_TRACE_BEGIN();
    assert( classifier != NULL );
    assert( data_classifier_is_valid( classifier ) );
    u8_error_t write_err = U8_ERROR_NONE;
    u8_error_t data_err = U8_ERROR_NONE;

    /* iterate over all features */
    data_feature_iterator_t feature_iterator;
    data_err |= data_feature_iterator_init_empty( &feature_iterator );
    data_err |= data_database_reader_get_features_by_classifier_id( (*this_).db_reader,
                                                                    data_classifier_get_row_id( classifier ),
                                                                    &feature_iterator
                                                                  );

    while ( data_feature_iterator_has_next( &feature_iterator ) )
    {
        /* get feature */
        data_err |= data_feature_iterator_next( &feature_iterator, &((*this_).temp_from_feature) );
        if ( data_feature_is_valid( &((*this_).temp_from_feature) ) )
        {
            const bool is_lifeline
                =( DATA_FEATURE_TYPE_LIFELINE == data_feature_get_main_type( &((*this_).temp_from_feature) ) );

            if ( ! is_lifeline )
            {
                const data_classifier_type_t classifier_type = data_classifier_get_main_type( classifier );
                write_err |= io_element_writer_start_feature( (*this_).element_writer, classifier_type,
                                                              &((*this_).temp_from_feature)
                                                            );
                write_err |= io_element_writer_assemble_feature( (*this_).element_writer,
                                                                 classifier,
                                                                 &((*this_).temp_from_feature)
                                                               );
                write_err |= io_element_writer_end_feature( (*this_).element_writer,
                                                            classifier_type,
                                                            &((*this_).temp_from_feature)
                                                          );
            }
            /* else: */
            /* lifelines are handled in io_export_model_traversal_private_fake_interactions_of_node */
        }
        else
        {
            assert( false );
        }
    }
    data_err |= data_feature_iterator_destroy( &feature_iterator );

    U8_TRACE_END_ERR( write_err | data_err );
    return write_err | data_err;
}

u8_error_t io_export_model_traversal_private_iterate_node_relationships ( io_export_model_traversal_t *this_,
                                                                          bool nested_to_foreign_node,
                                                                          const data_classifier_t *host,
                                                                          const data_classifier_t *classifier )
{
    U8_TRACE_BEGIN();
    assert( classifier != NULL );
    assert( data_classifier_is_valid( classifier ) );
    u8_error_t write_err = U8_ERROR_NONE;
    u8_error_t data_err = U8_ERROR_NONE;

    const data_classifier_type_t host_type
        = (host==NULL) ? DATA_CLASSIFIER_TYPE_PACKAGE : data_classifier_get_main_type( host );  /* a uml:Model is a uml:Package*/
    const data_id_t classifier_id = data_classifier_get_data_id( classifier );

    /* iterate over all relationships */
    data_relationship_iterator_t rel_iterator;
    data_err |= data_relationship_iterator_init_empty( &rel_iterator );
    data_err |= data_database_reader_get_relationships_by_classifier_id( (*this_).db_reader,
                                                                         data_classifier_get_row_id( classifier ),
                                                                         &rel_iterator
                                                                       );

    while ( data_relationship_iterator_has_next( &rel_iterator ) )
    {
        /* get relationship */
        data_err |= data_relationship_iterator_next( &rel_iterator, &((*this_).temp_relationship) );
        if ( data_relationship_is_valid( &((*this_).temp_relationship) ) )
        {
            /* determine if the relationship is a duplicate */
            const data_id_t relation_id = data_relationship_get_data_id( &((*this_).temp_relationship) );
            const bool duplicate_relationship
                = ( -1 != universal_array_list_get_index_of( &((*this_).written_id_set), &relation_id ) );
            if ( ! duplicate_relationship )
            {
                const data_id_t from_classifier_id = data_relationship_get_from_classifier_data_id( &((*this_).temp_relationship) );
                const bool source_already_written
                    = ( -1 != universal_array_list_get_index_of( &((*this_).written_id_set), &from_classifier_id ) );
                const data_id_t to_classifier_id = data_relationship_get_to_classifier_data_id( &((*this_).temp_relationship) );
                const bool destination_already_written
                    = ( -1 != universal_array_list_get_index_of( &((*this_).written_id_set), &to_classifier_id ) );

                const data_relationship_type_t r_type = data_relationship_get_main_type( &((*this_).temp_relationship) );
                const bool is_relationship_compliant_here
                    = io_element_writer_can_classifier_nest_relationship( (*this_).element_writer, host_type, r_type );
                const data_id_t from_feature_id = data_relationship_get_from_feature_data_id( &((*this_).temp_relationship) );
                const bool from_here
                    = ( ( ! data_id_is_valid( &from_feature_id ) )
                    && ( data_id_equals( &from_classifier_id, &classifier_id ) ) );

                /* nested_to_foreign_node is kind of yellow or even red emergency node:
                 * source_already_written and possibly destination_already_written must have passed
                 * to ensure that there is no other solution;
                 * either (yellow) is_relationship_compliant_here
                 * or (red) nesting to package shall be true.
                 */
                const bool foreign_yellow_ok
                    = nested_to_foreign_node && source_already_written && is_relationship_compliant_here;
                const bool foreign_red_ok
                    = nested_to_foreign_node && source_already_written && destination_already_written
                    && ( host_type == DATA_CLASSIFIER_TYPE_PACKAGE );

                /* in uml, the source is the dependant, the destination has no link to the source
                 */
                const bool local_ok = ( ! nested_to_foreign_node ) && is_relationship_compliant_here && from_here;

                if ( local_ok || foreign_yellow_ok || foreign_red_ok )
                {
                    /* add the relationship to the duplicates list */
                    write_err |= universal_array_list_append( &((*this_).written_id_set), &relation_id );

                    /* get the element types at both ends of the relationship */
                    data_classifier_init_empty( &((*this_).temp_from_classifier) );
                    data_feature_init_empty( &((*this_).temp_from_feature) );
                    data_classifier_init_empty( &((*this_).temp_to_classifier) );
                    data_feature_init_empty( &((*this_).temp_to_feature) );

                    const u8_error_t d_err
                        = io_export_model_traversal_private_get_relationship_ends( this_,
                                                                                   classifier,
                                                                                   &((*this_).temp_relationship),
                                                                                   &((*this_).temp_from_classifier),
                                                                                   &((*this_).temp_from_feature),
                                                                                   &((*this_).temp_to_classifier),
                                                                                   &((*this_).temp_to_feature)
                                                                                 );

                    if ( d_err == U8_ERROR_NONE )
                    {
                        /* destination classifier found, print the relation */
                        write_err |= io_element_writer_start_relationship( (*this_).element_writer, host_type, &((*this_).temp_relationship) );
                        write_err |= io_element_writer_assemble_relationship( (*this_).element_writer,
                                                                              host,
                                                                              &((*this_).temp_relationship),
                                                                              &((*this_).temp_from_classifier),
                                                                              &((*this_).temp_from_feature),
                                                                              &((*this_).temp_to_classifier),
                                                                              &((*this_).temp_to_feature)
                                                                            );
                        write_err |= io_element_writer_end_relationship( (*this_).element_writer, host_type, &((*this_).temp_relationship) );
                    }
                    else
                    {
                        write_err |= d_err;
                    }

                    data_classifier_destroy( &((*this_).temp_from_classifier) );
                    data_feature_destroy( &((*this_).temp_from_feature) );
                    data_classifier_destroy( &((*this_).temp_to_classifier) );
                    data_feature_destroy( &((*this_).temp_to_feature) );
                }
            }
        }
        else
        {
            assert( false );
        }
    }
    data_err |= data_relationship_iterator_destroy( &rel_iterator );

    U8_TRACE_END_ERR( write_err | data_err );
    return write_err | data_err;
}

u8_error_t io_export_model_traversal_private_get_relationship_ends( io_export_model_traversal_t *this_,
                                                                    const data_classifier_t *classifier,
                                                                    const data_relationship_t *relation,
                                                                    data_classifier_t *out_from_c,
                                                                    data_feature_t *out_from_f,
                                                                    data_classifier_t *out_to_c,
                                                                    data_feature_t *out_to_f )
{
    U8_TRACE_BEGIN();
    assert( classifier != NULL );
    assert( relation != NULL );
    assert( out_from_c != NULL );
    assert( out_from_f != NULL );
    assert( out_to_c != NULL );
    assert( out_to_f != NULL );
    u8_error_t data_err = U8_ERROR_NONE;

    {
        /* get from classifier */
        const data_row_t from_classifier_row_id = data_relationship_get_from_classifier_row_id( relation );
        if ( from_classifier_row_id == data_classifier_get_row_id ( classifier ) )
        {
            data_classifier_replace( out_from_c, classifier );
        }
        else
        {
            data_err |= data_database_reader_get_classifier_by_id( (*this_).db_reader,
                                                                   from_classifier_row_id,
                                                                   out_from_c
                                                                 );
        }

        /* get from feature */
        const data_row_t from_feature_row_id = data_relationship_get_from_feature_row_id( relation );
        if ( from_feature_row_id != DATA_ROW_VOID )
        {
            data_err |=  data_database_reader_get_feature_by_id( (*this_).db_reader,
                                                                 from_feature_row_id,
                                                                 out_from_f
                                                               );
        }
        else
        {
            data_feature_init_empty( out_from_f );
        }

        /* get to classifier */
        const data_row_t to_classifier_row_id = data_relationship_get_to_classifier_row_id( relation );
        if ( to_classifier_row_id == data_classifier_get_row_id ( classifier ) )
        {
            data_classifier_replace( out_to_c, classifier );
        }
        else
        {
            data_err |= data_database_reader_get_classifier_by_id( (*this_).db_reader,
                                                                   to_classifier_row_id,
                                                                   out_to_c
                                                                 );
        }

        /* get to feature */
        const data_row_t to_feature_row_id = data_relationship_get_to_feature_row_id( relation );
        if ( to_feature_row_id != DATA_ROW_VOID )
        {
            data_err |= data_database_reader_get_feature_by_id( (*this_).db_reader,
                                                                to_feature_row_id,
                                                                out_to_f
                                                              );
        }
        else
        {
            data_feature_init_empty( out_to_f );
        }
    }

    if ( data_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_INT( "A relationship references classifier(s) and/or feature(s) that do not exist:",
                          data_relationship_get_row_id ( relation )
                        );
    }
    U8_TRACE_END_ERR( data_err );
    return data_err;
}

u8_error_t io_export_model_traversal_private_fake_interactions_of_node ( io_export_model_traversal_t *this_,
                                                                         data_classifier_type_t nesting_type,
                                                                         const data_classifier_t *classifier )
{
    U8_TRACE_BEGIN();
    assert( classifier != NULL );
    assert( data_classifier_is_valid( classifier ) );
    u8_error_t write_err = U8_ERROR_NONE;
    u8_error_t data_err = U8_ERROR_NONE;

    const data_id_t classifier_id = data_classifier_get_data_id( classifier );

    /* iterate over all features, search for lifelines */
    data_feature_iterator_t feature_iterator;
    data_err |= data_feature_iterator_init_empty( &feature_iterator );
    data_err |= data_database_reader_get_features_by_classifier_id( (*this_).db_reader,
                                                                    data_classifier_get_row_id( classifier ),
                                                                    &feature_iterator
                                                                  );

    while ( data_feature_iterator_has_next( &feature_iterator ) )
    {
        /* get feature */
        data_err |= data_feature_iterator_next( &feature_iterator, &((*this_).temp_from_feature) );
        if ( data_feature_is_valid( &((*this_).temp_from_feature) ) )
        {
            /* determine if the feature is a duplicate */
            const data_id_t feature_id = data_feature_get_data_id( &((*this_).temp_from_feature) );
            const bool duplicate_feature
                = ( -1 != universal_array_list_get_index_of( &((*this_).written_id_set), &feature_id ) );

            /* determine if the feature is a lifeline */
            const data_feature_type_t feat_type = data_feature_get_main_type( &((*this_).temp_from_feature) );
            const bool is_lifeline = ( feat_type == DATA_FEATURE_TYPE_LIFELINE );

            if (( ! duplicate_feature )&&( is_lifeline ))  /* just an optimization, checked again by (*this_).interaction_helper */
            {
                static const data_classifier_type_t FAKE_INTERACTION
                    = DATA_CLASSIFIER_TYPE_CLASS; /* interaction is subclass of class */
                const bool is_interaction_compliant_here
                    = io_element_writer_can_classifier_nest_classifier( (*this_).element_writer, nesting_type, FAKE_INTERACTION );

                if ( is_interaction_compliant_here )
                {
                    write_err |= io_export_interaction_traversal_iterate_classifier_occurrences( &((*this_).interaction_helper),
                                                                                                 nesting_type,
                                                                                                 classifier_id
                                                                                               );
                }
            }
        }
        else
        {
            assert( false );
        }
    }
    data_err |= data_feature_iterator_destroy( &feature_iterator );

    U8_TRACE_END_ERR( write_err | data_err );
    return write_err | data_err;
}


/*
Copyright 2020-2025 Andreas Warnke

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
