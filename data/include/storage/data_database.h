/* File: data_database.h; Copyright and License: see below */

#ifndef DATA_DATABASE_H
#define DATA_DATABASE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Opens and closes a database.
 */

#include "storage/data_database_listener.h"
#include "storage/data_change_notifier.h"
#include "util/string/utf8stringbuf.h"
#include <sqlite3.h>
#include <stdbool.h>

/*!
 *  \brief constants of data_database_t
 */
enum data_database_max_enum {
    GUI_DATABASE_MAX_FILEPATH = 1024,  /*!< maximum length of filepath */
    GUI_DATABASE_MAX_LISTENERS = 20,  /*!< maximum listeners. Max 4 Windows and max 3 readers and 2 writers -> 20 is sufficient */
};

/*!
 *  \brief all data attributes needed for the database functions
 */
struct data_database_struct {
    sqlite3 *db;
    utf8stringbuf_t db_file_name;
    char private_db_file_name_buffer[GUI_DATABASE_MAX_FILEPATH];
    bool is_open;
    data_change_notifier_t notifier;  /*!< sends notifications at changes to the database */
    data_database_listener_t *(listener_list[GUI_DATABASE_MAX_LISTENERS]);  /*!< array of database change listeners */
};

typedef struct data_database_struct data_database_t;

/*!
 *  \brief initializes the data_database_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_database_init ( data_database_t *this_ );

/*!
 *  \brief opens a database file
 *
 *  \param this_ pointer to own object attributes
 */
data_error_t data_database_open ( data_database_t *this_, const char* db_file_path );

/*!
 *  \brief closes the current database file
 *
 *  \param this_ pointer to own object attributes
 */
data_error_t data_database_close ( data_database_t *this_ );

/*!
 *  \brief destroys the data_database_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_database_destroy ( data_database_t *this_ );

/*!
 *  \brief returns a pointer to the sqlite database
 *
 *  \param this_ pointer to own object attributes
 */
static inline sqlite3 *data_database_get_database_ptr ( data_database_t *this_ );

/*!
 *  \brief returns the database filename
 *
 *  \param this_ pointer to own object attributes
 *  \return NULL if no database is open or the filename
 */
static inline const char *data_database_get_filename_ptr ( data_database_t *this_ );

/*!
 *  \brief returns a pointer to the data_change_notifier_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline data_change_notifier_t *data_database_get_notifier_ptr ( data_database_t *this_ );

/*!
 *  \brief initializes the tables in the database if not yet existant
 */
static void data_database_private_initialize_tables( sqlite3 *db );

/*!
 *  \brief adds a db-file changed listener to the database
 *
 *  \param this_ pointer to own object attributes
 *  \param listener pointer to a listener to be added; the referenced object needs to stay valid till removal
 *  \return DATA_ERROR_ARRAY_BUFFER_EXCEEDED if max listeners reached, DATA_ERROR_NONE otherwise.
 */

data_error_t data_database_add_db_listener( data_database_t *this_, data_database_listener_t *listener );

/*!
 *  \brief removes a db-file changed listener to the database
 *
 *  \param this_ pointer to own object attributes
 *  \param listener pointer to a listener to be removed
 *  \return DATA_ERROR_INVALID_REQUEST if listener was not registered, DATA_ERROR_NONE otherwise.
 */
data_error_t data_database_remove_db_listener( data_database_t *this_, data_database_listener_t *listener );

/*!
 *  \brief clears the db-file changed listerner array
 */
static void data_database_private_clear_db_listener_list( data_database_t *this_ );

/*!
 *  \brief notifies all db-file changed listerners
 */
static void data_database_private_notify_db_listeners( data_database_t *this_, data_database_listener_signal_t signal_id );

#include "storage/data_database.inl"

#endif  /* DATA_DATABASE_H */


/*
Copyright 2016-2016 Andreas Warnke

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
