/*
 *
 * LOGGER v0.0.3
 * A simple logger for c/c++ under linux, multiprocess-safe
 *
 * ---- CopyLeft by Felix021 @ http://www.felix021.com ----
 *
 * LOG Format:
 * --LEVEL_NOTE--\x7 [Y-m-d H:m:s]\x7 [FILE:LINE]\x7 [EXTRA_INFO]\x7 log_info
 *   // LEVEL_NOTE stands for one of DEBUG/TRACE/NOTICE...
 *   // \x7 is a special character to separate logged fields.
 *
 * Usage:
 *   //Open log file first. Supply a log file name.
 *   log_open("log.txt");
 *
 *   //use it just as printf
 *   FM_LOG_TRACE("some info %d", 123);
 *
 *   //6 level: DEBUG, TRACE, NOTICE, MONITOR, WARNING, FATAL
 *   FM_LOG_DEBUG("hi there");
 *
 *   //Need EXTRA_INFO to be logged automatically?
 *   log_add_info("pid:123");
 *
 *   //You don't need to call log_close manually, it'll be called at exit
 *   log_close();
 *
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <error.h>
#include <sys/file.h>

#define LOG_FATAL 0
#define LOG_WARNING 1
#define LOG_MONITOR 2
#define LOG_NOTICE 3
#define LOG_TRACE 4
#define LOG_DEBUG 5

int log_open(const char *filename);
void log_close();
void log_write(int, const char *, const int, const char *, ...);
void log_add_info(const char *info);

#define FM_LOG_DEBUG(x...)   log_write(LOG_DEBUG, __FILE__, __LINE__, ##x)
#define FM_LOG_TRACE(x...)   log_write(LOG_TRACE, __FILE__, __LINE__, ##x)
#define FM_LOG_NOTICE(x...)  log_write(LOG_NOTICE, __FILE__, __LINE__, ##x)
#define FM_LOG_MONITOR(x...)  log_write(LOG_MONITOR, __FILE__ __LINE__, ##x)
#define FM_LOG_WARNING(x...) log_write(LOG_WARNING, __FILE__, __LINE__, ##x)
#define FM_LOG_FATAL(x...)   log_write(LOG_FATAL, __FILE__, __LINE__, ##x)

#endif
