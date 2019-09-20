/* File: tslog.h; Copyright and License: see below */

#ifndef TSLOG_H
#define TSLOG_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Prints error and event logs to a byte stream
 *
 *  In contrast to syslog and printf, these macros are typesafe.
 *
 *  An error is a condition that leads to an observalbe malfunction.
 *  E.g. removing the device on which the database is stored.
 *
 *  A warning is issued when a condition may possibly lead to a malfunction.
 *  E.g. Opening a read-only database file.
 *
 *  An anomaly is a non-standard condition that is expected to not cause a malfunction.
 *  This should be logged to easier analyze issues. E.g. paste an empty clipboard.
 *
 *  An event is a signal that is send to or received from external software parts.
 */

#include <stdio.h>
#include <stdlib.h>
#ifdef __linux__
#include <syslog.h>
#endif // __linux__
#ifndef NDEBUG
#include <unistd.h>
#include <assert.h>
#endif  // NDEBUG

/*define TSLOG_OUT_STREAM stderr -- stdout is better because traces and logs are merged to the same stream in the right order */
#define TSLOG_OUT_STREAM stdout
#define WARN_DELAY 3
#define ERR_DELAY 3

#ifndef NDEBUG  /* SWITCH RELEASE/DEBUG */

/*!
 *  \brief starts to log
 */
#define TSLOG_INIT(program_id) {}

/*!
 *  \brief stops logging
 */
#define TSLOG_DESTROY() {}

/*!
 *  \brief logs an error string
 */
#define TSLOG_ERROR(x) { const char *string_test = x; fprintf(TSLOG_OUT_STREAM,"\nERR : %s\n",string_test); fflush(TSLOG_OUT_STREAM); sleep(ERR_DELAY); }

/*!
 *  \brief logs an error string and an integer
 */
#define TSLOG_ERROR_INT(x,i) { const char *string_test = x; const int int_test = i; fprintf(TSLOG_OUT_STREAM,"\nERR : %s %i\n",string_test,int_test); fflush(TSLOG_OUT_STREAM); sleep(ERR_DELAY); }

/*!
 *  \brief logs an error string and a hexadecimal integer
 */
#define TSLOG_ERROR_HEX(x,i) { const char *string_test = x; const unsigned int int_test = i; fprintf(TSLOG_OUT_STREAM,"\nERR : %s 0x%x\n",string_test,int_test); fflush(TSLOG_OUT_STREAM); sleep(ERR_DELAY); }

/*!
 *  \brief logs an error string and an information string
 */
#define TSLOG_ERROR_STR(x,s) { const char *string_test = x; const char *string2_test = s; fprintf(TSLOG_OUT_STREAM,"\nERR : %s %s\n",string_test,string2_test); fflush(TSLOG_OUT_STREAM); sleep(ERR_DELAY); }

/*!
 *  \brief logs a warning string
 */
#define TSLOG_WARNING(x) { const char *string_test = x; fprintf(TSLOG_OUT_STREAM,"\nWARN: %s\n",string_test); fflush(TSLOG_OUT_STREAM); sleep(WARN_DELAY); }

/*!
 *  \brief logs a warning string and an integer
 */
#define TSLOG_WARNING_INT(x,i) { const char *string_test = x; const int int_test = i; fprintf(TSLOG_OUT_STREAM,"\nWARN: %s %i\n",string_test,int_test); fflush(TSLOG_OUT_STREAM); sleep(WARN_DELAY); }

/*!
 *  \brief logs a warning string and a hexadecimal integer
 */
#define TSLOG_WARNING_HEX(x,i) { const char *string_test = x; const unsigned int int_test = i; fprintf(TSLOG_OUT_STREAM,"\nWARN: %s 0x%x\n",string_test,int_test); fflush(TSLOG_OUT_STREAM); sleep(WARN_DELAY); }

/*!
 *  \brief logs a warning string and an information string
 */
#define TSLOG_WARNING_STR(x,s) { const char *string_test = x; const char *string2_test = s; fprintf(TSLOG_OUT_STREAM,"\nWARN: %s %s\n",string_test,string2_test); fflush(TSLOG_OUT_STREAM); sleep(WARN_DELAY); }

/*!
 *  \brief logs an anomaly string
 */
#define TSLOG_ANOMALY(x) { const char *string_test = x; fprintf(TSLOG_OUT_STREAM,"ANOM: %s\n",string_test); }

