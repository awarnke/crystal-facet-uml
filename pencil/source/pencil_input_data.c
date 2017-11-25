/* File: pencil_input_data.c; Copyright and License: see below */

#include "pencil_input_data.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_input_data_init( pencil_input_data_t *this_ )
{
    TRACE_BEGIN();

    data_diagram_init_empty( &((*this_).diagram) );
    (*this_).visible_classifier_count = 0;
    (*this_).feature_count = 0;
    (*this_).relationship_count = 0;

    TRACE_END();
}

void pencil_input_data_destroy( pencil_input_data_t *this_ )
{
    TRACE_BEGIN();

    data_diagram_destroy( &((*this_).diagram) );
    pencil_input_data_private_destroy_visible_classifiers( this_ );
    pencil_input_data_private_destroy_features( this_ );
    pencil_input_data_private_destroy_relationships( this_ );

    TRACE_END();
}

void pencil_input_data_load( pencil_input_data_t *this_, int64_t diagram_id, data_database_reader_t *db_reader )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );

    if ( DATA_ID_VOID_ID == diagram_id )
    {
        /* re-init */
        data_diagram_reinit_empty( &((*this_).diagram) );

        pencil_input_data_private_destroy_visible_classifiers( this_ );
        pencil_input_data_private_destroy_features( this_ );
        pencil_input_data_private_destroy_relationships( this_ );
    }
    else
    {
        data_error_t db_err;

        data_diagram_destroy( &((*this_).diagram) );
        pencil_input_data_private_destroy_visible_classifiers( this_ );
        pencil_input_data_private_destroy_features( this_ );
        pencil_input_data_private_destroy_relationships( this_ );

        /* load diagram */
        db_err = data_database_reader_get_diagram_by_id ( db_reader, diagram_id, &((*this_).diagram) );

        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_STRING_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_STRING_BUFFER_EXCEEDED at loading a diagram" );
        }
        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_ARRAY_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_ARRAY_BUFFER_EXCEEDED at loading a diagram" );
        }
        if ( DATA_ERROR_NONE != (db_err & ~(DATA_ERROR_STRING_BUFFER_EXCEEDED|DATA_ERROR_ARRAY_BUFFER_EXCEEDED)) )
        {
            /* error at loading */
            data_diagram_reinit_empty( &((*this_).diagram) );
        }

        /* load classifiers */
        db_err = data_database_reader_get_classifiers_by_diagram_id ( db_reader,
                                                                      diagram_id,
                                                                      PENCIL_INPUT_DATA_MAX_CLASSIFIERS,
                                                                      &((*this_).visible_classifiers),
                                                                      &((*this_).visible_classifier_count)
                                                                    );

        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_STRING_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_STRING_BUFFER_EXCEEDED at loading visible classifiers of a diagram" );
        }
        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_ARRAY_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_ARRAY_BUFFER_EXCEEDED at loading visible classifiers of a diagram" );
        }
        if ( DATA_ERROR_NONE != (db_err & ~(DATA_ERROR_STRING_BUFFER_EXCEEDED|DATA_ERROR_ARRAY_BUFFER_EXCEEDED)) )
        {
            /* error at loading */
            (*this_).visible_classifier_count = 0;
        }

        /* load features */
        db_err = data_database_reader_get_features_by_diagram_id ( db_reader,
                                                                   diagram_id,
                                                                   PENCIL_INPUT_DATA_MAX_FEATURES,
                                                                   &((*this_).features),
                                                                   &((*this_).feature_count)
                                                                 );

        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_STRING_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_STRING_BUFFER_EXCEEDED at loading features of a diagram" );
        }
        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_ARRAY_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_ARRAY_BUFFER_EXCEEDED at loading features of a diagram" );
        }
        if ( DATA_ERROR_NONE != (db_err & ~(DATA_ERROR_STRING_BUFFER_EXCEEDED|DATA_ERROR_ARRAY_BUFFER_EXCEEDED)) )
        {
            /* error at loading */
            (*this_).feature_count = 0;
        }

        /* load relationships */
        db_err = data_database_reader_get_relationships_by_diagram_id ( db_reader,
                                                                        diagram_id,
                                                                        PENCIL_INPUT_DATA_MAX_RELATIONSHIPS,
                                                                        &((*this_).relationships),
                                                                        &((*this_).relationship_count)
                                                                      );

        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_STRING_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_STRING_BUFFER_EXCEEDED at loading relationships of a diagram" );
        }
        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_ARRAY_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_ARRAY_BUFFER_EXCEEDED at loading relationships of a diagram" );
        }
        if ( DATA_ERROR_NONE != (db_err & ~(DATA_ERROR_STRING_BUFFER_EXCEEDED|DATA_ERROR_ARRAY_BUFFER_EXCEEDED)) )
        {
            /* error at loading */
            (*this_).relationship_count = 0;
        }
    }

    TRACE_END();
}

