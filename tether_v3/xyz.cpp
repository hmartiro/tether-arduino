/****************************************************
* Position Tracking Module
* Prefix: XYZ_
****************************************************/

#include "xyz.h"

// --------------------------------------------------
// PRIVATE CONSTANTS
// --------------------------------------------------

#define SMOOTHING_MAX 50
#define SMOOTHING_DEFAULT 50

#define X_POT_PIN A0
#define Y_POT_PIN A1

// Encoder pins
#define ENC_A 19
#define ENC_B 18

// Button pins
#define BUTTON_1_PIN 7

// What is the max value of the analog read
#define ADC_MAX 1024.0 //65536.0

// What is the range of phi and theta?
#define JOYSTICK_ANGLE 48.0

// Math helpers
#define DEG_TO_RAD PI/180.0
#define RAD_TO_DEG 180.0/PI

// Default scaling
#define THETA_SCALE JOYSTICK_ANGLE / ADC_MAX * DEG_TO_RAD
#define PHI_SCALE JOYSTICK_ANGLE / ADC_MAX * DEG_TO_RAD
#define RHO_SCALE 0.032 // Centimeters per encoder tick

// Default offsets
#define X_POT_ZERO ADC_MAX/2
#define Y_POT_ZERO ADC_MAX/2
#define ENC_ZERO 0.0

// Orientations
#define ORIENTATION_PX_PY 0
#define ORIENTATION_PX_NY 1
#define ORIENTATION_NX_NY 2
#define ORIENTATION_NX_PY 3

// --------------------------------------------------
// PRIVATE VARIABLES
// --------------------------------------------------

// Encoder object
Encoder enc(ENC_A, ENC_B);

// Potentiometer values
int x_pot = 0;
int y_pot = 0;

// Encoder counter
int enc_counter = 0;

static boolean rotatingA = false;
static boolean rotatingB = false;

// Number of past values to average with
int smoothing;

// Zero points
float x_pot_zero;
float y_pot_zero;
float enc_zero;

// Scaling factors
float theta_scale;
float phi_scale;
float rho_scale;

// Spherical coordinates
float theta = 0.0;
float phi = 0.0;
float rho = 0.0;

// These variables store the position in millimeters
// relative to the tether's exit point
float x = 0.0;
float y = 0.0;
float z = 0.0;

// This array holds the past values, used for smoothing
int x_hist[SMOOTHING_MAX];
int y_hist[SMOOTHING_MAX];
int z_hist[SMOOTHING_MAX];

// These variables hold the integer values for x, y, z
// that represent tenths of a millimeter
int xInt = 0;
int yInt = 0;
int zInt = 0;

// These hold the last values of xInt, yInt, zInt
int xIntLast = 0;
int yIntLast = 0;
int zIntLast = 0;

// This holds the button status
int button_1_pressed;

// This boolean says whether position changes are counting
boolean tracking;
float x_pot_stop_state;
float y_pot_stop_state;
float rho_stop_state;

int orientation;

// --------------------------------------------------
// PRIVATE METHODS
// --------------------------------------------------

void XYZ_reset() {
  smoothing = SMOOTHING_DEFAULT;
  x_pot_zero = X_POT_ZERO;
  y_pot_zero = Y_POT_ZERO;
  enc_zero = ENC_ZERO;
  theta_scale = THETA_SCALE;
  phi_scale = PHI_SCALE;
  rho_scale = RHO_SCALE;
  tracking = true;
  button_1_pressed = FALSE;
  orientation = ORIENTATION_PX_PY;
  for(int i = 0; i < SMOOTHING_MAX; i++) {
    x_hist[i] = 0;
    y_hist[i] = 0;
    z_hist[i] = 0;
  }
  COMM_send_bluetooth_command("AOK positions reset");
}

// --------------------------------------------------
// PRIVATE METHODS
// --------------------------------------------------

int XYZ_button_1() {
  return button_1_pressed;
}

int XYZ_x() {
  return xInt;
}

int XYZ_y() {
  return yInt;
}

int XYZ_z() {
  return zInt;
}

int XYZ_change_x() {
  return xInt - xIntLast;
}

int XYZ_change_y() {
  return yInt - yIntLast;
}

int XYZ_change_z() {
  return xInt - xIntLast;
}

void XYZ_zero() {
  x_pot_zero = x_pot;
  y_pot_zero = y_pot;
  enc_zero = enc_counter;
  COMM_send_bluetooth_command("AOK zero position set");
}

void XYZ_tracking(int state) {
  if(state == 0) {
    COMM_send_bluetooth_command("AOK tracking off");
    if(tracking == state) return;
    tracking = 0;
    x_pot_stop_state = theta/theta_scale;
    y_pot_stop_state = phi/phi_scale;
    rho_stop_state = rho/rho_scale;
  } else if (state == 1) {
    COMM_send_bluetooth_command("AOK tracking on");
    if(tracking == state) return;
    tracking = 1;
    XYZ_zero();
    x_pot_zero -= x_pot_stop_state;
    y_pot_zero -= y_pot_stop_state;
    enc_zero -= rho_stop_state;
  } else {
    COMM_send_bluetooth_command("ERROR invalid tracking state, need 0 or 1");
  }
}

