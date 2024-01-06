/* File: ctrl_undo_redo_entry.inl; Copyright and License: see below */

#include "data_feature_type.h"
#include <assert.h>

static inline void ctrl_undo_redo_entry_init_empty ( ctrl_undo_redo_entry_t *this_ )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY;
}

static inline void ctrl_undo_redo_entry_init ( ctrl_undo_redo_entry_t *this_, ctrl_undo_redo_entry_type_t action_type )
{
    (*this_).action_type = action_type;
}

static inline void ctrl_undo_redo_entry_reinit ( ctrl_undo_redo_entry_t *this_, ctrl_undo_redo_entry_type_t action_type )
{
    (*this_).action_type = action_type;
}

static inline void ctrl_undo_redo_entry_destroy ( ctrl_undo_redo_entry_t *this_ )
{
}

static inline ctrl_undo_redo_entry_type_t ctrl_undo_redo_entry_get_action_type ( const ctrl_undo_redo_entry_t *this_ )
{
    return (*this_).action_type;
}

static inline data_classifier_t *ctrl_undo_redo_entry_get_classifier_before_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_before_action.classifier);
}

static inline data_diagram_t *ctrl_undo_redo_entry_get_diagram_before_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_before_action.diagram);
}

static inline data_diagramelement_t *ctrl_undo_redo_entry_get_diagramelement_before_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_before_action.diagramelement);
}

static inline data_relationship_t *ctrl_undo_redo_entry_get_relationship_before_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_before_action.relationship);
}

static inline data_feature_t *ctrl_undo_redo_entry_get_feature_before_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_before_action.feature);
}

static inline data_classifier_t *ctrl_undo_redo_entry_get_classifier_after_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_after_action.classifier);
}

static inline data_diagram_t *ctrl_undo_redo_entry_get_diagram_after_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_after_action.diagram);
}

static inline data_diagramelement_t *ctrl_undo_redo_entry_get_diagramelement_after_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_after_action.diagramelement);
}

static inline data_relationship_t *ctrl_undo_redo_entry_get_relationship_after_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_after_action.relationship);
}

static inline data_feature_t *ctrl_undo_redo_entry_get_feature_after_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_after_action.feature);
}

static inline void ctrl_undo_redo_entry_to_statistics ( const ctrl_undo_redo_entry_t *this_, bool undo, bool err, data_stat_t *io_stat )
{
    assert( NULL != io_stat );

    data_stat_table_t table = DATA_STAT_TABLE_LIFELINE;
    data_stat_series_t series = DATA_STAT_SERIES_ERROR;
    switch( (*this_).action_type )
    {
        case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAM:
        {
            table = DATA_STAT_TABLE_DIAGRAM;
            series = undo ? DATA_STAT_SERIES_CREATED : DATA_STAT_SERIES_DELETED;
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAM:
        {
            table = DATA_STAT_TABLE_DIAGRAM;
            series = DATA_STAT_SERIES_MODIFIED;
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAM:
        {
            table = DATA_STAT_TABLE_DIAGRAM;
            series = undo ? DATA_STAT_SERIES_DELETED : DATA_STAT_SERIES_CREATED;
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAMELEMENT:
        {
            table = DATA_STAT_TABLE_DIAGRAMELEMENT;
            series = undo ? DATA_STAT_SERIES_CREATED : DATA_STAT_SERIES_DELETED;
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAMELEMENT:
        {
            table = DATA_STAT_TABLE_DIAGRAMELEMENT;
            series = DATA_STAT_SERIES_MODIFIED;
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAMELEMENT:
        {
            table = DATA_STAT_TABLE_DIAGRAMELEMENT;
            series = undo ? DATA_STAT_SERIES_DELETED : DATA_STAT_SERIES_CREATED;
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_CLASSIFIER:
        {
            table = DATA_STAT_TABLE_CLASSIFIER;
            series = undo ? DATA_STAT_SERIES_CREATED : DATA_STAT_SERIES_DELETED;
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_CLASSIFIER:
        {
            table = DATA_STAT_TABLE_CLASSIFIER;
            series = DATA_STAT_SERIES_MODIFIED;
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_CLASSIFIER:
        {
            table = DATA_STAT_TABLE_CLASSIFIER;
            series = undo ? DATA_STAT_SERIES_DELETED : DATA_STAT_SERIES_CREATED;
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_FEATURE:
        {
            const data_feature_t *const old_feat = &((*this_).data_before_action.feature);
            const data_feature_type_t feat_type = data_feature_get_main_type( old_feat );
            table = (feat_type == DATA_FEATURE_TYPE_LIFELINE) ? DATA_STAT_TABLE_LIFELINE : DATA_STAT_TABLE_FEATURE;
            series = undo ? DATA_STAT_SERIES_CREATED : DATA_STAT_SERIES_DELETED;
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_FEATURE:
        {
            const data_feature_t *const old_feat = &((*this_).data_before_action.feature);
            const data_feature_type_t feat_type = data_feature_get_main_type( old_feat );
            table = (feat_type == DATA_FEATURE_TYPE_LIFELINE) ? DATA_STAT_TABLE_LIFELINE : DATA_STAT_TABLE_FEATURE;
            series = DATA_STAT_SERIES_MODIFIED;
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_FEATURE:
        {
            const data_feature_t *const new_feat = &((*this_).data_after_action.feature);
            const data_feature_type_t feat_type = data_feature_get_main_type( new_feat );
            table = (feat_type == DATA_FEATURE_TYPE_LIFELINE) ? DATA_STAT_TABLE_LIFELINE : DATA_STAT_TABLE_FEATURE;
            series = undo ? DATA_STAT_SERIES_DELETED : DATA_STAT_SERIES_CREATED;
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_RELATIONSHIP:
        {
            table = DATA_STAT_TABLE_RELATIONSHIP;
            series = undo ? DATA_STAT_SERIES_CREATED : DATA_STAT_SERIES_DELETED;
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_RELATIONSHIP:
        {
            table = DATA_STAT_TABLE_RELATIONSHIP;
            series = DATA_STAT_SERIES_MODIFIED;
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_RELATIONSHIP:
        {
            table = DATA_STAT_TABLE_RELATIONSHIP;
            series = undo ? DATA_STAT_SERIES_DELETED : DATA_STAT_SERIES_CREATED;
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY:
        {
            /* nothing to count */
        }
        break;

        default :
        {
            /* internal error, switch-case statement incomplete */
            U8_LOG_ERROR("incomplete switch statement in ctrl_undo_redo_entry_to_statistics");
        }
        break;
    }

    data_stat_inc_count ( io_stat, table, (err ? DATA_STAT_SERIES_ERROR : series) );
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
