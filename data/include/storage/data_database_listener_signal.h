/* File: data_database_listener_signal.h; Copyright and License: see below */

#ifndef DATA_DATABASE_LISTENER_SIGNAL_H
#define DATA_DATABASE_LISTENER_SIGNAL_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Declares signal-ids for closing and opening the database.
 */

/*!
 *  \brief signal ids
 */
enum data_database_listener_signal_enum {
    DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE = 1,  /*!< send before the currently active db closes */
    DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED = 2,  /*!< send after a new db has been opened */
};

typedef enum data_database_listener_signal_enum data_database_listener_signal_t;

#endif  /* DATA_DATABASE_LISTENER_SIGNAL_H */


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
