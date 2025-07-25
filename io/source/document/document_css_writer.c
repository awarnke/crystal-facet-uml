/* File: document_css_writer.c; Copyright and License: see below */

#include "document/document_css_writer.h"
#include "utf8stringbuf/utf8string.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <stdbool.h>
#include <assert.h>

/* IO_FILE_FORMAT_CSS */

static const char CSS_GENERAL[]
="body {\n"
"    background-color: rgb(255,255,255);\n"
"    font-family: Helvetica,Arial,sans-serif;\n"
"}\n"
"article {\n"
"    margin-left: 200px;\n"
"    padding-top: 8px;\n"
"    padding-right: 8px;\n"
"    padding-bottom: 8px;\n"
"    padding-left: 24px;\n"
"}\n"
"nav {\n"
"    background-color: rgb(240,248,255);\n"
"    left: 0px;\n"
"    top: 0px;\n"
"    width: 200px;\n"
"    height: 100%;\n"
"    position: fixed;\n"
"    overflow: auto;\n"
"    padding: 2px;\n"
"    margin: 0px;\n"
"}\n";

static const char CSS_TOC[]
="a:hover {\n"
"    background-color: rgb(255,255,255);\n"
"}\n"
".toc {\n"
"    list-style-type: none;\n"
"    list-style-position: inside;\n"
"    padding: 0px;\n"
"    margin: 0px;\n"
"}\n"
".toc1 {\n"
"    padding-left: 0px;\n"
"    padding-top: 3px;\n"
"    padding-right: 0px;\n"
"    padding-bottom: 3px;\n"
"    color: rgb(192,128,0);\n"
"    font-size: small;\n"
"    counter-reset: cnt-head-two;\n"
"}\n"
".toc2::before {\n"
"    counter-increment: cnt-head-two;\n"
"    content: counter(cnt-head-two) \"\\0000a0 \";\n"
"}\n"
".toc2 {\n"
"    padding-left: 8px;\n"
"    padding-top: 2px;\n"
"    padding-right: 0px;\n"
"    padding-bottom: 2px;\n"
"    color: rgb(192,128,0);\n"
"    font-size: small;\n"
"    counter-reset: cnt-head-three;\n"
"}\n"
".toc3::before {\n"
"    counter-increment: cnt-head-three;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \"\\0000a0 \";\n"
"}\n"
".toc3 {\n"
"    padding-left: 8px;\n"
"    padding-top: 2px;\n"
"    padding-right: 0px;\n"
"    padding-bottom: 1px;\n"
"    color: rgb(192,128,0);\n"
"    font-size: x-small;\n"
"    counter-reset: cnt-head-four;\n"
"}\n"
".toc4::before {\n"
"    counter-increment: cnt-head-four;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \".\" counter(cnt-head-four) \"\\0000a0 \";\n"
"}\n"
".toc4 {\n"
"    padding-left: 8px;\n"
"    padding-top: 1px;\n"
"    padding-right: 0px;\n"
"    padding-bottom: 1px;\n"
"    border-left: 2px solid #FFFFFF;\n"
"    color: rgb(192,128,0);\n"
"    font-size: xx-small;\n"
"    counter-reset: cnt-head-five;\n"
"}\n"
".toc5::before {\n"
"    counter-increment: cnt-head-five;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \".\" counter(cnt-head-four) \".\" counter(cnt-head-five) \"\\0000a0 \";\n"
"}\n"
".toc5 {\n"
"    padding-left: 8px;\n"
"    padding-top: 1px;\n"
"    padding-right: 0px;\n"
"    padding-bottom: 0px;\n"
"    color: rgb(192,128,0);\n"
"    font-size: xx-small;\n"
"    counter-reset: cnt-head-six;\n"
"}\n"
".toc6::before {\n"
"    counter-increment: cnt-head-six;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \".\" counter(cnt-head-four) \".\" counter(cnt-head-five) \".\" counter(cnt-head-six) \"\\0000a0 \";\n"
"}\n"
".toc6 {\n"
"    padding-left: 8px;\n"
"    color: rgb(192,128,0);\n"
"    font-size: xx-small;\n"
"}\n";

