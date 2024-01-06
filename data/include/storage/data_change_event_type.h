/* File: data_change_event_type.h; Copyright and License: see below */

#ifndef DATA_CHANGE_EVENT_TYPE_H
#define DATA_CHANGE_EVENT_TYPE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides an enumeration for create, update, delete
 */

/*!
 *  \brief enumeration for create, update, delete
 */
enum data_change_event_type_enum {
    DATA_CHANGE_EVENT_TYPE_CREATE,  /*!< a database record was created */
    DATA_CHANGE_EVENT_TYPE_UPDATE,  /*!< a database record was updated */
    DATA_CHANGE_EVENT_TYPE_DELETE,  /*!< a database record was deleted */
    DATA_CHANGE_EVENT_TYPE_MULTI,  /*!< multiple database records have been created, uddated, deleted */
    DATA_CHANGE_EVENT_TYPE_DB_OPENED,  /*!< a new database file is opened */
    DATA_CHANGE_EVENT_TYPE_DB_PREPARE_CLOSE,  /*!< the current database file will be closed */
    DATA_CHANGE_EVENT_TYPE_DB_CLOSED,  /*!< the current database file is closed */
};

typedef enum data_change_event_type_enum data_change_event_type_t;

#endif  /* DATA_CHANGE_EVENT_TYPE_H */


/*
Copyright 2018-2024 Andreas Warnke

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
