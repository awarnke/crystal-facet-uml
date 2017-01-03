/* File: gui_simple_message_content.h; Copyright and License: see below */

#ifndef GUI_SIMPLE_MESSAGE_CONTENT_H
#define GUI_SIMPLE_MESSAGE_CONTENT_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Defines a list of message text-ids
 */

/*!
 *  \brief enumeration on message text-ids
 */
enum gui_simple_message_content_enum {
    GUI_SIMPLE_MESSAGE_CONTENT_ABOUT,  /*!< shows the about text */
    GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_NOT_OPENED,  /*!< the chosen database file cannot be used */
    GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_OPENED_WITH_ERROR,  /*!< the chosen database file was opened but with a warning/error */
    GUI_SIMPLE_MESSAGE_CONTENT_MAX_WINDOWS_ALREADY_OPEN,  /*!< the maximum number of windows is already open */
    GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED,  /*!< a stringlength is exceeded, a string was truncated */
    GUI_SIMPLE_MESSAGE_CONTENT_NO_SELECTION,  /*!< nothing is selected, action could not be performed */
    GUI_SIMPLE_MESSAGE_CONTENT_DELETING_NOT_POSSIBLE,  /*!< some objects could not be deleted because they are still referenced */
    GUI_SIMPLE_MESSAGE_CONTENT_NO_MORE_UNDO,  /*!< undo failed, nothing more to undo */
    GUI_SIMPLE_MESSAGE_CONTENT_UNDO_NOT_POSSIBLE,  /*!< undo failed, list bounds of undo exceeded */
    GUI_SIMPLE_MESSAGE_CONTENT_NO_MORE_REDO,  /*!< redo failed, nothing more to undo */
    GUI_SIMPLE_MESSAGE_CONTENT_NO_INPUT_DATA,  /*!< paste failed, no input data */
    GUI_SIMPLE_MESSAGE_CONTENT_INVALID_INPUT_DATA,  /*!< paste failed, parser error at input data */
    GUI_SIMPLE_MESSAGE_CONTENT_FILE_EXPORT_FAILED,  /*!< some or all diagrams could not be exported */
    GUI_SIMPLE_MESSAGE_CONTENT_NAME_NOT_UNIQUE,  /*!< object not created or name not changed */
    GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED,  /*!< files exported successfully */
    GUI_SIMPLE_MESSAGE_CONTENT_SET_PARTLY_UNSUITABLE,  /*!< operation cannot be performed on some elements in the set */

    GUI_SIMPLE_MESSAGE_CONTENT_NOT_YET_IMPLEMENTED,  /*!< the feature is not yet implemented */
};

typedef enum gui_simple_message_content_enum gui_simple_message_content_t;

#endif  /* GUI_SIMPLE_MESSAGE_CONTENT_H */


/*
Copyright 2016-2017 Andreas Warnke

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
