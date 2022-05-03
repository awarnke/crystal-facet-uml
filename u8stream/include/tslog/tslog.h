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

/*define TSLOG_OUT_STREAM_ stderr -- stdout is better because traces and logs are merged to the same stream in the right order */
#define TSLOG_OUT_STREAM_ stdout

#ifndef NDEBUG  /* SWITCH RELEASE/DEBUG */
#ifdef __linux__  /* SWITCH RELEASE SYSLOG */

#define TSLOG_RED_ "\033[37;1;41m"
#define TSLOG_YELLOW_ "\033[30;43m"
#define TSLOG_CYAN_ "\033[30;46m"
#define TSLOG_INVERSE_ "\033[7m"
#define TSLOG_NORM_ "\033[0m"
#define TSLOG_PREFIX_ERR TSLOG_RED_ "ERR " TSLOG_NORM_ ": "
#define TSLOG_PREFIX_WARN TSLOG_YELLOW_ "WARN" TSLOG_NORM_ ": "
#define TSLOG_PREFIX_ANOM TSLOG_CYAN_ "ANOM" TSLOG_NORM_ ": "
#define TSLOG_PREFIX_EVT TSLOG_INVERSE_ "EVT " TSLOG_NORM_ ": "

#define TSLOG_PRINTF_ERR_1_(FMT,P1) \
    fprintf(TSLOG_OUT_STREAM_, "\n" TSLOG_PREFIX_ERR FMT "\n\n", P1); fflush(TSLOG_OUT_STREAM_);\
    syslog(LOG_ERR, "ERR : " FMT, P1);
#define TSLOG_PRINTF_ERR_2_(FMT,P1,P2) \
    fprintf(TSLOG_OUT_STREAM_, "\n" TSLOG_PREFIX_ERR FMT "\n\n", P1, P2); fflush(TSLOG_OUT_STREAM_);\
    syslog(LOG_ERR, "ERR : " FMT, P1, P2);
#define TSLOG_PRINTF_WARN_1_(FMT,P1) \
    fprintf(TSLOG_OUT_STREAM_, "\n" TSLOG_PREFIX_WARN FMT "\n\n", P1); fflush(TSLOG_OUT_STREAM_);\
    syslog(LOG_WARNING, "WARN: " FMT, P1);
#define TSLOG_PRINTF_WARN_2_(FMT,P1,P2) \
    fprintf(TSLOG_OUT_STREAM_, "\n" TSLOG_PREFIX_WARN FMT "\n\n", P1, P2); fflush(TSLOG_OUT_STREAM_);\
    syslog(LOG_WARNING, "WARN: " FMT, P1, P2);
#define TSLOG_PRINTF_ANOM_1_(FMT,P1) \
    fprintf(TSLOG_OUT_STREAM_, "\n" TSLOG_PREFIX_ANOM FMT "\n\n", P1);\
    syslog(LOG_INFO, "ANOM: " FMT, P1);
#define TSLOG_PRINTF_ANOM_2_(FMT,P1,P2) \
    fprintf(TSLOG_OUT_STREAM_, "\n" TSLOG_PREFIX_ANOM FMT "\n\n", P1, P2);\
    syslog(LOG_INFO, "ANOM: " FMT, P1, P2);
#define TSLOG_PRINTF_EVT_1_(FMT,P1) \
    fprintf(TSLOG_OUT_STREAM_, "\n" TSLOG_PREFIX_EVT FMT "\n\n", P1);\
    syslog(LOG_INFO, "EVT : " FMT, P1);
#define TSLOG_PRINTF_EVT_2_(FMT,P1,P2) \
    fprintf(TSLOG_OUT_STREAM_, "\n" TSLOG_PREFIX_EVT FMT "\n\n", P1, P2);\
    syslog(LOG_INFO, "EVT : " FMT, P1, P2);

#else   /* SWITCH RELEASE SYSLOG */

#define TSLOG_PREFIX_ERR "ERR : "
#define TSLOG_PREFIX_WARN "WARN: "
#define TSLOG_PREFIX_ANOM "ANOM: "
#define TSLOG_PREFIX_EVT "EVT : "

#define TSLOG_PRINTF_ERR_1_(FMT,P1) \
    fprintf(TSLOG_OUT_STREAM_, "\n" TSLOG_PREFIX_ERR FMT "\n\n", P1); fflush(TSLOG_OUT_STREAM_);
#define TSLOG_PRINTF_ERR_2_(FMT,P1,P2) \
    fprintf(TSLOG_OUT_STREAM_, "\n" TSLOG_PREFIX_ERR FMT "\n\n", P1, P2); fflush(TSLOG_OUT_STREAM_);
