/* File: data_database_reader.c; Copyright and License: see below */

#include "storage/data_database_reader.h"
#include "trace.h"
#include "tslog.h"
#include "util/string/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>

u8_error_t data_database_reader_init ( data_database_reader_t *this_, data_database_t *database )
{
    TRACE_BEGIN();
    assert( NULL != database );
    u8_error_t result = U8_ERROR_NONE;

    (*this_).database = database;
    (*this_).is_open = false;

    data_database_listener_init ( &((*this_).me_as_listener), this_, (void (*)(void*,data_database_listener_signal_t)) &data_database_reader_db_change_callback );
    data_database_add_db_listener( database, &((*this_).me_as_listener) );

    if ( data_database_is_open( database ) )
    {
        /* if the database is open, open also the reader */
        result |= data_database_reader_private_open( this_ );
    }

    TRACE_END_ERR(result);
    return result;
}

u8_error_t data_database_reader_destroy ( data_database_reader_t *this_ )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result |= data_database_reader_private_close( this_ );
    }

    data_database_remove_db_listener( (*this_).database, &((*this_).me_as_listener) );

    (*this_).database = NULL;

    TRACE_END_ERR(result);
    return result;
}

void data_database_reader_db_change_callback ( data_database_reader_t *this_, data_database_listener_signal_t signal_id )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    switch ( signal_id )
    {
        case DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE:
        {
            TRACE_INFO( "DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE" );
            if ( (*this_).is_open )
            {
                result |= data_database_reader_private_close( this_ );
            }
        }
        break;

        case DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED:
        {
            TRACE_INFO( "DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED" );
            if ( (*this_).is_open )
            {
                result |= data_database_reader_private_close( this_ );
            }
            result |= data_database_reader_private_open( this_ );
        }
        break;

        default:
        {
            TSLOG_ERROR( "unexpected data_database_listener_signal_t" );
        }
    }

    TRACE_END();
}

/* ================================ DIAGRAM ================================ */

