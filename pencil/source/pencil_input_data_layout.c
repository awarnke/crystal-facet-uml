/* File: pencil_input_data_layout.c; Copyright and License: see below */

#include "pencil_input_data_layout.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

void pencil_input_data_layout_init( pencil_input_data_layout_t *this_ )
{
    TRACE_BEGIN();
    TRACE_INFO_INT( "sizeof(pencil_input_data_layout_t):", sizeof(pencil_input_data_layout_t) );

    (*this_).diagram_valid = false;
    (*this_).visible_classifier_count = 0;
    (*this_).feature_count = 0;
    (*this_).relationship_count = 0;
    (*this_).input_data = NULL;

    TRACE_END();
}

void pencil_input_data_layout_reinit( pencil_input_data_layout_t *this_, pencil_input_data_t *input_data )
{
    TRACE_BEGIN();
    assert ( NULL != input_data );

    /* clean up */
    pencil_input_data_layout_destroy( this_ );

    /* init input data */
    (*this_).input_data = input_data;

    /* init diagram */
    {
        (*this_).diagram_valid = true;
        layout_diagram_init ( &((*this_).diagram_layout),
                              pencil_input_data_get_diagram_ptr( input_data )
                            );

        /* TRACE_INFO_INT ( "diagram      data    objects:", 1 ); */
        /* TRACE_INFO_INT ( "diagram      ignored objects:", 0 ); */  /* we do not ignore diagram objects */
        /* TRACE_INFO_INT ( "diagram      layout  objects:", 1 ); */
    }

    /* init visible classifiers */
    {
        const uint32_t data_classifier_count = pencil_input_data_get_visible_classifier_count( input_data );
        (*this_).visible_classifier_count = data_classifier_count;
        assert ( (*this_).visible_classifier_count <= PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS );

        for ( uint32_t c_idx = 0; c_idx < data_classifier_count; c_idx ++ )
        {
            layout_visible_classifier_init( &((*this_).visible_classifier_layout[c_idx]),
                                            pencil_input_data_get_visible_classifier_ptr( input_data, c_idx )
                                          );
        }

        TRACE_INFO_INT ( "classifier   data    objects:", data_classifier_count );
        TRACE_INFO_INT ( "classifier   ignored objects:", 0 );  /* we do not ignore classifier objects */
        TRACE_INFO_INT ( "classifier   layout  objects:", (*this_).visible_classifier_count );
    }

    /* init features */
    {
        const uint32_t data_feature_count = pencil_input_data_get_feature_count( input_data );
        uint32_t debug_dropped_features;
        debug_dropped_features = 0;
        (*this_).feature_count = 0;

        for ( uint32_t f_idx = 0; f_idx < data_feature_count; f_idx ++ )
        {
            const data_feature_t *feature_data;
            feature_data = pencil_input_data_get_feature_ptr( input_data, f_idx );
            const int64_t parent_id = data_feature_get_classifier_id( feature_data );
            uint32_t layout_feature_count = 0;

            for ( uint32_t c_idx2 = 0; c_idx2 < (*this_).visible_classifier_count; c_idx2 ++ )
            {
                layout_visible_classifier_t *parent_classifier;
                parent_classifier = &((*this_).visible_classifier_layout[c_idx2]);

                if ( parent_id == layout_visible_classifier_get_classifier_id( parent_classifier ) )
                {
                    if ( (*this_).feature_count < PENCIL_INPUT_DATA_LAYOUT_MAX_FEATURES )
                    {
                        layout_feature_init( &((*this_).feature_layout[(*this_).feature_count]),
                                             feature_data,
                                             parent_classifier
                                           );
                        (*this_).feature_count ++;
                        layout_feature_count ++;
                    }
                    else
                    {
                        TSLOG_WARNING( "PENCIL_INPUT_DATA_LAYOUT_MAX_FEATURES exceeded, some features not visible" );
                    }
                }
            }

            if ( layout_feature_count == 0 )
            {
                /* no warning here, dropping features - e.g. lifelines for other diagrams - is ok */
                debug_dropped_features ++;
            }
        }

        TRACE_INFO_INT ( "feature      data    objects:", data_feature_count );
        TRACE_INFO_INT ( "feature      ignored objects:", debug_dropped_features );
        TRACE_INFO_INT ( "feature      layout  objects:", (*this_).feature_count );
    }

    /* init relationships */
    {
        const uint32_t data_relationship_count = pencil_input_data_get_relationship_count( input_data );
        uint32_t debug_dropped_relationships;
        debug_dropped_relationships = 0;
        (*this_).relationship_count = 0;

        for ( uint32_t r_idx = 0; r_idx < data_relationship_count; r_idx ++ )
        {
            const data_relationship_t *relationship_data;
            relationship_data = pencil_input_data_get_relationship_ptr( input_data, r_idx );
            const int64_t from_classifier_id = data_relationship_get_from_classifier_id( relationship_data );
            const int64_t to_classifier_id = data_relationship_get_to_classifier_id( relationship_data );
            const int64_t from_feature_id = data_relationship_get_from_feature_id( relationship_data );
            const int64_t to_feature_id = data_relationship_get_to_feature_id( relationship_data );
            uint32_t layout_relationship_count = 0;

            if ( DATA_ID_VOID_ID == from_feature_id )  /* search source in classifiers */
            {
                for ( uint32_t c_idx3 = 0; c_idx3 < (*this_).visible_classifier_count; c_idx3 ++ )
                {
                    layout_visible_classifier_t *probe3_classifier;
                    probe3_classifier = &((*this_).visible_classifier_layout[c_idx3]);

                    if ( from_classifier_id == layout_visible_classifier_get_classifier_id( probe3_classifier ) )  /* source found */
                    {
                        if ( DATA_ID_VOID_ID == to_feature_id )  /* search destination in classifiers */
                        {
                            for ( uint32_t c_idx4 = 0; c_idx4 < (*this_).visible_classifier_count; c_idx4 ++ )
                            {
                                layout_visible_classifier_t *probe4_classifier;
                                probe4_classifier = &((*this_).visible_classifier_layout[c_idx4]);

                                if ( to_classifier_id == layout_visible_classifier_get_classifier_id( probe4_classifier ) )  /* destination found */
                                {
                                    if ( (*this_).relationship_count < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS )
                                    {
                                        layout_relationship_init( &((*this_).relationship_layout[(*this_).relationship_count]),
                                                                  pencil_input_data_get_relationship_ptr( input_data, r_idx ),
                                                                  probe3_classifier,
                                                                  probe4_classifier,
                                                                  NULL,
                                                                  NULL
                                                                );
                                        (*this_).relationship_count ++;
                                        layout_relationship_count ++;
                                    }
                                    else
                                    {
                                        TSLOG_WARNING( "PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS exceeded, some relationships not visible" );
                                    }
                                }
                            }
                        }
                        else  /* search destination in features */
                        {
                            for ( uint32_t f_idx4 = 0; f_idx4 < (*this_).feature_count; f_idx4 ++ )
                            {
                                layout_feature_t *probe4_feature;
                                probe4_feature = &((*this_).feature_layout[f_idx4]);

                                if ( to_feature_id == layout_feature_get_feature_id( probe4_feature ) )  /* destination found */
                                {
                                    if ( (*this_).relationship_count < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS )
                                    {
                                        layout_relationship_init( &((*this_).relationship_layout[(*this_).relationship_count]),
                                                                  pencil_input_data_get_relationship_ptr( input_data, r_idx ),
                                                                  probe3_classifier,
                                                                  layout_feature_get_classifier_ptr( probe4_feature ),
                                                                  NULL,
                                                                  probe4_feature
                                                                );
                                        (*this_).relationship_count ++;
                                        layout_relationship_count ++;
                                    }
                                    else
                                    {
                                        TSLOG_WARNING( "PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS exceeded, some relationships not visible" );
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else  /* search source in features */
            {
                for ( uint32_t f_idx3 = 0; f_idx3 < (*this_).feature_count; f_idx3 ++ )
                {
                    layout_feature_t *probe3_feature;
                    probe3_feature = &((*this_).feature_layout[f_idx3]);

                    if ( from_feature_id == layout_feature_get_feature_id( probe3_feature ) )  /* source found */
                    {
                        if ( DATA_ID_VOID_ID == to_feature_id )  /* search destination in classifiers */
                        {
                            for ( uint32_t c_idx5 = 0; c_idx5 < (*this_).visible_classifier_count; c_idx5 ++ )
                            {
                                layout_visible_classifier_t *probe5_classifier;
                                probe5_classifier = &((*this_).visible_classifier_layout[c_idx5]);

                                if ( to_classifier_id == layout_visible_classifier_get_classifier_id( probe5_classifier ) )  /* destination found */
                                {
                                    if ( (*this_).relationship_count < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS )
                                    {
                                        layout_relationship_init( &((*this_).relationship_layout[(*this_).relationship_count]),
                                                                  pencil_input_data_get_relationship_ptr( input_data, r_idx ),
                                                                  layout_feature_get_classifier_ptr( probe3_feature ),
                                                                  probe5_classifier,
                                                                  probe3_feature,
                                                                  NULL
                                                                );
                                        (*this_).relationship_count ++;
                                        layout_relationship_count ++;
                                    }
                                    else
                                    {
                                        TSLOG_WARNING( "PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS exceeded, some relationships not visible" );
                                    }
                                }
                            }
                        }
                        else  /* search destination in features */
                        {
                            for ( uint32_t f_idx5 = 0; f_idx5 < (*this_).feature_count; f_idx5 ++ )
                            {
                                layout_feature_t *probe5_feature;
                                probe5_feature = &((*this_).feature_layout[f_idx5]);

                                if ( to_feature_id == layout_feature_get_feature_id( probe5_feature ) )  /* destination found */
                                {
                                    if ( (*this_).relationship_count < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS )
                                    {
                                        layout_relationship_init( &((*this_).relationship_layout[(*this_).relationship_count]),
                                                                  pencil_input_data_get_relationship_ptr( input_data, r_idx ),
                                                                  layout_feature_get_classifier_ptr( probe3_feature ),
                                                                  layout_feature_get_classifier_ptr( probe5_feature ),
                                                                  probe3_feature,
                                                                  probe5_feature
                                                                );
                                        (*this_).relationship_count ++;
                                        layout_relationship_count ++;
                                    }
                                    else
                                    {
                                        TSLOG_WARNING( "PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS exceeded, some relationships not visible" );
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if ( layout_relationship_count == 0 )
            {
                /* no warning here, dropping relationships - e.g. only one end-object visible in current diagram - is ok */
                debug_dropped_relationships ++;
            }
        }

        TRACE_INFO_INT ( "relationship data    objects:", data_relationship_count );
        TRACE_INFO_INT ( "relationship ignored objects:", debug_dropped_relationships );
        TRACE_INFO_INT ( "relationship layout  objects:", (*this_).relationship_count );
    }

    TRACE_END();
}

void pencil_input_data_layout_destroy( pencil_input_data_layout_t *this_ )
{
    TRACE_BEGIN();

    if ( (*this_).diagram_valid )
    {
        layout_diagram_destroy ( &((*this_).diagram_layout) );
    }

    for ( uint32_t c_idx = 0; c_idx < (*this_).visible_classifier_count; c_idx ++ )
    {
        layout_visible_classifier_destroy( &((*this_).visible_classifier_layout[c_idx]) );
    }

    for ( uint32_t f_idx = 0; f_idx < (*this_).feature_count; f_idx ++ )
    {
        layout_feature_destroy( &((*this_).feature_layout[f_idx]) );
    }

    for ( uint32_t r_idx = 0; r_idx < (*this_).relationship_count; r_idx ++ )
    {
        layout_relationship_destroy( &((*this_).relationship_layout[r_idx]) );
    }

    TRACE_END();
}


/*
Copyright 2017-2018 Andreas Warnke

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
