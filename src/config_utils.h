#ifndef CONFIG_UTILS_H
#define CONFIG_UTILS_H

/*******************************************************************************
 * CONFIG_UTILS
 *
 * This module provides the necessary functions to read and load the configfile
 *
 *******************************************************************************/


/** location of config file */
#define CONFIG_FILE "/etc/fan_control.conf"

/** semantic macros for successful/problematic termination of methods */
#define SUCC_C 1
#define ERR_C 0


/**
 * \brief represents a data type that will store values of the config file
 * \param: temp1 is the first temperature threshold that will switch the fan from OFF to LOW
 * \param: temp2 is the second temperature threshold that will switch the fan from LOW to HIGH
 */
typedef struct configs {
    float temp1;
    float temp2;
} confs;


/** represents a pointer of the struct confs */
typedef confs* settings;


/**
 * \brief retrieves information from config file
 * \return settings which will store the values of the config file
 */
settings get_confs();


/**
 * \brief reloads configurations for the daemon
 * \param pointer to the confs struct which will be in the global scope of the main funciton
 */
int load_config(settings);


#endif
