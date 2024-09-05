#include "logging_utils.h"
#include <syslog.h>
#include <wiringPi.h>


void adjust_fan_speed(int *velocity, int speed, const char *message, int pwm_value) {
    syslog(LOG_INFO, message);
    log_message(message, LOG_FILE);
    *velocity = speed;
    pwmWrite(FAN_PIN, pwm_value);
}
