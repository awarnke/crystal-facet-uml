/* File: u8_log.h; Copyright and License: see below */

#ifndef U8_LOG_H
#define U8_LOG_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Prints error and event logs to a byte (u8) stream
 *
 *  In contrast to syslog and printf, these macros are typesafe
 *  even when used with older compilers.
 *
 *  An error is a condition that leads to an observable malfunction.
 *  E.g. removing the device on which the database is stored.
 *
 *  A warning is issued when a condition may possibly lead to a malfunction.
 *  E.g. Opening a read-only database file.
 *
 *  An anomaly is a non-standard condition that is expected to not cause a malfunction.
 *  This should be logged to easier analyze issues. E.g. paste an empty clipboard.
 *
 *  An event is a signal that is send to or received from external software parts.
 *
 *  Only non-confidential information may be logged, the program flow for example,
 *  but not the data a user entered, like names or descriptions of elements.
 *
 *  Target audience is the operator of the application
 *  or the developer if the history of events is attached to bug reports.
 *
 *  Use traces for developing.
 *
 *  Return error codes to address the user of the application
 *  or to defer the decision to the calling function.
 */

#include <stdio.h>
#include <stdlib.h>
#ifdef __linux__
#include <syslog.h>
#endif // __linux__
#ifndef NDEBUG
#include <assert.h>
#endif  // NDEBUG

/*define U8_LOG_OUT_STREAM_ stderr -- stdout is better because traces and logs are merged to the same stream in the right order */
#define U8_LOG_OUT_STREAM_ stdout

extern __thread unsigned int u8_log_stat_err;
extern __thread unsigned int u8_log_stat_warn;
extern __thread unsigned int u8_log_stat_anom;
extern __thread unsigned int u8_log_stat_evt;

#ifndef NDEBUG  /* SWITCH RELEASE/DEBUG : CASE DEBUG */
#ifdef __linux__  /* SWITCH TARGET OS : CASE LINUX */

#define U8_LOG_RED_ "\033[37;1;41m"
#define U8_LOG_YELLOW_ "\033[30;43m"
#define U8_LOG_CYAN_ "\033[30;46m"
#define U8_LOG_INVERSE_ "\033[7m"
#define U8_LOG_NORM_ "\033[0m"
#define U8_LOG_PREFIX_ERR U8_LOG_RED_ "ERR " U8_LOG_NORM_ ": "
#define U8_LOG_PREFIX_WARN U8_LOG_YELLOW_ "WARN" U8_LOG_NORM_ ": "
#define U8_LOG_PREFIX_ANOM U8_LOG_CYAN_ "ANOM" U8_LOG_NORM_ ": "
#define U8_LOG_PREFIX_EVT U8_LOG_INVERSE_ "EVT " U8_LOG_NORM_ ": "

#define U8_LOG_PRINTF_ERR_1_(FMT,P1) \
    fprintf(U8_LOG_OUT_STREAM_, "\n" U8_LOG_PREFIX_ERR FMT "\n\n", P1); fflush(U8_LOG_OUT_STREAM_);\
    syslog(LOG_ERR, "ERR : " FMT, P1);
#define U8_LOG_PRINTF_ERR_2_(FMT,P1,P2) \
    fprintf(U8_LOG_OUT_STREAM_, "\n" U8_LOG_PREFIX_ERR FMT "\n\n", P1, P2); fflush(U8_LOG_OUT_STREAM_);\
    syslog(LOG_ERR, "ERR : " FMT, P1, P2);
#define U8_LOG_PRINTF_WARN_1_(FMT,P1) \
    fprintf(U8_LOG_OUT_STREAM_, "\n" U8_LOG_PREFIX_WARN FMT "\n\n", P1); fflush(U8_LOG_OUT_STREAM_);\
    syslog(LOG_WARNING, "WARN: " FMT, P1);
#define U8_LOG_PRINTF_WARN_2_(FMT,P1,P2) \
    fprintf(U8_LOG_OUT_STREAM_, "\n" U8_LOG_PREFIX_WARN FMT "\n\n", P1, P2); fflush(U8_LOG_OUT_STREAM_);\
    syslog(LOG_WARNING, "WARN: " FMT, P1, P2);
#define U8_LOG_PRINTF_ANOM_1_(FMT,P1) \
    fprintf(U8_LOG_OUT_STREAM_, "\n" U8_LOG_PREFIX_ANOM FMT "\n\n", P1); fflush(U8_LOG_OUT_STREAM_);\
    syslog(LOG_INFO, "ANOM: " FMT, P1);
