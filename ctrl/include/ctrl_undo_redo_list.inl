/* File: ctrl_undo_redo_list.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

static inline void ctrl_undo_redo_list_init ( ctrl_undo_redo_list_t *this_, data_database_reader_t *db_reader, data_database_writer_t *db_writer )
{
    assert( NULL != db_reader );
    assert( NULL != db_writer );

    (*this_).db_reader = db_reader;
    (*this_).db_writer = db_writer;

    ctrl_undo_redo_entry_init( &((*this_).buffer[0]), CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY );
    (*this_).start = 0;
    (*this_).length = 1;
    (*this_).current = 1;
    (*this_).buffer_incomplete = false;
}

static inline void ctrl_undo_redo_list_destroy ( ctrl_undo_redo_list_t *this_ )
{
    ctrl_undo_redo_list_clear( this_ );
    (*this_).db_reader = NULL;
    (*this_).db_writer = NULL;
}

static inline void ctrl_undo_redo_list_clear ( ctrl_undo_redo_list_t *this_ )
{
    assert( (*this_).start < CTRL_UNDO_REDO_LIST_MAX_SIZE );
    assert( (*this_).length <= CTRL_UNDO_REDO_LIST_MAX_SIZE );
    assert( (*this_).current <= (*this_).length );

    /* call destructors of elements: */
    for ( uint32_t pos = 0; pos < (*this_).length; pos ++ )
    {
        uint32_t index = ((*this_).start + pos) % CTRL_UNDO_REDO_LIST_MAX_SIZE;
        ctrl_undo_redo_entry_destroy( &((*this_).buffer[index]) );
    }

    /* reset: */
    ctrl_undo_redo_entry_init( &((*this_).buffer[0]), CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY );
    (*this_).start = 0;
    (*this_).length = 1;
    (*this_).current = 1;
    (*this_).buffer_incomplete = false;
}

static inline u8_error_t ctrl_undo_redo_list_add_boundary ( ctrl_undo_redo_list_t *this_ )
{
    u8_error_t result = U8_ERROR_NONE;
    ctrl_undo_redo_entry_t *list_entry;

    /* add and re-initialize the list entry */
    list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_reinit( list_entry, CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY );

    /* check if >=1 complete set of transactions is still in the undo-redo-list */
    if ( 1 == ctrl_undo_redo_list_private_count_boundaries( this_, (*this_).start, (*this_).length ) )
    {
        result = U8_ERROR_ARRAY_BUFFER_EXCEEDED;
    }

    return result;
}

static inline u8_error_t ctrl_undo_redo_list_get_last_statistics ( ctrl_undo_redo_list_t *this_, data_stat_t *io_stat )
{
    assert( NULL != io_stat );
    u8_error_t result = U8_ERROR_NONE;

    bool finished = false;
    if ( (*this_).current > 0 )
    {
        /* at pos==(*this_).current, there should be a CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY */
        /* therefore we start 1 left to this */
        for ( uint32_t pos = (*this_).current-1; (pos!=0)&&(!finished); pos-- )
        {
            /*    B0   A1   A2   A3   B1   A4   B2   X   X    */
            /*  0    1    2    3    4    5    6    7   8   9  */
            /*  ^ start                            ^ current  */

            /* get entry left of pos */
            const uint32_t index = ((*this_).start + (pos+CTRL_UNDO_REDO_LIST_MAX_SIZE-1)) % CTRL_UNDO_REDO_LIST_MAX_SIZE;
            const ctrl_undo_redo_entry_t *const cur_entry = &((*this_).buffer[index]);
            if ( CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY == ctrl_undo_redo_entry_get_action_type( cur_entry ) )
            {
                finished = true;
            }
            else
            {
                ctrl_undo_redo_entry_to_statistics ( cur_entry, false /*=undo*/, false /*=err*/, io_stat );
            }
        }
    }
    if ( ! finished )
    {
        result = U8_ERROR_ARRAY_BUFFER_EXCEEDED;
    }
    return result;
}

static inline u8_error_t ctrl_undo_redo_list_get_undo_iterator ( const ctrl_undo_redo_list_t *this_,
                                                                 ctrl_undo_redo_iterator_t *out_undo_iterator )
{
    assert( NULL != out_undo_iterator );
    u8_error_t result = U8_ERROR_NONE;

    result = U8_ERROR_NOT_YET_IMPLEMENTED;

    return result;
}

static inline u8_error_t ctrl_undo_redo_list_get_redo_iterator ( const ctrl_undo_redo_list_t *this_,
                                                                 ctrl_undo_redo_iterator_t *out_redo_iterator )
{
    assert( NULL != out_redo_iterator );
    u8_error_t result = U8_ERROR_NONE;

    result = U8_ERROR_NOT_YET_IMPLEMENTED;

    return result;
}

/* ================================ DIAGRAM ================================ */

