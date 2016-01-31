/* File: log.h; Copyright 2016-2016: Andreas Warnke; License: Apache 2.0 */

#ifndef LOG_H
#define LOG_H

/* public file for the doxygen documentation: */
/*! \file */

#include <stdio.h>

#define LOG_OUT_STREAM stdout

#define LOG_ERROR(x) { const char *string_test = x; fprintf(LOG_OUT_STREAM,"ERR : %s\n",string_test); }
#define LOG_ERROR_INT(x,i) { const char *string_test = x; const int int_test = i; fprintf(LOG_OUT_STREAM,"ERR : %s %i\n",string_test,int_test); }
#define LOG_WARNING(x) { const char *string_test = x; fprintf(LOG_OUT_STREAM,"WARN: %s\n",string_test); }
#define LOG_WARNING_INT(x,i) { const char *string_test = x; const int int_test = i; fprintf(LOG_OUT_STREAM,"WARN: %s %i\n",string_test,int_test); }
#define LOG_EVENT(x) { const char *string_test = x; fprintf(LOG_OUT_STREAM,"EVT : %s\n",string_test); }
#define LOG_EVENT_INT(x,i) { const char *string_test = x; const int int_test = i; fprintf(LOG_OUT_STREAM,"EVT : %s %i\n",string_test,int_test); }

#endif  /* LOG_H */


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
