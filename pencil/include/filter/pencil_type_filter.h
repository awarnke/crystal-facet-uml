/* File: pencil_type_filter.h; Copyright and License: see below */

#ifndef PENCIL_TYPE_FILTER_H
#define PENCIL_TYPE_FILTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines filter for of objects
 */

/*!
 *  \brief enumeration of filter types
 */
enum pencil_type_filter_enum {
    PENCIL_TYPE_FILTER_NONE,  /*!< all objects pass the filter */
    PENCIL_TYPE_FILTER_LIFELINE,  /*!< all objects except lifeline-features pass the filter */
};

typedef enum pencil_type_filter_enum pencil_type_filter_t;

#endif  /* PENCIL_TYPE_FILTER_H */


/*
Copyright 2019-2024 Andreas Warnke

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
