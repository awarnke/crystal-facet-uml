/* File: u8_u64.h; Copyright and License: see below */

#ifndef U8_U64_H
#define U8_U64_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides some int/string conversions for type uint_64
 */

#include <stdint.h>
#include <stddef.h>

enum u8_u64_max_enum {
    U8_U64_MAX_HEX_LENGTH = 16,  /*!< maximum length of an hexadecimal printed uint_64*/
    U8_U64_MAX_HEX_SIZE = 17,  /*!< maximum hexadecimal length of an uint_64, plus 1 termniating zero */
    U8_U64_MAX_DEC_LENGTH = 20,  /*!< maximum length of a decimal printed uint_64: 18,446,744,073,709,551,615 */
    U8_U64_MAX_DEC_SIZE = 21,  /*!< maximum decimal length of an uint_64, plus 1 termniating zero */
};

/*!
 *  \brief a character array to store a hexadecimal printed uint_64
 */
typedef char u8_u64_hex_t[U8_U64_MAX_HEX_SIZE];

/*!
 *  \brief a character array to store a decimal printed uint_64
 */
typedef char u8_u64_dec_t[U8_U64_MAX_DEC_SIZE];

/*!
 *  \brief gets the hexadecimal representation of an uint_64
 *
 *  \param value a number to render as hexadecimal
 *  \param[out] a character array, 0-terminated, containting the hexadecimal representation of value
 */
static inline void u8_u64_get_hex ( const uint64_t value, u8_u64_hex_t *out_hex );

/*!
 *  \brief gets the decimal representation of an uint_64
 *
 *  \param value a number to render as decimal
 *  \param[out] a character array, 0-terminated, containting the decimal representation of value
 */
static inline void u8_u64_get_dec ( const uint64_t value, u8_u64_dec_t *out_dec );

#include "u8/u8_u64.inl"

#endif  /* U8_U64_H */


/*
Copyright 2025-2026 Andreas Warnke

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
