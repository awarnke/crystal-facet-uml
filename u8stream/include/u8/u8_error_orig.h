/* File: u8/u8_error_orig.h; Copyright and License: see below */

#ifndef U8_ERROR_ORIG_H
#define U8_ERROR_ORIG_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief  Declares a common error base type and common error origin flags
 */

/*!
 *  \brief error constants which list possible origins of errors
 *
 *  When calling a function and receiving an error, the caller typically has already an idea of what might have gone wrong.
 *  When e.g. opening a file fails, the file may not exist, access-rights may be limited or disk space is out.
 *  This enumeration provides a standard bit-pattern to distinguish these cases.
 *
 *  It is possible to bitwise-or multiple error-origins to collect errors over several statements,
 *  e.g. { strerr |= data_database_reader_get_classifier_by_name(...);
 *         strerr |= data_database_reader_get_diagrams_by_classifier_id(...); },
 *  and evaluate multiple errors only once at the end.
 */
enum u8_error_orig_enum {
    U8_ERROR_ORIG_NONE = 0x0000,  /*!< origin: unknown */
    /* == Origins on application level == */
    U8_ERROR_ORIG_TIME = 0x8000,  /*!< origin: timeouts, resume-after-suspend, timer expired */
    U8_ERROR_ORIG_SYNC = 0x4000,  /*!< origin: threads, mutexes, signals, scheduling, wrong-state */
    U8_ERROR_ORIG_MISS = 0x2000,  /*!< origin: missing resources on application level: key-in-dict, filename */
    U8_ERROR_ORIG_DATA = 0x1000,  /*!< origin: internal data, input-data, configuration-options, database, */
                                  /*!<         signatures, hashes, compression, encryption, parsing, streaming */
    /* == Origins on device level == */
    U8_ERROR_ORIG_MEMO = 0x0800,  /*!< origin: own memory, shared memory, null-pointer, memory-overwrites */
                                  /*!<    e.g. out of memory, buffer size exceeded */
    U8_ERROR_ORIG_FILE = 0x0400,  /*!< origin: file, file systems, block devices */
                                  /*!<    e.g. out of disk space, device removed, unsuitable file permissions */
    U8_ERROR_ORIG_NETW = 0x0200,  /*!< origin: network, peer-status, pipes, streams, messages */
                                  /*!<    e.g. interrupted connection, unexpected end-of-stream */
    U8_ERROR_ORIG_CDEV = 0x0100,  /*!< origin: character devices like console, serial line, shell-interactions */
                                  /*!<    e.g. connection-reset, no-data */
};

typedef enum u8_error_orig_enum u8_error_orig_t;

#endif  /* U8_ERROR_ORIG_H */


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