bool pencil_input_data_is_parent ( pencil_input_data_t *this_, int64_t parent_id, int64_t child_id )
{
    TRACE_BEGIN();
    assert( (*this_).relationship_count <= PENCIL_INPUT_DATA_MAX_RELATIONSHIPS );
    bool result = false;

    for ( uint32_t rel_idx = 0; rel_idx < (*this_).relationship_count; rel_idx ++ )
    {
        data_relationship_t *the_relationship;
        the_relationship = &((*this_).relationships[rel_idx]);

        data_relationship_type_t the_type;
        the_type = data_relationship_get_main_type ( the_relationship );

        if ( DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT == the_type )
        {
            int64_t from_id;
            int64_t to_id;
            from_id = data_relationship_get_from_classifier_id ( the_relationship );
            to_id = data_relationship_get_to_classifier_id ( the_relationship );
            if (( from_id == parent_id )&&( to_id == child_id ))
            {
                result = true;
                break;
            }
        }
    }

    TRACE_END();
    return result;
}

bool pencil_input_data_is_ancestor ( pencil_input_data_t *this_, int64_t ancestor_id, int64_t descendant_id )
{
    TRACE_BEGIN();
    assert( (*this_).relationship_count <= PENCIL_INPUT_DATA_MAX_RELATIONSHIPS );
    bool result = false;

    /* initialize one set of descendants of ancestor and another set of ancestors of decendants */
    data_small_set_t ancestor_set;
    data_small_set_t descendant_set;
    data_small_set_init ( &ancestor_set );
    data_small_set_init ( &descendant_set );

    data_error_t data_err = DATA_ERROR_NONE;
    data_err |= data_small_set_add_row_id ( &ancestor_set, DATA_TABLE_CLASSIFIER, ancestor_id );
    data_err |= data_small_set_add_row_id ( &descendant_set, DATA_TABLE_CLASSIFIER, descendant_id );

    static const int MAX_SEARCH_PASSES = 3;  /* with 3 passes, we can find ancesters and decendants which are related via 2*3=6 links */
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
                int64_t from_id;
                int64_t to_id;
                from_id = data_relationship_get_from_classifier_id ( the_relationship );
                to_id = data_relationship_get_to_classifier_id ( the_relationship );

                if ( data_small_set_contains_row_id( &ancestor_set, DATA_TABLE_CLASSIFIER, from_id ) )
                {
                    if ( data_small_set_contains_row_id( &descendant_set, DATA_TABLE_CLASSIFIER, to_id ) )
                    {
                        /* this is the final missing link */
                        result = true;
                        new_containments_found = false; /* break also the outer loop */
                        break;
                    }
                    else
                    {
                        /* this is the right ancestor, add to_id */
                        data_err |= data_small_set_add_row_id ( &ancestor_set, DATA_TABLE_CLASSIFIER, to_id );
                        new_containments_found = true;
                    }
                }
                else
                {
                    if ( data_small_set_contains_row_id( &descendant_set, DATA_TABLE_CLASSIFIER, to_id ) )
                    {
                        /* this is the right ancestor, add to_id */
                        data_err |= data_small_set_add_row_id ( &descendant_set, DATA_TABLE_CLASSIFIER, from_id );
                        new_containments_found = true;
                    }
                    else
                    {
                        /* this relation has nothing to do with descendant_set or ancestor_set */
                    }
                }
            }
        }
    }

    if ( data_err != DATA_ERROR_NONE )
    {
        TRACE_INFO_HEX( "Some error occurred at adding items to a data_small_set_t:", data_err );
    }

    TRACE_END();
    return result;
}

data_visible_classifier_t *pencil_input_data_get_visible_classifier_by_id_ptr ( pencil_input_data_t *this_, int64_t diagramelement_id )
{
    TRACE_BEGIN();
    assert( (*this_).visible_classifier_count <= PENCIL_INPUT_DATA_MAX_CLASSIFIERS );
    data_visible_classifier_t *result = NULL;

    /* iterate over all visible classifiers */
    uint32_t count;
    for ( uint32_t index = 0; index < (*this_).visible_classifier_count; index ++ )
    {
        data_visible_classifier_t *visible_classifier;
        visible_classifier = &((*this_).visible_classifiers[index]);

        if ( data_visible_classifier_is_valid( visible_classifier ) )
        {
            data_diagramelement_t *diagramelement;
            diagramelement = data_visible_classifier_get_diagramelement_ptr( visible_classifier );
            if ( data_diagramelement_get_id( diagramelement ) == diagramelement_id )
            {
                result = visible_classifier;
                break;
            }
        }
    }

    TRACE_END();
    return result;
}


/*
Copyright 2016-2017 Andreas Warnke

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
