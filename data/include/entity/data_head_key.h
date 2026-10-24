/* File: data_head_key.h; Copyright and License: see below */

#ifndef DATA_HEAD_KEY_H
#define DATA_HEAD_KEY_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines the keys of head values
 */

/* extern const char *const DATA_HEAD_KEY_DATA_FILE_FORMAT; */

/*!
 *  \brief a prefix for head parameters to distinguish internal, session-lifetime parameters
 *         from parameters that are stored to a file.
 */
extern const char *const DATA_HEAD_KEY_INTERNAL_PREFIX;

/*!
 *  \brief stores the filename (but not the file path) to the json file that is currently open
 */
extern const char *const DATA_HEAD_KEY_FILE_NAME;

/*!
 *  \brief stored the modification time (as hex) when the currently open database was last synched to json (ex- or import)
 */
extern const char *const DATA_HEAD_KEY_FILE_LAST_SYNC_MOD_TIME;

/*!
 *  \brief The major version of the storage file format
 */
extern const char *const DATA_HEAD_KEY_FILE_MAJOR_VERSION;

/*!
 *  \brief The minor version of the storage file format
 */
extern const char *const DATA_HEAD_KEY_FILE_MINOR_VERSION;

/*!
 *  \brief The model id prefix of the data model
 */
extern const char *const DATA_HEAD_KEY_MODEL_ID_PREFIX;

/*!
 *  \brief The model uuid of the data model. This is preserved over versions and branches.
 */
extern const char *const DATA_HEAD_KEY_MODEL_UUID;

#endif  /* DATA_HEAD_KEY_H */


/*
Copyright 2023-2026 Andreas Warnke

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
