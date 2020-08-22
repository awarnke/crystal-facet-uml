/* File: io_export_model_traversal.c; Copyright and License: see below */

#include "io_export_model_traversal.h"
#include "trace.h"
#include "data_diagram.h"
#include "data_classifier.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void io_export_model_traversal_init( io_export_model_traversal_t *this_,
                                     data_database_reader_t *db_reader,
                                     io_format_writer_t *format_writer )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != format_writer );

    (*this_).db_reader = db_reader;
    (*this_).format_writer = format_writer;

    universal_array_list_init ( &((*this_).written_id_set),
                                sizeof((*this_).written_id_set_buf)/sizeof(data_id_t),
                                &((*this_).written_id_set_buf),
                                sizeof(data_id_t),
                                ((char*)(&((*this_).written_id_set_buf[1])))-((char*)(&((*this_).written_id_set_buf[0]))),
                                (void (*)(void *, const void *)) &data_id_copy,
                                (bool (*)(const void *, const void *)) &data_id_equals,
                                (void (*)(void *)) &data_id_destroy
                              );

    TRACE_END();
}

void io_export_model_traversal_destroy( io_export_model_traversal_t *this_ )
{
    TRACE_BEGIN();

    universal_array_list_destroy ( &((*this_).written_id_set) );

    (*this_).db_reader = NULL;

    TRACE_END();
}

