/* File: pencil_input_data.c; Copyright and License: see below */

#include "pencil_input_data.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>

void pencil_input_data_init( pencil_input_data_t *this_ )
{
    TRACE_BEGIN();

    data_diagram_init_empty( &((*this_).diagram) );

    TRACE_END();
}

void pencil_input_data_destroy( pencil_input_data_t *this_ )
{
    TRACE_BEGIN();

    data_diagram_destroy( &((*this_).diagram) );

    TRACE_END();
}

void pencil_input_data_load( pencil_input_data_t *this_, int64_t diagram_id, data_database_reader_t *db_reader )
{
    TRACE_BEGIN();

    if ( DATA_DIAGRAM_ID_VOID_ID == diagram_id )
    {
        /* re-init */
        data_diagram_destroy( &((*this_).diagram) );
        data_diagram_init_empty( &((*this_).diagram) );
    }
    else
    {
        data_error_t db_err;
        db_err= data_database_reader_get_diagram_by_id ( db_reader, diagram_id, &((*this_).diagram) );
        if ( DATA_ERROR_NONE != db_err )
        {
            /* error at loading */
            data_diagram_destroy( &((*this_).diagram) );
            data_diagram_init_empty( &((*this_).diagram) );
        }
    }

    TRACE_END();
}


/*
Copyright 2016-2016 Andreas Warnke

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