#define TSLOG_PRINTF_WARN_1_(FMT,P1) \
    fprintf(TSLOG_OUT_STREAM_, "\n" TSLOG_PREFIX_WARN FMT "\n\n", P1); fflush(TSLOG_OUT_STREAM_);
#define TSLOG_PRINTF_WARN_2_(FMT,P1,P2) \
    fprintf(TSLOG_OUT_STREAM_, "\n" TSLOG_PREFIX_WARN FMT "\n\n", P1, P2); fflush(TSLOG_OUT_STREAM_);
#define TSLOG_PRINTF_ANOM_1_(FMT,P1) \
    fprintf(TSLOG_OUT_STREAM_, "\n" TSLOG_PREFIX_ANOM FMT "\n\n", P1);
#define TSLOG_PRINTF_ANOM_2_(FMT,P1,P2) \
    fprintf(TSLOG_OUT_STREAM_, "\n" TSLOG_PREFIX_ANOM FMT "\n\n", P1, P2);
#define TSLOG_PRINTF_EVT_1_(FMT,P1) \
    fprintf(TSLOG_OUT_STREAM_, "\n" TSLOG_PREFIX_EVT FMT "\n\n", P1);
#define TSLOG_PRINTF_EVT_2_(FMT,P1,P2) \
    fprintf(TSLOG_OUT_STREAM_, "\n" TSLOG_PREFIX_EVT FMT "\n\n", P1, P2);

#endif  /* SWITCH RELEASE SYSLOG */
#else             /* SWITCH RELEASE/DEBUG */
#ifdef __linux__  /* SWITCH RELEASE SYSLOG */

#define TSLOG_PRINTF_ERR_1_(FMT,P1) syslog(LOG_ERR, "ERR : " FMT, P1);
#define TSLOG_PRINTF_ERR_2_(FMT,P1,P2) syslog(LOG_ERR, "ERR : " FMT, P1, P2);
#define TSLOG_PRINTF_WARN_1_(FMT,P1) syslog(LOG_WARNING, "WARN: " FMT, P1);
#define TSLOG_PRINTF_WARN_2_(FMT,P1,P2) syslog(LOG_WARNING, "WARN: " FMT, P1, P2);
#define TSLOG_PRINTF_ANOM_1_(FMT,P1) syslog(LOG_INFO, "ANOM: " FMT, P1);
#define TSLOG_PRINTF_ANOM_2_(FMT,P1,P2) syslog(LOG_INFO, "ANOM: " FMT, P1, P2);
#define TSLOG_PRINTF_EVT_1_(FMT,P1) (void)P1;  /* cast to void to prevent warnings on unused variables */
#define TSLOG_PRINTF_EVT_2_(FMT,P1,P2) (void)P1; (void)P2;  /* cast to void to prevent warnings on unused variables */

#else   /* SWITCH RELEASE SYSLOG */

#define TSLOG_PRINTF_ERR_1_(FMT,P1) (void)P1;  /* cast to void to prevent warnings on unused variables */
#define TSLOG_PRINTF_ERR_2_(FMT,P1,P2) (void)P1; (void)P2;  /* cast to void to prevent warnings on unused variables */
#define TSLOG_PRINTF_WARN_1_(FMT,P1) (void)P1;  /* cast to void to prevent warnings on unused variables */
#define TSLOG_PRINTF_WARN_2_(FMT,P1,P2) (void)P1; (void)P2;  /* cast to void to prevent warnings on unused variables */
#define TSLOG_PRINTF_ANOM_1_(FMT,P1) (void)P1;  /* cast to void to prevent warnings on unused variables */
#define TSLOG_PRINTF_ANOM_2_(FMT,P1,P2) (void)P1; (void)P2;  /* cast to void to prevent warnings on unused variables */
#define TSLOG_PRINTF_EVT_1_(FMT,P1) (void)P1;  /* cast to void to prevent warnings on unused variables */
#define TSLOG_PRINTF_EVT_2_(FMT,P1,P2) (void)P1; (void)P2;  /* cast to void to prevent warnings on unused variables */

#endif  /* SWITCH RELEASE SYSLOG */
#endif  /* SWITCH RELEASE/DEBUG */


#ifdef __linux__  /* INIT SYSLOG ON LINUX ONLY */

/*!
 *  \brief starts to log
 */
#define TSLOG_INIT(program_id) { const char *string_test = (program_id); openlog( /* ident: */ string_test, /* options: */ 0, /* facility: */ LOG_USER  ); }

/*!
 *  \brief stops logging
 */
