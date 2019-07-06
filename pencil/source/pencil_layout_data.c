/* File: pencil_layout_data.c; Copyright and License: see below */

#include "pencil_layout_data.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

void pencil_layout_data_init( pencil_layout_data_t *this_ )
{
    TRACE_BEGIN();
    TRACE_INFO_INT( "sizeof(pencil_layout_data_t):", sizeof(pencil_layout_data_t) );

    (*this_).diagram_valid = false;
    (*this_).visible_classifier_count = 0;
    (*this_).feature_count = 0;
    (*this_).relationship_count = 0;
    (*this_).input_data = NULL;
    data_rules_init ( &((*this_).filter_rules) );

    TRACE_END();
}

void pencil_layout_data_reinit( pencil_layout_data_t *this_, data_visible_set_t *input_data )
{
    TRACE_BEGIN();
    assert ( NULL != input_data );

    /* clean up */
    pencil_layout_data_destroy( this_ );

    /* init input data */
    (*this_).input_data = input_data;

    /* initialite the layout data objects */
    pencil_layout_data_private_init_diagram( this_ );
    pencil_layout_data_private_init_classifiers( this_ );
    pencil_layout_data_private_init_features( this_ );
    pencil_layout_data_private_init_relationships( this_ );

    assert ( pencil_layout_data_is_valid( this_ ) );
    TRACE_END();
}

void pencil_layout_data_private_init_diagram( pencil_layout_data_t *this_ )
{
    TRACE_BEGIN();
    assert ( NULL != (*this_).input_data );

    (*this_).diagram_valid = true;
    layout_diagram_init ( &((*this_).diagram_layout),
                          data_visible_set_get_diagram_const( (*this_).input_data )
                        );

    TRACE_INFO_INT ( "diagram      data    objects:", 1 );
    TRACE_INFO_INT ( "diagram      ignored objects:", 0 );  /* we do not ignore diagram objects */
    TRACE_INFO_INT ( "layout_diagram       objects:", 1 );

    TRACE_END();
}

void pencil_layout_data_private_init_classifiers( pencil_layout_data_t *this_ )
{
    TRACE_BEGIN();
    assert ( NULL != (*this_).input_data );

    const uint32_t data_classifier_count = data_visible_set_get_visible_classifier_count( (*this_).input_data );
    (*this_).visible_classifier_count = 0;
    assert ( data_classifier_count <= PENCIL_LAYOUT_DATA_MAX_CLASSIFIERS );

    for ( uint32_t c_idx = 0; c_idx < data_classifier_count; c_idx ++ )
    {
        const data_visible_classifier_t *classifier_data;
        classifier_data = data_visible_set_get_visible_classifier_const( (*this_).input_data, c_idx );

        if ( ( NULL != classifier_data ) && data_visible_classifier_is_valid( classifier_data ) )
        {
            layout_visible_classifier_init( &((*this_).visible_classifier_layout[(*this_).visible_classifier_count]),
                                            classifier_data
                                          );
            (*this_).visible_classifier_count ++;
        }
        else
        {
            TSLOG_ERROR("error in input_data: illegal classifier.");
            assert(false);
        }
    }  /* end for all classifier_data */

    TRACE_INFO_INT ( "classifier   data    objects:", data_classifier_count );
    TRACE_INFO_INT ( "classifier   ignored objects:", data_classifier_count - (*this_).visible_classifier_count );
    TRACE_INFO_INT ( "layout_v._classifier objects:", (*this_).visible_classifier_count );

    TRACE_END();
}

