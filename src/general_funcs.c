#include <time.h>
#include "general_funcs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <syslog.h>


extern int temp1;
extern int temp2;

void signal_handler(int sig){

    if(sig==SIGTERM) {
        syslog(LOG_INFO, "fan_control daemon is gracefully shutdown");
        log_message("fan_control is gracefully shutdown", LOG_FILE);
        closelog();
        exit(EXIT_SUCCESS);
    } else if(sig==SIGHUP) {
        syslog(LOG_INFO, "Received SIGHUP signal: config is reloaded");
        load_config();
    } else {
        syslog(LOG_WARNING, "Some unexpected signal %d appeared", sig);
    }

}


void load_config() {
    settings cf = get_confs();
    
    log_message("Loading configs", LOG_FILE);

    if(cf != NULL) {
        temp1 = cf->temp1;
        temp2 = cf->temp2;
        free(cf);
        syslog(LOG_INFO, "Configs successfully reloaded!"); 
        log_message("Reloaded configs", LOG_FILE);

    } else {

        syslog(LOG_WARNING, "Something went wrong while loading configs");
        log_message("Something went wrong while reloading configs", LOG_FILE);
    }
}


settings get_confs() {

    settings res = malloc(sizeof(confs));
    if(res == NULL) {
        log_message("ERROR while getting configs (get_confs)", LOG_FILE);
        return NULL;
    }

    res->temp1 = 30;
    res->temp2 = 50;
    //strlcpy(res->name,"default", 20);

    FILE* fb = fopen(CONFIG_FILE, "r");
    if(fb==NULL) {
        syslog(LOG_INFO,"Config file %s could not be read", CONFIG_FILE);
        char mess[256];
        sprintf(mess, "Config file %s could not be read", CONFIG_FILE);
        log_message(mess, LOG_FILE);
        return res;
    }

    char line[256];

    while(fgets(line, sizeof(line),fb)) {
        if(strstr(line, "[General]") != NULL) {
            break;
        }
    }
    
    int i =0;
    while(fgets(line,sizeof(line), fb)) {
        
        i++;
        
        if(strstr(line,"temp1") != NULL) {
            if(sscanf(line, "temp1 : %d\n", &(res->temp1)) != 1) {
                log_message("WARNING: temp1 could not be read from config-file", LOG_FILE);
                free(res);
                fclose(fb);
                return NULL;
            }

        }

        if(strstr(line,"temp2") != NULL) {
            if(sscanf(line, "temp2 = %d\n", &(res->temp2)) != 1) {
                log_message("WARNING: temp2 could not be read from config-file", LOG_FILE);
                free(res);
                fclose(fb);
                return NULL;
            }

        }
        
        /*
        if(strstr(line,"name1") != NULL) {
            if(sscanf(line, "name1 = %19s\n", &(res->name)) != 1) {
                perror("Could not read name1");
            }

        }
        */

        if(strstr(line,"[") != NULL && strstr(line, "]") != NULL) {
            break;
        } 

    }

    fclose(fb);

    return res;
}





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
        
