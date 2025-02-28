/* File: main.c; Copyright and License: see below */

#include "main_execute.h"

/* Note: in test mode, the symbol 'main' must not be defined - therefore a redirection to main_execute: */
int main (int argc, char **argv) {
    return main_execute( argc, argv );
}


/*
Copyright 2016-2025 Andreas Warnke

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