u8_error_t data_database_reader_get_diagram_by_id ( data_database_reader_t *this_, data_row_id_t id, data_diagram_t *out_diagram )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_diagram_reader_get_diagram_by_id( &((*this_).temp_diagram_reader), id, out_diagram );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_diagram_by_uuid ( data_database_reader_t *this_, const char *uuid, data_diagram_t *out_diagram )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_diagram_reader_get_diagram_by_uuid( &((*this_).temp_diagram_reader), uuid, out_diagram );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_diagrams_by_parent_id ( data_database_reader_t *this_,
                                                              data_row_id_t parent_id,
                                                              uint32_t max_out_array_size,
                                                              data_diagram_t (*out_diagram)[],
                                                              uint32_t *out_diagram_count )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_diagram_reader_get_diagrams_by_parent_id( &((*this_).temp_diagram_reader),
                                                                         parent_id,
                                                                         max_out_array_size,
                                                                         out_diagram,
                                                                         out_diagram_count
                                                                       );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_diagrams_by_classifier_id ( data_database_reader_t *this_,
                                                                  data_row_id_t classifier_id,
                                                                  uint32_t max_out_array_size,
                                                                  data_diagram_t (*out_diagram)[],
                                                                  uint32_t *out_diagram_count )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_diagram_reader_get_diagrams_by_classifier_id( &((*this_).temp_diagram_reader),
                                                                             classifier_id,
                                                                             max_out_array_size,
                                                                             out_diagram,
                                                                             out_diagram_count
                                                                           );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_diagram_ids_by_parent_id ( data_database_reader_t *this_,
                                                                 data_row_id_t parent_id,
                                                                 data_small_set_t *out_diagram_ids )
{
    TRACE_BEGIN();
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
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_diagram_ids_by_classifier_id ( data_database_reader_t *this_,
                                                                     data_row_id_t classifier_id,
                                                                     data_small_set_t *out_diagram_ids )
{
    TRACE_BEGIN();
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
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

/* ================================ CLASSIFIER ================================ */

u8_error_t data_database_reader_get_classifier_by_id ( data_database_reader_t *this_, data_row_id_t id, data_classifier_t *out_classifier )
{
    TRACE_BEGIN();
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
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_classifier_by_name ( data_database_reader_t *this_, const char *name, data_classifier_t *out_classifier )
{
    TRACE_BEGIN();
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
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_classifier_by_uuid ( data_database_reader_t *this_, const char *uuid, data_classifier_t *out_classifier )
{
    TRACE_BEGIN();
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
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_classifiers_by_diagram_id ( data_database_reader_t *this_,
                                                                  data_row_id_t diagram_id,
                                                                  uint32_t max_out_array_size,
                                                                  data_visible_classifier_t (*out_visible_classifier)[],
                                                                  uint32_t *out_visible_classifier_count )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_classifiers_by_diagram_id( &((*this_).temp_classifier_reader),
                                                                                diagram_id,
                                                                                max_out_array_size,
                                                                                out_visible_classifier,
                                                                                out_visible_classifier_count
                                                                              );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_all_classifiers_iterator ( data_database_reader_t *this_,
                                                                 data_database_iterator_classifiers_t *io_classifier_iterator
                                                               )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_all_classifiers_iterator( &((*this_).temp_classifier_reader),
                                                                               io_classifier_iterator
                                                                             );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

/* ================================ DIAGRAMELEMENT ================================ */

u8_error_t data_database_reader_get_diagramelement_by_id ( data_database_reader_t *this_,
                                                             data_row_id_t id,
                                                             data_diagramelement_t *out_diagramelement )
{
    TRACE_BEGIN();
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
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_diagramelement_by_uuid ( data_database_reader_t *this_,
                                                               const char *uuid,
                                                               data_diagramelement_t *out_diagramelement )
{
    TRACE_BEGIN();
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
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_diagramelements_by_diagram_id ( data_database_reader_t *this_,
                                                                      data_row_id_t diagram_id,
                                                                      uint32_t max_out_array_size,
                                                                      data_diagramelement_t (*out_diagramelement)[],
                                                                      uint32_t *out_diagramelement_count )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_diagram_reader_get_diagramelements_by_diagram_id( &((*this_).temp_diagram_reader),
                                                                                 diagram_id,
                                                                                 max_out_array_size,
                                                                                 out_diagramelement,
                                                                                 out_diagramelement_count
                                                                               );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_diagramelements_by_classifier_id ( data_database_reader_t *this_,
                                                                         data_row_id_t classifier_id,
                                                                         uint32_t max_out_array_size,
                                                                         data_diagramelement_t (*out_diagramelement)[],
                                                                         uint32_t *out_diagramelement_count )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_diagram_reader_get_diagramelements_by_classifier_id( &((*this_).temp_diagram_reader),
                                                                                    classifier_id,
                                                                                    max_out_array_size,
                                                                                    out_diagramelement,
                                                                                    out_diagramelement_count
                                                                                  );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

/* ================================ FEATURE ================================ */

u8_error_t data_database_reader_get_feature_by_id ( data_database_reader_t *this_,
                                                      data_row_id_t id,
                                                      data_feature_t *out_feature )
{
    TRACE_BEGIN();
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
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_feature_by_uuid ( data_database_reader_t *this_,
                                                        const char *uuid,
                                                        data_feature_t *out_feature )
{
    TRACE_BEGIN();
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
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}


u8_error_t data_database_reader_get_features_by_classifier_id ( data_database_reader_t *this_,
                                                                  data_row_id_t classifier_id,
                                                                  uint32_t max_out_array_size,
                                                                  data_feature_t (*out_feature)[],
                                                                  uint32_t *out_feature_count )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_features_by_classifier_id( &((*this_).temp_classifier_reader),
                                                                                classifier_id,
                                                                                max_out_array_size,
                                                                                out_feature,
                                                                                out_feature_count
                                                                              );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_features_by_diagram_id ( data_database_reader_t *this_,
                                                               data_row_id_t diagram_id,
                                                               uint32_t max_out_array_size,
                                                               data_feature_t (*out_feature)[],
                                                               uint32_t *out_feature_count )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_features_by_diagram_id( &((*this_).temp_classifier_reader),
                                                                             diagram_id,
                                                                             max_out_array_size,
                                                                             out_feature,
                                                                             out_feature_count
                                                                           );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

/* ================================ RELATIONSHIP ================================ */

u8_error_t data_database_reader_get_relationship_by_id ( data_database_reader_t *this_,
                                                           data_row_id_t id,
                                                           data_relationship_t *out_relationship )
{
    TRACE_BEGIN();
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
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_relationship_by_uuid ( data_database_reader_t *this_,
                                                             const char *uuid,
                                                             data_relationship_t *out_relationship )
{
    TRACE_BEGIN();
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
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_relationships_by_classifier_id ( data_database_reader_t *this_,
                                                                       data_row_id_t classifier_id,
                                                                       uint32_t max_out_array_size,
                                                                       data_relationship_t (*out_relationship)[],
                                                                       uint32_t *out_relationship_count )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_relationships_by_classifier_id( &((*this_).temp_classifier_reader),
                                                                                     classifier_id,
                                                                                     max_out_array_size,
                                                                                     out_relationship,
                                                                                     out_relationship_count
                                                                                   );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_relationships_by_feature_id ( data_database_reader_t *this_,
                                                                    data_row_id_t feature_id,
                                                                    uint32_t max_out_array_size,
                                                                    data_relationship_t (*out_relationship)[],
                                                                    uint32_t *out_relationship_count )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_relationships_by_feature_id( &((*this_).temp_classifier_reader),
                                                                                  feature_id,
                                                                                  max_out_array_size,
                                                                                  out_relationship,
                                                                                  out_relationship_count
                                                                                );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_reader_get_relationships_by_diagram_id ( data_database_reader_t *this_,
                                                                    data_row_id_t diagram_id,
                                                                    uint32_t max_out_array_size,
                                                                    data_relationship_t (*out_relationship)[],
                                                                    uint32_t *out_relationship_count )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result = data_database_classifier_reader_get_relationships_by_diagram_id( &((*this_).temp_classifier_reader),
                                                                                  diagram_id,
                                                                                  max_out_array_size,
                                                                                  out_relationship,
                                                                                  out_relationship_count
                                                                                );
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

/* ================================ private ================================ */

u8_error_t data_database_reader_private_open ( data_database_reader_t *this_ )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( ! (*this_).is_open )
    {
        result |= data_database_classifier_reader_init( &((*this_).temp_classifier_reader), (*this_).database );
        result |= data_database_diagram_reader_init( &((*this_).temp_diagram_reader), (*this_).database );

        if ( result == U8_ERROR_NONE )
        {
            (*this_).is_open = true;
        }
    }
    else
    {
        result |= U8_ERROR_INVALID_REQUEST;
        TSLOG_WARNING( "Database is already open." );
    }

    TRACE_END_ERR(result);
    return result;
}

u8_error_t data_database_reader_private_close ( data_database_reader_t *this_ )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result |= data_database_diagram_reader_destroy( &((*this_).temp_diagram_reader) );
        result |= data_database_classifier_reader_destroy( &((*this_).temp_classifier_reader) );

        (*this_).is_open = false;
    }
    else
    {
        result |= U8_ERROR_INVALID_REQUEST;
        TSLOG_WARNING( "Database was not open." );
    }

    TRACE_END_ERR(result);
    return result;
}


/*
Copyright 2016-2021 Andreas Warnke

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