/*!
 *  \brief logs an anomaly string and an integer
 */
#define TSLOG_ANOMALY_INT(x,i) { const char *string_test = x; const int int_test = i; fprintf(TSLOG_OUT_STREAM,"ANOM: %s %i\n",string_test,int_test); }

/*!
 *  \brief logs an anomaly string and a hexadecimal integer
 */
#define TSLOG_ANOMALY_HEX(x,i) { const char *string_test = x; const unsigned int int_test = i; fprintf(TSLOG_OUT_STREAM,"ANOM: %s 0x%x\n",string_test,int_test); }

/*!
 *  \brief logs an anomaly string and an information string
 */
#define TSLOG_ANOMALY_STR(x,s) { const char *string_test = x; const char *string2_test = s; fprintf(TSLOG_OUT_STREAM,"ANOM: %s %s\n",string_test,string2_test); }

/*!
 *  \brief logs an event string
 */
#define TSLOG_EVENT(x) { const char *string_test = x; fprintf(TSLOG_OUT_STREAM,"EVT : %s\n",string_test); }

/*!
 *  \brief logs an event string and an integer
 */
#define TSLOG_EVENT_INT(x,i) { const char *string_test = x; const int int_test = i; fprintf(TSLOG_OUT_STREAM,"EVT : %s %i\n",string_test,int_test); }

/*!
 *  \brief logs an event string and a hexadecimal integer
 */
#define TSLOG_EVENT_HEX(x,i) { const char *string_test = x; const unsigned int int_test = i; fprintf(TSLOG_OUT_STREAM,"EVT : %s 0x%x\n",string_test,int_test); }

/*!
 *  \brief logs an event string and an information string
 */
#define TSLOG_EVENT_STR(x,s) { const char *string_test = x; const char *string2_test = s; fprintf(TSLOG_OUT_STREAM,"EVT : %s %s\n",string_test,string2_test); }

#else             /* SWITCH RELEASE/DEBUG */
#ifdef __linux__  /* SWITCH RELEASE SYSLOG */

/*!
 *  \brief starts to log
 */
#define TSLOG_INIT(program_id) { openlog( /* ident: */ program_id, /* options: */ 0, /* facility: */ LOG_USER  ); }

/*!
 *  \brief stops logging
 */
#define TSLOG_DESTROY() { closelog(); }

/*!
 *  \brief logs an error string
 */
#define TSLOG_ERROR(x) { const char *string_test = x; syslog(LOG_ERR,"ERR : %s",string_test); }

/*!
 *  \brief logs an error string and an integer
 */
#define TSLOG_ERROR_INT(x,i) { const char *string_test = x; const int int_test = i; syslog(LOG_ERR,"ERR : %s %i",string_test,int_test); }

/*!
 *  \brief logs an error string and a hexadecimal integer
 */
#define TSLOG_ERROR_HEX(x,i) { const char *string_test = x; const unsigned int int_test = i; syslog(LOG_ERR,"ERR : %s 0x%x",string_test,int_test); }

/*!
 *  \brief logs an error string and an information string
 */
#define TSLOG_ERROR_STR(x,s) { const char *string_test = x; const char *string2_test = s; syslog(LOG_ERR,"ERR : %s %s",string_test,string2_test); }

/*!
 *  \brief logs a warning string
 */
#define TSLOG_WARNING(x) { const char *string_test = x; syslog(LOG_WARNING,"WARN: %s",string_test); }

/*!
 *  \brief logs a warning string and an integer
 */
#define TSLOG_WARNING_INT(x,i) { const char *string_test = x; const int int_test = i; syslog(LOG_WARNING,"WARN: %s %i",string_test,int_test); }

/*!
 *  \brief logs a warning string and a hexadecimal integer
 */
#define TSLOG_WARNING_HEX(x,i) { const char *string_test = x; const unsigned int int_test = i; syslog(LOG_WARNING,"WARN: %s 0x%x",string_test,int_test); }

/*!
 *  \brief logs a warning string and an information string
 */
#define TSLOG_WARNING_STR(x,s) { const char *string_test = x; const char *string2_test = s; syslog(LOG_WARNING,"WARN: %s %s",string_test,string2_test); }

/*!
 *  \brief logs an anomaly string
 */
#define TSLOG_ANOMALY(x) { const char *string_test = x; syslog(LOG_INFO,"ANOM: %s",string_test); }

