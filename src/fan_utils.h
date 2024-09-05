#ifndef FAN_UTILS_H
#define FAN_UTILS_H


/*******************************************************************************
 * FAN_UTILS
 *
 * This module provides the necessary functions to perform the fan adjustments
 *
 *******************************************************************************/

/** semantic macros for speed levels */
#define HIGH_FSPEED 2
#define LOW_FSPEED 1
#define OFF_FSPEED 0

/** GPIO pin */
#define FAN_PIN 1

/** semantic macro for max pwm range */
#define PWM_RANGE 1024
/** semantic macro for pwm setting on mid fan speed */
#define PWM_LOW_SPEED 500


/**
 * \brief: initializes the velocity parameter
 * \param: int pointer to global velocity parameter
 */
void init_fanveloc(int*);

/**
 * \brief: initializes wiringPi setup
 */
void init_fansettings();

/**
 * \brief: adjusts the fan speed of the cooling system with necessary logging etc
 * \param: pointer to int which describes the current velocity state
 * \param: int describing the new state of tje velocity
 * \param: string of the message that will be logged when change is performed
 * \param: int storing the pwm value to adjust the fan speed
 */
void adjust_fan_speed(int *, int , const char *, int);

#endif // !FAN_UTILS_H
