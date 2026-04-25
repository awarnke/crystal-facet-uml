/* File: consistency_stat.inl; Copyright and License: see below */

#include "utf8stringbuf/utf8stringbuf.h"
#include "u8/u8_trace.h"
#include <assert.h>

static inline void consistency_stat_init ( consistency_stat_t *this_ )
{
    *this_ = CONSISTENCY_STAT_ZERO;
}

static inline void consistency_stat_destroy ( consistency_stat_t *this_ )
{
}

static inline int32_t consistency_stat_get_total_count ( const consistency_stat_t *this_ )
{
    const int32_t result
        = (*this_).classifiers + (*this_).features + (*this_).lifelines + (*this_).relationships;
    return result;
}

static inline void consistency_stat_transfer_to ( const consistency_stat_t *this_, data_stat_t *io_target )
{
    assert( NULL != io_target );
    if ( (*this_).classifiers > 0 )
    {
        data_stat_add_count ( io_target, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_CREATED, (*this_).classifiers );
    }
    else
    {
        data_stat_add_count ( io_target, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_DELETED, -((*this_).classifiers) );
    }
    if ( (*this_).features > 0 )
    {
        data_stat_add_count ( io_target, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_CREATED, (*this_).features );
    }
    else
    {
        data_stat_add_count ( io_target, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_DELETED, -((*this_).features) );
    }
    if ( (*this_).lifelines > 0 )
    {
        data_stat_add_count ( io_target, DATA_STAT_TABLE_LIFELINE, DATA_STAT_SERIES_CREATED, (*this_).lifelines );
    }
    else
    {
        data_stat_add_count ( io_target, DATA_STAT_TABLE_LIFELINE, DATA_STAT_SERIES_DELETED, -((*this_).lifelines) );
    }
    if ( (*this_).relationships > 0 )
    {
        data_stat_add_count ( io_target, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_CREATED, (*this_).relationships );
    }
    else
    {
        data_stat_add_count ( io_target, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_DELETED, -((*this_).relationships) );
    }
}

static inline void consistency_stat_increment_classifiers ( consistency_stat_t *this_ )
{
    (*this_).classifiers ++;
}

static inline void consistency_stat_decrement_classifiers ( consistency_stat_t *this_ )
{
    (*this_).classifiers --;
}

static inline int32_t consistency_stat_get_classifiers ( const consistency_stat_t *this_ )
{
    return (*this_).classifiers;
}

static inline void consistency_stat_increment_features ( consistency_stat_t *this_ )
{
    (*this_).features ++;
}

static inline void consistency_stat_decrement_features ( consistency_stat_t *this_ )
{
    (*this_).features --;
}

static inline int32_t consistency_stat_get_features ( const consistency_stat_t *this_ )
{
    return (*this_).features;
}

static inline void consistency_stat_increment_lifelines ( consistency_stat_t *this_ )
{
    (*this_).lifelines ++;
}

static inline void consistency_stat_decrement_lifelines ( consistency_stat_t *this_ )
{
    (*this_).lifelines --;
}

static inline int32_t consistency_stat_get_lifelines ( const consistency_stat_t *this_ )
{
    return (*this_).lifelines;
}

static inline void consistency_stat_increment_relationships ( consistency_stat_t *this_ )
{
    (*this_).relationships ++;
}

static inline void consistency_stat_decrement_relationships ( consistency_stat_t *this_ )
{
    (*this_).relationships --;
}

static inline void consistency_stat_subtract_relationships ( consistency_stat_t *this_, int32_t deleted_relationships )
{
    (*this_).relationships -= deleted_relationships;
}

static inline int32_t consistency_stat_get_relationships ( const consistency_stat_t *this_ )
{
    return (*this_).relationships;
}

static inline void consistency_stat_trace ( const consistency_stat_t *this_ )
{
    U8_TRACE_INFO_INT_INT( "consistency_stat_t[CLAS,REL] ", (*this_).classifiers, (*this_).relationships );
    U8_TRACE_INFO_INT_INT( "consistency_stat_t[FEAT,LIFE]", (*this_).features, (*this_).lifelines );
}


/*
Copyright 2026-2026 Andreas Warnke

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
