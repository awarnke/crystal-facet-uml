/* File: universal_output_stream_if.h; Copyright and License: see below */

#ifndef UNIVERSAL_OUTPUT_STREAM_IF_H
#define UNIVERSAL_OUTPUT_STREAM_IF_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a set of function pointers that allow to implement an output stream.
 *
 *  This is an interface only - to be used in a pipes-and-filters architecture
 *  where pipes and filters do not know their predecessor or successor.
 *
 *  A user of this interface needs a) a pointer to a concrete intance of this interface
 *  and b) a pointer to an object that implements the interface (this_).
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*!
 *  \brief function pointers of a universal_output_stream_if_t, similar to a vmt.
 *
 */
struct universal_output_stream_if_struct {
    int (*open)(void* this_, const char* identifier); /*!< a function to open an output stream; returns 0 if success, -1 in case of error */
    int (*write)(void* this_, const void *start, size_t length); /*!< a function to write data to an output stream; returns 0 if success, -1 in case of error */
    int (*flush)(void* this_); /*!< a function to flush data to an output stream; returns 0 if success, -1 in case of error */
    int (*close)(void* this_); /*!< a function to close an output stream; returns 0 if success, -1 in case of error */
    int (*destroy)(void* this_); /*!< the destructor of the output stream; returns 0 if success, -1 in case of error */
};

typedef struct universal_output_stream_if_struct universal_output_stream_if_t;

#endif  /* UNIVERSAL_OUTPUT_STREAM_IF_H */


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