#define U8_LOG_PRINTF_ANOM_2_(FMT,P1,P2) \
    fprintf(U8_LOG_OUT_STREAM_, "\n" U8_LOG_PREFIX_ANOM FMT "\n\n", P1, P2); fflush(U8_LOG_OUT_STREAM_);\
    syslog(LOG_INFO, "ANOM: " FMT, P1, P2);
#define U8_LOG_PRINTF_EVT_1_(FMT,P1) \
    fprintf(U8_LOG_OUT_STREAM_, "\n" U8_LOG_PREFIX_EVT FMT "\n\n", P1);\
    syslog(LOG_INFO, "EVT : " FMT, P1);
#define U8_LOG_PRINTF_EVT_2_(FMT,P1,P2) \
    fprintf(U8_LOG_OUT_STREAM_, "\n" U8_LOG_PREFIX_EVT FMT "\n\n", P1, P2);\
    syslog(LOG_INFO, "EVT : " FMT, P1, P2);
#define U8_LOG_PRINT_STAT_() \
    fprintf(U8_LOG_OUT_STREAM_, "\nstat: " U8_LOG_PREFIX_ERR " %d; " U8_LOG_PREFIX_WARN " %d; " U8_LOG_PREFIX_ANOM " %d; "  U8_LOG_PREFIX_EVT " %d\n\n", u8_log_stat_err, u8_log_stat_warn, u8_log_stat_anom, u8_log_stat_evt );

#else   /* SWITCH TARGET OS : CASE NON-LINUX */

#define U8_LOG_PREFIX_ERR "ERR : "
#define U8_LOG_PREFIX_WARN "WARN: "
#define U8_LOG_PREFIX_ANOM "ANOM: "
#define U8_LOG_PREFIX_EVT "EVT : "

#define U8_LOG_PRINTF_ERR_1_(FMT,P1) \
    fprintf(U8_LOG_OUT_STREAM_, "\n" U8_LOG_PREFIX_ERR FMT "\n\n", P1); fflush(U8_LOG_OUT_STREAM_);
#define U8_LOG_PRINTF_ERR_2_(FMT,P1,P2) \
    fprintf(U8_LOG_OUT_STREAM_, "\n" U8_LOG_PREFIX_ERR FMT "\n\n", P1, P2); fflush(U8_LOG_OUT_STREAM_);
#define U8_LOG_PRINTF_WARN_1_(FMT,P1) \
    fprintf(U8_LOG_OUT_STREAM_, "\n" U8_LOG_PREFIX_WARN FMT "\n\n", P1); fflush(U8_LOG_OUT_STREAM_);
#define U8_LOG_PRINTF_WARN_2_(FMT,P1,P2) \
    fprintf(U8_LOG_OUT_STREAM_, "\n" U8_LOG_PREFIX_WARN FMT "\n\n", P1, P2); fflush(U8_LOG_OUT_STREAM_);
#define U8_LOG_PRINTF_ANOM_1_(FMT,P1) \
    fprintf(U8_LOG_OUT_STREAM_, "\n" U8_LOG_PREFIX_ANOM FMT "\n\n", P1);
#define U8_LOG_PRINTF_ANOM_2_(FMT,P1,P2) \
    fprintf(U8_LOG_OUT_STREAM_, "\n" U8_LOG_PREFIX_ANOM FMT "\n\n", P1, P2);
#define U8_LOG_PRINTF_EVT_1_(FMT,P1) \
    fprintf(U8_LOG_OUT_STREAM_, "\n" U8_LOG_PREFIX_EVT FMT "\n\n", P1);
#define U8_LOG_PRINTF_EVT_2_(FMT,P1,P2) \
    fprintf(U8_LOG_OUT_STREAM_, "\n" U8_LOG_PREFIX_EVT FMT "\n\n", P1, P2);
#define U8_LOG_PRINT_STAT_() \
    fprintf(U8_LOG_OUT_STREAM_, "\nstat: " U8_LOG_PREFIX_ERR " %d; " U8_LOG_PREFIX_WARN " %d; " U8_LOG_PREFIX_ANOM " %d; "  U8_LOG_PREFIX_EVT " %d\n\n", u8_log_stat_err, u8_log_stat_warn, u8_log_stat_anom, u8_log_stat_evt );

