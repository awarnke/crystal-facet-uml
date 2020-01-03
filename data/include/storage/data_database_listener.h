/* File: data_database_listener.h; Copyright and License: see below */

#ifndef DATA_DATABASE_LISTENER_H
#define DATA_DATABASE_LISTENER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines an interface that is called on database switches.
 */

#include "storage/data_database_listener_signal.h"

/*!
 *  \brief attributes of an data_database_listener_t
 */
struct data_database_listener_struct {
    void *listener_instance;
    void (*listener_callback)(void* listener_instance, data_database_listener_signal_t signal_id);
};

typedef struct data_database_listener_struct data_database_listener_t;

/*!
 *  \brief initializes the data_database_listener_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param listener_instance a pointer to data that is passed to the callback function as first parameter
 *  \param listener_callback a callback function. Simply dereference the function name, omit parameters and add a typecast, e.g. (void (*)(void*,data_database_listener_signal_t)) &my_callback_function
 */
static inline void data_database_listener_init ( data_database_listener_t *this_,
                                                 void *listener_instance,
                                                 void (*listener_callback)(void* listener_instance, data_database_listener_signal_t signal_id)
                                               );

/*!
 *  \brief destroys the data_database_listener_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_database_listener_destroy ( data_database_listener_t *this_ );

/*!
 *  \brief synchronously calls a callback function at the data_database_listener_t
 *
 *  \param this_ pointer to own object attributes
 *  \param signal_id an id that is passed to the callback function as second parameter
 */
static inline void data_database_listener_notify ( data_database_listener_t *this_, data_database_listener_signal_t signal_id );

#include "data_database_listener.inl"

#endif  /* DATA_DATABASE_LISTENER_H */


/*
Copyright 2016-2020 Andreas Warnke

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
