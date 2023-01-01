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
#include "storage/data_database_state.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <sqlite3.h>
#include <stdbool.h>
#include <glib.h>

/*!
 *  \brief a magic byte sequence by which an Sqlite 3 database file can be recognized
 */
extern const char DATA_DATABASE_SQLITE3_MAGIC[16];

/*!
 *  \brief constants of data_database_t
 */
enum data_database_max_enum {
    DATA_DATABASE_MAX_FILEPATH = 1024,  /*!< maximum length of filepath */
    DATA_DATABASE_MAX_LISTENERS = 20,  /*!< maximum listeners. Max 3 Windows and max 3 readers and 2 writers */
                                       /*!< and 1 controller = 16 -> 20 is sufficient */
};

/*!
 *  \brief all data attributes needed for the database functions
 */
struct data_database_struct {
    sqlite3 *db;
    data_change_notifier_t notifier;  /*!< sends notifications at every change to the database */

    GMutex private_lock; /*!< lock to ensure that db_file_name, is_open and listener_list are used by only one thread at a time */
    utf8stringbuf_t db_file_name;
    char private_db_file_name_buffer[DATA_DATABASE_MAX_FILEPATH];
    data_database_state_t db_state;
    uint_fast8_t transaction_recursion;  /*!< current transaction depth, 0 if no transaction active */

