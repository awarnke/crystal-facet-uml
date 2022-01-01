/* File: data_change_message.h; Copyright and License: see below */

#ifndef DATA_CHANGE_MESSAGE_H
#define DATA_CHANGE_MESSAGE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief The message attached to notifications on every change to the database.
 */

#include "storage/data_change_event_type.h"
#include "data_id.h"
#include "data_row_id.h"
#include "data_table.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes send when notifying changes to listeners
 */
struct data_change_message_struct {
    data_change_event_type_t event;
    data_id_t modified;
    data_id_t parent;
};

typedef struct data_change_message_struct data_change_message_t;

/*!
 *  \brief initializes the data_change_message_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param event event type that caused the notification
 *  \param modified source object id that caused the notification.
 *                  This is a void object for database open/close events.
 *  \param parent parent id of the modified source object that caused the notification.
 *                For created/deleted diagrams, this is the parent diagram,
 *                for created/deleted diagramelements, this is the surrounding diagram,
 *                For created/deleted classifiers, this is a void object,
 *                For created/deleted relationships, this is the from_classifier,
 *                For created/deleted features, this is the classifier to which the feature is attached.
 *                If an existing record was updated only, parent is void.
 */
static inline void data_change_message_init ( data_change_message_t *this_,
                                              data_change_event_type_t event,
                                              data_id_t modified,
                                              data_id_t parent
                                            );

/*!
 *  \brief re-initializes the data_change_message_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param event event type that caused the notification
 *  \param modified source object id that caused the notification
 *  \param parent parent id of the modified source object that caused the notification.
 */
static inline void data_change_message_reinit ( data_change_message_t *this_,
                                                data_change_event_type_t event,
                                                data_id_t modified,
                                                data_id_t parent
                                              );

/*!
 *  \brief destroys the data_change_message_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_change_message_destroy ( data_change_message_t *this_ );

/*!
 *  \brief gets the value of attribute event
 *
 *  \param this_ pointer to own object attributes
 *  \return value of attribute event
 */
static inline data_change_event_type_t data_change_message_get_event ( const data_change_message_t *this_ );

/*!
 *  \brief gets the value of attribute modified
 *
 *  \param this_ pointer to own object attributes
 *  \return value of attribute modified
 */
static inline data_id_t data_change_message_get_modified ( const data_change_message_t *this_ );

/*!
 *  \brief gets the value of attribute parent
 *
 *  \param this_ pointer to own object attributes
 *  \return value of attribute parent
 */
static inline data_id_t data_change_message_get_parent ( const data_change_message_t *this_ );

/*!
 *  \brief prints the data_change_message_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_change_message_trace ( const data_change_message_t *this_ );

#include "storage/data_change_message.inl"

#endif  /* DATA_CHANGE_MESSAGE_H */


/*
Copyright 2018-2022 Andreas Warnke

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
