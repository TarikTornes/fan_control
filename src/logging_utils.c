#include "logging_utils.h"
#include <stdio.h>
#include <time.h>




void timestamp_log(char* buffer, int buffer_size) {
    time_t t;
    struct tm *tm_local;

    t = time(NULL);
    tm_local = localtime(&t);

    if(buffer != NULL && buffer_size > 0) {
        strftime(buffer, buffer_size, "[%x-%X]", tm_local);
    }
}


int log_message(const char* message, const char * filename) {
    
    char time[20];
    timestamp_log(time, sizeof(time));

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
