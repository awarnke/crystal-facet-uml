/* File: utf8stringbuf_cross_language.cpp; Copyright and License: see below */

#include "utf8stringbuf_cross_language.h"

static char unitTestResultArr[512];
static utf8stringbuf_t unitTestResultBuf = UTF8STRINGBUF( unitTestResultArr );

static utf8stringbufCrossLanguage utf8StringBufCrossLanguageObject = utf8stringbufCrossLanguage();

utf8stringbuf_t utf8stringbuf_cross_language_create_stringbuf( const char* charSequence ) {
    return utf8StringBufCrossLanguageObject.CreateStringBuf( charSequence );
}

int utf8stringbuf_cross_language_check_stringbuf( utf8stringbuf_t buf, const char* charSequence ) {
    return utf8StringBufCrossLanguageObject.CheckStringBuf( buf, charSequence );
}

utf8stringbuf_t utf8stringbufCrossLanguage::CreateStringBuf( const char* charSequence ) {
    utf8stringbuf_clear( unitTestResultBuf );
    utf8stringbuf_append_str( unitTestResultBuf, charSequence );
    /* do some stuff simply to test some utf8stringbuf functions from cpp linkage context */
    {
        int len = utf8stringbuf_get_length( unitTestResultBuf );
        const int64_t someNumber = -98345769308475LL;
        utf8stringbuf_append_int( unitTestResultBuf, someNumber );
        int len2 = utf8stringbuf_find_last_str( unitTestResultBuf, "-9834" );
        if ( len == len2 ) {
            utf8stringbuf_delete_to_end( unitTestResultBuf, len2 );
        }
    }
    return unitTestResultBuf;
}

int utf8stringbufCrossLanguage::CheckStringBuf( const utf8stringbuf_t buf, const char* charSequence ) {
    /* do some stuff simply to test some utf8stringbuf functions from cpp linkage context */
    if ( utf8stringbuf_get_length(buf) >= 5 ) {
        char unitTestArr[5];
        utf8stringbuf_t unitTestBuf = UTF8STRINGBUF( unitTestArr );
        utf8stringbuf_copy_region_from_buf( unitTestBuf, buf, 1, 4 );
        utf8stringbuf_delete( buf, 1, 4 );
        utf8stringbuf_insert_buf( buf, 1, unitTestBuf );
    }
    if ( CompareCodePoint( utf8stringbuf_get_char_at( buf, 0 ), charSequence ) != 1 ) {
        return 0;
    }
    return utf8stringbuf_equals_str( buf, charSequence );
}

int utf8stringbufCrossLanguage::CompareCodePoint( utf8codepoint_t theChar, const char* charSequence ) {
    utf8codepoint_t test = utf8codepoint_init( charSequence, strlen(charSequence) );
    return ( utf8codepoint_get_char(theChar) == utf8codepoint_get_char(test) ) ? (1) : (0);
}


/*
 * Copyright 2012-2021 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