void pencil_layout_data_private_init_features( pencil_layout_data_t *this_ )
{
    TRACE_BEGIN();
    assert ( NULL != (*this_).input_data );

    const uint32_t data_feature_count = data_visible_set_get_feature_count( (*this_).input_data );
    uint32_t debug_dropped_features;
    debug_dropped_features = 0;
    uint32_t warn_dropped_features;
    warn_dropped_features = 0;
    (*this_).feature_count = 0;

    for ( uint32_t f_idx = 0; f_idx < data_feature_count; f_idx ++ )
    {
        const data_feature_t *feature_data;
        feature_data = data_visible_set_get_feature_const( (*this_).input_data, f_idx );
        uint32_t layout_feature_count = 0;

        if ( ( NULL != feature_data ) && data_feature_is_valid( feature_data ) )
        {
            const int64_t parent_id = data_feature_get_classifier_id( feature_data );

            for ( uint32_t c_idx2 = 0; c_idx2 < (*this_).visible_classifier_count; c_idx2 ++ )
            {
                layout_visible_classifier_t *parent_classifier;
                parent_classifier = &((*this_).visible_classifier_layout[c_idx2]);

                const bool one_parent_found = ( parent_id == layout_visible_classifier_get_classifier_id( parent_classifier ) );
                if ( one_parent_found )
                {
                    const int64_t feature_id = data_feature_get_id( feature_data );
                    const bool show = data_rules_diagram_shows_feature ( &((*this_).filter_rules),
                                                                         (*this_).input_data,
                                                                         feature_id
                                                                       );

                    /* filter lifelines if they are not applicable */
                    /* TODO - is this duplicate? */
                    bool filter;
                    if ( DATA_FEATURE_TYPE_LIFELINE == data_feature_get_main_type (feature_data) )
                    {
                        const data_diagramelement_t *diag_ele;
                        diag_ele = layout_visible_classifier_get_diagramelement_ptr( parent_classifier );
                        if ( feature_id == data_diagramelement_get_focused_feature_id( diag_ele ) )
                        {
                            const data_diagram_t *the_diagram;
                            the_diagram = layout_diagram_get_data_ptr( &((*this_).diagram_layout) );
                            data_diagram_type_t diag_type;
                            diag_type = data_diagram_get_diagram_type( the_diagram );
                            if (( diag_type == DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM )
                                || ( diag_type == DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM )
                                || ( diag_type == DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM ))
                            {
                                filter = false;
                            }
                            else
                            {
                                /* wrong diagram type */
                                filter = true;
                            }
                        }
                        else
                        {
                            /* lifeline of other visible_classifier */
                            filter = true;
                        }
                    }
                    else
                    {
                        filter = false;
                    }

                    if ( show && ( ! filter ))
                    {
                        if ( (*this_).feature_count < PENCIL_LAYOUT_DATA_MAX_FEATURES )
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
                            warn_dropped_features ++;
                        }
                    }
                }  /* one_parent_found */
            }  /* end search-for parent_classifier */
        }
        else
        {
            TSLOG_ERROR("error in input_data: illegal feature.");
            assert(false);
        }

        if ( layout_feature_count == 0 )
        {
            /* no warning here, dropping features - e.g. lifelines for other diagrams - is ok */
            debug_dropped_features ++;
        }
    }  /* end for all feature_data */

    TRACE_INFO_INT ( "feature      data    objects:", data_feature_count );
    TRACE_INFO_INT ( "feature      ignored objects:", debug_dropped_features );
    TRACE_INFO_INT ( "layout_feature       objects:", (*this_).feature_count );
    if ( 0 != warn_dropped_features )
    {
        TSLOG_WARNING_INT( "PENCIL_LAYOUT_DATA_MAX_FEATURES exceeded, layout_features not visible:", warn_dropped_features );
    }

    TRACE_END();
}

void pencil_layout_data_private_init_relationships( pencil_layout_data_t *this_ )
{
    TRACE_BEGIN();
    assert ( NULL != (*this_).input_data );

    const uint32_t data_relationship_count = data_visible_set_get_relationship_count( (*this_).input_data );
    uint32_t debug_dropped_relationships;
    debug_dropped_relationships = 0;
    uint32_t warn_dropped_relationships;
    warn_dropped_relationships = 0;
    (*this_).relationship_count = 0;

    for ( uint32_t r_idx = 0; r_idx < data_relationship_count; r_idx ++ )
    {
        const data_relationship_t *relationship_data;
        relationship_data = data_visible_set_get_relationship_const( (*this_).input_data, r_idx );

        uint32_t layout_relationship_count = 0;
        if ( ( NULL != relationship_data ) && data_relationship_is_valid( relationship_data ) )
        {
            const bool show = data_rules_diagram_shows_relationship ( &((*this_).filter_rules),
                                                                      (*this_).input_data,
                                                                      data_relationship_get_id( relationship_data )
            );

            if ( show )
            {
                layout_relationship_count = pencil_layout_data_private_init_relationship( this_,
                                                                                          relationship_data,
                                                                                          &warn_dropped_relationships
                                                                                        );
            }
        }
        else
        {
            TSLOG_ERROR("error in input_data: illegal relationship.");
            assert(false);
        }

        if ( layout_relationship_count == 0 )
        {
            /* no warning here, dropping relationships - e.g. only one end-object visible in current diagram - is ok */
            debug_dropped_relationships ++;
        }
    }  /* end for all relationship_data */

    TRACE_INFO_INT ( "relationship data    objects:", data_relationship_count );
    TRACE_INFO_INT ( "relationship ignored objects:", debug_dropped_relationships );
    TRACE_INFO_INT ( "layout_relationship  objects:", (*this_).relationship_count );
    if ( 0 != warn_dropped_relationships )
    {
        TSLOG_WARNING_INT( "PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS exceeded, layout_relationships not visible:", warn_dropped_relationships );
    }

    TRACE_END();
}

