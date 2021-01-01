/* File: data_change_notifier.h; Copyright and License: see below */

#ifndef DATA_CHANGE_NOTIFIER_H
#define DATA_CHANGE_NOTIFIER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Notifies on every change to the database.
 */

#include "storage/data_change_event_type.h"
#include "data_error.h"
#include "data_table.h"
#include "data_row_id.h"
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
 *  \param event_type the event_type which causes the signal: created, updated or deleted
 *  \param table the table in which a row was created, updated or deleted
 *  \param row_id the row_id which was created, updated or deleted
 *  \param parent_table the table of the parent element.
 *                      DATA_TABLE_VOID if ((table==DATA_TABLE_CLASIFIER)||(event_type==DATA_CHANGE_EVENT_TYPE_UPDATE))
 *                      because classifiers have no parent and in case of update, no parent is needed.
 *  \param parent_row_id the row_id of the parent element
 *                      DATA_ROW_ID_VOID if ((table==DATA_TABLE_CLASIFIER)||(event_type==DATA_CHANGE_EVENT_TYPE_UPDATE))
 *                      because classifiers have no parent and in case of update, no parent is needed.
 */
void data_change_notifier_emit_signal ( data_change_notifier_t *this_,
                                        data_change_event_type_t event_type,
                                        data_table_t table,
                                        data_row_id_t row_id,
                                        data_table_t parent_table,
                                        data_row_id_t parent_row_id
                                      );

/*!
 *  \brief notifies on changes to the database without providing a parent
 *
 *  This method id applicable to all changes
 *  where a message receiver can determine if to invalidate/reload caches
 *  by the row_id alone.
 *
 *  \param this_ pointer to own object attributes
 *  \param event_type the event_type which causes the signal: created, updated or deleted
 *  \param table the table in which a row was created, updated or deleted
 *  \param row_id the row_id which was created, updated or deleted
 */
static inline void data_change_notifier_emit_signal_without_parent ( data_change_notifier_t *this_,
                                                                     data_change_event_type_t event_type,
                                                                     data_table_t table,
                                                                     data_row_id_t row_id
                                                                   );

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

#include "storage/data_change_notifier.inl"

#endif  /* DATA_CHANGE_NOTIFIER_H */


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
