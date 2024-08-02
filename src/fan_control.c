#include "general_funcs.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <signal.h>



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


    load_config();
    int iter = 0;
    char buf[100];
    
    while (1) {
        
        //fan_control action

        iter++;
        sleep(8);
        snprintf(buf,100, "Working on %d iteration", iter);
        log_message(buf,LOG_FILE);


    }

    log_message("fan_control daemon shutdown", LOG_FILE);

    closelog();
    return(EXIT_SUCCESS);

    return 0;
}


