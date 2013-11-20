/****************************************************
* Position Tracking Module
* Prefix: HID_
****************************************************/

#include "hid.h"

// --------------------------------------------------
// PRIVATE CONSTANTS
// --------------------------------------------------

#define MOUSE_MOD 3
#define MOUSE_SCALE 0.75

#define SCROLL_MOD 10
#define SCROLL_SCALE -0.1

#define SCROLL_TIMEOUT 500

// --------------------------------------------------
// PRIVATE VARIABLES
// --------------------------------------------------`

int mouse_on;
int joystick_on;

// Running values for mouse movement
float runningMouseX;
float runningMouseY;
float runningMouseZ;

int scrolling;
long scrolling_time;
long z_change_time;

// --------------------------------------------------
// PUBLIC METHODS
// --------------------------------------------------

void HID_reset() {
  runningMouseX = 0.0;
  runningMouseY = 0.0;
  runningMouseZ = 0.0;
  scrolling = FALSE;
  HID_set_mouse(FALSE);
  HID_set_joystick(FALSE);
}

void HID_set_joystick(int arg) {
  joystick_on = arg;
  if (joystick_on == TRUE)
    mouse_on = FALSE;
  else {    
    Joystick.X(512);
    Joystick.Y(512);
    Joystick.Z(512);
    Joystick.button(1, FALSE);
    Joystick.button(2, FALSE);
  }
}

void HID_set_mouse(int arg) {
  mouse_on = arg;
  if (mouse_on == TRUE) {
    Joystick.X(512);
    Joystick.Y(512);
    Joystick.Z(512);
    Joystick.button(1, FALSE);
    Joystick.button(2, FALSE);
    joystick_on = FALSE;
  }
}

void HID_set_joystick_command(int arg) {
 if((arg != TRUE) && (arg != FALSE)) {
   COMM_send_bluetooth_command("ERROR invalid arg for JOYSTICK, 1 or 0");
   return;
 }
 HID_set_joystick(arg);
 COMM_send_bluetooth_command("AOK mouse set to " + String(arg));
}

void HID_set_mouse_command(int arg) {
 if((arg != TRUE) && (arg != FALSE)) {
   COMM_send_bluetooth_command("ERROR invalid arg for MOUSE, 1 or 0");
   return;
 }
 HID_set_mouse(arg);
 COMM_send_bluetooth_command("AOK mouse set to " + String(arg));
}

// --------------------------------------------------
// PRIVATE METHODS
// --------------------------------------------------

void HID_button_1_event(int isButtonPressed) {
  
  if(mouse_on) {
    
    // Handle left click
    if(Mouse.isPressed(MOUSE_LEFT) == FALSE) {
      if(isButtonPressed == TRUE) {
        if(XYZ_button_2() == TRUE) {
          Serial.println("MIDDLE PRESSED");
          Mouse.release(MOUSE_RIGHT);
          Mouse.press(MOUSE_MIDDLE);
        } else {
          Serial.println("LEFT PRESSED");
          Mouse.press(MOUSE_LEFT);
        }
      }
    } else {
      if(isButtonPressed == FALSE) {
        if(XYZ_button_2() == TRUE) {
        } else {
          Serial.println("LEFT RELEASED");
          Mouse.release(MOUSE_LEFT);
        }
      }
    }
    
    if(isButtonPressed == FALSE) {
      if(Mouse.isPressed(MOUSE_MIDDLE) == TRUE) {
        Serial.println("MIDDLE RELEASED");
        Mouse.release(MOUSE_MIDDLE);
       } 
    }
  }
  
  if(joystick_on) {
    Joystick.button(1, isButtonPressed);
  }
}

void HID_button_2_event(int isButtonPressed) {
  
  if(mouse_on) {
    
    // Handle left click
    if(Mouse.isPressed(MOUSE_RIGHT) == FALSE) {
      if(isButtonPressed == TRUE) {
        if(XYZ_button_1() == TRUE) {
          Serial.println("MIDDLE pressed");
          Mouse.release(MOUSE_LEFT);
          Mouse.press(MOUSE_MIDDLE);
        } else {
          Serial.println("RIGHT pressed");
          Mouse.press(MOUSE_RIGHT);
        }
      }
    } else {
      if(isButtonPressed == FALSE) {
        if(XYZ_button_1() == TRUE) {
        } else {
          Serial.println("RIGHT released");
          Mouse.release(MOUSE_RIGHT);
        }
      }
    } 
    if(isButtonPressed == FALSE) {
      if(Mouse.isPressed(MOUSE_MIDDLE) == TRUE) {
        Serial.println("MIDDLE RELEASED");
        Mouse.release(MOUSE_MIDDLE);
       } 
    }
  }
  
  if(joystick_on) {
    Joystick.button(2, isButtonPressed);
  }
}

