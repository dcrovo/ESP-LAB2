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

queue buffer;
UART_flow uart_config;

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
        receive_data(UART_flow &uart_config);
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
                    if(buffer.q_tam){
                    ch = queue_out(&buffer);
                    } 
				break;
				case 37: //%
                    if(buffer.q_tam){
                        ch = queue_out(&buffer);
                    } 
					/*uart_send_byte(XOFF); // XOFF
					//iniciar time out
					Tm_Inicie_timeout(&c_tiempo, N_TO_DISPLAY_OFF, 1600); //2 SEG
					//TRX XOFF
					dis_state = D_OFF_MODE;*/
                    
					break;
				case 36: //$ //NORMAL
					tr_state = NORMAL_MODE;
                    if(buffer.q_tam){
                        ch = queue_out(&buffer);
                    } 
					break;
				case 35: //#
                    if(buffer.q_tam){
                        ch = queue_out(&buffer);
                    } 
					//Cambiar PWM
					/*Tm_Inicie_timeout(&c_tiempo, N_TO_ACT_LOW_INTENSITY, 1); //5 SEG
					//iniciar time out
					Tm_Inicie_timeout(&c_tiempo, N_TO_LOW_INTENSITY, 4000); //5 SEG*/
					break;
                default:
                    tr_state = NORMAL_MODE;
                    
				}  
                switch (tr_state) {
				case NORMAL_MODE:
                    display(GET_LSB(ch,4,1));
					break;
				case NEG_MODE:
					display(15 - GET_LSB(ch, 4, 1)); 
					break;
				} 
            }
            Tm_Baje_periodo(&c_tiempo, PER_625);
            /*if (Tm_Hubo_periodo(&c_tiempo, N_PER_NORMAL)) { // 160Hz

				//-------------------------------------------------- OBTENER DATO DE BUFFER
				if (ringBuffer_getCount(pRingBufferRx) != 0) {

					ringBuffer_getData(pRingBufferRx, &ch);
					
					 uart_send_string("C=");
					 uart_send_byte(ch);
					 myprintf(" CO=%d\r\n", ringBuffer_getCount(pRingBufferRx));
					 
					if (ringBuffer_isFull(pRingBufferRx)) {
						uart_send_byte(XOFF); // XOFF
						flag_xoff = SI;
					}
				}

				if (flag_xoff && ringBuffer_getCount(pRingBufferRx) == 0) {

					//-------------------------------- imprimir ultimo dato

					//ringBuffer_getData(pRingBufferRx, &ch);
					ringBuffer_getData(pRingBufferRx, &ch);
					/*uart_send_string("C=");
					 uart_send_byte(ch);
					 myprintf(" CO=%d\r\n", ringBuffer_getCount(pRingBufferRx));
					 
					//--------------------------------
					uart_send_byte(XON); //0x11 XON || 0x17 XON REALTERM
					flag_xoff = NO;
				}*/
				//--------------------------------------------------------------------------

				//------------------------ ESTADOS
				//------------------------
				//if(ch == 35 ){
				//	myprintf("P %d",ch);
				//}
				//&& ch != 35
				/*if (!flag_timeout_2 && ch != 38 && ch != 36 && ch != 35
						&& ch != 37 && ch != 13) {
					lcdScan(rxvalueMNS);
				}

				Tm_Baje_periodo(&c_tiempo, N_PER_NORMAL);*/
			}
		break;

		case INACTIVO:
			/*lcdOff();
			if (flag_timeout_0
					!= Tm_Hubo_timeout(&c_tiempo, N_TO_DISPLAY_OFF)) {
				//uart_send_string("CH EST\r\n");
				//cambia estado
				dis_state = D_NORMAL;
				//TRX XON
				uart_send_byte(XON); // XOFF
			}
			flag_timeout_0 = Tm_Hubo_timeout(&c_tiempo, N_TO_DISPLAY_OFF);*/
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


