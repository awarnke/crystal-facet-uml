/* File: gui_error.h; Copyright and License: see below */

#ifndef GUI_ERROR_H
#define GUI_ERROR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Notifies errors in the gui module
 */

#include "u8/u8_error.h"

/*!
 *  \brief error constants which explain errors which occurred in the gui module
 */
enum gui_error_enum {
    GUI_ERROR_NONE = 0,  /*!< 0: success */
    GUI_ERROR_OUT_OF_BOUNDS = U8_ERROR_PARAM_OUT_OF_RANGE,  /*!< input parameters like coordinates are out of bounds */
    GUI_ERROR_UNKNOWN_OBJECT = U8_ERROR_NOT_FOUND,  /*!< referenced object id is not in current diagram */
};

typedef enum gui_error_enum gui_error_t;

#endif  /* GUI_ERROR_H */


/*
Copyright 2018-2026 Andreas Warnke

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
