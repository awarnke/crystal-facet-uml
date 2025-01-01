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
    unsigned int indent;  /*!< indentation level: 7 and 10 are supported */
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
 *  \brief writes a json member declaration of type integer (which converts to number)
 *
 *  \param this_ pointer to own object attributes
 *  \param name name of the member to declare
 *  \param description description of the member
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t json_schema_writer_private_declare_integer( json_schema_writer_t *this_,
                                                       const char* name,
                                                       const char* description
                                                     );

/*!
 *  \brief writes a json member declaration of type string
 *
 *  \param this_ pointer to own object attributes
 *  \param name name of the member to declare
 *  \param description description of the member
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t json_schema_writer_private_declare_string( json_schema_writer_t *this_,
                                                      const char* name,
                                                      const char* description
                                                    );

/*!
 *  \brief writes a json member declaration of type array of strings
 *
 *  \param this_ pointer to own object attributes
 *  \param name name of the member to declare
 *  \param description description of the member
 *  \param element_description description of the array element
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t json_schema_writer_private_declare_array_of_string( json_schema_writer_t *this_,
                                                               const char* name,
                                                               const char* description,
                                                               const char* element_description
                                                             );

/*!
 *  \brief writes a json member declaration of type uuid (which converts to string)
 *
 *  \param this_ pointer to own object attributes
 *  \param name name of the member to declare
 *  \param description description of the member
 *  \param has_next true if the member declaration shall end with a comma to allow a next member declaration
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t json_schema_writer_private_declare_uuid( json_schema_writer_t *this_,
                                                    const char* name,
                                                    const char* description,
                                                    bool has_next
                                                  );

#endif  /* JSON_SCHEMA_WRITER_H */


/*
Copyright 2023-2025 Andreas Warnke

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
