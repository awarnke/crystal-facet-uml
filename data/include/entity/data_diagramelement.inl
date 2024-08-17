/* File: data_diagramelement.inl; Copyright and License: see below */

#include "entity/data_id.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

static inline void data_diagramelement_init_empty ( data_diagramelement_t *this_ )
{
    (*this_).id = DATA_ROW_ID_VOID;
    (*this_).diagram_id = DATA_ROW_ID_VOID;
    (*this_).classifier_id = DATA_ROW_ID_VOID;
    (*this_).display_flags = DATA_DIAGRAMELEMENT_FLAG_NONE;
    (*this_).focused_feature_id = DATA_ROW_ID_VOID;
    data_uuid_init_new( &((*this_).uuid) );
}

static inline void data_diagramelement_reinit_empty ( data_diagramelement_t *this_ )
{
    /* data_diagramelement_destroy( this_ );  -- not necessary */
    data_diagramelement_init_empty( this_ );
}

static inline void data_diagramelement_init_new ( data_diagramelement_t *this_,
                                                  data_row_id_t diagram_id,
                                                  data_row_id_t classifier_id,
                                                  data_diagramelement_flag_t display_flags,
                                                  data_row_id_t focused_feature_id )
{
    (*this_).id = DATA_ROW_ID_VOID;
    (*this_).diagram_id = diagram_id;
    (*this_).classifier_id = classifier_id;
    (*this_).focused_feature_id = focused_feature_id;
    (*this_).display_flags = display_flags;
    data_uuid_init_new( &((*this_).uuid) );
}

static inline u8_error_t data_diagramelement_init ( data_diagramelement_t *this_,
                                                    data_row_id_t id,
                                                    data_row_id_t diagram_id,
                                                    data_row_id_t classifier_id,
                                                    data_diagramelement_flag_t display_flags,
                                                    data_row_id_t focused_feature_id,
                                                    const char* uuid )
{
    assert( NULL != uuid );
    u8_error_t result = U8_ERROR_NONE;

    (*this_).id = id;
    (*this_).diagram_id = diagram_id;
    (*this_).classifier_id = classifier_id;
    (*this_).focused_feature_id = focused_feature_id;
    (*this_).display_flags = display_flags;
    result |= data_uuid_init( &((*this_).uuid), uuid );

    return result;
}

static inline u8_error_t data_diagramelement_reinit ( data_diagramelement_t *this_,
                                                      data_row_id_t id,
                                                      data_row_id_t diagram_id,
                                                      data_row_id_t classifier_id,
                                                      data_diagramelement_flag_t display_flags,
                                                      data_row_id_t focused_feature_id,
                                                      const char* uuid )
{
    assert( NULL != uuid );
    u8_error_t result = U8_ERROR_NONE;

    (*this_).id = id;
    (*this_).diagram_id = diagram_id;
    (*this_).classifier_id = classifier_id;
    (*this_).focused_feature_id = focused_feature_id;
    (*this_).display_flags = display_flags;
    result |= data_uuid_init( &((*this_).uuid), uuid );

    return result;
}

static inline void data_diagramelement_copy ( data_diagramelement_t *this_, const data_diagramelement_t *original )
{
    (*this_) = (*original);
    /* repair the overwritten pointers */
    data_uuid_copy( &((*this_).uuid), &((*original).uuid) );
}

static inline void data_diagramelement_replace ( data_diagramelement_t *this_, const data_diagramelement_t *that )
{
    (*this_) = (*that);
    /* repair the overwritten pointers */
    data_uuid_replace( &((*this_).uuid), &((*that).uuid) );
}

static inline void data_diagramelement_destroy ( data_diagramelement_t *this_ )
{
    (*this_).id = DATA_ROW_ID_VOID;
    data_uuid_destroy( &((*this_).uuid) );
}

static inline bool data_diagramelement_is_valid ( const data_diagramelement_t *this_ )
{
    return ( DATA_ROW_ID_VOID != (*this_).id );
}

