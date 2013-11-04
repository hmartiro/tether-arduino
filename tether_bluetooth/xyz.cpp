/****************************************************
* Position Tracking Module
* Prefix: XYZ_
****************************************************/

#include "xyz.h"

// --------------------------------------------------
// PRIVATE CONSTANTS
// --------------------------------------------------

// --------------------------------------------------
// PRIVATE VARIABLES
// --------------------------------------------------

// --------------------------------------------------
// PRIVATE METHODS
// --------------------------------------------------

void XYZ_init(Timer* t) {
  
  t->every(XYZ_UPDATE_RATE, XYZ_update);
  t->every(XYZ_PRINT_RATE, XYZ_print);
  
}

void XYZ_commands() {  
  
  // Commands
  //CMD_check_command(String("STOP"), XYZ_stop);
}

