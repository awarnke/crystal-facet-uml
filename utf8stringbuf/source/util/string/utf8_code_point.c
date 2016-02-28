/*!
 *  \file utf8_code_point.c
 * 
 *  \note License: Use this code according to the license: Apache 2.0.
 *  \author (c) 2012-2016 A.Warnke; Email-contact: utf8stringbuf-at-andreaswarnke-dot-de
 */ 

#include "util/string/utf8_code_point.h"

/* UTF8_CODE_POINT_INVAL_CHAR is a constant, may be stored in a read-only memory page */
const utf8_code_point_t UTF8_CODE_POINT_INVAL_CHAR = { UTF8_CODE_POINT_INVALID_LEN, 0x0, };

/* function to initialize an code point from a multi-byte character, non-inline */
utf8_code_point_t utf8_code_point_private_init_from_multi_byte( const char *that, unsigned int max_size ) {
    utf8_code_point_t result = { UTF8_CODE_POINT_INVALID_LEN, 0x0, };
    if ( max_size >= 1 ) {
        unsigned char firstByte = (unsigned char) (that[0]);
        unsigned int byte_length = 
            ( firstByte < 0xe0 ) 
            ? (
                    ( firstByte < 0xc0 ) ? 1 : 2
            )
            : (
                    ( firstByte < 0xf8 )
                    ? (
                            ( firstByte < 0xf0 ) ? 3 : 4
                    )
                    : (
                            ( firstByte < 0xfc ) ? 5 : 6
                    )
            );
        switch ( byte_length ) {
        case 1:
            if ( firstByte < 0x80 ) {
                /* first byte is valid */
                result.byte_length = 1;
                result.code_point = firstByte;
            }
            break;
        case 2:
            if ( max_size >= 2 ) {
                unsigned char secondByte = (unsigned char) (that[1]);
                if (( 0xc0 & secondByte ) == 0x80 ) {
                    /* second byte is valid */
                    result.byte_length = 2;
                    result.code_point = (((uint32_t)(firstByte & 0x1f))<<6)
                                       |(secondByte & 0x3f); 
                }
            }
            break;
        case 3:
            if ( max_size >= 3 ) {
                unsigned char secondByte = (unsigned char) (that[1]);
                unsigned char thirdByte = (unsigned char) (that[2]);
                if (( ( 0xc0 & secondByte ) == 0x80 )
                    && ( ( 0xc0 & thirdByte ) == 0x80 ))
                {
                    /* second and third bytes are valid */
                    result.byte_length = 3;
                    result.code_point = (((uint32_t)(firstByte & 0x0f))<<12)
                                       |(((uint32_t)(secondByte & 0x3f))<<6)
                                       |(thirdByte & 0x3f); 
                }
            }
            break;
        case 4:
            if ( max_size >= 4 ) {
                unsigned char secondByte = (unsigned char) (that[1]);
                unsigned char thirdByte = (unsigned char) (that[2]);
                unsigned char fourthByte = (unsigned char) (that[3]);
                if (( ( 0xc0 & secondByte ) == 0x80 )
                    && ( ( 0xc0 & thirdByte ) == 0x80 ) 
                    && ( ( 0xc0 & fourthByte ) == 0x80 ))
                {
                    /* second, third and fourth bytes are valid */
                    result.byte_length = 4;
                    result.code_point = (((uint32_t)(firstByte & 0x07))<<18)
                                       |(((uint32_t)(secondByte & 0x3f))<<12)
                                       |(((uint32_t)(thirdByte & 0x3f))<<6)
                                       |(fourthByte & 0x3f); 
                }
            }
            break;
        case 5:
            if ( max_size >= 5 ) {
                unsigned char secondByte = (unsigned char) (that[1]);
                unsigned char thirdByte = (unsigned char) (that[2]);
                unsigned char fourthByte = (unsigned char) (that[3]);
                unsigned char fifthByte = (unsigned char) (that[4]);
                if (( ( 0xc0 & secondByte ) == 0x80 )
                    && ( ( 0xc0 & thirdByte ) == 0x80 ) 
                    && ( ( 0xc0 & fourthByte ) == 0x80 )
                    && ( ( 0xc0 & fifthByte ) == 0x80 ))
                {
                    /* second, third, fourth and fifth bytes are valid */
                    result.byte_length = 5;
                    result.code_point = (((uint32_t)(firstByte & 0x03))<<24)
                                       |(((uint32_t)(secondByte & 0x3f))<<18)
                                       |(((uint32_t)(thirdByte & 0x3f))<<12)
                                       |(((uint32_t)(fourthByte & 0x3f))<<6)
                                       |(fifthByte & 0x3f); 
                }
            }
            break;
        case 6:
            if ( max_size >= 6 ) {
                unsigned char secondByte = (unsigned char) (that[1]);
                unsigned char thirdByte = (unsigned char) (that[2]);
                unsigned char fourthByte = (unsigned char) (that[3]);
                unsigned char fifthByte = (unsigned char) (that[4]);
                unsigned char sixthByte = (unsigned char) (that[5]);
                if (( ( 0xc0 & secondByte ) == 0x80 )
                    && ( ( 0xc0 & thirdByte ) == 0x80 ) 
                    && ( ( 0xc0 & fourthByte ) == 0x80 )
                    && ( ( 0xc0 & fifthByte ) == 0x80 )
                    && ( ( 0xc0 & sixthByte ) == 0x80 )
                    && ( ( 0xfe & firstByte ) == 0xfc ))
                {
                    /* second, third, fourth, fifth and sixth bytes are valid */
                    result.byte_length = 6;
                    result.code_point = (((uint32_t)(firstByte & 0x01))<<30)
                                       |(((uint32_t)(secondByte & 0x3f))<<24)
                                       |(((uint32_t)(thirdByte & 0x3f))<<18)
                                       |(((uint32_t)(fourthByte & 0x3f))<<12)
                                       |(((uint32_t)(fifthByte & 0x3f))<<6)
                                       |(sixthByte & 0x3f); 
                }
            }
            break;
        default:
            break;
        }
    }
    return result;
}

