/* File: utf8stringbuf_cross_language.h; Copyright and License: see below */

#ifndef UTF8STRINGBUF_CROSS_LANGUAGE_H_
#define UTF8STRINGBUF_CROSS_LANGUAGE_H_

#include "util/string/utf8stringbuf.h"
#include "util/string/utf8codepoint.h"

#ifdef __cplusplus

/* function to test if a utf8stringbuf_t generated by cpp code works in c linkage environment */
extern "C" utf8stringbuf_t utf8stringbuf_cross_language_create_stringbuf( const char* charSequence );
/* function to test if a utf8stringbuf_t generated in c linkage environment works in cpp code */
extern "C" int utf8stringbuf_cross_language_check_stringbuf( utf8stringbuf_t buf, const char* charSequence );

/* class to initialize and test a utf8stringbuf_t struct */
class utf8stringbufCrossLanguage {
public:
    utf8stringbuf_t CreateStringBuf( const char* charSequence );
    int CheckStringBuf( const utf8stringbuf_t buf, const char* charSequence );
private:
    int CompareCodePoint( utf8codepoint_t theChar, const char* charSequence );
};

#else

/* function to test if a utf8stringbuf_t generated by cpp code works in c linkage environment */
extern utf8stringbuf_t utf8stringbuf_cross_language_create_stringbuf( const char* charSequence );
/* function to test if a utf8stringbuf_t generated in c linkage environment works in cpp code */
extern int utf8stringbuf_cross_language_check_stringbuf( utf8stringbuf_t buf, const char* charSequence );

#endif

#endif /*UTF8STRINGBUF_CROSS_LANGUAGE_H_*/


/*
 * Copyright 2012-2020 Andreas Warnke
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
