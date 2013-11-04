/****************************************************
* Communication Module
* Prefix: COMM_
****************************************************/

#include "communication.h"

// --------------------------------------------------
// PRIVATE CONSTANTS
// --------------------------------------------------

#define USB_BAUD 9600
#define BLUETOOTH_BAUD 9600

#define END_COMMAND '\n'
#define DELIMITER ' '

// --------------------------------------------------
// PRIVATE VARIABLES
// --------------------------------------------------

HardwareSerial bluetooth = HardwareSerial();

String usb_rx_buffer = "";
String bluetooth_rx_buffer = "";

// --------------------------------------------------
// PRIVATE METHODS
// --------------------------------------------------

void COMM_send_command(String command) {
  COMM_send_usb_command(command);
  COMM_send_bluetooth_command(command);
}

void COMM_send_command(String command, int arg) {
  COMM_send_usb_command(command, arg);
  COMM_send_bluetooth_command(command, arg);
}

void COMM_send_usb_command(String command) {
  Serial.print(command);
  Serial.print(END_COMMAND);
}

void COMM_send_usb_command(String command, int arg) {
  Serial.print(command);
  Serial.print(DELIMITER);
  Serial.print(arg);
  Serial.print(END_COMMAND);
}

void COMM_send_bluetooth_command(String command) {
  //Serial.println("Command sent over bluetooth: " + command);
  bluetooth.print(command);
  bluetooth.print(END_COMMAND);
}

void COMM_send_bluetooth_command(String command, int arg) {
  //Serial.println("Command sent over bluetoth: " + command + ", arg: " + String(arg));
  bluetooth.print(command);
  bluetooth.print(DELIMITER);
  bluetooth.print(arg);
  bluetooth.print(END_COMMAND);
}

void COMM_parse_command(String command) {
  Serial.println("Command received: " + command);
  /*
  int delimiter_index = command.indexOf(DELIMITER);
  if(delimiter_index != -1) {
    String command_name = command.substring(0, delimiter_index);
    String arg_str = command.substring(delimiter_index+1);
      int arg = arg_str.toInt();
      if ((arg == 0) && (arg_str != "0"))
        Serial.println("Integer argument converted to zero, ignoring!!");
      else
        CMD_command_received(command_name, arg);
  } else {
    CMD_command_received(command);
  }
  */
}

void COMM_update() {
  
  if(Serial.available()) {
    usb_rx_buffer = usb_rx_buffer + String((char)Serial.read());
    //Serial.println("USB buffer increased: " + usb_rx_buffer);
    
    int end_command_index = usb_rx_buffer.indexOf(END_COMMAND);
    
    if(end_command_index != -1) {
      String command = usb_rx_buffer.substring(0, end_command_index);
      usb_rx_buffer = usb_rx_buffer.substring(end_command_index+1);
      COMM_parse_command(command);
    }
  }
  
  if(bluetooth.available()) {
    bluetooth_rx_buffer = bluetooth_rx_buffer + String((char)bluetooth.read());
    //Serial.println("Bluesmirf buffer increased: " + bluetooth_rx_buffer);
    
    int end_command_index = bluetooth_rx_buffer.indexOf(END_COMMAND);
    
    if(end_command_index != -1) {
      String command = bluetooth_rx_buffer.substring(0, end_command_index);
      bluetooth_rx_buffer = bluetooth_rx_buffer.substring(end_command_index+1);
      COMM_parse_command(command);
    }
  }
}

void COMM_init(Timer* t) {
  
  Serial.begin(USB_BAUD);
  bluetooth.begin(BLUETOOTH_BAUD);
  
  t->every(COMM_UPDATE_RATE, COMM_update);
  
  Serial.println("COMM: Communication module initialized!");
}

void COMM_commands() {
  
  // Commands
}

