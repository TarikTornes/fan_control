#include "logging_utils.h"
#include "fan_utils.h"
#include <syslog.h>
#include <wiringPi.h>


void init_fanveloc(int *velocity) {
    *velocity = OFF_FSPEED;
}


void init_fansettings() {
    pinMode(FAN_PIN, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetClock(192);
    pwmSetRange(PWM_RANGE);
    pwmWrite(FAN_PIN, 0);
}

void adjust_fan_speed(int *velocity, int speed, const char *message, int pwm_value) {
    syslog(LOG_INFO, message);
    log_message(message, LOG_FILE);
    *velocity = speed;
    pwmWrite(FAN_PIN, pwm_value);
}
