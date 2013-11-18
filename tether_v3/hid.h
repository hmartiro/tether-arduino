/****************************************************
* HID Device Emulation Module
* Prefix: HID_
****************************************************/

#ifndef HID_H
#define	HID_H

#include <Arduino.h>

#include "Timer.h"

#include "communication.h"
#include "xyz.h"

// --------------------------------------------------
// PUBLIC CONSTANTS
// --------------------------------------------------

#define HID_UPDATE_RATE 10

// --------------------------------------------------
// PUBLIC VARIABLES
// --------------------------------------------------

// --------------------------------------------------
// PUBLIC METHODS
// --------------------------------------------------

void HID_reset();
void HID_set_mouse(int arg);
void HID_set_joystick(int arg);
void HID_button_1_event(int arg);

void HID_init(Timer* t);
void HID_commands();

#endif	/* HID_H */

