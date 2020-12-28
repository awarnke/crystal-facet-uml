/* File: test_env_check_xml.c; Copyright and License: see below */

#include "io_export_model_traversal_test.h"
#include "io_export_model_traversal.h"
#include "xmi/xmi_element_writer.h"
#include "stream/universal_memory_output_stream.h"
#include "set/data_stat.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "trace.h"
#include "test_assert.h"
#include <glib.h>
        
int test_env_check_xml_is_valid(const char* xml_string)
{
    bool success;
    
    static GMarkupParser DEV_NULL_PARSER = {
        .start_element=NULL,
        .end_element=NULL,
        .text=NULL,
        .passthrough=NULL,
        .error=NULL
    };

    GMarkupParseContext *my_ctx = g_markup_parse_context_new ( &DEV_NULL_PARSER, 0, NULL, NULL );
    TEST_ENVIRONMENT_ASSERT( NULL != my_ctx );
    success = g_markup_parse_context_parse ( my_ctx, xml_string, strlen(xml_string), NULL );
    success &= g_markup_parse_context_end_parse ( my_ctx, NULL );
    g_markup_parse_context_free( my_ctx );
    
    return ( success ? 0 : -1 );
}


/*
 * Copyright 2020-2020 Andreas Warnke
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
