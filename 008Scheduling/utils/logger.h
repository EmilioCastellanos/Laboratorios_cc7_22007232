#ifndef LOGGER_H
#define LOGGER_H

#include "../core/process.h"

void logger_init();

void log_event(Process *p, const char *msg);

void log_header(const char *header);

void logger_destroy();

void log_fifo_start(Process *p, double wait_time);

#endif