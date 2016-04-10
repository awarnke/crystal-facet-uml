/* File: unittest_main.c; Copyright and License: see below */

#include "data_change_notifier_test.h"
#include "ctrl_diagram_controller_test.h"
#include "ctrl_classifier_controller_test.h"
#include "trace.h"
#include <embUnit/embUnit.h>

/*!
 *  \brief main runs the unit tests
 */
int main (int argc, char *argv[]) {
    TRACE_BEGIN();
    TRACE_TIMESTAMP();
    TRACE_INFO( "--------------------" );
    int exit_code = 0;

    TestRunner_start();
    TestRunner_runTest( data_change_notifier_test_get_list() );
    TestRunner_runTest( ctrl_diagram_controller_test_get_list() );
    TestRunner_runTest( ctrl_classifier_controller_test_get_list() );
    TestRunner_end();

    TRACE_INFO( "--------------------" );
    TRACE_TIMESTAMP();
    TRACE_END_ERR( exit_code );
    return exit_code;
}


/*
 * Copyright 2016-2016 Andreas Warnke
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
