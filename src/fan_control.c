#include "general_funcs.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <signal.h>
#include <wiringPi.h>



float temp1 = 30;
float temp2 = 50;


int main() {


    // LOGGING MESSAGE START
    /* 
    if (daemon(0, 0) == -1) {
        perror("Fan control could not be daemonized");
        exit(EXIT_FAILURE);
    }
    */



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
    int velocity = OFF_F;
    char buf[200];

    pinMode(FAN_PIN, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetClock(192);
    pwmSetRange(1024);

    pwmWrite(FAN_PIN, 0);

    
    while (1) {
        float temp = get_cpu_temperature();
        //snprintf(buf, 200, "CPU-temp: %.2f, veloc: %d, temp1: %.2f, temp2: %.2f", temp, velocity, temp1, temp2);
        //log_message(buf, LOG_FILE);

        
        //fan_control action

        if ((temp > temp2) && (velocity != MAX_F)) {
            syslog(LOG_INFO, "fan speed adjusted to MAX_F");
            log_message("Changed to high fan speed", LOG_FILE);
            velocity = MAX_F;
            pwmWrite(FAN_PIN, 1024);
        } else if ((temp > temp1) && (velocity != MID_F)) {
            syslog(LOG_INFO, "fan speed adjusted to MID_F");
            log_message("Changed to low fan speed", LOG_FILE);
            velocity = MID_F;
            pwmWrite(FAN_PIN, 500);
        } else if ((temp <= temp1) && (velocity != OFF_F)) {
            syslog(LOG_INFO, "fan speed adjusted to OFF_F");
            log_message("Turned fan OFF_F", LOG_FILE);
            velocity = OFF_F;
            pwmWrite(FAN_PIN, 0);
        }

        sleep(10);

    }

    log_message("fan_control daemon shutdown", LOG_FILE);

    closelog();
    return(EXIT_SUCCESS);

    return 0;
}


