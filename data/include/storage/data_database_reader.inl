/* File: data_database_reader.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include "u8/u8_trace.h"
#include <assert.h>

static inline bool data_database_reader_is_open( data_database_reader_t *this_ )
{
    bool result;
    result = (*this_).is_open;
    return result;
}

/* ================================ DIAGRAM ================================ */

static inline u8_error_t data_database_reader_get_diagram_by_id ( data_database_reader_t *this_,
                                                                  data_row_t id,
                                                                  data_diagram_t *out_diagram )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_diagram_reader_get_diagram_by_id( &((*this_).temp_diagram_reader), id, out_diagram );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_diagram_by_uuid ( data_database_reader_t *this_,
                                                                    const char *uuid,
                                                                    data_diagram_t *out_diagram )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_diagram_reader_get_diagram_by_uuid( &((*this_).temp_diagram_reader), uuid, out_diagram );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_diagrams_by_parent_id ( data_database_reader_t *this_,
                                                                          data_row_t parent_id,
                                                                          data_diagram_iterator_t *io_diagram_iterator )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_diagram_reader_get_diagrams_by_parent_id( &((*this_).temp_diagram_reader),
                                                                         parent_id,
                                                                         io_diagram_iterator
                                                                       );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_diagrams_by_classifier_id ( data_database_reader_t *this_,
                                                                              data_row_t classifier_id,
                                                                              data_diagram_iterator_t *io_diagram_iterator )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_diagram_reader_get_diagrams_by_classifier_id( &((*this_).temp_diagram_reader),
                                                                             classifier_id,
                                                                             io_diagram_iterator
                                                                           );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_diagram_ids_by_parent_id ( data_database_reader_t *this_,
                                                                             data_row_t parent_id,
                                                                             data_small_set_t *out_diagram_ids )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_diagram_reader_get_diagram_ids_by_parent_id( &((*this_).temp_diagram_reader),
                                                                            parent_id,
                                                                            out_diagram_ids
                                                                          );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_diagram_ids_by_classifier_id ( data_database_reader_t *this_,
                                                                                 data_row_t classifier_id,
                                                                                 data_small_set_t *out_diagram_ids )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_diagram_reader_get_diagram_ids_by_classifier_id( &((*this_).temp_diagram_reader),
                                                                                classifier_id,
                                                                                out_diagram_ids
                                                                              );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ CLASSIFIER ================================ */

static inline u8_error_t data_database_reader_get_classifier_by_id ( data_database_reader_t *this_,
                                                                     data_row_t id,
                                                                     data_classifier_t *out_classifier )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_classifier_by_id( &((*this_).temp_classifier_reader),
                                                                       id,
                                                                       out_classifier
                                                                     );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_classifier_by_name ( data_database_reader_t *this_,
                                                                       const char *name,
                                                                       data_classifier_t *out_classifier )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_classifier_by_name( &((*this_).temp_classifier_reader),
                                                                         name,
                                                                         out_classifier
                                                                       );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_classifier_by_uuid ( data_database_reader_t *this_,
                                                                       const char *uuid,
                                                                       data_classifier_t *out_classifier )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_classifier_by_uuid( &((*this_).temp_classifier_reader),
                                                                         uuid,
                                                                         out_classifier
                                                                       );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_all_classifiers ( data_database_reader_t *this_,
                                                                    bool hierarchical,
                                                                    data_classifier_iterator_t *io_classifier_iterator )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_all_classifiers( &((*this_).temp_classifier_reader),
                                                                      hierarchical,
                                                                      io_classifier_iterator
                                                                    );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ VISIBLE_CLASSIFIER ================================ */

static inline u8_error_t data_database_reader_get_visible_classifiers_by_diagram_id( data_database_reader_t *this_,
                                                                                     data_row_t diagram_id,
                                                                                      data_visible_classifier_iterator_t *io_visible_classifier_iterator )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_diagram_reader_get_visible_classifiers_by_diagram_id( &((*this_).temp_diagram_reader),
                                                                                     diagram_id,
                                                                                     io_visible_classifier_iterator
                                                                                   );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ DIAGRAMELEMENT ================================ */

