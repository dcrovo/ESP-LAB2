
// Inicializa la comunicación serial con los valores requeridos
void uart_iniciar(uart_config_t &config;){
     UART_GetDefaultConfig(&config); // Se configura UART por defecto
    // Se aplican configuraciones del requerimiento
    config.baudRate_Bps = 2400; 
    config.enableTx = true; 
    config.enableRx = true; 
    config.parityMode = kUART_ParityDisabled; 
    config.stopBitCount = kUART_OneStopBit;
    //Se inicializa el UART con la configuración deseada
    UART_Init(UART1, &config, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    UART_EnableTx(UART1, true); // Enable UART transmitter
}


//Detiene la comunicación serial
void uart_stop(UART_flow *ser_com){
    if(!(ser_com->flowctr)){
        UART_EnableTx(UART1, false); // disable UART transmitter
        UART1->D = XOFF;
    }
}

//Reanuda la comunicación serial
void uart_start(UART_flow *ser_com){
    if((ser_com->flowctr)){
        UART_EnableTx(UART1, true); // Enable UART transmitter
        UART1->D = XOFF;
    }
}

