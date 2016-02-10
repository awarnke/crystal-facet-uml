/* File: ctrl_classifier.c; Copyright and License: see below */

#include "ctrl_classifier.h"
#include "trace.h"
#include "log.h"

void ctrl_classifier_init ( ctrl_classifier_t *this_, data_database_t *database )
{
    TRACE_BEGIN();
    
    (*this_).database = database;
    
    TRACE_END();
}

void ctrl_classifier_destroy ( ctrl_classifier_t *this_ )
{
    TRACE_BEGIN();
    
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