static const char CSS_DATA_LAYOUT[]
=".diag-title {\n"
"    color: rgb(192,128,0);\n"
"}\n"
".diag-stereo::before {\n"
"    content:\"\\0000a0 \\0000ab\";\n"
"}\n"
".diag-stereo {\n"
"    color: rgb(80,80,80);\n"
"}\n"
".diag-stereo::after {\n"
"    content: \"\\0000bb\";\n"
"}\n"
".diag-name {\n"
"    color: rgb(128,128,128);\n"
"}\n"
".diag-type::before {\n"
"    content:\"\\0000a0 \\0000a0 {type=\";\n"
"}\n"
".diag-type {\n"
"    color: rgb(160,160,160);\n"
"}\n"
".diag-type::after {\n"
"    content: \"}\";\n"
"}\n"
".diag-id::before {\n"
"    content:\"\\0000a0 \\0000a0 {id=\";\n"
"}\n"
".diag-id {\n"
"    color: rgb(160,160,160);\n"
"}\n"
".diag-id::after {\n"
"    content: \"}\";\n"
"}\n"
".diag-descr {\n"
"    padding: 16px 8px;\n"
"    margin-left: 2px;\n"
"    margin-right: 2px;\n"
"    margin-top: 2px;\n"
"    margin-bottom: 12px;\n"
"    border: 1px solid #DDAA88;\n"
"}\n"
".clas {\n"
"    margin-top: 8px;\n"
"    border-top: 1px solid #CCCCCC;\n"
"    border-right: 1px solid #CCCCCC;\n"
"    border-bottom: 1px solid #CCCCCC;\n"
"}\n"
".clas-stereo::before {\n"
"    content:\"\\0000a0 \\0000ab\";\n"
"}\n"
".clas-stereo {\n"
"    color: rgb(80,80,80);\n"
"}\n"
".clas-stereo::after {\n"
"    content: \"\\0000bb\";\n"
"}\n"
".clas-name {\n"
"    color: rgb(192,128,0);\n"
"}\n"
".clas-type::before {\n"
"    content:\"\\0000a0 \\0000a0 {type=\";\n"
"}\n"
".clas-type {\n"
"    color: rgb(160,160,160);\n"
"}\n"
".clas-type::after {\n"
"    content: \"}\";\n"
"}\n"
".clas-id::before {\n"
"    content:\"\\0000a0 \\0000a0 {id=\";\n"
"}\n"
".clas-id {\n"
"    color: rgb(160,160,160);\n"
"}\n"
".clas-id::after {\n"
"    content: \"}\";\n"
"}\n"
".clas-see::before {\n"
"    content:\"appears in \";\n"
"}\n"
".clas-see {\n"
"    background-color: rgb(240,240,240);\n"
"    text-align: right;\n"
"    font-size: x-small;\n"
"    color: rgb(160,160,160);\n"
"}\n"
".clas-descr {\n"
"    padding: 16px 8px;\n"
"    margin: 2px;\n"
"    border: 1px solid #CCCCCC;\n"
"}\n"
".feat {\n"
"    font-size: small;\n"
"    margin-left: 32px;\n"
"    margin-right: 0px;\n"
"}\n"
".feat-stereo::before {\n"
"    content:\"\\0000a0 \\0000ab\";\n"
"}\n"
".feat-stereo {\n"
"    color: rgb(80,80,80);\n"
"}\n"
".feat-stereo::after {\n"
"    content: \"\\0000bb\";\n"
"}\n"
/* ".feat-name {\n" */
/* "    color: rgb(0,0,0);\n" */
/* "}\n" */
".feat-type::before {\n"
"    content:\"\\0000a0 \\0000a0 {type=\";\n"
"}\n"
".feat-type {\n"
"    color: rgb(160,160,160);\n"
"}\n"
".feat-type::after {\n"
"    content: \"}\";\n"
"}\n"
".feat-id::before {\n"
"    content: \"\\0000a0 \\0000a0 {id=\";\n"
"}\n"
".feat-id {\n"
"    color: rgb(160,160,160);\n"
"}\n"
".feat-id::after {\n"
"    content :\"}\";\n"
"}\n"
".feat-descr {\n"
"    padding: 8px 8px;\n"
"    margin: 2px;\n"
"    border: 1px solid #CCCCCC;\n"
"}\n"
".rel {\n"
"    font-size: small;\n"
"    margin-left: 32px;\n"
"    margin-right: 0px;\n"
"}\n"
".rel-stereo::before {\n"
"    content:\"\\0000a0 \\0000ab\";\n"
"}\n"
".rel-stereo {\n"
"    color: rgb(80,80,80);\n"
"}\n"
".rel-stereo::after {\n"
"    content: \"\\0000bb\";\n"
"}\n"
/* ".rel-name {\n" */
/* "    color: rgb(0,0,0);\n" */
/* "}\n" */
/* ".rel-dest {\n" */
/* "    color: rgb(0,0,0);\n" */
/* "}\n" */
".rel-type::before {\n"
"    content:\"\\0000a0 \\0000a0 {type=\";\n"
"}\n"
".rel-type {\n"
"    color: rgb(160,160,160);\n"
"}\n"
".rel-type::after {\n"
"    content: \"}\";\n"
"}\n"
".rel-id::before {\n"
"    content: \"\\0000a0 \\0000a0 {id=\";\n"
"}\n"
".rel-id {\n"
"    color: rgb(160,160,160);\n"
"}\n"
".rel-id::after {\n"
"    content: \"}\";\n"
"}\n"
".rel-descr {\n"
"    padding: 8px 8px;\n"
"    margin: 2px;\n"
"    border: 1px solid #CCCCCC;\n"
"}\n";