static inline void ctrl_undo_redo_list_add_delete_diagram ( ctrl_undo_redo_list_t *this_, data_diagram_t *old_value )
{
    ctrl_undo_redo_entry_t *list_entry;
    data_diagram_t *list_entry_old;

    /* add and re-initialize the list entry */
    list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_reinit( list_entry, CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAM );

    /* copy the values */
    list_entry_old = ctrl_undo_redo_entry_get_diagram_before_action_ptr( list_entry );
    data_diagram_replace( list_entry_old, old_value);
}

static inline void ctrl_undo_redo_list_add_update_diagram ( ctrl_undo_redo_list_t *this_, data_diagram_t *old_value, data_diagram_t *new_value )
{
    ctrl_undo_redo_entry_t *list_entry;
    data_diagram_t *list_entry_old;
    data_diagram_t *list_entry_new;

    /* add and re-initialize the list entry */
    list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_reinit( list_entry, CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAM );

    /* copy the values */
    list_entry_old = ctrl_undo_redo_entry_get_diagram_before_action_ptr( list_entry );
    data_diagram_replace( list_entry_old, old_value);
    list_entry_new = ctrl_undo_redo_entry_get_diagram_after_action_ptr( list_entry );
    data_diagram_replace( list_entry_new, new_value);
}

static inline void ctrl_undo_redo_list_add_create_diagram ( ctrl_undo_redo_list_t *this_, data_diagram_t *new_value )
{
    ctrl_undo_redo_entry_t *list_entry;
    data_diagram_t *list_entry_new;

    /* add and re-initialize the list entry */
    list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_reinit( list_entry, CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAM );

    /* copy the values */
    list_entry_new = ctrl_undo_redo_entry_get_diagram_after_action_ptr( list_entry );
    data_diagram_replace( list_entry_new, new_value);
}

/* ================================ DIAGRAMELEMENT ================================ */

static inline void ctrl_undo_redo_list_add_delete_diagramelement ( ctrl_undo_redo_list_t *this_, data_diagramelement_t *old_value )
{
    ctrl_undo_redo_entry_t *list_entry;
    data_diagramelement_t *list_entry_old;

    /* add and re-initialize the list entry */
    list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_reinit( list_entry, CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAMELEMENT );

    /* copy the values */
    list_entry_old = ctrl_undo_redo_entry_get_diagramelement_before_action_ptr( list_entry );
    data_diagramelement_replace( list_entry_old, old_value);
}

static inline void ctrl_undo_redo_list_add_update_diagramelement ( ctrl_undo_redo_list_t *this_, data_diagramelement_t *old_value, data_diagramelement_t *new_value )
{
    ctrl_undo_redo_entry_t *list_entry;
    data_diagramelement_t *list_entry_old;
    data_diagramelement_t *list_entry_new;

    /* add and re-initialize the list entry */
    list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_reinit( list_entry, CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAMELEMENT );

    /* copy the values */
    list_entry_old = ctrl_undo_redo_entry_get_diagramelement_before_action_ptr( list_entry );
    data_diagramelement_replace( list_entry_old, old_value);
    list_entry_new = ctrl_undo_redo_entry_get_diagramelement_after_action_ptr( list_entry );
    data_diagramelement_replace( list_entry_new, new_value);
}

static inline void ctrl_undo_redo_list_add_create_diagramelement ( ctrl_undo_redo_list_t *this_, data_diagramelement_t *new_value )
{
    ctrl_undo_redo_entry_t *list_entry;
    data_diagramelement_t *list_entry_new;

    /* add and re-initialize the list entry */
    list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_reinit( list_entry, CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAMELEMENT );

    /* copy the values */
    list_entry_new = ctrl_undo_redo_entry_get_diagramelement_after_action_ptr( list_entry );
    data_diagramelement_replace( list_entry_new, new_value);
}

/* ================================ CLASSIFIER ================================ */

static inline void ctrl_undo_redo_list_add_delete_classifier ( ctrl_undo_redo_list_t *this_, data_classifier_t *old_value )
{
    ctrl_undo_redo_entry_t *list_entry;
    data_classifier_t *list_entry_old;

    /* add and re-initialize the list entry */
    list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_reinit( list_entry, CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_CLASSIFIER );

    /* copy the values */
    list_entry_old = ctrl_undo_redo_entry_get_classifier_before_action_ptr( list_entry );
    data_classifier_replace( list_entry_old, old_value);
}

static inline void ctrl_undo_redo_list_add_update_classifier ( ctrl_undo_redo_list_t *this_, data_classifier_t *old_value, data_classifier_t *new_value )
{
    ctrl_undo_redo_entry_t *list_entry;
    data_classifier_t *list_entry_old;
    data_classifier_t *list_entry_new;

    /* add and re-initialize the list entry */
    list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_reinit( list_entry, CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_CLASSIFIER );

    /* copy the values */
    list_entry_old = ctrl_undo_redo_entry_get_classifier_before_action_ptr( list_entry );
    data_classifier_replace( list_entry_old, old_value);
    list_entry_new = ctrl_undo_redo_entry_get_classifier_after_action_ptr( list_entry );
    data_classifier_replace( list_entry_new, new_value);
}

