/* File: io_filter_flag.h; Copyright and License: see below */

#ifndef IO_FILTER_FLAG_H
#define IO_FILTER_FLAG_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines the filters to apply when exporting specific formats
 */

/*!
 *  \brief export filter to apply when exporting data
 */
enum io_filter_flag_enum {
    IO_FILTER_FLAG_NONE = 0x0000,  /*!< no objects shall be filtered */
    IO_FILTER_FLAG_DUPLICATES = 0x0001,  /*!< duplicate relationships and classifiers shall be filtered */
    IO_FILTER_FLAG_LIFELINES = 0x0010,  /*!< lifelines shall be filtered */
    IO_FILTER_FLAG_HIDDEN = 0x0008,  /*!< hidden features shall shall be filtered */
};

typedef enum io_filter_flag_enum io_filter_flag_t;

#endif  /* IO_FILTER_FLAG_H */


/*
Copyright 2016-2020 Andreas Warnke

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