static inline void data_diagramelement_trace ( const data_diagramelement_t *this_ )
{
    U8_TRACE_INFO( "data_diagramelement_t" );
    U8_TRACE_INFO_INT( "- id:", (*this_).id );
    U8_TRACE_INFO_INT( "- diagram_id:", (*this_).diagram_id );
    U8_TRACE_INFO_INT( "- classifier_id:", (*this_).classifier_id );
    U8_TRACE_INFO_HEX( "- display_flags:", (*this_).display_flags );
    U8_TRACE_INFO_INT( "- focused_feature_id:", (*this_).focused_feature_id );
    U8_TRACE_INFO_STR( "- uuid:", data_uuid_get_string( &((*this_).uuid) ) );
}

static inline data_row_id_t data_diagramelement_get_row_id ( const data_diagramelement_t *this_ )
{
    return (*this_).id;
}

static inline void data_diagramelement_set_row_id ( data_diagramelement_t *this_, data_row_id_t id )
{
    (*this_).id = id;
}

static inline data_id_t data_diagramelement_get_data_id ( const data_diagramelement_t *this_ )
{
    data_id_t result;
    data_id_init ( &result, DATA_TABLE_DIAGRAMELEMENT, (*this_).id );
    return result;
}

static inline data_row_id_t data_diagramelement_get_diagram_row_id ( const data_diagramelement_t *this_ )
{
    return (*this_).diagram_id;
}

static inline void data_diagramelement_set_diagram_row_id ( data_diagramelement_t *this_, data_row_id_t diagram_id )
{
    (*this_).diagram_id = diagram_id;
}

static inline data_id_t data_diagramelement_get_diagram_data_id ( const data_diagramelement_t *this_ )
{
    data_id_t result;
    data_id_init ( &result, DATA_TABLE_DIAGRAM, (*this_).diagram_id );
    return result;
}

static inline data_row_id_t data_diagramelement_get_classifier_row_id ( const data_diagramelement_t *this_ )
{
    return (*this_).classifier_id;
}

static inline void data_diagramelement_set_classifier_row_id ( data_diagramelement_t *this_, data_row_id_t classifier_id )
{
    (*this_).classifier_id = classifier_id;
}

static inline data_id_t data_diagramelement_get_classifier_data_id ( const data_diagramelement_t *this_ )
{
    data_id_t result;
    data_id_init ( &result, DATA_TABLE_CLASSIFIER, (*this_).classifier_id );
    return result;
}

static inline data_row_id_t data_diagramelement_get_focused_feature_row_id ( const data_diagramelement_t *this_ )
{
    return (*this_).focused_feature_id;
}

static inline void data_diagramelement_set_focused_feature_row_id ( data_diagramelement_t *this_, data_row_id_t focused_feature_id )
{
    (*this_).focused_feature_id = focused_feature_id;
}

static inline data_id_t data_diagramelement_get_focused_feature_data_id ( const data_diagramelement_t *this_ )
{
    data_id_t result;
    if ( (*this_).focused_feature_id == DATA_ROW_ID_VOID )
    {
        data_id_init_void ( &result );
    }
    else
    {
        data_id_init ( &result, DATA_TABLE_FEATURE, (*this_).focused_feature_id );
    }
    return result;
}

static inline data_diagramelement_flag_t data_diagramelement_get_display_flags ( const data_diagramelement_t *this_ )
{
    return (*this_).display_flags;
}

static inline void data_diagramelement_set_display_flags ( data_diagramelement_t *this_, data_diagramelement_flag_t display_flags )
{
    (*this_).display_flags = display_flags;
}

static inline const char *data_diagramelement_get_uuid_const ( const data_diagramelement_t *this_ )
{
    return data_uuid_get_string( &((*this_).uuid) );
}

static inline u8_error_t data_diagramelement_set_uuid ( data_diagramelement_t *this_, const char *uuid )
{
    assert( NULL != uuid );

    const u8_error_t result = data_uuid_reinit( &((*this_).uuid), uuid );

    return result;
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
