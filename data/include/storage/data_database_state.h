/* File: data_database_State.h; Copyright and License: see below */

#ifndef DATA_DATABASE_STATE_H
#define DATA_DATABASE_STATE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides an enumeration for db states closed, open-in-memory, open
 */

/*!
 *  \brief enumeration for  closed, open-in-memory, open
 *
 *  This enumeration shall not reflect if the database is read only because this can not be evaluated reliably.
 *  (Read-only may be a temporary situation and may be similar to disk-full or access-rights-missing)
 */
enum data_database_state_enum {
    DATA_DATABASE_STATE_CLOSED,  /*!< no database is available */
    DATA_DATABASE_STATE_IN_MEM,  /*!< the database is open in memory only (e.g. for testing) */
    DATA_DATABASE_STATE_OPEN,  /*!< the database is a native sqlite3 file and is open */
};

typedef enum data_database_state_enum data_database_state_t;

#endif  /* DATA_DATABASE_STATE_H */


/*
Copyright 2022-2023 Andreas Warnke

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