#define TSLOG_DESTROY() { closelog(); }

#else   /* INIT SYSLOG ON LINUX ONLY */

/*!
 *  \brief starts to log
 */
#define TSLOG_INIT(program_id) {}

/*!
 *  \brief stops logging
 */
#define TSLOG_DESTROY() {}

#endif  /* INIT SYSLOG ON LINUX ONLY */


/*!
 *  \brief logs an error string
 */
#define TSLOG_ERROR(x) { const char *string_test = (x); TSLOG_PRINTF_ERR_1_("%s", string_test); }

/*!
 *  \brief logs an error string and an integer
 */
#define TSLOG_ERROR_INT(x,i) { const char *string_test = (x); const int int_test = (i); TSLOG_PRINTF_ERR_2_("%s %i", string_test, int_test); }

/*!
 *  \brief logs an error string and a hexadecimal integer
 */
#define TSLOG_ERROR_HEX(x,i) { const char *string_test = (x); const unsigned int int_test = (i); TSLOG_PRINTF_ERR_2_("%s 0x%08x", string_test, int_test); }

/*!
 *  \brief logs an error string and an information string
 *
 *  Do not print confidential information to the log.
 */
#define TSLOG_ERROR_STR(x,s) { const char *string_test = (x); const char *string2_test = (s); TSLOG_PRINTF_ERR_2_("%s %s", string_test, string2_test); }

/*!
 *  \brief logs a warning string
 */
#define TSLOG_WARNING(x) { const char *string_test = (x); TSLOG_PRINTF_WARN_1_("%s", string_test); }

/*!
 *  \brief logs a warning string and an integer
 */
#define TSLOG_WARNING_INT(x,i) { const char *string_test = (x); const int int_test = (i); TSLOG_PRINTF_WARN_2_("%s %i", string_test, int_test); }

/*!
 *  \brief logs a warning string and a hexadecimal integer
 */
#define TSLOG_WARNING_HEX(x,i) { const char *string_test = (x); const unsigned int int_test = (i); TSLOG_PRINTF_WARN_2_("%s 0x%08x", string_test, int_test); }

/*!
 *  \brief logs a warning string and an information string
 *
 *  Do not print confidential information to the log.
 */
#define TSLOG_WARNING_STR(x,s) { const char *string_test = (x); const char *string2_test = (s); TSLOG_PRINTF_WARN_2_("%s %s", string_test, string2_test); }

/*!
 *  \brief logs an anomaly string
 */
#define TSLOG_ANOMALY(x) { const char *string_test = (x); TSLOG_PRINTF_ANOM_1_("%s", string_test); }

/*!
 *  \brief logs an anomaly string and an integer
 */
#define TSLOG_ANOMALY_INT(x,i) { const char *string_test = (x); const int int_test = (i); TSLOG_PRINTF_ANOM_2_("%s %i", string_test, int_test); }

/*!
 *  \brief logs an anomaly string and a hexadecimal integer
 */
#define TSLOG_ANOMALY_HEX(x,i) { const char *string_test = (x); const unsigned int int_test = (i); TSLOG_PRINTF_ANOM_2_("%s 0x%08x", string_test, int_test); }

/*!
 *  \brief logs an anomaly string and an information string
 *
 *  Do not print confidential information to the log.
 */
#define TSLOG_ANOMALY_STR(x,s) { const char *string_test = (x); const char *string2_test = (s); TSLOG_PRINTF_ANOM_2_("%s %s", string_test, string2_test); }

/*!
 *  \brief logs an event string
 */
#define TSLOG_EVENT(x) { const char *string_test = (x); TSLOG_PRINTF_EVT_1_("%s", string_test); }

/*!
 *  \brief logs an event string and an integer
 */
#define TSLOG_EVENT_INT(x,i) { const char *string_test = (x); const int int_test = (i); TSLOG_PRINTF_EVT_2_("%s %i", string_test, int_test); }

/*!
 *  \brief logs an event string and a hexadecimal integer
 */
#define TSLOG_EVENT_HEX(x,i) { const char *string_test = (x); const unsigned int int_test = (i); TSLOG_PRINTF_EVT_2_("%s 0x%08x", string_test, int_test); }

/*!
 *  \brief logs an event string and an information string
 *
 *  Do not print confidential information to the log.
 */
#define TSLOG_EVENT_STR(x,s) { const char *string_test = (x); const char *string2_test = (s); TSLOG_PRINTF_EVT_2_("%s %s", string_test, string2_test); }

#endif  /* TSLOG_H */


/*
Copyright 2016-2022 Andreas Warnke

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
