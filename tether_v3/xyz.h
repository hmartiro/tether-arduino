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

// --------------------------------------------------
// PUBLIC CONSTANTS
// --------------------------------------------------

#define XYZ_UPDATE_RATE 2 // 2 works well
#define XYZ_PRINT_RATE 1000 // 10 works well, 100fps

// --------------------------------------------------
// PUBLIC VARIABLES
// --------------------------------------------------

// --------------------------------------------------
// PUBLIC METHODS
// --------------------------------------------------

int XYZ_x();
int XYZ_y();
int XYZ_z();

void XYZ_init(Timer* t);
void XYZ_commands();

void XYZ_process_interrupt();

#endif	/* XYZ_H */

