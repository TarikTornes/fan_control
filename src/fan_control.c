#include "general_funcs.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <signal.h>
#include <wiringPi.h>



int temp1 = 30;
int temp2 = 50;


int main() {


    // LOGGING MESSAGE START
    
    if (daemon(0, 0) == -1) {
        perror("Fan control could not be daemonized");
        exit(EXIT_FAILURE);
    }


    openlog("fan_control", LOG_PID | LOG_CONS, LOG_DAEMON);

    signal(SIGHUP, signal_handler);
   // signal(SIGUSR1, signal_handler);
    signal(SIGTERM, signal_handler);

    syslog(LOG_NOTICE, "fan_control daemon started");
    log_message("fan_control daemon started", LOG_FILE);

    if (wiringPiSetup() == -1) {
        syslog(LOG_WARNING, "wiringPi could not be setup");
        log_message("wiringPi was not found", LOG_FILE);
        exit(EXIT_FAILURE);
    }

    load_config();
    int velocity = OFF;
    char buf[100];

    pinMode(FAN_PIN, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetClock(192);
    pwmSetRange(1024);

    pwmWrite(FAN_PIN, 0);

    log_message("before while loop", LOG_FILE);
    
    while (1) {
        log_message("in loop", LOG_FILE);
        float temp = get_cpu_temperature();
        snprintf(buf, 100, "CPU-temp: %.2f", temp);
        log_message(buf, LOG_FILE);
        
        //fan_control action

        if (temp > temp2 && velocity != HIGH) {
            syslog(LOG_INFO, "fan speed adjusted to HIGH");
            log_message("Changed to high fan speed", LOG_FILE);
            velocity = HIGH;
            pwmWrite(FAN_PIN, 1024);
        } else if (temp > temp1 && velocity != LOW) {
            syslog(LOG_INFO, "fan speed adjusted to LOW");
            log_message("Changed to low fan speed", LOG_FILE);
            velocity = LOW;
            pwmWrite(FAN_PIN, 500);
        } else if (temp <= temp1 && velocity != OFF) {
            syslog(LOG_INFO, "fan speed adjusted to OFF");
            log_message("Turned fan OFF", LOG_FILE);
            velocity = OFF;
            pwmWrite(FAN_PIN, 0);
        }
        sleep(5);

    }

    log_message("fan_control daemon shutdown", LOG_FILE);

    closelog();
    return(EXIT_SUCCESS);

    return 0;
}


