/****************************************************
* Communication Module
* Prefix: COMM_
****************************************************/

#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

#include <Arduino.h>

#include "Timer.h"

#include "xyz.h"

// --------------------------------------------------
// PUBLIC CONSTANTS
// --------------------------------------------------

#define COMM_UPDATE_RATE 2

#define TRUE 1
#define FALSE 0

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
void COMM_send_bluetooth_command(byte* bytes, int len);

void COMM_check_command(String pCommand, void (*callback)(void));
void COMM_check_command(String pCommand, void (*callback)(int));

#endif	/* COMMUNICATION_H */

