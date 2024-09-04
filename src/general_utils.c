#include "logging_utils.h"
#include "general_utils.h"
#include <stdio.h>
#include <syslog.h>


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
        
