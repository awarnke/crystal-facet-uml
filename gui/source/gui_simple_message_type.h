/* File: gui_simple_message_type.h; Copyright and License: see below */

#ifndef GUI_SIMPLE_MESSAGE_TYPE_H
#define GUI_SIMPLE_MESSAGE_TYPE_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Defines a list of message types
 */

/*!
 *  \brief enumeration on message types
 */
enum gui_simple_message_type_enum {
    GUI_SIMPLE_MESSAGE_TYPE_NO_MESSAGE,
    GUI_SIMPLE_MESSAGE_TYPE_INFO,
    GUI_SIMPLE_MESSAGE_TYPE_WARNING,
    GUI_SIMPLE_MESSAGE_TYPE_ERROR,
    GUI_SIMPLE_MESSAGE_TYPE_ABOUT,
};

typedef enum gui_simple_message_type_enum gui_simple_message_type_t;

#endif  /* GUI_SIMPLE_MESSAGE_TYPE_H */


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
