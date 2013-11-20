/****************************************************
* Position Tracking Module
* Prefix: XYZ_
****************************************************/

#ifndef XYZ_H
#define	XYZ_H

#include <Arduino.h>

#include "Timer.h"
#include "Encoder.h"

#include "communication.h"
#include "hid.h"

// --------------------------------------------------
// PUBLIC CONSTANTS
// --------------------------------------------------

#define XYZ_UPDATE_RATE 1 // 1 or 2 works well
#define XYZ_PRINT_RATE 500 // 10 works well, 100fps

// --------------------------------------------------
// PUBLIC VARIABLES
// --------------------------------------------------

// --------------------------------------------------
// PUBLIC METHODS
// --------------------------------------------------

int XYZ_x();
int XYZ_y();
int XYZ_z();
int XYZ_button_1();
int XYZ_button_2();

void XYZ_init(Timer* t);
void XYZ_commands();

void XYZ_process_interrupt();

int XYZ_change_x();
int XYZ_change_y();
int XYZ_change_z();

#endif	/* XYZ_H */

