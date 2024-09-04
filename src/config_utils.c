#include "config_utils.h"
#include "general_funcs.h"
#include <syslog.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void load_config(settings set) {
    settings cf = get_confs();
    
    syslog(LOG_INFO, "Loading configs");

    if(cf != NULL) {
        set->temp1 = cf->temp1;
        set->temp2 = cf->temp2;
        free(cf);
        syslog(LOG_INFO, "Configs successfully reloaded!"); 
        //log_message("Reloaded configs", LOG_FILE);

    } else {

        syslog(LOG_WARNING, "Something went wrong while loading configs");
        //log_message("Something went wrong while reloading configs", LOG_FILE);
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
        free(res);
        return NULL;
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
            if(sscanf(line, "temp1 : %f\n", &(res->temp1)) != 1) {
                log_message("WARNING: temp1 could not be read from config-file", LOG_FILE);
                free(res);
                fclose(fb);
                return NULL;
            }

        }

        if(strstr(line,"temp2") != NULL) {
            log_message(line, LOG_FILE);
            if(sscanf(line, "temp2 : %f\n", &(res->temp2)) != 1) {
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
