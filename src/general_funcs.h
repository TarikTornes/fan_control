#ifndef GENERAL_FUNCS
#define GENERAL_FUNCS

#include <stddef.h>

#define LOG_FILE "/var/log/fan_control.log"

#define FAN_PIN 1

#define MAX_F 2
#define MID_F 1
#define OFF_F 0


void signal_handler(int);


void current_time(char*, size_t);

int log_message(const char*, const char *);

float get_cpu_temperature();

#endif
