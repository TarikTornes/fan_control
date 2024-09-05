#include "general_utils.h"
#include "logging_utils.h"
#include "config_utils.h"
#include "fan_utils.h"
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <signal.h>
#include <wiringPi.h>

#define FAN_PIN

#define PWM_RANGE 1024   // Max range for PWM
#define PWM_LOW_SPEED 500
#define FAN_CONTROL_INTERVAL 10  // Sleep time in seconds

#define HIGH_FSPEED 2
#define LOW_FSPEED 1
#define OFF_FSPEED 0


confs global_configs = {.temp1 = 30, .temp2 = 50};


void signal_handler(int sig){

    if(sig==SIGTERM) {
        syslog(LOG_INFO, "fan_control daemon is gracefully shutdown");
        log_message("fan_control is gracefully shutdown", LOG_FILE);
        closelog();
        exit(EXIT_SUCCESS);
    } else if(sig==SIGHUP) {
        syslog(LOG_INFO, "Received SIGHUP signal: config is reloaded");
        load_config(&global_configs);
    } else {
        syslog(LOG_WARNING, "Some unexpected signal %d appeared", sig);
    }

}


int main() {


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

    load_config(&global_configs);
    int velocity = OFF_FSPEED;
    //char buf[200];

    pinMode(FAN_PIN, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetClock(192);
    pwmSetRange(1024);
    pwmWrite(FAN_PIN, 0);

    
    while (1) {
        float temp = get_cpu_temperature();
        //snprintf(buf, 200, "CPU-temp: %.2f, veloc: %d, temp1: %.2f, temp2: %.2f", temp, velocity, temp1, temp2);
        syslog(LOG_INFO, "CPU-temp: %.2f, veloc: %d, temp1: %.2f, temp2: %.2f", temp, velocity, global_configs.temp1, global_configs.temp2); 

        //log_message(buf, LOG_FILE);

        
        //fan_control action

        if ((temp > global_configs.temp2) && (velocity != HIGH_FSPEED)) {
            adjust_fan_speed(&velocity, HIGH_FSPEED, "Fan adjusted to HIGH SPEED", PWM_RANGE);
        } else if ((temp > global_configs.temp1) && (velocity != LOW_FSPEED)) {
            adjust_fan_speed(&velocity, LOW_FSPEED, "Fan adjusted to LOW SPEED", PWM_LOW_SPEED);
        } else if ((temp <= global_configs.temp1) && (velocity != OFF_FSPEED)) {
            adjust_fan_speed(&velocity, OFF_FSPEED, "Fan adjusted to OFF", 0);
        }

        sleep(FAN_CONTROL_INTERVAL);

    }

    log_message("fan_control daemon shutdown", LOG_FILE);

    closelog();
    return(EXIT_SUCCESS);

}


