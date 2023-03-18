/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    ESP-LAB2.c
 * @brief   Application entry point.
 */

/* TODO: insert other include files here. */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"
#include <limits.h>
/*#include <queue.h>*/
#include <stdlib.h>
#include "Time.h"
#include "Display.h"
#include "UART.h"
/*Estados Main*/
#define IDLE     0
#define ACTIVO   1
#define INACTIVO 2

#define NORMAL_MODE 0
#define NEG_MODE 1
/*Variables definidas*/
#define PER_625  0
#define PER_INT  1
#define N_TO_2S 0
#define N_TO_5S 1
#define Q_SIZE 64
/*Defnicion de periodos y timeouts*/
#define N_PER 2
#define N_TO 3



Queue buffer;
char State = IDLE;
char tr_state = NORMAL_MODE;
static Tm_Periodo periodos[N_PER];
static Tm_Timeout timeouts[N_TO];
Tm_Control c_tiempo;
int c = 0;


/*----------------------------Variables Cristian--------------------------*/

char reception;
bool checkear = TRUE, bandera_while = TRUE;
static char colitas[60] = {0};
static unsigned char i =0;
char display_state = IDLE;
char bits_Descarte;
char flag_LI = FALSE;
/*-----------------------------------------------------------------------*/

static char atenderTimer(char atienda){
	if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK)
	{
		if(atienda)
		{
			PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		}
		return TRUE;
	}else{
		return FALSE;
	}
};

/*
 * @brief   Application entry point.
 */


int main(void) {
    /* Init board hardware. */
	BOARD_InitPeripherals();
    BOARD_InitBootClocks();
    BOARD_BootClockRUN();

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
/*--------------------------------------------------------------------------*/
/*					Modules initialization									*/
/*--------------------------------------------------------------------------*/
    Tm_Inicie(&c_tiempo, periodos, N_PER, timeouts, N_TO, &atenderTimer);
    Tm_Inicie_periodo(&c_tiempo, PER_625, 50); /* periodo de 6.25ms*/
    Tm_Inicie_periodo(&c_tiempo, PER_INT, 5); /* periodo de 625us*/
	initDisplay();
    initPit(0xBB7,0); /*24000000*0.000125 - 1 ->125us*/
    initUart1();
	initQueue(&buffer, Q_SIZE);
/*--------------------------------------------------------------------------*/	
    //PRINTF("Hola");
    /*Loop de pooling*/
    while(1){

		if(atenderTimer(FALSE))
			Tm_Procese(&c_tiempo);
	    //PRINTF("Hola");
		displayLowIntensity(PER_INT, 2,  &c);
		//display(2);
		//reception =	receiveChar();
		enqueue(&buffer,receiveChar());
		switch (display_state)
		{
			case IDLE:
				displayOff();
				if(!(queueIsEmpty(&buffer))){
					display_state = ACTIVO;
				}
			break;
			case ACTIVO:
				if(Tm_Hubo_periodo(&c_tiempo, PER_625)){
					if(!(queueIsEmpty(&buffer))){
						reception = dequeue(&buffer);
						if(queueFull_75(&buffer)){
							stopCommunication();
						}
					}
					/*Se evaluan los casos especiales*/
					switch(reception){
						case 38: //&
							tr_state = NEG_MODE;
						break;
						case 37:  //%
							Tm_Inicie_timeout(&c_tiempo, N_TO_2S,1600);
							State = INACTIVO;
						break;
						case 36: // $
							tr_state = NORMAL_MODE;
						break;
						case 35: // #
							flag_LI = TRUE;
							Tm_Inicie_periodo(&c_tiempo, PER_INT, /*Cuanto es el tiempo?*/ );
							Tm_Inicie_timeout(&c_tiempo, N_TO_5S,/*Tiempo de espera*/);
						break;
					}
					/*---Descarte de datos los bits màs significativos--*/
					switch (tr_state){

						case NORMAL_MODE:
							bits_Descarte = GET_LSB(reception, 4, 1);
						break;

						case NEG_MODE:
							bits_Descarte = ~GET_LSB(reception, 4, 1);
						break;
					}
					if(reception != 38 & reception != 37 & reception != 36 & reception != 35 & State != INACTIVO){
						if(!flag_LI){
							display(bits_Descarte);
						}
						else{
							displayLowIntensity(PER_INT, bits_Descarte, &c);
							if(Tm_Hubo_timeout(&c_tiempo,N_TO_5S)){
								flag_LI = FALSE;
							}
						}
						
					}
				}
			break;
			case INACTIVO:
				displayOff();
				if(Tm_Hubo_timeout(&c_tiempo,N_TO_2S)){
					State = ACTIVO;
				}
			break;
		}
		if(queue_25(&buffer)){
			startCommunication();
		}
		
	
		/*----------------------------------------------------

		    	if (reception != '\n' && reception != '@' && reception != '\0'){
		    		colitas[i] = reception;
		    		sendChar(colitas[i]);
		    		++i;
		    		PRINTF("La cadena ½c", reception);
		    	}*/

	}
	return 0;
    
}


