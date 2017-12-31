/* File: data_change_notifier.h; Copyright and License: see below */

#ifndef DATA_CHANGE_NOTIFIER_H
#define DATA_CHANGE_NOTIFIER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Notifies on every change to the database.
 */

#include "data_error.h"
#include "data_table.h"
#include <glib-object.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief definition of max constants like max array sizes
 */
enum data_change_notifier_max_enum {
    DATA_CHANGE_NOTIFIER_MAX_LISTENERS = 48,
};

/*!
 *  \brief all data attributes needed for notifying changes
 */
struct data_change_notifier_struct {
    int32_t num_listeners;
    GObject *(listener_array[DATA_CHANGE_NOTIFIER_MAX_LISTENERS]);
};

typedef struct data_change_notifier_struct data_change_notifier_t;

/*!
 *  \brief signal name of the signal that is emitted in case of a change in the database.
 *
 *  Parameter of the signal is of type data_id_t*. If a record was changed, data_id_t contains table and row-id.
 *  If the whole database was changed, data_id_t contains DATA_TABLE_VOID as table-id.
 *  callback signature for the signal shoud be: void signal_callback( GObject *widget, data_id_t *object_id, gpointer data )
 */
extern const char *DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME;

/*!
 *  \brief initializes the data_change_notifier_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_change_notifier_init ( data_change_notifier_t *this_ );

/*!
 *  \brief destroys the data_change_notifier_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_change_notifier_destroy ( data_change_notifier_t *this_ );

/*!
 *  \brief notifies on changes to the database
 *
 *  \param this_ pointer to own object attributes
 *  \param table the table in which a row was created, updated or deleted
 *  \param row_id the row_id which was created, updated or deleted
 */
void data_change_notifier_emit_signal ( data_change_notifier_t *this_, data_table_t table, int64_t row_id );

/*!
 *  \brief adds an object as listener
 *
 *  \param this_ pointer to own object attributes
 *  \param new_listener pointer to a GObject that shall listen on the DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME signal
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_ARRAY_BUFFER_EXCEEDED if too many listeners are registered,
 *          DATA_ERROR_INVALID_REQUEST if the object was already a listener or data_change_notifier_t not initialized.
 */
data_error_t data_change_notifier_add_listener ( data_change_notifier_t *this_, GObject *new_listener );

/*!
 *  \brief removes an object as listener
 *
 *  \param this_ pointer to own object attributes
 *  \param no_listener pointer to a GObject that shall be removed from the listener list
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_INVALID_REQUEST if the object was no listener or data_change_notifier_t not initialized.
 */
data_error_t data_change_notifier_remove_listener ( data_change_notifier_t *this_, GObject *no_listener );

#endif  /* DATA_CHANGE_NOTIFIER_H */


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
