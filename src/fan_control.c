#include "general_utils.h"
#include "logging_utils.h"
#include "config_utils.h"
#include "fan_utils.h"

#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <signal.h>
#include <wiringPi.h>


#define FAN_CONTROL_INTERVAL 10  // Sleep time in seconds


int velocity;   // stores the level on which the fan operates

confs global_configs;   // stores the current config parameters

/**
 * \brief: catches incoming signals and treats them
 * \param: int represents the incoming signal
 */
void signal_handler(int);



int main() {


    signal(SIGHUP, signal_handler);    // reloading configs
    signal(SIGTERM, signal_handler);    // gracefully terminating daemon
    // signal(SIGUSR1, signal_handler);
    

    openlog("fan_control", LOG_PID | LOG_CONS, LOG_DAEMON);
    syslog(LOG_NOTICE, "fan_control daemon started");
    log_message("fan_control daemon started", LOG_FILE);



    if (wiringPiSetup() == -1) {
        syslog(LOG_WARNING, "wiringPi could not be setup");
        log_message("wiringPi was not found", LOG_FILE);
        exit(EXIT_FAILURE);
    }


    
    init_global_configs(&global_configs);
    load_config(&global_configs);
    init_fanveloc(&velocity);
    init_fansettings();


        
    while (1) {

        float temp = get_cpu_temperature();

        syslog(LOG_INFO, "CPU-temp: %.2f, veloc: %d, temp1: %.2f, temp2: %.2f", temp, velocity, global_configs.temp1, global_configs.temp2); 

        
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
    syslog(LOG_INFO, "fan_control terminated loop");

    closelog();
    return(EXIT_SUCCESS);

}




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
