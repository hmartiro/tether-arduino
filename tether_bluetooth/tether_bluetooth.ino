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
  //XYZ_init(&t);
  
  Serial.println("");
  Serial.println("Tether initialized!");
}

void loop() {
  
  t.update();
}

/*
const byte ledPin = 13; // the pin that the LED is attached to

float l_zero = 0, l_scale = 0, rotx_zero = 0, rotx_scale = 0, roty_zero = 0, roty_scale = 0;

double l = 0, rotx = 0, roty = 0;
double x = 0, y = 0, z = 0;

boolean calibrated = false;

void setup() 
{ 
  Serial.begin(9600); //start serial comm at 9600 baud
  pinMode(ledPin, OUTPUT);  // initialize the LED pin as an output
}

void loop()
{
  if(!calibrated) {
    calibrate();
  } 
  
  l = (((double)analogRead(0))-l_zero)*l_scale;
  rotx = (((double)analogRead(1))-rotx_zero)*rotx_scale;
  roty = (((double)analogRead(2))-roty_zero)*roty_scale;
  /*
  Serial.print("Position in spherical coords: ");
  Serial.print(l);
  Serial.print(" ");
  Serial.print(rotx);
  Serial.print(" ");
  Serial.println(roty);
  *
  //x - along the metal runner connected to pot axis
  //y - along the plastic hinge connected to pot axis
  // perpendicular to x and y
  x = l*sin(roty);
  y = l*cos(roty)*sin(rotx);
  z = l*cos(roty)*cos(rotx);
  //Serial.print("Position in catersian coords: ");
  Serial.print(x,2);
  Serial.print("\t");
  Serial.print(y,2);
  Serial.print("\t");
  Serial.println(z,2);
  delay(300);
}

void calibrate()
{
  float temp;
  Serial.println("Calibrating...");
  Serial.println("Touch (X ,Y ,Z ) points as requested");
  Serial.println("Touch (0 ,0 ,10 )");
  while (!Serial.available()) {
  }
  Serial.read();
  l_zero = analogRead(0);
  rotx_zero = analogRead(1);
  roty_zero = analogRead(2);
  Serial.println("Touch (0 ,0 ,5 )");
  while (!Serial.available()) {
  }
  Serial.read();
  l_scale = l_zero;  
  temp = analogRead(0);
  l_zero = l_zero - 2*(l_zero - temp);
  l_scale = 5.0/(l_scale - temp);
  Serial.println("Touch (3 ,0 ,10)");
  while (!Serial.available()) {
  }
  Serial.read();
  roty_scale = (0.29145/((float)analogRead(2) - roty_zero));
  Serial.println("Touch (0 ,3 ,10)");
  while (!Serial.available()) {
  }
  Serial.read();
  rotx_scale = (0.29145/((float)analogRead(1) - rotx_zero));
  Serial.println("Calibration done!");  
  calibrated = true;
}*/