    data_database_listener_t *(listener_list[DATA_DATABASE_MAX_LISTENERS]);  /*!< array of db-file change listeners. */
                                                              /*!< Only in case of a changed db-file, listeners are informed. */
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
 *  It is not allowed to open an already opened database.
 *
 *  Note that this function opens a database writeable or in read-only mode if the file in read-only.
 *
 *  \param this_ pointer to own object attributes
 *  \param db_file_path a relative or absolute file path
 *  \return U8_ERROR_READ_ONLY_DB or U8_ERROR_AT_DB if the db cannot be initialized,
 *          U8_ERROR_NO_DB if the db file cannot be created (e.g. read only parent directory)
 *          U8_ERROR_INVALID_REQUEST if a previous database was not closed before,
 *          U8_ERROR_NONE in case of success
 */
static inline u8_error_t data_database_open ( data_database_t *this_, const char* db_file_path );

/*!
 *  \brief opens a database file in read only mode
 *
 *  Useful for exporting data.
 *
 *  Note that this function opens even a possibly writeable file in read-only mode.
 *
 *  \param this_ pointer to own object attributes
 *  \param db_file_path a relative or absolute file path
 *  \return U8_ERROR_READ_ONLY_DB or U8_ERROR_AT_DB if the db cannot be initialized,
 *          U8_ERROR_NO_DB if the db file cannot be created (e.g. read only parent directory)
 *          U8_ERROR_INVALID_REQUEST if a previous database was not closed before,
 *          U8_ERROR_NONE in case of success
 */
static inline u8_error_t data_database_open_read_only ( data_database_t *this_, const char* db_file_path );

/*!
 *  \brief opens a new database in memory
 *
 *  Creates a new datanase in memory for testing purposes
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_READ_ONLY_DB or U8_ERROR_AT_DB if the db cannot be initialized,
 *          U8_ERROR_INVALID_REQUEST if a previous database was not closed before,
 *          U8_ERROR_NONE in case of success
 */
static inline u8_error_t data_database_open_in_memory ( data_database_t *this_ );

/*!
 *  \brief closes the current database file
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_close ( data_database_t *this_ );

/*!
 *  \brief destroys the data_database_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_database_destroy ( data_database_t *this_ );

/*!
 *  \brief opens a database file
 *
 *  \param this_ pointer to own object attributes
 *  \param db_file_path a relative or absolute file path
 *  \param sqlite3_flags sqlite3 flags as required for sqlite3_open_v2()
 *  \return U8_ERROR_NO_DB or U8_ERROR_AT_DB if file cannot be opened,
 *          U8_ERROR_NONE in case of success
 */
u8_error_t data_database_private_open ( data_database_t *this_, const char* db_file_path, int sqlite3_flags );

/*!
 *  \brief returns a pointer to the sqlite database
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to the sqlite database
 */
static inline sqlite3 *data_database_get_database_ptr ( data_database_t *this_ );

/*!
 *  \brief flushes caches of the current database
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_DB in case of an error
 */
u8_error_t data_database_flush_caches ( data_database_t *this_ );

/*!
 *  \brief prints statistics of the current database file to the trace output
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_trace_stats ( data_database_t *this_ );

/*!
 *  \brief returns the database filename
 *
 *  \param this_ pointer to own object attributes
 *  \return NULL if no database is open, the filename otherwise
 */
static inline const char *data_database_get_filename_ptr ( data_database_t *this_ );

/*!
 *  \brief returns a pointer to the data_change_notifier_t to be used to send notifications
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to the data_change_notifier_t
 */
static inline data_change_notifier_t *data_database_get_notifier_ptr ( data_database_t *this_ );

/*!
 *  \brief initializes the tables in the database if not yet existant
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_AT_DB if the current database is not a database or is encrypted; U8_ERROR_READ_ONLY_DB if read only
 */
u8_error_t data_database_private_initialize_tables( data_database_t *this_ );

/*!
 *  \brief initializes the indexes in the database if not yet existant
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_AT_DB in case of an error; U8_ERROR_READ_ONLY_DB if read only
 */
u8_error_t data_database_private_initialize_indexes( data_database_t *this_ );

/*!
 *  \brief upgrades old tables from older versions to current database scheme
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_AT_DB if the current database is not a database or is encrypted; U8_ERROR_READ_ONLY_DB if read only
 */
u8_error_t data_database_private_upgrade_tables( data_database_t *this_ );

/*!
 *  \brief adds a db-file changed listener to the database
 *
 *  \param this_ pointer to own object attributes
 *  \param listener pointer to a listener to be added; the referenced object needs to stay valid till removal
 *  \return U8_ERROR_ARRAY_BUFFER_EXCEEDED if max listeners reached, U8_ERROR_INVALID_REQUEST if listener already registered,
 *          U8_ERROR_NONE otherwise.
 */
u8_error_t data_database_add_db_listener( data_database_t *this_, data_database_listener_t *listener );

/*!
 *  \brief removes a db-file changed listener from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param listener pointer to a listener to be removed
 *  \return U8_ERROR_INVALID_REQUEST if listener was not registered, U8_ERROR_NONE otherwise.
 */
u8_error_t data_database_remove_db_listener( data_database_t *this_, data_database_listener_t *listener );

/*!
 *  \brief clears the db-file changed listerner array
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_database_private_clear_db_listener_list( data_database_t *this_ );

/*!
 *  \brief notifies all db-file changed listerners
 *
 *  \param this_ pointer to own object attributes
 *  \param signal_id one of DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE and DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED,
 *                   depending on the reason for this notification
 *  \return U8_ERROR_NONE in case of success.
 */
u8_error_t data_database_private_notify_db_listeners( data_database_t *this_, data_database_listener_signal_t signal_id );

/*!
 *  \brief gets a lock to protect data in data_database_t from concurrent access.
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
static inline u8_error_t data_database_private_lock ( data_database_t *this_ );

/*!
 *  \brief releases the lock.
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
static inline u8_error_t data_database_private_unlock ( data_database_t *this_ );

/*!
 *  \brief checks if the database file is open
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the database file is open
 */
static inline bool data_database_is_open( data_database_t *this_ );

/*!
 *  \brief executes a "BEGIN TRANSACTION" command.
 *
 *  This function may be called recursively.
 *
 *  This function does not care about locks. It does not sent notifications.
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, an error id otherwise, e.g. U8_ERROR_NO_DB in case the database is not open
 */
u8_error_t data_database_transaction_begin ( data_database_t *this_ );

/*!
 *  \brief executes a "COMMIT TRANSACTION" command
 *
 *  This function may be called recursively.
 *  The commit is only executed when this function is called once for each preceding data_database_transaction_begin call.
 *
 *  This function does not care about locks. It does not sent notifications.
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, an error id otherwise
 */
u8_error_t data_database_transaction_commit ( data_database_t *this_ );

/*!
 *  \brief checks if the database file is open and executes an sql statement
 *
 *  \param this_ pointer to own object attributes
 *  \param sql_command the sqk statement to execute
 *  \param ignore_errors if true, no errors are printed to syslog
 *  \return U8_ERROR_READ_ONLY_DB if read oly, U8_ERROR_AT_DB if other error, U8_ERROR_NONE if no error
 */
static inline u8_error_t data_database_private_exec_sql( data_database_t *this_, const char* sql_command, bool ignore_errors );


#include "storage/data_database.inl"

#endif  /* DATA_DATABASE_H */


/*
Copyright 2016-2023 Andreas Warnke

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
