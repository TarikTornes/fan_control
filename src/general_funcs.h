#ifndef GENERAL_FUNCS
#define GENERAL_FUNCS

#include <stddef.h>

#define CONFIG_FILE "/etc/fan_control.conf"
#define LOG_FILE "/var/log/fan_control.log"

#define FAN_PIN 1

#define MAX_F 2
#define MID_F 1
#define OFF_F 0


typedef struct configs {
    int temp1;
    int temp2;
} confs;

typedef confs* settings;

void signal_handler(int);

settings get_confs();

void load_config();

void current_time(char*, size_t);

int log_message(const char*, const char *);

float get_cpu_temperature();

#endif
