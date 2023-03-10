#include "UART.h"
#include "queue.h"
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
    if((ser_com->flowctr)){
        UART_EnableTx(UART0, true); // Enable UART transmitter
        UART0->D = XON;
    }
}

char receive_data(UART_flow *ser_com){
    if ((UART0_S1 & (1 << UART0_S1_RDRF_SHIFT))) {
		queue_in(&buffer, UART0_D);
		if (!queue_full(&buffer)) {
			ser_com->flowctr = TRUE;
		} else {
			ser_com->flowctr = FALSE;
            uart_stop(&ser_com);
		}
	}
}