int sign(float f) {
  if(f == 0) return 0;
  if(f < 0) return -1;
  return 1;
}

int far_apart(int a, int b, int res) {
  if((a < b) && (a + res < b)) return TRUE;
  if((a > b) && (a - res > b)) return TRUE;
  return FALSE;
}

void HID_update_mouse() {
  
  if(!mouse_on) return;

  // Handle mouse move
  float moveX = XYZ_change_x();
  float moveY = XYZ_change_y();
  float moveZ = XYZ_change_z();
  
  moveX *= MOUSE_SCALE;
  moveY *= MOUSE_SCALE;
  moveZ *= SCROLL_SCALE;
  
  moveX = pow(abs(moveX), 1.5) * sign(moveX);
  moveY = pow(abs(moveY), 1.5) * sign(moveY);
  moveZ = pow(abs(moveZ), 1.0) * sign(moveZ);
  
  runningMouseX += moveX;
  runningMouseY += moveY;
  runningMouseZ += moveZ;
  
  //Serial.println("X: " + String(runningMouseX) + ", Y: " + String(runningMouseY));
  
  int remX = (int)runningMouseX % MOUSE_MOD;
  int remY = (int)runningMouseY % MOUSE_MOD;
  int remZ = (int)runningMouseZ % SCROLL_MOD;
  
  int moveXInt = runningMouseX - remX;
  int moveYInt = runningMouseY - remY;
  int moveZInt = runningMouseZ - remZ;
  
  runningMouseX -= moveXInt;
  runningMouseY -= moveYInt;
  runningMouseZ -= moveZInt;
  
  if(scrolling == FALSE) moveZInt = 0;
  
  Mouse.move(moveXInt/MOUSE_MOD, -moveYInt/MOUSE_MOD, moveZInt/SCROLL_MOD);
}

void HID_update_joystick() {
  
  if(!joystick_on) return;
  
  int joyX = map(XYZ_x()/2, -500, 500, 0, 1023);
  int joyY = map(XYZ_y()/2, 500, -500, 0, 1023);
  int joyZ = map(XYZ_z()/2, 0, 2300, 0, 1023);
  
  joyX = constrain(joyX, 0, 1023);
  joyY = constrain(joyY, 0, 1023);
  joyZ = constrain(joyZ, 0, 1023);
  
  Serial.println("X: " + String(joyX) + ", Y: " + String(joyY) + ", Z: " + String(joyZ));
  
  Joystick.X(joyX);
  Joystick.Y(joyY);
  Joystick.Z(joyZ);
  
  Joystick.send_now();
}

void HID_update() {
  //Serial.println("Mouse: " + String(mouse_on) + ", Joystick: " + String(joystick_on));
  //HID_update_mouse();
  //Serial.println("button 1: " + String(XYZ_button_1()) + ", button 2: " + String(XYZ_button_2()));
  //Serial.println("LEFT: " + String(Mouse.isPressed(MOUSE_LEFT)) + ", MIDDLE: " + 
  //String(Mouse.isPressed(MOUSE_MIDDLE)) +
  //", RIGHT: " + String(Mouse.isPressed(MOUSE_RIGHT)));
  
  HID_update_joystick();
  
  if(Mouse.isPressed(MOUSE_MIDDLE)) {
    if(abs(XYZ_change_z()) >= 2) {
      Serial.println("SCROLLING ON");
      Mouse.release(MOUSE_MIDDLE);
      scrolling = true;
      scrolling_time = millis();
    }
  }
  
  if(scrolling) {
    
    long scrolling_time_new = millis();
    if(scrolling_time_new - scrolling_time > SCROLL_TIMEOUT) {
      Serial.println("SCROLLING OFF");
      scrolling = false;
    }
    if((abs(XYZ_change_z()) >= 2) && (abs(XYZ_change_x()) + abs(XYZ_change_y()) < 2)) {
      scrolling_time = scrolling_time_new;
    }
  }
}

void HID_init(Timer* t) {
  
  HID_reset();
  Joystick.useManualSend(true);
  
  t->every(HID_UPDATE_RATE, HID_update);
  
  Serial.println("HID: Human interface device module initialized!");
}

void HID_commands() {  
  
  // Commands
  COMM_check_command(String("MOUSE"), HID_set_mouse_command);
  COMM_check_command(String("JOYSTICK"), HID_set_joystick_command);
}
