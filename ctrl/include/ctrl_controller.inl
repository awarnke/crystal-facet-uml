/* File: ctrl_controller.inl; Copyright and License: see below */

static inline ctrl_classifier_controller_t *ctrl_controller_get_classifier_control_ptr ( ctrl_controller_t *this_ )
{
    return &((*this_).classifiers);
}

static inline ctrl_diagram_controller_t *ctrl_controller_get_diagram_control_ptr ( ctrl_controller_t *this_ )
{
    return &((*this_).diagrams);
}

/* ================================ interface for undo redo ================================ */

static inline ctrl_error_t ctrl_controller_undo ( ctrl_controller_t *this_, data_stat_t *io_stat )
{
    assert ( NULL != io_stat );
    return ctrl_undo_redo_list_undo( &((*this_).undo_redo_list), io_stat );
}

static inline ctrl_error_t ctrl_controller_redo ( ctrl_controller_t *this_, data_stat_t *io_stat )
{
    assert ( NULL != io_stat );
    return ctrl_undo_redo_list_redo( &((*this_).undo_redo_list), io_stat );
}

/* ================================ interface for database file ================================ */

static inline ctrl_error_t ctrl_controller_repair_database ( ctrl_controller_t *this_,
                                                             bool modify_db,
                                                             uint32_t *out_err,
                                                             uint32_t *out_fix,
                                                             utf8stringbuf_t out_report )
{
    ctrl_error_t result;
    result = ctrl_consistency_checker_repair_database( &((*this_).consistency_checker), modify_db, out_err, out_fix, out_report );
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