#endif  /* SWITCH TARGET OS */
#else             /* SWITCH RELEASE/DEBUG : CASE RELEASE */
#ifdef __linux__  /* SWITCH TARGET OS : CASE LINUX */

#define U8_LOG_PRINTF_ERR_1_(FMT,P1) syslog(LOG_ERR, "ERR : " FMT, P1);
#define U8_LOG_PRINTF_ERR_2_(FMT,P1,P2) syslog(LOG_ERR, "ERR : " FMT, P1, P2);
#define U8_LOG_PRINTF_WARN_1_(FMT,P1) syslog(LOG_WARNING, "WARN: " FMT, P1);
#define U8_LOG_PRINTF_WARN_2_(FMT,P1,P2) syslog(LOG_WARNING, "WARN: " FMT, P1, P2);
#define U8_LOG_PRINTF_ANOM_1_(FMT,P1) syslog(LOG_INFO, "ANOM: " FMT, P1);
#define U8_LOG_PRINTF_ANOM_2_(FMT,P1,P2) syslog(LOG_INFO, "ANOM: " FMT, P1, P2);
#define U8_LOG_PRINTF_EVT_1_(FMT,P1) (void)P1;  /* cast to void to prevent warnings on unused variables */
#define U8_LOG_PRINTF_EVT_2_(FMT,P1,P2) (void)P1; (void)P2;  /* cast to void to prevent warnings on unused variables */
#define U8_LOG_PRINT_STAT_() {}
/* fprintf(U8_LOG_OUT_STREAM_, "stat: ERR %d; WARN %d; ANOM %d\n", u8_log_stat_err, u8_log_stat_warn, u8_log_stat_anom ); */

#else   /* SWITCH TARGET OS : CASE NON-LINUX */

#define U8_LOG_PRINTF_ERR_1_(FMT,P1) (void)P1;  /* cast to void to prevent warnings on unused variables */
#define U8_LOG_PRINTF_ERR_2_(FMT,P1,P2) (void)P1; (void)P2;  /* cast to void to prevent warnings on unused variables */
#define U8_LOG_PRINTF_WARN_1_(FMT,P1) (void)P1;  /* cast to void to prevent warnings on unused variables */
#define U8_LOG_PRINTF_WARN_2_(FMT,P1,P2) (void)P1; (void)P2;  /* cast to void to prevent warnings on unused variables */
#define U8_LOG_PRINTF_ANOM_1_(FMT,P1) (void)P1;  /* cast to void to prevent warnings on unused variables */
#define U8_LOG_PRINTF_ANOM_2_(FMT,P1,P2) (void)P1; (void)P2;  /* cast to void to prevent warnings on unused variables */
#define U8_LOG_PRINTF_EVT_1_(FMT,P1) (void)P1;  /* cast to void to prevent warnings on unused variables */
#define U8_LOG_PRINTF_EVT_2_(FMT,P1,P2) (void)P1; (void)P2;  /* cast to void to prevent warnings on unused variables */
#define U8_LOG_PRINT_STAT_() {}

#endif  /* SWITCH TARGET OS */
#endif  /* SWITCH RELEASE/DEBUG */

#ifdef __linux__  /* INIT SYSLOG ON LINUX ONLY */

/*!
 *  \brief starts to log
 */
#define U8_LOG_INIT(program_id) { const char *string_test = (program_id);\
    openlog( /* ident: */ string_test, /* options: */ 0, /* facility: */ LOG_USER  ); }

/*!
 *  \brief prints statistics on logging
 */
#define U8_LOG_STATS() { U8_LOG_PRINT_STAT_(); }

/*!
 *  \brief stops logging
 */
#define U8_LOG_DESTROY() { closelog(); }

#else   /* INIT SYSLOG ON LINUX ONLY */

/*!
 *  \brief starts to log
 */
#define U8_LOG_INIT(program_id) {}

/*!
 *  \brief prints statistics on logging
 */
#define U8_LOG_STATS() { U8_LOG_PRINT_STAT_(); }

/*!
 *  \brief stops logging
 */
#define U8_LOG_DESTROY() {}

#endif  /* INIT SYSLOG ON LINUX ONLY */


/* ==== Type checks and conversions ==== */

/*!
 *  \brief logs an error string
 */
#define U8_LOG_ERROR(x) { const char *string_test = (x); U8_LOG_PRINTF_ERR_1_("%s", string_test); u8_log_stat_err++; }

/*!
 *  \brief logs an error string and an integer
 */