void XYZ_smoothing(int N) {
  if ((N > 0) && (N < SMOOTHING_MAX)) {
    smoothing = N;
    COMM_send_bluetooth_command("AOK smoothing set to " + String(N));
  } else {
    COMM_send_bluetooth_command("ERROR smoothing out of range");
  }
}

void XYZ_update() {
  
  x_pot = analogRead(X_POT_PIN);
  y_pot = analogRead(Y_POT_PIN);
  enc_counter = enc.read();
  
  theta = ((float)x_pot - x_pot_zero) * theta_scale;
  phi = ((float)y_pot - y_pot_zero) * phi_scale;
  rho = ((float)enc_counter - enc_zero) * rho_scale;
  
  z = rho / sqrt(1 + pow(tan(theta),2) + pow(tan(phi),2));
  x = -tan(theta) * z;
  y = -tan(phi) * z;
  
  if(tracking) {
    long xTotal = 0;
    long yTotal = 0;
    long zTotal = 0;
    
    for(int i = smoothing-1; i > 0; i--) {
      x_hist[i] = x_hist[i-1];
      y_hist[i] = y_hist[i-1];
      z_hist[i] = z_hist[i-1];
      
      xTotal += x_hist[i];
      yTotal += y_hist[i];
      zTotal += z_hist[i];
    }
    
    x_hist[0] = (int)(x * 100);
    y_hist[0] = (int)(y * 100);
    z_hist[0] = (int)(z * 100);
    
    xTotal += x_hist[0];
    yTotal += y_hist[0];
    zTotal += z_hist[0];
    
    xIntLast = xInt;
    yIntLast = yInt;
    zIntLast = zInt;

    zInt = zTotal/smoothing;
    
    if(orientation == ORIENTATION_PX_PY) {
      xInt = xTotal/smoothing;
      yInt = -yTotal/smoothing;
    } else if(orientation == ORIENTATION_NX_PY) {
      xInt = yTotal/smoothing;
      yInt = -xTotal/smoothing;
    } else if(orientation == ORIENTATION_NX_NY) {
      xInt = xTotal/smoothing;
      yInt = -yTotal/smoothing;
    } else if(orientation == ORIENTATION_PX_NY) {
      xInt = yTotal/smoothing;
      yInt = -xTotal/smoothing;
    }
    
  }
  
  int button_1_pressed_new = digitalRead(BUTTON_1_PIN);
  //Serial.println("btn_new: " + String(button_1_pressed_new) + ", btn: " + String(button_1_pressed));
  if(button_1_pressed_new != button_1_pressed) {
    COMM_send_bluetooth_command("BTN_1", button_1_pressed_new);
    HID_button_1_event(button_1_pressed_new);
  }
  button_1_pressed = button_1_pressed_new;
}

void XYZ_set_orientation(int arg) {
  orientation = arg;
}

void XYZ_set_orientation_command(int arg) {
  if ((arg < ORIENTATION_PX_PY) || (arg > ORIENTATION_NX_PY)) {
    COMM_send_bluetooth_command("ERROR bad arg for orientation, 0-3");
    return;
  }
  XYZ_set_orientation(arg);
  COMM_send_bluetooth_command("AOK set orientation to " + String(arg));
}

void XYZ_print() {
  Serial.print("millis: " + String(millis()));
  Serial.print(", rho: " + String(rho));
  Serial.print(", theta: " + String(theta * RAD_TO_DEG));
  Serial.print(", phi: " + String(phi * RAD_TO_DEG));
  Serial.print(", X: " + String(xInt));
  Serial.print(", Y: " + String(yInt));
  Serial.println(", Z: " + String(zInt));
  
  COMM_send_bluetooth_command("POS " + String(xInt) + " " + String(yInt) + " " + String(zInt));
}

void XYZ_init(Timer* t) {
  
  //analogReadRes(16);
  
  pinMode(X_POT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  
  XYZ_reset();
  
  t->every(XYZ_UPDATE_RATE, XYZ_update);
  t->every(XYZ_PRINT_RATE, XYZ_print);
  
  Serial.println("XYZ: Position tracking module initialized!");
}

void XYZ_commands() {  
  
  // Commands
  COMM_check_command(String("ZERO"), XYZ_zero);
  COMM_check_command(String("RESET"), XYZ_reset);
  COMM_check_command(String("SMOOTHING"), XYZ_smoothing);
  COMM_check_command(String("TRACKING"), XYZ_tracking);
  COMM_check_command(String("ORIENTATION"), XYZ_set_orientation_command);
}
