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
#include <queue.h>
#include <stdlib.h>
#include "Time.h"
#include "Display.h"

#include "UART.h"
#define IDLE     0
#define ACTIVO   1
#define INACTIVO 2
#define PER_625  0
#define PER_INT  1
#define NORMAL_MODE 0
#define NEG_MODE 1

#define N_PER 2
#define N_TO 3
#define N_TO_2S 0
#define N_TO_5S 1


queue buffer;
UART_flow uart_config;
char COMP_FLAG = 0;
char State = IDLE;
char ch = '$';
char tr_state = NORMAL_MODE;
static Tm_Periodo periodos[N_PER];
static Tm_Timeout timeouts[N_TO];
Tm_Control c_tiempo;
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
static void displayLowIntensity(){
    if(Tm_Hubo_periodo(&c_tiempo, PER_INT)){
    	c>=4 ? c=0:c++;
    	if(!c)
    	{
    		display(0);
    	}else{
    		displayOff();
    	}
        Tm_Baje_periodo(&c_tiempo,1);

    }
};

/*
 * @brief   Application entry point.
 */


int main(void) {
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
    //uart_iniciar(&uart_config);
    Tm_Inicie(&c_tiempo, periodos, N_PER, timeouts, N_TO, &atenderTimer);
    Tm_Inicie_periodo(&c_tiempo, PER_625, 50); // periodo de 6.25ms
    Tm_Inicie_periodo(&c_tiempo, PER_INT, 5); // periodo de 625us

    initDisplay();
    initPit(0xBB7,0); //24000000*0.000125 - 1 ->125us
    int c = 0;
    /*Loop de pooling*/
    while(1){
        if(atenderTimer(FALSE))
        {
            Tm_Procese(&c_tiempo);
        }
        receive_data(&uart_config);
       	//------------
		//uart_receive_byte();
		//------------
		switch (State) {

		case IDLE:
            if(buffer.q_tam){
                State = ACTIVO;
            }
		break;

		case ACTIVO:
            if(Tm_Hubo_periodo(&c_tiempo, PER_625)){
                if(buffer.q_tam){
                    ch = queue_out(&buffer);
                } 
                switch (ch) {
				case 38: //&
					tr_state = NEG_MODE;
                    COMP_FLAG = TRUE;
                    if(buffer.q_tam){
                    ch = queue_out(&buffer);
                    } 
				break;
				case 37: //%
                    State = INACTIVO;
                    Tm_Inicie_timeout(&c_tiempo, N_TO_2S,16000);
					                 
					break;
				case 36: //$ //NORMAL
					tr_state = NORMAL_MODE;
                    COMP_FLAG = FALSE;
                    if(buffer.q_tam){
                        ch = queue_out(&buffer);
                    } 
					break;
				case 35: //#
                    
					if(Tm_Hubo_periodo(&c_tiempo, PER_INT)){
                        if(!c){
                            display(ch);
                        }else{
                            displayOff();
                        }
                    }
                    c>=4 ? c=0:c++;
                    Tm_Baje_periodo(&c_tiempo,PER_INT);

    }
					break;
                default:
                    if(!COMP_FLAG){
                        tr_state = NORMAL_MODE;
                    }
                break;
                    
				}  
                switch (tr_state) {
				case NORMAL_MODE:
                    display(GET_LSB(ch,4,1));
					break;
				case NEG_MODE:
					display(15 - GET_LSB(ch, 4, 1)); 
					break;
				} 
                Tm_Baje_periodo(&c_tiempo, PER_625);
            }
        break;
		case INACTIVO:
            displayOff();
            if(Tm_Hubo_timeout(&c_tiempo, N_TO_2S)){
                State = ACTIVO;
                if(buffer.q_tam){
                    ch = queue_out(&buffer);
                }
        
            }
		break;
		}
        if(!(uart_config.flowctr) & queue_empty(&buffer)){
                uart_config.flowctr = TRUE;
                uart_start(&uart_config);
            }

		/*if (flag_timeout_3
				!= Tm_Hubo_timeout(&c_tiempo, N_TO_ACT_LOW_INTENSITY)) {
			Tm_Inicie_pwm(&c_tiempo, N_PWM_PTE_31, 8, 1, COM_PIN); //200 Hz
		}
		flag_timeout_3 = Tm_Hubo_timeout(&c_tiempo, N_TO_ACT_LOW_INTENSITY);

		if (flag_timeout_1 != Tm_Hubo_timeout(&c_tiempo, N_TO_LOW_INTENSITY)) {
			Tm_Inicie_pwm(&c_tiempo, N_PWM_PTE_31, 4, 3, COM_PIN); //200 Hz
		}
		flag_timeout_1 = Tm_Hubo_timeout(&c_tiempo, N_TO_LOW_INTENSITY);

		if (flag_timeout_2 != Tm_Hubo_timeout(&c_tiempo, N_TO_NEW_DATA)) {
			uart_send_string("TO DISPLAY\r\n");
			lcdScan('-');
		}
		flag_timeout_2 = Tm_Hubo_timeout(&c_tiempo, N_TO_NEW_DATA);*/

	}
	return 0;
    
}


