#ifndef UTF8STRINGNTUPLE_H_
#define UTF8STRINGNTUPLE_H_

/*!
 *  \file utf8stringntuple.h
 *  \brief utf8stringNTuple provides types for pairs, triples and n-tuples of c-strings.
 * 
 *  \note License: Use this code according to the license: Apache 2.0.
 *  \author (c) 2012-2016 A.Warnke; Email-contact: utf8stringbuf-at-andreaswarnke-dot-de
 */ 

#include "util/string/utf8error.h"

#ifdef __cplusplus
extern "C" {
#endif
    
/*! 
 *  \brief A pair of standard utf8-encoded C-Strings. 
 */
struct utf8string2tuple_struct {
    char* first;        /*!< pointer to the first 0-terminated and utf8-encoded string */
    char* second;       /*!< pointer to the second 0-terminated and utf8-encoded string */
    utf8error_t error;  /*!< the error code of the split function. See \link utf8stringbuf_split_in_2( utf8stringbuf_t, int ) utf8stringbuf_split_in_2 \endlink. */
};

/*!
 *  \typedef utf8string2tuple_t
 *  \brief A pair of standard utf8-encoded C-Strings. 
 */
typedef struct utf8string2tuple_struct utf8string2tuple_t;

/*! 
 *  \brief A triple of standard utf8-encoded C-Strings. 
 */
struct utf8string3tuple_struct {
    char* first;        /*!< pointer to the first 0-terminated and utf8-encoded string */
    char* second;       /*!< pointer to the second 0-terminated and utf8-encoded string */
    char* third;        /*!< pointer to the third 0-terminated and utf8-encoded string */
    utf8error_t error;  /*!< the error code of the split function. See \link utf8stringbuf_split_in_3( utf8stringbuf_t, int, int ) utf8stringbuf_split_in_3 \endlink. */
};

/*!
 *  \typedef utf8string3tuple_t
 *  \brief A triple of standard utf8-encoded C-Strings. 
 */
typedef struct utf8string3tuple_struct utf8string3tuple_t;

/*! 
 *  \brief A quadruple of standard utf8-encoded C-Strings. 
 */
struct utf8string4tuple_struct {
    char* first;        /*!< pointer to the first 0-terminated and utf8-encoded string */
    char* second;       /*!< pointer to the second 0-terminated and utf8-encoded string */
    char* third;        /*!< pointer to the third 0-terminated and utf8-encoded string */
    char* fourth;       /*!< pointer to the fourth 0-terminated and utf8-encoded string */
    utf8error_t error;  /*!< the error code of the split function. See \link utf8stringbuf_split_in_4( utf8stringbuf_t, int, int, int ) utf8stringbuf_split_in_4 \endlink. */
};

/*!
 *  \typedef utf8string4tuple_t
 *  \brief A quadruple of standard utf8-encoded C-Strings. 
 */
typedef struct utf8string4tuple_struct utf8string4tuple_t;

/*! 
 *  \brief A quintuple of standard utf8-encoded C-Strings. 
 */
struct utf8string5tuple_struct {
    char* first;        /*!< pointer to the first 0-terminated and utf8-encoded string */
    char* second;       /*!< pointer to the second 0-terminated and utf8-encoded string */
    char* third;        /*!< pointer to the third 0-terminated and utf8-encoded string */
    char* fourth;       /*!< pointer to the fourth 0-terminated and utf8-encoded string */
    char* fifth;        /*!< pointer to the fifth 0-terminated and utf8-encoded string */
    utf8error_t error;  /*!< the error code of the split function. See \link utf8stringbuf_split_in_5( utf8stringbuf_t, int, int, int, int ) utf8stringbuf_split_in_5 \endlink. */
};

/*!
 *  \typedef utf8string5tuple_t
 *  \brief A quintuple of standard utf8-encoded C-Strings. 
 */
typedef struct utf8string5tuple_struct utf8string5tuple_t;

#ifdef __cplusplus
}
#endif

#endif /*UTF8STRINGNTUPLE_H_*/
