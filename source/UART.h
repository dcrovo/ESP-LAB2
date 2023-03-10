//Librerias
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_uart.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

//Pines del UART
#define UART_TX_PIN 1U
#define UART_RX_PIN 2U

//Buffer del UART
#define BUFFER_SIZE 64

/* Define XON/XOFF characters */
#define XON 0x11
#define XOFF 0x13

typedef struct UART_flow{
    uart_config_t config;
    char flowctr;
}UART_flow;


// Inicializa la comunicación serial con los valores requeridos
void uart_iniciar(UART_flow *ser_com);

//Detiene la comunicación serial
void uart_stop(UART_flow *ser_com);

//Reanuda la comunicación serial
void uart_start(UART_flow *ser_com);