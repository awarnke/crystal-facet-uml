/* File: gui_simple_message_param_nature.h; Copyright and License: see below */

#ifndef GUI_SIMPLE_MESSAGE_PARAM_NATURE_H
#define GUI_SIMPLE_MESSAGE_PARAM_NATURE_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Defines a list of message parameter natures
 */

/*!
 *  \brief enumeration on message parameter nature/meaning
 */
enum gui_simple_message_param_nature_enum {
    GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID,  /*!< the parameter has no meaning */
    GUI_SIMPLE_MESSAGE_PARAM_NATURE_NAME,  /*!< the parameter is a name of an entity */
    GUI_SIMPLE_MESSAGE_PARAM_NATURE_ERROR_EXPL,  /*!< the parameter is a english description of an error that occurred */
    GUI_SIMPLE_MESSAGE_PARAM_NATURE_QUANTITY,  /*!< the parameter is a number that states a quantity/count */
    GUI_SIMPLE_MESSAGE_PARAM_NATURE_STREAM_POS,  /*!< the parameter is a number that states a stream or string position */
    GUI_SIMPLE_MESSAGE_PARAM_NATURE_ELEMENT_STATS,  /*!< the parameter is a statistic on data elements */
};

typedef enum gui_simple_message_param_nature_enum gui_simple_message_param_nature_t;

#endif  /* GUI_SIMPLE_MESSAGE_PARAM_NATURE_H */


/*
Copyright 2020-2020 Andreas Warnke

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
