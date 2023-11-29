/* File: document_css_writer.c; Copyright and License: see below */

#include "document/document_css_writer.h"
#include "utf8stringbuf/utf8string.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <stdbool.h>
#include <assert.h>

/* IO_FILE_FORMAT_CSS */

static const char CSS_ALL[]
="body {\n"
"    background-color: rgb(255,255,255);\n"
"    font-family: Helvetica,Arial,sans-serif;\n"
"}\n"
".toc1 {\n"
"    color: rgb(192,128,0);\n"
"    font-size: small;\n"
"    counter-reset: cnt-head-two;\n"
"}\n"
".toc2::before {\n"
"    counter-increment: cnt-head-two;\n"
"    content: counter(cnt-head-two) \"\\0000a0 \";\n"
"}\n"
".toc2 {\n"
"    color: rgb(192,128,0);\n"
"    font-size: small;\n"
"    counter-reset: cnt-head-three;\n"
"}\n"
".toc3::before {\n"
"    counter-increment: cnt-head-three;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \"\\0000a0 \";\n"
"}\n"
".toc3 {\n"
"    color: rgb(192,128,0);\n"
"    font-size: x-small;\n"
"    counter-reset: cnt-head-four;\n"
"}\n"
".toc4::before {\n"
"    counter-increment: cnt-head-four;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \".\" counter(cnt-head-four) \"\\0000a0 \";\n"
"}\n"
".toc4 {\n"
"    color: rgb(192,128,0);\n"
"    font-size: xx-small;\n"
"    counter-reset: cnt-head-five;\n"
"}\n"
".toc5::before {\n"
"    counter-increment: cnt-head-five;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \".\" counter(cnt-head-four) \".\" counter(cnt-head-five) \"\\0000a0 \";\n"
"}\n"
".toc5 {\n"
"    color: rgb(192,128,0);\n"
"    font-size: xx-small;\n"
"    counter-reset: cnt-head-six;\n"
"}\n"
".toc6::before {\n"
"    counter-increment: cnt-head-six;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \".\" counter(cnt-head-four) \".\" counter(cnt-head-five) \".\" counter(cnt-head-six) \"\\0000a0 \";\n"
"}\n"
".toc6 {\n"
"    color: rgb(192,128,0);\n"
"    font-size: xx-small;\n"
"}\n"
".diag-name {\n"
"    color: rgb(192,128,0);\n"
"}\n"
".diag-descr {\n"
"    background-color: rgb(240,240,240);\n"
"    padding: 6px;\n"
"    margin: 2px;\n"
"    border: 1px solid #CC8800;\n"
"}\n"
".clas {\n"
"    background-color: rgb(240,240,240);\n"
"}\n"
".clas-name {\n"
"    color: rgb(128,80,0);\n"
"}\n"
".clas-id::before {\n"
"    content:\" \\0000a0 \\0000a0 {id=\";\n"
"}\n"
".clas-id {\n"
"    color: rgb(160,160,160);\n"
"}\n"
".clas-id::after {\n"
"    content: \"}\";\n"
"}\n"
".clas-descr {\n"
"    background-color: rgb(255,255,255);\n"
"    padding: 6px;\n"
"    margin: 2px;\n"
"    border: 1px solid #CCCCCC;\n"
"}\n"
".feat {\n"
"    background-color: rgb(255,255,255);\n"
"    font-size: small;\n"
"    margin-left: 32px;\n"
"    margin-right: 32px;\n"
"    padding-bottom: 8px;\n"
"}\n"
".feat-name {\n"
"    color: rgb(80,80,80);\n"
"}\n"
".feat-id::before {\n"
"    content: \" \\0000a0 \\0000a0 {id=\";\n"
"}\n"
".feat-id {\n"
"    color: rgb(160,160,160);\n"
"}\n"
".feat-id::after {\n"
"    content :\"}\";\n"
"}\n"
".feat-descr {\n"
"    background-color: rgb(255,255,255);\n"
"    padding: 6px;\n"
"    margin: 2px;\n"
"    border: 1px solid #CCCCCC;\n"
"}\n"
".rel {\n"
"    background-color: rgb(255,255,255);\n"
"    font-size: small;\n"
"    margin-left: 32px;\n"
"    margin-right: 32px;\n"
"}\n"
".rel-name {\n"
"    color: rgb(80,80,80);\n"
"}\n"
".rel-id::before {\n"
"    content: \" \\0000a0 \\0000a0 {id=\";\n"
"}\n"
".rel-id {\n"
"    color: rgb(160,160,160);\n"
"}\n"
".rel-id::after {\n"
"    content: \"}\";\n"
"}\n"
".rel-descr {\n"
"    background-color: rgb(255,255,255);\n"
"    padding: 6px;\n"
"    margin: 2px;\n"
"    border: 1px solid #CCCCCC;\n"
"}\n"
"h1 {\n"
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

    utf8string_t text = CSS_ALL;
    const size_t text_len = utf8string_get_length(text);
    export_err |= universal_output_stream_write( (*this_).output, text, text_len);

    U8_TRACE_END_ERR( export_err );
    return export_err;
}


/*
Copyright 2017-2023 Andreas Warnke

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