int io_export_model_traversal_walk_model_nodes ( io_export_model_traversal_t *this_ )
{
    TRACE_BEGIN();
    int write_err = 0;

    {
        data_error_t data_err;
        data_database_iterator_classifiers_t classifier_iterator;

        /* test the iterator, init */
        data_database_iterator_classifiers_init_empty( &classifier_iterator );
        data_err = data_database_reader_get_all_classifiers_iterator ( (*this_).db_reader, &classifier_iterator );
        if ( data_err != DATA_ERROR_NONE )
        {
            write_err = -1;
        }
        else
        {
            while( data_database_iterator_classifiers_has_next( &classifier_iterator ) && ( write_err==0 ) )
            {
                data_err = data_database_iterator_classifiers_next( &classifier_iterator, &((*this_).temp_classifier) );
                if ( data_err != DATA_ERROR_NONE )
                {
                    write_err = -1;
                }
                else
                {
                    write_err |= io_export_model_traversal_private_walk_unique_node ( this_,
                                                                                      DATA_CLASSIFIER_TYPE_UML_PACKAGE, /* a uml:Model is a uml:Package*/
                                                                                      data_classifier_get_data_id( &((*this_).temp_classifier) ),
                                                                                      DATA_ID_VOID,
                                                                                      0 /* initial recursion_depth */
                                                                                    );
                    data_classifier_destroy( &((*this_).temp_classifier) );
                }
            }
        }
        data_err = data_database_iterator_classifiers_destroy( &classifier_iterator );
        if ( data_err != DATA_ERROR_NONE )
        {
            write_err = -1;
        }
    }

    TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_model_traversal_private_walk_unique_node ( io_export_model_traversal_t *this_,
                                                         data_classifier_type_t parent_type,
                                                         data_id_t classifier_id,
                                                         data_id_t containment_relationship_id,
                                                         unsigned int recursion_depth )
{
    TRACE_BEGIN();
    int write_err = 0;

    const bool duplicate_classifier
        =( -1 != universal_array_list_get_index_of( &((*this_).written_id_set), &classifier_id ) );
    if ( ! duplicate_classifier )
    {
        write_err |= universal_array_list_append( &((*this_).written_id_set), &classifier_id );
        if ( data_id_is_valid( &containment_relationship_id ) )
        {
            write_err |= universal_array_list_append( &((*this_).written_id_set), &containment_relationship_id );
        }

        /* walk node which is not a duplicate */
        write_err |= io_export_model_traversal_private_walk_node( this_, parent_type, classifier_id, recursion_depth );
    }

    TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_model_traversal_private_walk_node ( io_export_model_traversal_t *this_,
                                                  data_classifier_type_t parent_type,
                                                  data_id_t classifier_id,
                                                  unsigned int recursion_depth )
{
    TRACE_BEGIN();
    int write_err = 0;

    data_small_set_t contained_classifiers;
    data_small_set_init (&contained_classifiers);
    data_small_set_t containment_relations;
    data_small_set_init (&containment_relations);
    data_classifier_type_t classifier_type = DATA_CLASSIFIER_TYPE_UML_PACKAGE;  /* default */

    /* load and process the node set */
    {
        data_node_set_init( &((*this_).temp_node_data) );
        const data_error_t data_err_1 = data_node_set_load( &((*this_).temp_node_data),
                                                            data_id_get_row_id( &classifier_id ),
                                                            (*this_).db_reader
                                                          );
        if ( data_err_1 != DATA_ERROR_NONE )
        {
            write_err = -1;
        }
        else
        {
            /* get classifier */
            const data_classifier_t *const classifier
                = data_node_set_get_classifier_const ( &((*this_).temp_node_data) );
            classifier_type = data_classifier_get_main_type( classifier );
            write_err |= io_format_writer_start_nested_classifier( (*this_).format_writer, parent_type, classifier );
            write_err |= io_format_writer_write_classifier( (*this_).format_writer, classifier );

            write_err |= io_export_model_traversal_private_iterate_node_features( this_, &((*this_).temp_node_data) );
        }

        /* search containments in relationships */
        const uint32_t count = data_node_set_get_relationship_count ( &((*this_).temp_node_data) );
        for ( uint32_t index = 0; index < count; index ++ )
        {
            /* get relationship */
            const data_relationship_t *relation;
            relation = data_node_set_get_relationship_const ( &((*this_).temp_node_data), index );
            if (( relation != NULL ) && ( data_relationship_is_valid( relation ) ))
            {
                if ( DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT == data_relationship_get_main_type( relation ) )
                {
                    const data_id_t relationship_id = data_relationship_get_data_id( relation );
                    const data_id_t from_feature_id = data_relationship_get_from_feature_data_id( relation );
                    const data_id_t to_feature_id = data_relationship_get_to_feature_data_id( relation );
                    const data_id_t to_classifier_id = data_relationship_get_to_classifier_data_id( relation );
                    if ( ( ! data_id_is_valid( &from_feature_id ) )
                        && ( ! data_id_is_valid( &to_feature_id ) )
                        && ( ! data_id_equals( &to_classifier_id, &classifier_id ) ) )
                    {
                        data_small_set_add_obj ( &containment_relations, relationship_id );
                        data_small_set_add_obj ( &contained_classifiers, to_classifier_id );
                    }
                }
            }
            else
            {
                assert( false );
            }
        }

        data_node_set_destroy( &((*this_).temp_node_data) );
    }

    /* do recursion, all required data is stored on the stack now */
    if ( recursion_depth < IO_EXPORT_MODEL_TRAVERSAL_MAX_TREE_DEPTH )
    {
        const uint32_t children = data_small_set_get_count ( &contained_classifiers );
        assert( children == data_small_set_get_count ( &containment_relations ) );
        for ( uint32_t index = 0; index < children; index ++ )
        {
            const data_id_t child = data_small_set_get_id ( &contained_classifiers, index );
            const data_id_t c_rel = data_small_set_get_id ( &containment_relations, index );
            write_err |= io_export_model_traversal_private_walk_unique_node( this_, classifier_type, child, c_rel, recursion_depth+1 );
        }
    }

    /* load and process the node set a second time to print the rest */
    {
        data_node_set_init( &((*this_).temp_node_data) );
        const data_error_t data_err_2 = data_node_set_load( &((*this_).temp_node_data),
                                                            data_id_get_row_id( &classifier_id ),
                                                            (*this_).db_reader
                                                          );
        if ( data_err_2 != DATA_ERROR_NONE )
        {
            write_err = -1;
        }
        else
        {
            /* get classifier again */
            const data_classifier_t *const classifier
                = data_node_set_get_classifier_const ( &((*this_).temp_node_data) );
            write_err |= io_format_writer_end_nested_classifier( (*this_).format_writer, parent_type, classifier );

            write_err |= io_export_model_traversal_private_iterate_node_relationships( this_,
                                                                                       &((*this_).temp_node_data)
                                                                                     );
        }
    }

    data_small_set_destroy (&containment_relations);
    data_small_set_destroy (&contained_classifiers);

    TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_model_traversal_private_iterate_node_features ( io_export_model_traversal_t *this_,
                                                              const data_node_set_t *node_data )
{
    TRACE_BEGIN();
    assert( node_data != NULL );
    assert( data_node_set_is_valid( node_data ) );
    int write_err = 0;

    /* iterate over all features */
    const uint32_t count = data_node_set_get_feature_count ( node_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        /* get feature */
        const data_feature_t *feature;
        feature = data_node_set_get_feature_const ( node_data, index );
        if (( feature != NULL ) && ( data_feature_is_valid( feature ) ))
        {
            write_err |=  io_format_writer_write_feature( (*this_).format_writer, feature );
        }
        else
        {
            assert( false );
        }
    }

    TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_model_traversal_private_iterate_node_relationships ( io_export_model_traversal_t *this_,
                                                                   const data_node_set_t *node_data )
{
    TRACE_BEGIN();
    assert( node_data != NULL );
    assert( data_node_set_is_valid( node_data ) );
    int write_err = 0;

    /* iterate over all relationships */
    const uint32_t count = data_node_set_get_relationship_count ( node_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        /* get relationship */
        const data_relationship_t *relation;
        relation = data_node_set_get_relationship_const ( node_data, index );
        if (( relation != NULL ) && ( data_relationship_is_valid( relation ) ))
        {
            /* determine if the relationship is a duplicate */
            const data_id_t relation_id = data_relationship_get_data_id( relation );
            const bool duplicate_relationship
                = ( -1 != universal_array_list_get_index_of( &((*this_).written_id_set), &relation_id ) );

            const data_id_t from_classifier_id = data_relationship_get_from_classifier_data_id( relation );
            const bool source_already_written
                = ( -1 != universal_array_list_get_index_of( &((*this_).written_id_set), &from_classifier_id ) );
            const data_id_t to_classifier_id = data_relationship_get_to_classifier_data_id( relation );
            const bool destination_already_written
                = ( -1 != universal_array_list_get_index_of( &((*this_).written_id_set), &to_classifier_id ) );

            if ( source_already_written && destination_already_written && ( ! duplicate_relationship ))
            {
                /* add the relationship to the duplicates list */
                if ( ! duplicate_relationship )
                {
                    write_err |= universal_array_list_append( &((*this_).written_id_set), &relation_id );
                }

                /* destination classifier found, print the relation */
                write_err |= io_format_writer_write_relationship( (*this_).format_writer,
                                                                  relation,
                                                                  NULL /* destination classifier not at hand here */
                                                                );
            }
        }
        else
        {
            assert( false );
        }
    }

    TRACE_END_ERR( write_err );
    return write_err;
}


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
