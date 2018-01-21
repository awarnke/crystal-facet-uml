/* File: data_diagramelement.inl; Copyright and License: see below */

#include "data_id.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

static inline void data_diagramelement_init_empty ( data_diagramelement_t *this_ )
{
    (*this_).id = DATA_ID_VOID_ID;
    (*this_).diagram_id = DATA_ID_VOID_ID;
    (*this_).classifier_id = DATA_ID_VOID_ID;
    (*this_).display_flags = DATA_DIAGRAMELEMENT_FLAG_NONE;
    (*this_).focused_feature_id = DATA_ID_VOID_ID;
}

static inline void data_diagramelement_reinit_empty ( data_diagramelement_t *this_ )
{
    /* data_diagramelement_destroy( this_ );  -- not necessary */
    data_diagramelement_init_empty( this_ );
}

static inline void data_diagramelement_init_new ( data_diagramelement_t *this_,
                                                  int64_t diagram_id,
                                                  int64_t classifier_id,
                                                  data_diagramelement_flag_t display_flags,
                                                  int64_t focused_feature_id )
{
    (*this_).id = DATA_ID_VOID_ID;
    (*this_).diagram_id = diagram_id;
    (*this_).classifier_id = classifier_id;
    (*this_).display_flags = display_flags;
    (*this_).focused_feature_id = focused_feature_id;
}

static inline void data_diagramelement_init ( data_diagramelement_t *this_,
                                              int64_t id,
                                              int64_t diagram_id,
                                              int64_t classifier_id,
                                              data_diagramelement_flag_t display_flags,
                                              int64_t focused_feature_id )
{
    (*this_).id = id;
    (*this_).diagram_id = diagram_id;
    (*this_).classifier_id = classifier_id;
    (*this_).display_flags = display_flags;
    (*this_).focused_feature_id = focused_feature_id;
}

static inline void data_diagramelement_reinit ( data_diagramelement_t *this_,
                                                int64_t id,
                                                int64_t diagram_id,
                                                int64_t classifier_id,
                                                data_diagramelement_flag_t display_flags,
                                                int64_t focused_feature_id )
{
    (*this_).id = id;
    (*this_).diagram_id = diagram_id;
    (*this_).classifier_id = classifier_id;
    (*this_).display_flags = display_flags;
    (*this_).focused_feature_id = focused_feature_id;
}

static inline void data_diagramelement_copy ( data_diagramelement_t *this_, const data_diagramelement_t *original )
{
    (*this_) = (*original);
}

static inline void data_diagramelement_replace ( data_diagramelement_t *this_, const data_diagramelement_t *that )
{
    (*this_) = (*that);
}

static inline void data_diagramelement_destroy ( data_diagramelement_t *this_ )
{
    (*this_).id = DATA_ID_VOID_ID;
}

static inline bool data_diagramelement_is_valid ( const data_diagramelement_t *this_ )
{
    return ( DATA_ID_VOID_ID != (*this_).id );
}

static inline void data_diagramelement_trace ( const data_diagramelement_t *this_ )
{
    TRACE_INFO( "data_diagramelement_t" );
    TRACE_INFO_INT( "- id:", (*this_).id );
    TRACE_INFO_INT( "- diagram_id:", (*this_).diagram_id );
    TRACE_INFO_INT( "- classifier_id:", (*this_).classifier_id );
    TRACE_INFO_HEX( "- display_flags:", (*this_).display_flags );
    TRACE_INFO_INT( "- focused_feature_id:", (*this_).focused_feature_id );
}

static inline int64_t data_diagramelement_get_id ( const data_diagramelement_t *this_ )
{
    return (*this_).id;
}

static inline void data_diagramelement_set_id ( data_diagramelement_t *this_, int64_t id )
{
    (*this_).id = id;
}

static inline int64_t data_diagramelement_get_diagram_id ( const data_diagramelement_t *this_ )
{
    return (*this_).diagram_id;
}

static inline int64_t data_diagramelement_get_classifier_id ( const data_diagramelement_t *this_ )
{
    return (*this_).classifier_id;
}

static inline data_diagramelement_flag_t data_diagramelement_get_display_flags ( const data_diagramelement_t *this_ )
{
    return (*this_).display_flags;
}

static inline void data_diagramelement_set_display_flags ( data_diagramelement_t *this_, data_diagramelement_flag_t display_flags )
{
    (*this_).display_flags = display_flags;
}

static inline int64_t data_diagramelement_get_focused_feature_id ( const data_diagramelement_t *this_ )
{
    return (*this_).focused_feature_id;
}

static inline void data_diagramelement_set_focused_feature_id ( data_diagramelement_t *this_, int64_t focused_feature_id )
{
    (*this_).focused_feature_id = focused_feature_id;
}


/*
Copyright 2016-2018 Andreas Warnke

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
