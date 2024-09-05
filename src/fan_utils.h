#ifndef FAN_UTILS_H
#define FAN_UTILS_H


/*******************************************************************************
 * FAN_UTILS
 *
 * This module provides the necessary functions to perform the fan adjustments
 *
 *******************************************************************************/



/**
 * \brief: adjusts the fan speed of the cooling system with necessary logging etc
 * \param: pointer to int which describes the current velocity state
 * \param: int describing the new state of tje velocity
 * \param: string of the message that will be logged when change is performed
 * \param: int storing the pwm value to adjust the fan speed
 */
void adjust_fan_speed(int *, int , const char *, int);

#endif // !FAN_UTILS_H
