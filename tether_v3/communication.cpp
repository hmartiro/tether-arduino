/****************************************************
* Communication Module
* Prefix: COMM_
****************************************************/

#include "communication.h"

// --------------------------------------------------
// PRIVATE CONSTANTS
// --------------------------------------------------

#define USB_BAUD 115200
#define BLUETOOTH_BAUD 115200

#define END_COMMAND '\n'
#define DELIMITER ' '

#define NO_ARGUMENT -3333

// --------------------------------------------------
// PRIVATE VARIABLES
// --------------------------------------------------

HardwareSerial bluetooth = HardwareSerial();

String usb_rx_buffer = "";
String bluetooth_rx_buffer = "";

String last_command = String();
int last_arg = -1;

// --------------------------------------------------
// PRIVATE METHODS
// --------------------------------------------------

static int COMM_check_command_string(String* pStr) {
  
  String command = *pStr;
  
  if (command == last_command)
    return TRUE;
  return FALSE;
}

static void COMM_check_all_commands() {
  COMM_commands();
  XYZ_commands();
  HID_commands();
}

static void COMM_command_received(String command) {
  //Serial.println("COMM command received: " + command);
  last_command = command;
  last_command.toUpperCase();
  last_arg = NO_ARGUMENT;
  COMM_check_all_commands();
}

static void COMM_command_received(String command, int arg) {
  //Serial.println("COMM command received: " + command + ", arg: " + String(arg));
  last_command = command;
  last_command.toUpperCase();
  last_arg = arg;
  COMM_check_all_commands();
}

static void COMM_parse_command(String command) {
  Serial.println("Command received: " + command);
  
  int delimiter_index = command.indexOf(DELIMITER);
  if(delimiter_index != -1) {
    String command_name = command.substring(0, delimiter_index);
    String arg_str = command.substring(delimiter_index+1);
      int arg = arg_str.toInt();
      if ((arg == 0) && (arg_str != "0")) {
        Serial.println("Integer argument converted to zero, ignoring!!");
        COMM_send_bluetooth_command("ERROR bad command argument, not int");
      } else
        COMM_command_received(command_name, arg);
  } else {
    COMM_command_received(command);
  }
}

static void COMM_reset() {
  Serial.println("COMM module reset!");
}

// --------------------------------------------------
// PUBLIC METHODS
// --------------------------------------------------

void COMM_check_command(String pCommand, void (*callback)(void)) {
  
  if(last_arg != NO_ARGUMENT) {
    //Serial.println("Ignoring command, argument present!");
    return;
  }
  
  if(COMM_check_command_string(&pCommand))
    callback();
}

void COMM_check_command(String pCommand, void (*callback)(int)) {
  
  if(last_arg == NO_ARGUMENT) {
    return;
  }
  
  if(COMM_check_command_string(&pCommand))
    callback(last_arg);
}

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

void COMM_send_bluetooth_command(String command, int arg) {
  Serial.println("Command sent over bluetoth: " + command + ", arg: " + String(arg));
  Serial2.print(command);
  Serial2.print(DELIMITER);
  Serial2.print(arg);
  Serial2.print(END_COMMAND);
}

void COMM_send_bluetooth_command(String command) {
  Serial.println("Command sent over bluetooth: " + command);
  Serial2.print(command);
  Serial2.print(END_COMMAND);
}

void COMM_send_bluetooth_command(byte* bytes, int len) {
  int i;
  for (i = 0; i < len; i++) {
    Serial2.write(bytes[i]);
    //Serial.write(bytes[i]);
  }
  Serial2.print(END_COMMAND);
  Serial.println("wrote " + String(i+1) + " bytes to bluetooth");
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
  
  if(Serial2.available()) {
    bluetooth_rx_buffer = bluetooth_rx_buffer + String((char)Serial2.read());
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
  Serial2.begin(BLUETOOTH_BAUD);
  
  t->every(COMM_UPDATE_RATE, COMM_update);
  
  Serial.println("COMM: Communication module initialized!");
}

void COMM_commands() {
  
  // Commands
  COMM_check_command(String("RESET"), COMM_reset);
}

