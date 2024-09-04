#include <time.h>
#include "general_funcs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>



void current_time(char *buffer, size_t buffer_size) {
    time_t t;
    struct tm *tm_local;

    t = time(NULL);
    tm_local = localtime(&t);

    if (buffer != NULL && buffer_size > 0) {
        strftime(buffer, buffer_size, "[%x-%X]", tm_local);
    }
}



int log_message(const char* message, const char * filename) {
    
    char time[20];
    current_time(time, sizeof(time));

    FILE *f = fopen(filename, "a");
    if(f==NULL) {
        return 0;
    }

    if (fprintf(f,"%s %s\n", time, message)<0) {
        fclose(f);
        return 0;
    }
    fclose(f);
    return 1;
}


float get_cpu_temperature() {
    FILE *fp;
    char path[1035];
    float temperature = 0.0;

    fp = popen("vcgencmd measure_temp", "r");
    if (fp == NULL) {
        log_message("Failed to retrieve temperature (vcgencmd)", LOG_FILE);
        syslog(LOG_WARNING, "Failed to retrieve temperature (vcgencmd)");
        return 100.0;
    }

    while (fgets(path, sizeof(path), fp) != NULL) {
        sscanf(path, "temp=%f", &temperature);
    }

    pclose(fp);

    return temperature;
}
        
