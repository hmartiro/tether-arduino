/****************************************************
* Tether
* -----------------------
* Mishel Johns
* Hayk Martirosyan
* Scott McDonald
* Mark Stauber
****************************************************/

// Software serial capability
#include <SoftwareSerial.h>

// Timer library that makes events easy
#include "Timer.h"

// Encoder library
#include "Encoder.h"

// Module that outputs position readings
#include "xyz.h"

// Module that handles serial and bluetooth
// command parsing and communication
#include "communication.h"

// --------------------------------------------------
// CONSTANTS
// --------------------------------------------------

// --------------------------------------------------
// VARIABLES
// --------------------------------------------------

// Timer object which can handle 10 events at a time
Timer t;

// --------------------------------------------------
// METHODS
// --------------------------------------------------

void setup() { 
  
  COMM_init(&t);
  XYZ_init(&t);
  
  Serial.println("");
  Serial.println("Tether initialized!");
}

void loop() {
  
  t.update();
}
