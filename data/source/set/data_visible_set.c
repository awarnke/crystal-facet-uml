/* File: data_visible_set.c; Copyright and License: see below */

#include "set/data_visible_set.h"
#include "u8/u8_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void data_visible_set_init( data_visible_set_t *this_ )
{
    U8_TRACE_BEGIN();
    U8_TRACE_INFO_INT( "sizeof(data_visible_set_t):", sizeof(data_visible_set_t) );

    data_diagram_init_empty( &((*this_).diagram) );
    (*this_).visible_classifier_count = 0;
    (*this_).feature_count = 0;
    (*this_).relationship_count = 0;

    U8_TRACE_END();
}

void data_visible_set_reinit( data_visible_set_t *this_ )
{
    U8_TRACE_BEGIN();

    data_diagram_reinit_empty( &((*this_).diagram) );
    data_visible_set_private_destroy_visible_classifiers( this_ );
    data_visible_set_private_destroy_features( this_ );
    data_visible_set_private_destroy_relationships( this_ );

    U8_TRACE_END();

}

void data_visible_set_destroy( data_visible_set_t *this_ )
{
    U8_TRACE_BEGIN();

    data_diagram_destroy( &((*this_).diagram) );
    data_visible_set_private_destroy_visible_classifiers( this_ );
    data_visible_set_private_destroy_features( this_ );
    data_visible_set_private_destroy_relationships( this_ );

    U8_TRACE_END();
}

u8_error_t data_visible_set_load( data_visible_set_t *this_, data_row_id_t diagram_id, data_database_reader_t *db_reader )
{
    U8_TRACE_BEGIN();
    assert( NULL != db_reader );
    u8_error_t result = U8_ERROR_NONE;

    if ( DATA_ROW_ID_VOID == diagram_id )
    {
        /* re-init */
        data_diagram_reinit_empty( &((*this_).diagram) );

        data_visible_set_private_destroy_visible_classifiers( this_ );
        data_visible_set_private_destroy_features( this_ );
        data_visible_set_private_destroy_relationships( this_ );
    }
    else
    {
        data_diagram_destroy( &((*this_).diagram) );
        data_visible_set_private_destroy_visible_classifiers( this_ );
        data_visible_set_private_destroy_features( this_ );
        data_visible_set_private_destroy_relationships( this_ );

        /* load diagram */
        const u8_error_t db_err = data_database_reader_get_diagram_by_id ( db_reader, diagram_id, &((*this_).diagram) );

        if ( u8_error_contains( db_err, U8_ERROR_STRING_BUFFER_EXCEEDED ) )
        {
            U8_LOG_ERROR( "U8_ERROR_STRING_BUFFER_EXCEEDED at loading a diagram" );
        }
        if ( u8_error_more_than( db_err, U8_ERROR_STRING_BUFFER_EXCEEDED ) )
        {
            /* error at loading */
            data_diagram_reinit_empty( &((*this_).diagram) );
        }
        result |= db_err;  /* collect error flags */

        /* load classifiers */
        (*this_).visible_classifier_count = 0;
        data_visible_classifier_iterator_t visible_classifier_iterator;
        result |= data_visible_classifier_iterator_init_empty( &visible_classifier_iterator );
        result |= data_database_reader_get_visible_classifiers_by_diagram_id( db_reader,
                                                                              diagram_id,
                                                                              &visible_classifier_iterator
                                                                            );
        for ( int_fast32_t vc_idx = 0; (vc_idx < DATA_VISIBLE_SET_MAX_CLASSIFIERS) && data_visible_classifier_iterator_has_next( &visible_classifier_iterator ); vc_idx ++ )
        {
            result |= data_visible_classifier_iterator_next( &visible_classifier_iterator, &((*this_).visible_classifiers[vc_idx]) );
            (*this_).visible_classifier_count = vc_idx+1;
        }
        if ( data_visible_classifier_iterator_has_next( &visible_classifier_iterator ) )
        {
            result |= U8_ERROR_ARRAY_BUFFER_EXCEEDED;
            U8_LOG_ERROR( "U8_ERROR_ARRAY_BUFFER_EXCEEDED at loading visible classifiers of a diagram" );
        }
        result |= data_visible_classifier_iterator_destroy( &visible_classifier_iterator );

        /* load features */
        (*this_).feature_count = 0;
        data_feature_iterator_t feature_iterator;
        result |= data_feature_iterator_init_empty( &feature_iterator );
        result |= data_database_reader_get_features_by_diagram_id( db_reader,
                                                                   diagram_id,
                                                                   &feature_iterator
                                                                 );
        for ( int_fast32_t f_idx = 0; (f_idx < DATA_VISIBLE_SET_MAX_FEATURES) && data_feature_iterator_has_next( &feature_iterator ); f_idx ++ )
        {
            result |= data_feature_iterator_next( &feature_iterator, &((*this_).features[f_idx]) );
            (*this_).feature_count = f_idx+1;
        }
        if ( data_feature_iterator_has_next( &feature_iterator ) )
        {
            result |= U8_ERROR_ARRAY_BUFFER_EXCEEDED;
            U8_LOG_ERROR( "U8_ERROR_ARRAY_BUFFER_EXCEEDED at loading features of a diagram" );
        }
        result |= data_feature_iterator_destroy( &feature_iterator );

        /* load relationships */
        (*this_).relationship_count = 0;
        data_relationship_iterator_t rel_iterator;
        result |= data_relationship_iterator_init_empty( &rel_iterator );
        result |= data_database_reader_get_relationships_by_diagram_id( db_reader,
                                                                        diagram_id,
                                                                        &rel_iterator
                                                                      );
        for ( int_fast32_t r_idx = 0; (r_idx < DATA_VISIBLE_SET_MAX_RELATIONSHIPS) && data_relationship_iterator_has_next( &rel_iterator ); r_idx ++ )
        {
            result |= data_relationship_iterator_next( &rel_iterator, &((*this_).relationships[r_idx]) );
            (*this_).relationship_count = r_idx+1;
        }
        if ( data_feature_iterator_has_next( &feature_iterator ) )
        {
            result |= U8_ERROR_ARRAY_BUFFER_EXCEEDED;
            U8_LOG_ERROR( "U8_ERROR_ARRAY_BUFFER_EXCEEDED at loading relationships of a diagram" );
        }
        result |= data_relationship_iterator_destroy( &rel_iterator );
    }

    /* update the containment cache */
    data_visible_set_update_containment_cache( this_ );

    U8_TRACE_END_ERR(result);
    return result;
}

