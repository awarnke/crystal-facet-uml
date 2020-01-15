/* File: gui_search_runner.c; Copyright and License: see below */

#include "gui_search_runner.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

void gui_search_runner_init ( gui_search_runner_t *this_, GtkWidget *search_label, GtkWidget *search_entry, GtkWidget *search_button )
{
    TRACE_BEGIN();
    assert ( search_label != NULL );
    assert ( search_entry != NULL );
    assert ( search_button != NULL );

    TRACE_END();
}

void gui_search_runner_destroy ( gui_search_runner_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}


/*
Copyright 2020-2020 Andreas Warnke

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