static inline void ctrl_undo_redo_list_add_create_classifier ( ctrl_undo_redo_list_t *this_, data_classifier_t *new_value )
{
    ctrl_undo_redo_entry_t *list_entry;
    data_classifier_t *list_entry_new;

    /* add and re-initialize the list entry */
    list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_reinit( list_entry, CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_CLASSIFIER );

    /* copy the values */
    list_entry_new = ctrl_undo_redo_entry_get_classifier_after_action_ptr( list_entry );
    data_classifier_replace( list_entry_new, new_value);
}

/* ================================ FEATURE ================================ */

static inline void ctrl_undo_redo_list_add_delete_feature ( ctrl_undo_redo_list_t *this_, data_feature_t *old_value )
{
    ctrl_undo_redo_entry_t *list_entry;
    data_feature_t *list_entry_old;

    /* add and re-initialize the list entry */
    list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_reinit( list_entry, CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_FEATURE );

    /* copy the values */
    list_entry_old = ctrl_undo_redo_entry_get_feature_before_action_ptr( list_entry );
    data_feature_replace( list_entry_old, old_value);
}

static inline void ctrl_undo_redo_list_add_update_feature ( ctrl_undo_redo_list_t *this_, data_feature_t *old_value, data_feature_t *new_value )
{
    ctrl_undo_redo_entry_t *list_entry;
    data_feature_t *list_entry_old;
    data_feature_t *list_entry_new;

    /* add and re-initialize the list entry */
    list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_reinit( list_entry, CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_FEATURE );

    /* copy the values */
    list_entry_old = ctrl_undo_redo_entry_get_feature_before_action_ptr( list_entry );
    data_feature_replace( list_entry_old, old_value);
    list_entry_new = ctrl_undo_redo_entry_get_feature_after_action_ptr( list_entry );
    data_feature_replace( list_entry_new, new_value);
}

static inline void ctrl_undo_redo_list_add_create_feature ( ctrl_undo_redo_list_t *this_, data_feature_t *new_value )
{
    ctrl_undo_redo_entry_t *list_entry;
    data_feature_t *list_entry_new;

    /* add and re-initialize the list entry */
    list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_reinit( list_entry, CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_FEATURE );

    /* copy the values */
    list_entry_new = ctrl_undo_redo_entry_get_feature_after_action_ptr( list_entry );
    data_feature_replace( list_entry_new, new_value);
}

/* ================================ RELATIONSHIP ================================ */

static inline void ctrl_undo_redo_list_add_delete_relationship ( ctrl_undo_redo_list_t *this_, data_relationship_t *old_value )
{
    ctrl_undo_redo_entry_t *list_entry;
    data_relationship_t *list_entry_old;

    /* add and re-initialize the list entry */
    list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_reinit( list_entry, CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_RELATIONSHIP );

    /* copy the values */
    list_entry_old = ctrl_undo_redo_entry_get_relationship_before_action_ptr( list_entry );
    data_relationship_replace( list_entry_old, old_value);
}

static inline void ctrl_undo_redo_list_add_update_relationship ( ctrl_undo_redo_list_t *this_, data_relationship_t *old_value, data_relationship_t *new_value )
{
    ctrl_undo_redo_entry_t *list_entry;
    data_relationship_t *list_entry_old;
    data_relationship_t *list_entry_new;

    /* add and re-initialize the list entry */
    list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_reinit( list_entry, CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_RELATIONSHIP );

    /* copy the values */
    list_entry_old = ctrl_undo_redo_entry_get_relationship_before_action_ptr( list_entry );
    data_relationship_replace( list_entry_old, old_value);
    list_entry_new = ctrl_undo_redo_entry_get_relationship_after_action_ptr( list_entry );
    data_relationship_replace( list_entry_new, new_value);
}

static inline void ctrl_undo_redo_list_add_create_relationship ( ctrl_undo_redo_list_t *this_, data_relationship_t *new_value )
{
    ctrl_undo_redo_entry_t *list_entry;
    data_relationship_t *list_entry_new;

    /* add and re-initialize the list entry */
    list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_reinit( list_entry, CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_RELATIONSHIP );

    /* copy the values */
    list_entry_new = ctrl_undo_redo_entry_get_relationship_after_action_ptr( list_entry );
    data_relationship_replace( list_entry_new, new_value);
}

/* ================================ private ================================ */

static inline uint32_t ctrl_undo_redo_list_private_count_boundaries ( ctrl_undo_redo_list_t *this_, uint32_t start_idx, uint32_t search_len )
{
    assert( search_len <= CTRL_UNDO_REDO_LIST_MAX_SIZE );

    uint32_t result = 0;
    for ( uint32_t pos = 0; pos < search_len; pos ++ )
    {
        uint32_t index = (start_idx + pos) % CTRL_UNDO_REDO_LIST_MAX_SIZE;
        if ( CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY == ctrl_undo_redo_entry_get_action_type( &((*this_).buffer[index]) ) )
        {
            result ++;
        }
    }
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
