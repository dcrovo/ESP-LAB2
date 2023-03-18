#ifndef UART_H_
#define UART_H_


#include "fsl_device_registers.h"
#include "CircularQueue.h"
#include "fsl_debug_console.h"
#include "fsl_uart.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#define XON  0x11
#define XOFF 0x13

void initUart1();

void sendChar    (char data);
char receiveChar ();
void stopCommunication();
void startCommunication();

#endif
