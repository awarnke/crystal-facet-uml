/* File: ctrl_controller.inl; Copyright and License: see below */

static inline ctrl_classifier_controller_t *ctrl_controller_get_classifier_control_ptr ( ctrl_controller_t *this_ )
{
    return &( (*this_).classifiers );
}

static inline ctrl_diagram_controller_t *ctrl_controller_get_diagram_control_ptr ( ctrl_controller_t *this_ )
{
    return &( (*this_).diagrams );
}

static inline u8_error_t ctrl_controller_transaction_begin ( ctrl_controller_t *this_ )
{
    /* do not sent notifications while an explicitly requested transaction is active */
    data_change_notifier_enable_stealth_mode( data_database_get_notifier_ptr( (*this_).database ) );

    return data_database_transaction_begin( (*this_).database );
}

static inline u8_error_t ctrl_controller_transaction_commit ( ctrl_controller_t *this_ )
{
    const u8_error_t result = data_database_transaction_commit( (*this_).database );

    /* do sent notifications when committing an explicitly requested transaction */
    data_change_notifier_disable_stealth_mode( data_database_get_notifier_ptr( (*this_).database ) );

    return result;
}

/* ================================ interface for undo redo ================================ */

static inline u8_error_t ctrl_controller_undo ( ctrl_controller_t *this_, data_stat_t *io_stat )
{
    assert ( NULL != io_stat );
    return ctrl_undo_redo_list_undo( &((*this_).undo_redo_list), io_stat );
}

static inline u8_error_t ctrl_controller_redo ( ctrl_controller_t *this_, data_stat_t *io_stat )
{
    assert ( NULL != io_stat );
    return ctrl_undo_redo_list_redo( &((*this_).undo_redo_list), io_stat );
}

static inline u8_error_t ctrl_controller_get_undo_iterator ( const ctrl_controller_t *this_,
                                                             ctrl_undo_redo_iterator_t *out_undo_iterator )
{
    assert ( NULL != out_undo_iterator );
    return ctrl_undo_redo_list_get_undo_iterator( &((*this_).undo_redo_list), out_undo_iterator );
}

static inline u8_error_t ctrl_controller_get_redo_iterator ( const ctrl_controller_t *this_,
                                                             ctrl_undo_redo_iterator_t *out_redo_iterator )
{
    assert ( NULL != out_redo_iterator );
    return ctrl_undo_redo_list_get_redo_iterator( &((*this_).undo_redo_list), out_redo_iterator );
}

/* ================================ interface for database file ================================ */

static inline u8_error_t ctrl_controller_repair_database ( ctrl_controller_t *this_,
                                                           bool modify_db,
                                                           uint32_t *out_err,
                                                           uint32_t *out_fix,
                                                           utf8stream_writer_t *out_english_report )
{
    const u8_error_t result
        = ctrl_consistency_checker_repair_database( &((*this_).consistency_checker),
                                                    modify_db,
                                                    out_err,
                                                    out_fix,
                                                    out_english_report
                                                  );
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
