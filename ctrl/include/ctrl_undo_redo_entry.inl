/* File: ctrl_undo_redo_entry.inl; Copyright and License: see below */

#include "entity/data_feature_type.h"
#include <assert.h>

static inline void ctrl_undo_redo_entry_init_empty ( ctrl_undo_redo_entry_t *this_ )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY;
    (*this_).data_before_action.boundary_revision = DATA_REVISION_VOID;
    (*this_).data_after_action.boundary_revision = DATA_REVISION_VOID;
}

static inline void ctrl_undo_redo_entry_destroy ( ctrl_undo_redo_entry_t *this_ )
{
}

/* ================================ BOUNDARY ================================ */

static inline void ctrl_undo_redo_entry_init_boundary ( ctrl_undo_redo_entry_t *this_, data_revision_t boundary_revision )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY;
    (*this_).data_before_action.boundary_revision = DATA_REVISION_VOID;  /* unused */
    (*this_).data_after_action.boundary_revision = boundary_revision;
}

static inline data_revision_t ctrl_undo_redo_entry_get_boundary_revision ( const ctrl_undo_redo_entry_t *this_ )
{
    return (*this_).data_after_action.boundary_revision;
}

/* ================================ DIAGRAM ================================ */

static inline void ctrl_undo_redo_entry_init_create_diagram ( ctrl_undo_redo_entry_t *this_,
                                                              const data_diagram_t *data_after )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAM;
    data_diagram_init_empty( &((*this_).data_before_action.diagram) );
    data_diagram_replace( &((*this_).data_after_action.diagram), data_after );
}

static inline void ctrl_undo_redo_entry_init_update_diagram ( ctrl_undo_redo_entry_t *this_,
                                                              const data_diagram_t *data_before,
                                                              const data_diagram_t *data_after )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAM;
    data_diagram_replace( &((*this_).data_before_action.diagram), data_before );
    data_diagram_replace( &((*this_).data_after_action.diagram), data_after );
}

static inline void ctrl_undo_redo_entry_init_delete_diagram ( ctrl_undo_redo_entry_t *this_,
                                                              const data_diagram_t *data_before )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAM;
    data_diagram_replace( &((*this_).data_before_action.diagram), data_before );
    data_diagram_init_empty( &((*this_).data_after_action.diagram) );
}

static inline const data_diagram_t *ctrl_undo_redo_entry_get_diagram_after_action_const ( const ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_after_action.diagram);
}

static inline const data_diagram_t *ctrl_undo_redo_entry_get_diagram_before_action_const ( const ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_before_action.diagram);
}

/* ================================ DIAGRAMELEMENT ================================ */

static inline void ctrl_undo_redo_entry_init_create_diagramelement ( ctrl_undo_redo_entry_t *this_,
                                                                     const data_diagramelement_t *data_after )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAMELEMENT;
    data_diagramelement_init_empty( &((*this_).data_before_action.diagramelement) );
    data_diagramelement_replace( &((*this_).data_after_action.diagramelement), data_after );
}

static inline void ctrl_undo_redo_entry_init_update_diagramelement ( ctrl_undo_redo_entry_t *this_,
                                                                     const data_diagramelement_t *data_before,
                                                                     const data_diagramelement_t *data_after )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAMELEMENT;
    data_diagramelement_replace( &((*this_).data_before_action.diagramelement), data_before );
    data_diagramelement_replace( &((*this_).data_after_action.diagramelement), data_after );
}

static inline void ctrl_undo_redo_entry_init_delete_diagramelement ( ctrl_undo_redo_entry_t *this_,
                                                                     const data_diagramelement_t *data_before )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAMELEMENT;
    data_diagramelement_replace( &((*this_).data_before_action.diagramelement), data_before );
    data_diagramelement_init_empty( &((*this_).data_after_action.diagramelement) );
}

static inline const data_diagramelement_t *ctrl_undo_redo_entry_get_diagramelement_after_action_const ( const ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_after_action.diagramelement);
}

static inline const data_diagramelement_t *ctrl_undo_redo_entry_get_diagramelement_before_action_const ( const ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_before_action.diagramelement);
}

/* ================================ CLASSIFIER ================================ */

static inline void ctrl_undo_redo_entry_init_create_classifier ( ctrl_undo_redo_entry_t *this_ ,
                                                                 const data_classifier_t *data_after )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_CLASSIFIER;
    data_classifier_init_empty( &((*this_).data_before_action.classifier) );
    data_classifier_replace( &((*this_).data_after_action.classifier), data_after );
}

