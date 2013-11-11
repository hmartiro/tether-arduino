/****************************************************
* Position Tracking Module
* Prefix: XYZ_
****************************************************/

#include "xyz.h"

// --------------------------------------------------
// PRIVATE CONSTANTS
// --------------------------------------------------

#define X_POT_PIN A1
#define Y_POT_PIN A2

// Encoder pins
#define ENC_A 4
#define ENC_B 6

// What is the max value of the analog read
#define ADC_MAX 1024.0

// What is the range of phi and theta?
#define JOYSTICK_ANGLE 48.0

// Math helpers
#define DEG_TO_RAD PI/180.0
#define RAD_TO_DEG 180.0/PI

// --------------------------------------------------
// PRIVATE VARIABLES
// --------------------------------------------------

// Encoder object
//Encoder enc(ENC_A, ENC_B);

// Potentiometer values
int x_pot = 0;
int y_pot = 0;

// Encoder counter
int enc_counter = 0;

volatile int encoderRawPos = 0;

static boolean rotatingA = false;
static boolean rotatingB = false;

// Zero points
float x_pot_zero = ADC_MAX/2;
float y_pot_zero = ADC_MAX/2;
float enc_zero = 0.0;

// Scaling factors
float theta_scale = JOYSTICK_ANGLE / ADC_MAX * DEG_TO_RAD;
float phi_scale = JOYSTICK_ANGLE / ADC_MAX * DEG_TO_RAD;
float rho_scale = 0.032; // Centimeters per encoder tick

// Spherical coordinates
float theta = 0.0;
float phi = 0.0;
float rho = 0.0;

// These variables store the position in millimeters
// relative to the tether's exit point
float x = 0.0;
float y = 0.0;
float z = 0.0;

// --------------------------------------------------
// PRIVATE METHODS
// --------------------------------------------------

int XYZ_x() {
  return x;
}

int XYZ_y() {
  return y;
}

int XYZ_z() {
  return z;
}

void XYZ_process_interrupt() {
  
  boolean rotatingA2 = rotatingA;
  boolean rotatingB2 = rotatingB;
  
  rotatingA = false;
  rotatingB = false;
  
  while(rotatingA2 || rotatingB2) {
    delayMicroseconds(100);
    
    int enc_a = digitalRead(ENC_A);
    int enc_b = digitalRead(ENC_B);
    
    if (rotatingA2) {
      
      if ((enc_a == 1) && (enc_b == 1)) encoderRawPos++;
      if ((enc_a == 1) && (enc_b == 0)) encoderRawPos--;
      if ((enc_a == 0) && (enc_b == 1)) encoderRawPos--;
      if ((enc_a == 0) && (enc_b == 0)) encoderRawPos++;
      rotatingA2 = false; // Reset the flag back to false
    }
    
    if (rotatingB2) {
      //if ((enc_a == 1) && (enc_b == 1)) encoderRawPos--;
      //if ((enc_a == 0) && (enc_b == 1)) encoderRawPos++;
      rotatingB2 = false;
    }
  }
}

void XYZ_update() {
  
  x_pot = analogRead(X_POT_PIN);
  y_pot = analogRead(Y_POT_PIN);
  //enc_counter = enc.read();
  enc_counter = encoderRawPos;
  
  theta = ((float)x_pot - x_pot_zero) * theta_scale;
  phi = ((float)y_pot - y_pot_zero) * phi_scale;
  rho = ((float)enc_counter - enc_zero) * rho_scale;
  
  z = rho / sqrt(1 + pow(tan(theta),2) + pow(tan(phi),2));
  x = -tan(theta) * z;
  y = -tan(phi) * z;
}

void XYZ_print() {
  Serial.print("rho: " + String(rho));
  Serial.print(", theta: " + String(theta * RAD_TO_DEG));
  Serial.print(", phi: " + String(phi * RAD_TO_DEG));
  Serial.print(", X: " + String(x));
  Serial.print(", Y: " + String(y));
  Serial.println(", Z: " + String(z));
  
  COMM_send_bluetooth_command(String(x) + " " + String(y) + " " + String(z));
}

void encoderA() {
  rotatingA = true;
}
void encoderB() {
  rotatingB = true;
}

void XYZ_init(Timer* t) {
  
  pinMode(ENC_A, INPUT_PULLUP); 
  digitalWrite(ENC_A, HIGH);       
  pinMode(ENC_B, INPUT_PULLUP); 
  digitalWrite(ENC_B, HIGH); 

  attachInterrupt(ENC_A, encoderA, CHANGE);
  attachInterrupt(ENC_B, encoderB, CHANGE);
  
  t->every(XYZ_UPDATE_RATE, XYZ_update);
  t->every(XYZ_PRINT_RATE, XYZ_print);
}

void XYZ_commands() {  
  
  // Commands
  //CMD_check_command(String("X"), XYZ_print);
}