static const char CSS_SECTIONS[]
="h1 {\n"
"    counter-reset: cnt-head-two;\n"
"}\n"
"h2::before {\n"
"    counter-increment: cnt-head-two;\n"
"    content: counter(cnt-head-two) \"\\0000a0 \";\n"
"}\n"
"h2 {\n"
"    counter-reset: cnt-head-three;\n"
"}\n"
"h3::before {\n"
"    counter-increment: cnt-head-three;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \"\\0000a0 \";\n"
"}\n"
"h3 {\n"
"    counter-reset: cnt-head-four;\n"
"}\n"
"h4::before {\n"
"    counter-increment: cnt-head-four;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \".\" counter(cnt-head-four) \"\\0000a0 \";\n"
"}\n"
"h4 {\n"
"    counter-reset: cnt-head-five;\n"
"}\n"
"h5::before {\n"
"    counter-increment: cnt-head-five;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \".\" counter(cnt-head-four) \".\" counter(cnt-head-five) \"\\0000a0 \";\n"
"}\n"
"h5 {\n"
"    counter-reset: cnt-head-six;\n"
"}\n"
"h6::before {\n"
"    counter-increment: cnt-head-six;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \".\" counter(cnt-head-four) \".\" counter(cnt-head-five) \".\" counter(cnt-head-six) \"\\0000a0 \";\n"
"}\n"
"p {\n"
"    padding: 2px;\n"
"    margin: 0px;\n"
"}\n";

void document_css_writer_init ( document_css_writer_t *this_,
                                universal_output_stream_t *output )
{
    U8_TRACE_BEGIN();
    assert( NULL != output );

    (*this_).output = output;

    U8_TRACE_END();
}

void document_css_writer_destroy( document_css_writer_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).output = NULL;

    U8_TRACE_END();
}

u8_error_t document_css_writer_write_stylesheet( document_css_writer_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    utf8string_t *text_1 = CSS_GENERAL;
    const size_t text_1_len = utf8string_get_length(text_1);
    export_err |= universal_output_stream_write( (*this_).output, text_1, text_1_len );

    utf8string_t *text_2 = CSS_TOC;
    const size_t text_2_len = utf8string_get_length(text_2);
    export_err |= universal_output_stream_write( (*this_).output, text_2, text_2_len );

    utf8string_t *text_3 = CSS_DATA_LAYOUT;
    const size_t text_3_len = utf8string_get_length(text_3);
    export_err |= universal_output_stream_write( (*this_).output, text_3, text_3_len );

    utf8string_t *text_4 = CSS_SECTIONS;
    const size_t text_4_len = utf8string_get_length(text_4);
    export_err |= universal_output_stream_write( (*this_).output, text_4, text_4_len );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}


/*
Copyright 2017-2025 Andreas Warnke

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
