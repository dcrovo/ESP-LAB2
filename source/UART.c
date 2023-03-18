
#include "UART.h"
#include "MKL46Z4.h"
#include "Utilities.h"

/*extern queue buffer;*/

/*-----------------------------------------
    Routine to initialise the UART module
-------------------------------------------*/
void initUart1(){

	/*Set UART1 to TX/RX from external connections*/
	SIM->SOPT5 &= ~((1<<4)|(1<<5)|(1<<6));
	/*Enable UART clock gate position 11*/
    SIM->SCGC4 |= 1<<11;
    /*Enable clock gate to PORTE*/
    SIM->SCGC5 |= 1<<13;
    /*Set PORTE 0 and 1 to mux*/

    PORTE->PCR[0] |= 1<<8 | 1<<9;
    PORTE->PCR[1] |= 1<<8 | 1<<9;


    /*------------------*
     * UART1 Configuration
     *------------------*/

    /*Disable UART before configuring*/
    UART1->C2 &= ~((1<<2)|(1<<3));
    /* Set baud rate BDL =  clock/(16*9600)*/
    UART1->BDH = 0x00;
    UART1->BDL = 156;

    /*Non parity, 8bit normal mode*/
    UART1->C1 = 0x00;
    /* Non TXINV not interrupts*/
     UART1->C3 = 0x00;
    /*Clear LBKDIF, RXDGIF, Non RXINV*/
    UART1->S2 |= 0xC0;

    /*Enable Tx and Rx*/
    UART1->C2 = (1<<2)|(1<<3);

};

void sendChar(char data)
{
	while(!(UART1->S1 & 0x80U));
	UART1->D =data;


};
char receiveChar ()
{
	  while (!(UART1->S1 & 0x20));
	  return (UART1->D);
};
void stopCommunication(){
    sendChar(XOFF);
};
void startCommunication(){
    sendChar(XON);
};