#define U8_LOG_ERROR_INT(x,i) { const char *string_test = (x); const int int_test = (i); U8_LOG_PRINTF_ERR_2_("%s %i", string_test, int_test); u8_log_stat_err++; }

/*!
 *  \brief logs an error string and a hexadecimal integer
 */
#define U8_LOG_ERROR_HEX(x,i) { const char *string_test = (x); const unsigned int int_test = (i); U8_LOG_PRINTF_ERR_2_("%s 0x%08x", string_test, int_test); u8_log_stat_err++; }

/*!
 *  \brief logs an error string and an information string
 *
 *  Do not print confidential information to the log.
 */
#define U8_LOG_ERROR_STR(x,s) { const char *string_test = (x); const char *string2_test = (s); U8_LOG_PRINTF_ERR_2_("%s %s", string_test, string2_test); u8_log_stat_err++; }

/*!
 *  \brief logs a warning string
 */
#define U8_LOG_WARNING(x) { const char *string_test = (x); U8_LOG_PRINTF_WARN_1_("%s", string_test); u8_log_stat_warn++; }

/*!
 *  \brief logs a warning string and an integer
 */
#define U8_LOG_WARNING_INT(x,i) { const char *string_test = (x); const int int_test = (i); U8_LOG_PRINTF_WARN_2_("%s %i", string_test, int_test); u8_log_stat_warn++; }

/*!
 *  \brief logs a warning string and a hexadecimal integer
 */
#define U8_LOG_WARNING_HEX(x,i) { const char *string_test = (x); const unsigned int int_test = (i); U8_LOG_PRINTF_WARN_2_("%s 0x%08x", string_test, int_test); u8_log_stat_warn++; }

/*!
 *  \brief logs a warning string and an information string
 *
 *  Do not print confidential information to the log.
 */
#define U8_LOG_WARNING_STR(x,s) { const char *string_test = (x); const char *string2_test = (s); U8_LOG_PRINTF_WARN_2_("%s %s", string_test, string2_test); u8_log_stat_warn++; }

/*!
 *  \brief logs an anomaly string
 */
#define U8_LOG_ANOMALY(x) { const char *string_test = (x); U8_LOG_PRINTF_ANOM_1_("%s", string_test); u8_log_stat_anom++; }

/*!
 *  \brief logs an anomaly string and an integer
 */
#define U8_LOG_ANOMALY_INT(x,i) { const char *string_test = (x); const int int_test = (i); U8_LOG_PRINTF_ANOM_2_("%s %i", string_test, int_test); u8_log_stat_anom++; }

/*!
 *  \brief logs an anomaly string and a hexadecimal integer
 */
#define U8_LOG_ANOMALY_HEX(x,i) { const char *string_test = (x); const unsigned int int_test = (i); U8_LOG_PRINTF_ANOM_2_("%s 0x%08x", string_test, int_test); u8_log_stat_anom++; }

/*!
 *  \brief logs an anomaly string and an information string
 *
 *  Do not print confidential information to the log.
 */
#define U8_LOG_ANOMALY_STR(x,s) { const char *string_test = (x); const char *string2_test = (s); U8_LOG_PRINTF_ANOM_2_("%s %s", string_test, string2_test); u8_log_stat_anom++; }

/*!
 *  \brief logs an event string
 */
#define U8_LOG_EVENT(x) { const char *string_test = (x); U8_LOG_PRINTF_EVT_1_("%s", string_test); u8_log_stat_evt++; }

/*!
 *  \brief logs an event string and an integer
 */
#define U8_LOG_EVENT_INT(x,i) { const char *string_test = (x); const int int_test = (i); U8_LOG_PRINTF_EVT_2_("%s %i", string_test, int_test); u8_log_stat_evt++; }

/*!
 *  \brief logs an event string and a hexadecimal integer
 */
#define U8_LOG_EVENT_HEX(x,i) { const char *string_test = (x); const unsigned int int_test = (i); U8_LOG_PRINTF_EVT_2_("%s 0x%08x", string_test, int_test); u8_log_stat_evt++; }

/*!
 *  \brief logs an event string and an information string
 *
 *  Do not print confidential information to the log.
 */
#define U8_LOG_EVENT_STR(x,s) { const char *string_test = (x); const char *string2_test = (s); U8_LOG_PRINTF_EVT_2_("%s %s", string_test, string2_test); u8_log_stat_evt++; }

#endif  /* U8_LOG_H */


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
