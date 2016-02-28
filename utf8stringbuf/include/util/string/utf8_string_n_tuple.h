#ifndef UTF8_STRING_N_TUPLE_H_
#define UTF8_STRING_N_TUPLE_H_

/*!
 *  \file utf8_string_n_tuple.h
 *  \brief utf8_stringNTuple provides types for pairs, triples and n-tuples of c-strings.
 * 
 *  \note License: Use this code according to the license: Apache 2.0.
 *  \author (c) 2012-2016 A.Warnke; Email-contact: utf8stringbuf-at-andreaswarnke-dot-de
 */ 

#include "util/string/utf8_error.h"

#ifdef __cplusplus
extern "C" {
#endif
    
/*! 
 *  \brief A pair of standard utf8-encoded C-Strings. 
 */
struct utf8_string_2_tuple_struct {
    char* first;        /*!< pointer to the first 0-terminated and utf8-encoded string */
    char* second;       /*!< pointer to the second 0-terminated and utf8-encoded string */
    utf8_error_t error;  /*!< the error code of the split function. See \link utf8_string_buf_split_in_2( utf8_string_buf_t, int ) utf8_string_buf_split_in_2 \endlink. */
};

/*!
 *  \typedef utf8_string_2_tuple_t
 *  \brief A pair of standard utf8-encoded C-Strings. 
 */
typedef struct utf8_string_2_tuple_struct utf8_string_2_tuple_t;

/*! 
 *  \brief A triple of standard utf8-encoded C-Strings. 
 */
struct utf8_string_3_tuple_struct {
    char* first;        /*!< pointer to the first 0-terminated and utf8-encoded string */
    char* second;       /*!< pointer to the second 0-terminated and utf8-encoded string */
    char* third;        /*!< pointer to the third 0-terminated and utf8-encoded string */
    utf8_error_t error;  /*!< the error code of the split function. See \link utf8_string_buf_split_in_3( utf8_string_buf_t, int, int ) utf8_string_buf_split_in_3 \endlink. */
};

/*!
 *  \typedef utf8_string_3_tuple_t
 *  \brief A triple of standard utf8-encoded C-Strings. 
 */
typedef struct utf8_string_3_tuple_struct utf8_string_3_tuple_t;

/*! 
 *  \brief A quadruple of standard utf8-encoded C-Strings. 
 */
struct utf8_string_4_tuple_struct {
    char* first;        /*!< pointer to the first 0-terminated and utf8-encoded string */
    char* second;       /*!< pointer to the second 0-terminated and utf8-encoded string */
    char* third;        /*!< pointer to the third 0-terminated and utf8-encoded string */
    char* fourth;       /*!< pointer to the fourth 0-terminated and utf8-encoded string */
    utf8_error_t error;  /*!< the error code of the split function. See \link utf8_string_buf_split_in_4( utf8_string_buf_t, int, int, int ) utf8_string_buf_split_in_4 \endlink. */
};

/*!
 *  \typedef utf8_string_4_tuple_t
 *  \brief A quadruple of standard utf8-encoded C-Strings. 
 */
typedef struct utf8_string_4_tuple_struct utf8_string_4_tuple_t;

/*! 
 *  \brief A quintuple of standard utf8-encoded C-Strings. 
 */
struct utf8_string_5_tuple_struct {
    char* first;        /*!< pointer to the first 0-terminated and utf8-encoded string */
    char* second;       /*!< pointer to the second 0-terminated and utf8-encoded string */
    char* third;        /*!< pointer to the third 0-terminated and utf8-encoded string */
    char* fourth;       /*!< pointer to the fourth 0-terminated and utf8-encoded string */
    char* fifth;        /*!< pointer to the fifth 0-terminated and utf8-encoded string */
    utf8_error_t error;  /*!< the error code of the split function. See \link utf8_string_buf_split_in_5( utf8_string_buf_t, int, int, int, int ) utf8_string_buf_split_in_5 \endlink. */
};

/*!
 *  \typedef utf8_string_5_tuple_t
 *  \brief A quintuple of standard utf8-encoded C-Strings. 
 */
typedef struct utf8_string_5_tuple_struct utf8_string_5_tuple_t;

#ifdef __cplusplus
}
#endif

#endif /*UTF8_STRING_N_TUPLE_H_*/
