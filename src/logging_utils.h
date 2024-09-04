#ifndef LOGGING_UTILS_H
#define LOGGING_UTILS_H

/*******************************************************************************
 * LOGGING_UTILS MODULE
 *
 *This module provides functions for logging messages to a separate file.
 * It enables users to perform logging in plain text, ensuring that logs 
 * are stored clearly and accessibly in a text file.
 *
 *******************************************************************************/

/** location of log file */
#define LOG_FILE "/var/log/fan_control.log"

/** semantic macros for success/failure of log_message */
#define SUCC_L 1
#define ERR_L 0


/**
 * \brief: creates timestamp in a specific format for logging
 * \param: variable of the buffer where the timestamp will be stored
 * \param: size of the buffer
 */
void timestamp_log(char*, int);


/**
 * \brief: performs the logging by writing a message and the corresponding information into a file
 * \param: a string that contains the message to be logged
 * \param: a strnig indicating the path to the config file
 * \return: returns 1 if message was successfully logged and 0 if not
 */
int log_message(const char*, const char*);


#endif