static inline void ctrl_undo_redo_entry_init_update_classifier ( ctrl_undo_redo_entry_t *this_ ,
                                                                 const data_classifier_t *data_before,
                                                                 const data_classifier_t *data_after )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_CLASSIFIER;
    data_classifier_replace( &((*this_).data_before_action.classifier), data_before );
    data_classifier_replace( &((*this_).data_after_action.classifier), data_after );
}

static inline void ctrl_undo_redo_entry_init_delete_classifier ( ctrl_undo_redo_entry_t *this_ ,
                                                                 const data_classifier_t *data_before )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_CLASSIFIER;
    data_classifier_replace( &((*this_).data_before_action.classifier), data_before );
    data_classifier_init_empty( &((*this_).data_after_action.classifier) );
}

static inline data_classifier_t *ctrl_undo_redo_entry_get_classifier_before_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_before_action.classifier);
}

static inline data_classifier_t *ctrl_undo_redo_entry_get_classifier_after_action_ptr ( ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_after_action.classifier);
}

static inline const data_classifier_t *ctrl_undo_redo_entry_get_classifier_after_action_const ( const ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_after_action.classifier);
}

static inline const data_classifier_t *ctrl_undo_redo_entry_get_classifier_before_action_const ( const ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_before_action.classifier);
}

/* ================================ FEATURE ================================ */

static inline void ctrl_undo_redo_entry_init_create_feature ( ctrl_undo_redo_entry_t *this_,
                                                              const data_feature_t *data_after )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_FEATURE;
    data_feature_init_empty( &((*this_).data_before_action.feature) );
    data_feature_replace( &((*this_).data_after_action.feature), data_after );
}

static inline void ctrl_undo_redo_entry_init_update_feature ( ctrl_undo_redo_entry_t *this_,
                                                              const data_feature_t *data_before,
                                                              const data_feature_t *data_after )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_FEATURE;
    data_feature_replace( &((*this_).data_before_action.feature), data_before );
    data_feature_replace( &((*this_).data_after_action.feature), data_after );
}

static inline void ctrl_undo_redo_entry_init_delete_feature ( ctrl_undo_redo_entry_t *this_,
                                                              const data_feature_t *data_before )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_FEATURE;
    data_feature_replace( &((*this_).data_before_action.feature), data_before );
    data_feature_init_empty( &((*this_).data_after_action.feature) );
}

static inline const data_feature_t *ctrl_undo_redo_entry_get_feature_after_action_const ( const ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_after_action.feature);
}

static inline const data_feature_t *ctrl_undo_redo_entry_get_feature_before_action_const ( const ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_before_action.feature);
}

/* ================================ RELATIONSHIP ================================ */

static inline void ctrl_undo_redo_entry_init_create_relationship ( ctrl_undo_redo_entry_t *this_,
                                                                   const data_relationship_t *data_after )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_RELATIONSHIP;
    data_relationship_init_empty( &((*this_).data_before_action.relationship) );
    data_relationship_replace( &((*this_).data_after_action.relationship), data_after );
}

static inline void ctrl_undo_redo_entry_init_update_relationship ( ctrl_undo_redo_entry_t *this_,
                                                                   const data_relationship_t *data_before,
                                                                   const data_relationship_t *data_after )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_RELATIONSHIP;
    data_relationship_replace( &((*this_).data_before_action.relationship), data_before );
    data_relationship_replace( &((*this_).data_after_action.relationship), data_after );
}

static inline void ctrl_undo_redo_entry_init_delete_relationship ( ctrl_undo_redo_entry_t *this_,
                                                                   const data_relationship_t *data_before )
{
    (*this_).action_type = CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_RELATIONSHIP;
    data_relationship_replace( &((*this_).data_before_action.relationship), data_before );
    data_relationship_init_empty( &((*this_).data_after_action.relationship) );
}

static inline const data_relationship_t *ctrl_undo_redo_entry_get_relationship_after_action_const ( const ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_after_action.relationship);
}

static inline const data_relationship_t *ctrl_undo_redo_entry_get_relationship_before_action_const ( const ctrl_undo_redo_entry_t *this_ )
{
    return &((*this_).data_before_action.relationship);
}

/* ================================ ENTRY_TYPE ================================ */

static inline ctrl_undo_redo_entry_type_t ctrl_undo_redo_entry_get_action_type ( const ctrl_undo_redo_entry_t *this_ )
{
    return (*this_).action_type;
}

/* ================================ STATISTICS ================================ */

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
Copyright 2016-2026 Andreas Warnke

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
