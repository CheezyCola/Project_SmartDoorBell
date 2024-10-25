/* A class to handle GPIO controls
 * Created by Tong Tin Yi (2023)
 */

/**************************************/
/* all functions return int value for */
/* error handling. value 0 means okay */
/* if value is -1, error occurs       */
/**************************************/

#ifndef CONTROLS_H
#define CONTROLS_H

#include <wiringPi.h>
#include <softTone.h>
#include <softPwm.h>
#include <vector>
#include <iostream>

// BCM GPIO and WPI pin mapping
#define BCM26 25
#define BCM6 22
#define BCM12 26
#define BCM18 1 //PWM 0
#define BCM13 23 // PWM 1

/**
* @brief A class to handle GPIO controls
*/
class Controls
{
    public:
        Controls();
        virtual ~Controls();
        /**
         * @brief Initialize the GPIO pins
         * @return -1, if error occurs
         */
        int Initialize();
        /**
         * @brief Play ringtone of door bell with buzzer
         * @return -1, if error occurs
         */
        int DoorBell();
        /**
         * @brief Play ringtone when system is on with buzzer
         * @return -1, if error occurs
         */
        int OnMusic();
        /**
         * @brief Play beep sound with buzzer
         * @return -1, if error occurs
         */
        int BeepSound();
        /**
         * @brief Play sound to alert visitorwith buzzer
         * @return -1, if error occurs
         */
        int AlertSound();
        /**
         * @brief Open the door by controlling servo motor
         * @return -1, if error occurs
         */
        int openDoor();
        /**
         * @brief Close the door by controlling servo motor
         * @return -1, if error occurs
         */
        int closeDoor();
        /**
         * @brief Check if the door is open (contact switch status)
         * @param status is true, if door is open
         * @return -1, if error occurs
         */
        int CheckIsDoorOpen(bool &status);
        /**
         * @brief Check if door bell is pressed
         * @param status is true, if the door bell button is pressed
         * @return -1, if error occurs
         */
        int CheckIsDoorBellRing(bool &status);

    protected:

    private:
        int TonePin = BCM12;
        int ServoPin = BCM13;
        int SwitchPin = BCM6;
        int LEDButtonPin = BCM26;
        int LEDPin = BCM18;
        std::vector<int> tone;
        std::vector<int> time;
};
#endif // CONTROLS_H
