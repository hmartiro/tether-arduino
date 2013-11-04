/****************************************************
* Communication Module
* Prefix: COMM_
****************************************************/

#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#include "Timer.h"

// --------------------------------------------------
// PUBLIC CONSTANTS
// --------------------------------------------------

#define COMM_UPDATE_RATE 2

// --------------------------------------------------
// PUBLIC VARIABLES
// --------------------------------------------------

// --------------------------------------------------
// PUBLIC METHODS
// --------------------------------------------------

void COMM_init(Timer* t);
void COMM_commands();

void COMM_send_command(String command);
void COMM_send_command(String command, int arg);

void COMM_send_usb_command(String command);
void COMM_send_usb_command(String command, int arg);

void COMM_send_bluetooth_command(String command);
void COMM_send_bluetooth_command(String command, int arg);

#endif	/* COMMUNICATION_H */

