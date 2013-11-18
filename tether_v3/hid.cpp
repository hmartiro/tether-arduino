/****************************************************
* Position Tracking Module
* Prefix: HID_
****************************************************/

#include "hid.h"

// --------------------------------------------------
// PRIVATE CONSTANTS
// --------------------------------------------------

// --------------------------------------------------
// PRIVATE VARIABLES
// --------------------------------------------------

int mouse_on;
int joystick_on;

// --------------------------------------------------
// PUBLIC METHODS
// --------------------------------------------------

void HID_reset() {
  HID_set_mouse(FALSE);
  HID_set_joystick(FALSE);
}

void HID_set_joystick(int arg) {
  joystick_on = arg;
  if (joystick_on == TRUE)
    mouse_on = FALSE;
}

void HID_set_mouse(int arg) {
  mouse_on = arg;
  if (mouse_on == TRUE)
    joystick_on = FALSE;
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
      if(isButtonPressed == TRUE)
        Mouse.press(MOUSE_LEFT);
    } else {
      if(isButtonPressed == FALSE)
        Mouse.release(MOUSE_LEFT);
    }
  }
  
  if(joystick_on) {
    
  }
}

void HID_update_mouse() {
  
    // Handle mouse move
    int moveX = XYZ_change_x();
    int moveY = XYZ_change_y();
    Mouse.move(moveX, moveY);
}

void HID_update_joystick() {
  
}

void HID_update() {
  //Serial.println("Mouse: " + String(mouse_on) + ", Joystick: " + String(joystick_on));
  
  if(mouse_on) {
    HID_update_mouse();
  }
  
  if(joystick_on) {
    HID_update_joystick();
  }
}

void HID_init(Timer* t) {
  
  HID_reset();
  
  t->every(HID_UPDATE_RATE, HID_update);
  
  Serial.println("HID: Human interface device module initialized!");
}

void HID_commands() {  
  
  // Commands
  COMM_check_command(String("MOUSE"), HID_set_mouse_command);
  COMM_check_command(String("JOYSTICK"), HID_set_joystick_command);
}
