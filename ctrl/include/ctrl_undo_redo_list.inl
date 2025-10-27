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

/* ================================ DIAGRAM ================================ */

static inline void ctrl_undo_redo_list_add_delete_diagram ( ctrl_undo_redo_list_t *this_, const data_diagram_t *old_value )
{
    /* add and re-initialize the list entry */
    ctrl_undo_redo_entry_t *list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_init_delete_diagram( list_entry, old_value );
}

static inline void ctrl_undo_redo_list_add_update_diagram ( ctrl_undo_redo_list_t *this_, const data_diagram_t *old_value, const data_diagram_t *new_value )
{
    /* add and re-initialize the list entry */
    ctrl_undo_redo_entry_t *list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_init_update_diagram( list_entry, old_value, new_value );
}

static inline void ctrl_undo_redo_list_add_create_diagram ( ctrl_undo_redo_list_t *this_, const data_diagram_t *new_value )
{
    /* add and re-initialize the list entry */
    ctrl_undo_redo_entry_t *list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_init_create_diagram( list_entry, new_value );
}

/* ================================ DIAGRAMELEMENT ================================ */

static inline void ctrl_undo_redo_list_add_delete_diagramelement ( ctrl_undo_redo_list_t *this_, data_diagramelement_t *old_value )
{
    /* add and re-initialize the list entry */
    ctrl_undo_redo_entry_t *list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_init_delete_diagramelement( list_entry, old_value );
}

static inline void ctrl_undo_redo_list_add_update_diagramelement ( ctrl_undo_redo_list_t *this_, data_diagramelement_t *old_value, data_diagramelement_t *new_value )
{
    /* add and re-initialize the list entry */
    ctrl_undo_redo_entry_t *list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_init_update_diagramelement( list_entry, old_value, new_value );
}

static inline void ctrl_undo_redo_list_add_create_diagramelement ( ctrl_undo_redo_list_t *this_, data_diagramelement_t *new_value )
{
    /* add and re-initialize the list entry */
    ctrl_undo_redo_entry_t *list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_init_create_diagramelement( list_entry, new_value );
}

/* ================================ CLASSIFIER ================================ */

static inline void ctrl_undo_redo_list_add_delete_classifier ( ctrl_undo_redo_list_t *this_, data_classifier_t *old_value )
{
    /* add and re-initialize the list entry */
    ctrl_undo_redo_entry_t *list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_init_delete_classifier( list_entry, old_value );
}

static inline void ctrl_undo_redo_list_add_update_classifier ( ctrl_undo_redo_list_t *this_, data_classifier_t *old_value, data_classifier_t *new_value )
{
    /* add and re-initialize the list entry */
    ctrl_undo_redo_entry_t *list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_init_update_classifier( list_entry, old_value, new_value );
}

static inline void ctrl_undo_redo_list_add_create_classifier ( ctrl_undo_redo_list_t *this_, data_classifier_t *new_value )
{
    /* add and re-initialize the list entry */
    ctrl_undo_redo_entry_t *list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_init_create_classifier( list_entry, new_value );
}

/* ================================ FEATURE ================================ */

static inline void ctrl_undo_redo_list_add_delete_feature ( ctrl_undo_redo_list_t *this_, data_feature_t *old_value )
{
    /* add and re-initialize the list entry */
    ctrl_undo_redo_entry_t *list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_init_delete_feature( list_entry, old_value );
}

static inline void ctrl_undo_redo_list_add_update_feature ( ctrl_undo_redo_list_t *this_, data_feature_t *old_value, data_feature_t *new_value )
{
    /* add and re-initialize the list entry */
    ctrl_undo_redo_entry_t *list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_init_update_feature( list_entry, old_value, new_value );
}

static inline void ctrl_undo_redo_list_add_create_feature ( ctrl_undo_redo_list_t *this_, data_feature_t *new_value )
{
    /* add and re-initialize the list entry */
    ctrl_undo_redo_entry_t *list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_init_create_feature( list_entry, new_value );
}

/* ================================ RELATIONSHIP ================================ */

static inline void ctrl_undo_redo_list_add_delete_relationship ( ctrl_undo_redo_list_t *this_, data_relationship_t *old_value )
{
    /* add and re-initialize the list entry */
    ctrl_undo_redo_entry_t *list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_init_delete_relationship( list_entry, old_value );
}

static inline void ctrl_undo_redo_list_add_update_relationship ( ctrl_undo_redo_list_t *this_, data_relationship_t *old_value, data_relationship_t *new_value )
{
    /* add and re-initialize the list entry */
    ctrl_undo_redo_entry_t *list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_init_update_relationship( list_entry, old_value, new_value );
}

static inline void ctrl_undo_redo_list_add_create_relationship ( ctrl_undo_redo_list_t *this_, data_relationship_t *new_value )
{
    /* add and re-initialize the list entry */
    ctrl_undo_redo_entry_t *list_entry = ctrl_undo_redo_list_private_add_entry_ptr( this_ );
    ctrl_undo_redo_entry_init_create_relationship( list_entry, new_value );
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
