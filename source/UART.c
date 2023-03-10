#include <queue.h>
#include "UART.h"
#include "Utilities.h"

extern queue *buffer;

// Inicializa la comunicación serial con los valores requeridos
void uart_iniciar( UART_flow *ser_com){
    queue_init(&buffer, BUFFER_SIZE);
	uart_config_t config = ser_com->config;
    UART_GetDefaultConfig(&config); // Se configura UART por defecto
    // Se aplican configuraciones del requerimiento
    config.baudRate_Bps = 2400; 
    config.enableTx = true; 
    config.enableRx = true; 
    config.parityMode = kUART_ParityDisabled; 
    config.stopBitCount = kUART_OneStopBit;
    ser_com->flowctr = TRUE;
    //Se inicializa el UART con la configuración deseada
    UART_Init(UART0, &config, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    UART_EnableTx(UART0, true); // Enable UART transmitter
}


//Detiene la comunicación serial
void uart_stop(UART_flow *ser_com){
    if(!(ser_com->flowctr)){
        UART0->D = XOFF;
        UART_EnableTx(UART0, false); // disable UART transmitter
    }
}

//Reanuda la comunicación serial
void uart_start(UART_flow *ser_com){
    if((ser_com->flowctr & queue_empty(&buffer))){
        UART_EnableTx(UART0, true); // Enable UART transmitter
        UART0->D = XON;
    }
}

char receive_data(UART_flow *ser_com){

    if (kUART_RxActiveFlag) {
        //Tm_Inicie_timeout();
		if (!queue_full(&buffer)) {
			ser_com->flowctr = TRUE;
            queue_in(&buffer, UART0);
		} else {
			ser_com->flowctr = FALSE;
            uart_stop(&ser_com);
		}
	}
}