void data_visible_set_update_containment_cache ( data_visible_set_t *this_ )
{
    U8_TRACE_BEGIN();
    assert( (*this_).relationship_count <= DATA_VISIBLE_SET_MAX_RELATIONSHIPS );
    assert( (*this_).visible_classifier_count <= DATA_VISIBLE_SET_MAX_CLASSIFIERS );

    /* reset the containment array */
    memset( &((*this_).containment_cache), '\0', sizeof( (*this_).containment_cache ) );

    static const int MAX_SEARCH_PASSES = 4;  /* with 4 passes, we can find ancesters and decendants which are related via 2*4=8 links and more */
    bool new_containments_found = true;

    for ( int pass = 0; ( pass < MAX_SEARCH_PASSES ) && new_containments_found; pass ++ )
    {
        new_containments_found = false;

        for ( uint32_t rel_idx = 0; rel_idx < (*this_).relationship_count; rel_idx ++ )
        {
            data_relationship_t *the_relationship;
            the_relationship = &((*this_).relationships[rel_idx]);

            data_relationship_type_t the_type;
            the_type = data_relationship_get_main_type ( the_relationship );

            if ( DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT == the_type )
            {
                data_row_id_t parent_id;
                data_row_id_t child_id;
                parent_id = data_relationship_get_from_classifier_row_id ( the_relationship );
                child_id = data_relationship_get_to_classifier_row_id ( the_relationship );
                int32_t parent_index;
                int32_t child_index;
                parent_index = data_visible_set_get_classifier_index ( this_, parent_id );
                child_index = data_visible_set_get_classifier_index ( this_, child_id );
                if ( ( parent_index != -1 )&&( child_index != -1 ) )
                {
                    assert ( 0 <= parent_index );
                    assert ( parent_index < (*this_).visible_classifier_count );
                    assert ( 0 <= child_index );
                    assert ( child_index < (*this_).visible_classifier_count );

                    /* add the current relation to the containment_cache */
                    (*this_).containment_cache[parent_index][child_index] = true;

                    /* all ancestors of parent are ancestors of child */
                    for ( uint32_t ancestor_index = 0; ancestor_index < (*this_).visible_classifier_count; ancestor_index ++ )
                    {
                        if ( (*this_).containment_cache[ancestor_index][parent_index] )
                        {
                            if ( ! (*this_).containment_cache[ancestor_index][child_index] )
                            {
                                new_containments_found = true;
                                (*this_).containment_cache[ancestor_index][child_index] = true;
                            }
                        }
                    }

                    /* all descendants of child are descendants of parent */
                    for ( uint32_t descendant_index = 0; descendant_index < (*this_).visible_classifier_count; descendant_index ++ )
                    {
                        if ( (*this_).containment_cache[child_index][descendant_index] )
                        {
                            if ( ! (*this_).containment_cache[parent_index][descendant_index] )
                            {
                                new_containments_found = true;
                                (*this_).containment_cache[parent_index][descendant_index] = true;
                            }
                        }
                    }

                }
                /* else relation points outside the set of classifiers visible in current diagram */
            }
        }
    }

    U8_TRACE_END();
}


/*
Copyright 2016-2024 Andreas Warnke

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
