/* File: draw_line_breaker.h; Copyright and License: see below */

#ifndef DRAW_LINE_BREAKER_H
#define DRAW_LINE_BREAKER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Inserts zero-width spaces (U+200B) at possible line break positions.
 *
 *  This allows the pango library to break lines at more suitable positions
 */

#include "utf8stringbuf/utf8stringbuf.h"
#include "utf8stringbuf/utf8stringview.h"
#include <stdbool.h>

/*!
 *  \brief attributes of the line breaker
 */
struct draw_line_breaker_struct {
    bool more;  /*!< true if more possible line breaks shall be inserted */
};

typedef struct draw_line_breaker_struct draw_line_breaker_t;

/*!
 *  \brief initializes the draw_line_breaker_t
 *
 *  \param this_ pointer to own object attributes
 *  \param more find more possible line breaks
 */
static inline void draw_line_breaker_init( draw_line_breaker_t *this_, bool more );

/*!
 *  \brief destroys the draw_line_breaker_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void draw_line_breaker_destroy( draw_line_breaker_t *this_ );

/*!
 *  \brief inserts zero-width spaces (U+200B) at possible line break positions.
 *
 *  \param this_ pointer to own object attributes
 *  \param in_text label, title or description that shall be processed
 *  \param out_text stream where the result shall be written to
 *  \return U8ERROR_NONE in case of success
 */
u8_error_t draw_line_breaker_append ( const draw_line_breaker_t *this_,
                                      const utf8stringview_t *in_text,
                                      utf8stream_writer_t *out_text
                                    );

#include "draw_line_breaker.inl"

#endif  /* DRAW_LINE_BREAKER_H */


/*
Copyright 2024-2024 Andreas Warnke

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