/*!
 *  \brief logs an anomaly string and an integer
 */
#define TSLOG_ANOMALY_INT(x,i) { const char *string_test = x; const int int_test = i; syslog(LOG_INFO,"ANOM: %s %i",string_test,int_test); }

/*!
 *  \brief logs an anomaly string and a hexadecimal integer
 */
#define TSLOG_ANOMALY_HEX(x,i) { const char *string_test = x; const unsigned int int_test = i; syslog(LOG_INFO,"ANOM: %s 0x%x",string_test,int_test); }

/*!
 *  \brief logs an anomaly string and an information string
 */
#define TSLOG_ANOMALY_STR(x,s) { const char *string_test = x; const char *string2_test = s; syslog(LOG_INFO,"ANOM: %s %s",string_test,string2_test); }

/*!
 *  \brief logs an event string
 */
#define TSLOG_EVENT(x) { const char *string_test __attribute__((unused)) = x; /*syslog(LOG_INFO,"EVT : %s",string_test);*/ }

/*!
 *  \brief logs an event string and an integer
 */
#define TSLOG_EVENT_INT(x,i) { const char *string_test __attribute__((unused)) = x; const int int_test __attribute__((unused))  = i; /*syslog(LOG_INFO,"EVT : %s %i",string_test,int_test);*/ }

/*!
 *  \brief logs an event string and a hexadecimal integer
 */
#define TSLOG_EVENT_HEX(x,i) { const char *string_test __attribute__((unused)) = x; const unsigned int int_test __attribute__((unused)) = i; /*syslog(LOG_INFO,"EVT : %s 0x%x",string_test,int_test);*/ }

/*!
 *  \brief logs an event string and an information string
 */
#define TSLOG_EVENT_STR(x,s) { const char *string_test __attribute__((unused)) = x; const char *string2_test __attribute__((unused)) = s; /*syslog(LOG_INFO,"EVT : %s %s",string_test,string2_test);*/ }

#else   /* SWITCH RELEASE SYSLOG */

/*!
 *  \brief starts to log
 */
#define TSLOG_INIT(program_id) {}

/*!
 *  \brief stops logging
 */
#define TSLOG_DESTROY() {}

/*!
 *  \brief logs an error string
 */
#define TSLOG_ERROR(x) {}

/*!
 *  \brief logs an error string and an integer
 */
#define TSLOG_ERROR_INT(x,i) {}

/*!
 *  \brief logs an error string and a hexadecimal integer
 */
#define TSLOG_ERROR_HEX(x,i) {}

/*!
 *  \brief logs an error string and an information string
 */
#define TSLOG_ERROR_STR(x,s) {}

/*!
 *  \brief logs a warning string
 */
#define TSLOG_WARNING(x) {}

/*!
 *  \brief logs a warning string and an integer
 */
#define TSLOG_WARNING_INT(x,i) {}

/*!
 *  \brief logs a warning string and a hexadecimal integer
 */
#define TSLOG_WARNING_HEX(x,i) {}

/*!
 *  \brief logs a warning string and an information string
 */
#define TSLOG_WARNING_STR(x,s) {}

/*!
 *  \brief logs an anomaly string
 */
#define TSLOG_ANOMALY(x) {}

/*!
 *  \brief logs an anomaly string and an integer
 */
#define TSLOG_ANOMALY_INT(x,i) {}

/*!
 *  \brief logs an anomaly string and a hexadecimal integer
 */
#define TSLOG_ANOMALY_HEX(x,i) {}

/*!
 *  \brief logs an anomaly string and an information string
 */
#define TSLOG_ANOMALY_STR(x,s) {}

/*!
 *  \brief logs an event string
 */
#define TSLOG_EVENT(x) {}

/*!
 *  \brief logs an event string and an integer
 */
#define TSLOG_EVENT_INT(x,i) {}

/*!
 *  \brief logs an event string and a hexadecimal integer
 */
#define TSLOG_EVENT_HEX(x,i) {}

/*!
 *  \brief logs an event string and an information string
 */
#define TSLOG_EVENT_STR(x,s) {}

#endif  /* SWITCH RELEASE SYSLOG */
#endif  /* SWITCH RELEASE/DEBUG */

#endif  /* TSLOG_H */


/*
Copyright 2016-2019 Andreas Warnke

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