uint32_t pencil_layout_data_private_init_relationship( pencil_layout_data_t *this_,
                                                   const data_relationship_t *relationship_data,
                                                   uint32_t *io_dropped_relationships )
{
    TRACE_BEGIN();
    assert ( NULL != (*this_).input_data );
    assert ( NULL != relationship_data );
    assert ( NULL != io_dropped_relationships );
    uint32_t layout_relationship_count = 0;

    const int64_t from_classifier_id = data_relationship_get_from_classifier_id( relationship_data );
    const int64_t to_classifier_id = data_relationship_get_to_classifier_id( relationship_data );
    const int64_t from_feature_id = data_relationship_get_from_feature_id( relationship_data );
    const int64_t to_feature_id = data_relationship_get_to_feature_id( relationship_data );

    if ( DATA_ID_VOID_ID == from_feature_id )  /* search source(from) in classifiers */
    {
        for ( uint32_t c_idx3 = 0; c_idx3 < (*this_).visible_classifier_count; c_idx3 ++ )
        {
            layout_visible_classifier_t *probe3_classifier;
            probe3_classifier = &((*this_).visible_classifier_layout[c_idx3]);

            const bool one_from_classifier_found = ( from_classifier_id == layout_visible_classifier_get_classifier_id( probe3_classifier ) );
            if ( one_from_classifier_found )
            {
                if ( DATA_ID_VOID_ID == to_feature_id )  /* search destination(to) in classifiers */
                {
                    for ( uint32_t c_idx4 = 0; c_idx4 < (*this_).visible_classifier_count; c_idx4 ++ )
                    {
                        layout_visible_classifier_t *probe4_classifier;
                        probe4_classifier = &((*this_).visible_classifier_layout[c_idx4]);

                        const bool one_to_classifier_found =  ( to_classifier_id == layout_visible_classifier_get_classifier_id( probe4_classifier ) );
                        if ( one_to_classifier_found )
                        {
                            if ( (*this_).relationship_count < PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS )
                            {
                                layout_relationship_init( &((*this_).relationship_layout[(*this_).relationship_count]),
                                                          relationship_data,
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
                                (*io_dropped_relationships) ++;
                            }
                        }  /* one_to_classifier_found */
                    }  /* end search-for to_classifier */
                }
                else  /* search destination(to) in features */
                {
                    for ( uint32_t f_idx4 = 0; f_idx4 < (*this_).feature_count; f_idx4 ++ )
                    {
                        layout_feature_t *probe4_feature;
                        probe4_feature = &((*this_).feature_layout[f_idx4]);

                        const bool one_to_feature_found = ( to_feature_id == layout_feature_get_feature_id( probe4_feature ) );
                        if ( one_to_feature_found )
                        {
                            const bool to_feature_ok = ( to_classifier_id == data_feature_get_classifier_id(layout_feature_get_data_ptr( probe4_feature )) );
                            if ( to_feature_ok )
                            {
                                if ( (*this_).relationship_count < PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS )
                                {
                                    layout_relationship_init( &((*this_).relationship_layout[(*this_).relationship_count]),
                                                              relationship_data,
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
                                    (*io_dropped_relationships) ++;
                                }
                            }
                            else
                            {
                                TSLOG_ERROR("error in input_data: relationship links to feature and inconsistent classifier.");
                            }
                        }  /* one_to_feature_found */
                    }  /* end search-for to_feature */
                }
            }  /* one_from_classifier_found */
        }  /* end search-for from_classifier */
    }
    else  /* search source(from) in features */
    {
        for ( uint32_t f_idx3 = 0; f_idx3 < (*this_).feature_count; f_idx3 ++ )
        {
            layout_feature_t *probe3_feature;
            probe3_feature = &((*this_).feature_layout[f_idx3]);

            const bool one_from_feature_found = ( from_feature_id == layout_feature_get_feature_id( probe3_feature ) );
            if ( one_from_feature_found )
            {
                const bool from_feature_ok = ( from_classifier_id == data_feature_get_classifier_id(layout_feature_get_data_ptr( probe3_feature )) );
                if ( from_feature_ok )
                {
                    if ( DATA_ID_VOID_ID == to_feature_id )  /* search destination(to) in classifiers */
                    {
                        for ( uint32_t c_idx5 = 0; c_idx5 < (*this_).visible_classifier_count; c_idx5 ++ )
                        {
                            layout_visible_classifier_t *probe5_classifier;
                            probe5_classifier = &((*this_).visible_classifier_layout[c_idx5]);

                            const bool one_to_classifier_found = ( to_classifier_id == layout_visible_classifier_get_classifier_id( probe5_classifier ) );
                            if ( one_to_classifier_found )
                            {
                                if ( (*this_).relationship_count < PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS )
                                {
                                    layout_relationship_init( &((*this_).relationship_layout[(*this_).relationship_count]),
                                                              relationship_data,
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
                                    (*io_dropped_relationships) ++;
                                }
                            }  /* one_to_classifier_found */
                        }  /* end search-for to_classifier */
                    }
                    else  /* search destination(to) in features */
                    {
                        for ( uint32_t f_idx5 = 0; f_idx5 < (*this_).feature_count; f_idx5 ++ )
                        {
                            layout_feature_t *probe5_feature;
                            probe5_feature = &((*this_).feature_layout[f_idx5]);

                            const bool one_to_feature_found = ( to_feature_id == layout_feature_get_feature_id( probe5_feature ) );
                            if ( one_to_feature_found )
                            {
                                const bool to_feature_ok = ( to_classifier_id == data_feature_get_classifier_id(layout_feature_get_data_ptr( probe5_feature )) );
                                if ( to_feature_ok )
                                {
                                    if ( (*this_).relationship_count < PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS )
                                    {
                                        layout_relationship_init( &((*this_).relationship_layout[(*this_).relationship_count]),
                                                                  relationship_data,
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
                                        (*io_dropped_relationships) ++;
                                    }
                                }
                                else
                                {
                                    TSLOG_ERROR("error in input_data: relationship links to feature and inconsistent classifier.");
                                }
                            }  /* one_to_feature_found */
                        }  /* end search-for to_feature */
                    }
                }
                else
                {
                    TSLOG_ERROR("error in input_data: relationship links from feature and inconsistent classifier.");
                }
            }  /* one_from_feature_found */
        }  /* end search-for from_feature */
    }  /* endif: search source(from) */

    return layout_relationship_count;
    TRACE_END();
}

void pencil_layout_data_destroy( pencil_layout_data_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).visible_classifier_count <= PENCIL_LAYOUT_DATA_MAX_CLASSIFIERS );
    assert( (*this_).feature_count <= PENCIL_LAYOUT_DATA_MAX_FEATURES );
    assert( (*this_).relationship_count <= PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS );

    data_rules_destroy ( &((*this_).filter_rules) );

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

/* ================================ misc ================================ */

bool pencil_layout_data_is_valid ( pencil_layout_data_t *this_ )
{
    bool result = true;

    /* check input data */
    if ( NULL == (*this_).input_data )
    {
        result = false;
    }
    else
    {
        if ( ! data_visible_set_is_valid( (*this_).input_data ) )
        {
            result = false;
        }
    }

    /* check diagram */
    if ( ! (*this_).diagram_valid )
    {
        result = false;
    }
    else
    {
        if ( ! layout_diagram_is_valid( &((*this_).diagram_layout) ) )
        {
            result = false;
        }
    }

    /* check classifiers */
    if ( (*this_).visible_classifier_count > PENCIL_LAYOUT_DATA_MAX_CLASSIFIERS )
    {
        /* if the object is already initialized, this is a severe error */
        result = false;
    }
    else
    {
        for ( uint_fast32_t c_idx = 0; c_idx < (*this_).visible_classifier_count;  c_idx ++ )
        {
            layout_visible_classifier_t *current = &((*this_).visible_classifier_layout[c_idx]);
            if ( ! layout_visible_classifier_is_valid( current ) )
            {
                result = false;
            }
        }
    }

    /* check features */
    if ( (*this_).feature_count > PENCIL_LAYOUT_DATA_MAX_FEATURES )
    {
        /* if the object is already initialized, this is a severe error */
        result = false;
    }
    else
    {
        for ( uint_fast32_t f_idx = 0; f_idx < (*this_).feature_count;  f_idx ++ )
        {
            layout_feature_t *current = &((*this_).feature_layout[f_idx]);
            if ( ! layout_feature_is_valid( current ) )
            {
                result = false;
            }
        }
    }

    /* check relationships */
    if ( (*this_).relationship_count > PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS )
    {
        /* if the object is already initialized, this is a severe error */
        result = false;
    }
    else
    {
        for ( uint_fast32_t r_idx = 0; r_idx < (*this_).relationship_count;  r_idx ++ )
        {
            layout_relationship_t *current = &((*this_).relationship_layout[r_idx]);
            if ( ! layout_relationship_is_valid( current ) )
            {
                result = false;
            }
        }
    }

    return result;
}


/*
Copyright 2017-2019 Andreas Warnke

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