static inline u8_error_t data_database_reader_get_diagramelement_by_id ( data_database_reader_t *this_,
                                                                         data_row_t id,
                                                                         data_diagramelement_t *out_diagramelement )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_diagram_reader_get_diagramelement_by_id( &((*this_).temp_diagram_reader),
                                                                        id,
                                                                        out_diagramelement
                                                                      );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_diagramelement_by_uuid ( data_database_reader_t *this_,
                                                                           const char *uuid,
                                                                           data_diagramelement_t *out_diagramelement )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_diagram_reader_get_diagramelement_by_uuid( &((*this_).temp_diagram_reader),
                                                                          uuid,
                                                                          out_diagramelement
                                                                        );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_diagramelements_by_diagram_id ( data_database_reader_t *this_,
                                                                                  data_row_t diagram_id,
                                                                                  data_diagramelement_iterator_t *io_diagramelement_iterator )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_diagram_reader_get_diagramelements_by_diagram_id( &((*this_).temp_diagram_reader),
                                                                                 diagram_id,
                                                                                 io_diagramelement_iterator
                                                                               );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_diagramelements_by_classifier_id ( data_database_reader_t *this_,
                                                                                     data_row_t classifier_id,
                                                                                     data_diagramelement_iterator_t *io_diagramelement_iterator )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_diagram_reader_get_diagramelements_by_classifier_id( &((*this_).temp_diagram_reader),
                                                                                    classifier_id,
                                                                                    io_diagramelement_iterator
                                                                                  );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ FEATURE ================================ */

static inline u8_error_t data_database_reader_get_feature_by_id ( data_database_reader_t *this_,
                                                                  data_row_t id,
                                                                  data_feature_t *out_feature )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_feature_by_id( &((*this_).temp_classifier_reader),
                                                                    id,
                                                                    out_feature
                                                                  );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_feature_by_uuid ( data_database_reader_t *this_,
                                                                    const char *uuid,
                                                                    data_feature_t *out_feature )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_feature_by_uuid( &((*this_).temp_classifier_reader),
                                                                      uuid,
                                                                      out_feature
                                                                    );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_features_by_classifier_id ( data_database_reader_t *this_,
                                                                              data_row_t classifier_id,
                                                                              data_feature_iterator_t *io_feature_iterator )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_features_by_classifier_id( &((*this_).temp_classifier_reader),
                                                                                classifier_id,
                                                                                io_feature_iterator
                                                                              );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_features_by_diagram_id ( data_database_reader_t *this_,
                                                                           data_row_t diagram_id,
                                                                           data_feature_iterator_t *io_feature_iterator )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_features_by_diagram_id( &((*this_).temp_classifier_reader),
                                                                             diagram_id,
                                                                             io_feature_iterator
                                                                           );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ RELATIONSHIP ================================ */

static inline u8_error_t data_database_reader_get_relationship_by_id ( data_database_reader_t *this_,
                                                                       data_row_t id,
                                                                       data_relationship_t *out_relationship )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_relationship_by_id( &((*this_).temp_classifier_reader),
                                                                         id,
                                                                         out_relationship
                                                                       );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_relationship_by_uuid ( data_database_reader_t *this_,
                                                                         const char *uuid,
                                                                         data_relationship_t *out_relationship )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_relationship_by_uuid( &((*this_).temp_classifier_reader),
                                                                           uuid,
                                                                           out_relationship
                                                                         );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_relationships_by_classifier_id ( data_database_reader_t *this_,
                                                                                   data_row_t classifier_id,
                                                                                   data_relationship_iterator_t *io_relationship_iterator )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_relationships_by_classifier_id( &((*this_).temp_classifier_reader),
                                                                                     classifier_id,
                                                                                     io_relationship_iterator
                                                                                   );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_relationships_by_feature_id ( data_database_reader_t *this_,
                                                                                data_row_t feature_id,
                                                                                data_relationship_iterator_t *io_relationship_iterator )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_relationships_by_feature_id( &((*this_).temp_classifier_reader),
                                                                                  feature_id,
                                                                                  io_relationship_iterator
                                                                                );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_reader_get_relationships_by_diagram_id ( data_database_reader_t *this_,
                                                                                data_row_t diagram_id,
                                                                                data_relationship_iterator_t *io_relationship_iterator )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_relationships_by_diagram_id( &((*this_).temp_classifier_reader),
                                                                                  diagram_id,
                                                                                  io_relationship_iterator
                                                                                );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        U8_TRACE_INFO( "Database not open, cannot request data." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2016-2025 Andreas Warnke

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
