/* File: json_schema_writer.h; Copyright and License: see below */

#ifndef JSON_SCHEMA_WRITER_H
#define JSON_SCHEMA_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Writes a json schema to one file output stream.
 */

#include "u8stream/universal_output_stream.h"
#include "utf8stream/utf8stream_writer.h"

/*!
 *  \brief attributes of the json schema writer
 */
struct json_schema_writer_struct {
    utf8stream_writer_t writer;  /*!< an own universal_output_stream_t, wrapping the *output */
};

typedef struct json_schema_writer_struct json_schema_writer_t;

/*!
 *  \brief initializes the json schema writer
 *
 *  \param this_ pointer to own object attributes
 *  \param output output stream where to write the generated output to
 */
void json_schema_writer_init( json_schema_writer_t *this_, universal_output_stream_t *output );

/*!
 *  \brief destroys the json schema writer
 *
 *  \param this_ pointer to own object attributes
 */
void json_schema_writer_destroy( json_schema_writer_t *this_ );

/*!
 *  \brief writes a json schema file
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t json_schema_writer_write_schema( json_schema_writer_t *this_ );

/*!
 *  \brief writes a json member declaration of type number
 *
 *  \param this_ pointer to own object attributes
 *  \param indent indentation of the member declaration in the json schema file
 *  \param name name of the member to declare
 *  \param description description of the member
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t json_schema_writer_declare_number( json_schema_writer_t *this_,
                                              const char* indent,
                                              const char* name,
                                              const char* description
                                            );

/*!
 *  \brief writes a json member declaration of type string
 *
 *  \param this_ pointer to own object attributes
 *  \param indent indentation of the member declaration in the json schema file
 *  \param name name of the member to declare
 *  \param description description of the member
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t json_schema_writer_declare_string( json_schema_writer_t *this_,
                                              const char* indent,
                                              const char* name,
                                              const char* description
                                            );

#endif  /* JSON_SCHEMA_WRITER_H */


/*
Copyright 2023-2023 Andreas Warnke

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
