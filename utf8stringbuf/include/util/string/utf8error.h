#ifndef UTF8ERROR_H_
#define UTF8ERROR_H_

/*!
 *  \file utf8error.h
 *  \brief utf8error is an enumeration of error constants.
 * 
 *  \note License: Use this code according to the license: Apache 2.0.
 *  \author (c) 2012-2016 A.Warnke; Email-contact: utf8stringbuf-at-andreaswarnke-dot-de
 */ 

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \enum utf8error_enum
 *
 *  Enumeration for success and error states. All errors are negative, positive values are reserved for 
 *  byte-lengths or index-values in case of success. 
 */
enum utf8error_enum { 
    UTF8ERROR_SUCCESS = 0, /*!< success, there was no error */
    UTF8ERROR_NOT_FOUND = -1, /*!< pattern not found */
    UTF8ERROR_NULL_PARAM = -2, /*!< NULL was provided as parameter instead of a valid pointer */
    UTF8ERROR_OUT_OF_RANGE = -3, /*!< some integer parameter was out of range */
    UTF8ERROR_TRUNCATED = -4, /*!< the resulting string did not fit into the buffer, the string was truncated */
    UTF8ERROR_NOT_A_CODEPOINT = -5, /*!< a codepoint was out of range: only 0x00000000 to 0x7fffffff are valid in utf8 */
};

/*!
 *  \typedef utf8error_t
 *  \brief Enumeration of error constants
 */
typedef enum utf8error_enum utf8error_t;

#ifdef __cplusplus
}
#endif

#endif /*UTF8ERROR_H_*/
