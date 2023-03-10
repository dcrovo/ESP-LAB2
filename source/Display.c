#include "Display.h"



const uint32_t displayTable[] = {
		/*0*/
        (0<<SSG_PIN)|(1<<SSF_PIN)|(1<<SSE_PIN)|(1<<SSD_PIN)
        |(1<<SSC_PIN)|(1<<SSB_PIN)|(1<<SSA_PIN)|(0<<SSP_PIN),
        /*1*/
        (0<<SSG_PIN)|(0<<SSF_PIN)|(0<<SSE_PIN)|(0<<SSD_PIN)
        |(1<<SSC_PIN)|(1<<SSB_PIN)|(0<<SSA_PIN)|(0<<SSP_PIN),
        /*2*/
        (1<<SSG_PIN)|(0<<SSF_PIN)|(1<<SSE_PIN)|(1<<SSD_PIN)
        |(0<<SSC_PIN)|(1<<SSB_PIN)|(1<<SSA_PIN)|(0<<SSP_PIN),
        /*3*/
        (1<<SSG_PIN)|(0<<SSF_PIN)|(0<<SSE_PIN)|(1<<SSD_PIN)
        |(1<<SSC_PIN)|(1<<SSB_PIN)|(1<<SSA_PIN)|(0<<SSP_PIN),
        /*4*/
        (1<<SSG_PIN)|(1<<SSF_PIN)|(0<<SSE_PIN)|(0<<SSD_PIN)
        |(1<<SSC_PIN)|(1<<SSB_PIN)|(0<<SSA_PIN)|(0<<SSP_PIN),
        /*5*/
        (1<<SSG_PIN)|(1<<SSF_PIN)|(0<<SSE_PIN)|(1<<SSD_PIN)
        |(1<<SSC_PIN)|(0<<SSB_PIN)|(1<<SSA_PIN)|(0<<SSP_PIN),
        /*6*/
        (1<<SSG_PIN)|(1<<SSF_PIN)|(1<<SSE_PIN)|(1<<SSD_PIN)
        |(1<<SSC_PIN)|(0<<SSB_PIN)|(1<<SSA_PIN)|(0<<SSP_PIN),
        /*7*/
        (0<<SSG_PIN)|(0<<SSF_PIN)|(0<<SSE_PIN)|(0<<SSD_PIN)
        |(1<<SSC_PIN)|(1<<SSB_PIN)|(1<<SSA_PIN)|(0<<SSP_PIN),
        /*8*/
        (1<<SSG_PIN)|(1<<SSF_PIN)|(1<<SSE_PIN)|(1<<SSD_PIN)
        |(1<<SSC_PIN)|(1<<SSB_PIN)|(1<<SSA_PIN)|(0<<SSP_PIN),
        /*9*/
        (1<<SSG_PIN)|(1<<SSF_PIN)|(0<<SSE_PIN)|(0<<SSD_PIN)
        |(1<<SSC_PIN)|(1<<SSB_PIN)|(1<<SSA_PIN)|(0<<SSP_PIN),
        /*a*/
        (1<<SSG_PIN)|(1<<SSF_PIN)|(1<<SSE_PIN)|(0<<SSD_PIN)
        |(1<<SSC_PIN)|(1<<SSB_PIN)|(1<<SSA_PIN)|(0<<SSP_PIN),
        /*b*/
        (1<<SSG_PIN)|(1<<SSF_PIN)|(1<<SSE_PIN)|(1<<SSD_PIN)
        |(1<<SSC_PIN)|(0<<SSB_PIN)|(0<<SSA_PIN)|(0<<SSP_PIN),
        /*c*/
        (0<<SSG_PIN)|(1<<SSF_PIN)|(1<<SSE_PIN)|(1<<SSD_PIN)
        |(0<<SSC_PIN)|(0<<SSB_PIN)|(1<<SSA_PIN)|(0<<SSP_PIN), 
        /*d*/
        (1<<SSG_PIN)|(0<<SSF_PIN)|(1<<SSE_PIN)|(1<<SSD_PIN)
        |(1<<SSC_PIN)|(1<<SSB_PIN)|(0<<SSA_PIN)|(0<<SSP_PIN),
        /*e*/
        (1<<SSG_PIN)|(1<<SSF_PIN)|(1<<SSE_PIN)|(1<<SSD_PIN)
        |(0<<SSC_PIN)|(0<<SSB_PIN)|(1<<SSA_PIN)|(0<<SSP_PIN),
        /*f*/
        (1<<SSG_PIN)|(1<<SSF_PIN)|(1<<SSE_PIN)|(0<<SSD_PIN)
        |(0<<SSC_PIN)|(0<<SSB_PIN)|(1<<SSA_PIN)|(0<<SSP_PIN),


};

void initDisplay()
{
    /*Enable clock to PORT E*/
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    /*Set portE pin mux as GPIO*/
    *(PORTE->PCR + SSA_PIN) |= PORT_PCR_MUX(1);
    *(PORTE->PCR + SSB_PIN) |= PORT_PCR_MUX(1);
    *(PORTE->PCR + SSC_PIN) |= PORT_PCR_MUX(1);
    *(PORTE->PCR + SSD_PIN) |= PORT_PCR_MUX(1);
    *(PORTE->PCR + SSE_PIN) |= PORT_PCR_MUX(1);
    *(PORTE->PCR + SSF_PIN) |= PORT_PCR_MUX(1);
    *(PORTE->PCR + SSG_PIN) |= PORT_PCR_MUX(1);
    *(PORTE->PCR + SSP_PIN) |= PORT_PCR_MUX(1);
    /* Set GPIO as Output pin*/
    GPIOE->PDDR |= ((1<<SSA_PIN) | (1<<SSB_PIN) | (1<<SSC_PIN) 
                    | (1<<SSD_PIN) | (1<<SSE_PIN) | (1<<SSF_PIN) 
                    | (1<<SSG_PIN) | (1<<SSP_PIN));
    /*Initialise output pins as 1 since inverse logic*/
    GPIOE->PSOR |= ((1<<SSA_PIN) | (1<<SSB_PIN) | (1<<SSC_PIN) 
                    | (1<<SSD_PIN) | (1<<SSE_PIN) | (1<<SSF_PIN) 
                    | (1<<SSG_PIN) | (1<<SSP_PIN));

}
void display(char input){
	displayOff();
    if(input == '-')
        GPIOE->PCOR |= 1<<SSG_PIN;
    else
        GPIOE->PCOR |= displayTable[((uint32_t)input)];
}
void displayOff(){
	GPIOE->PSOR |= ((1<<SSA_PIN) | (1<<SSB_PIN) | (1<<SSC_PIN)
            | (1<<SSD_PIN) | (1<<SSE_PIN) | (1<<SSF_PIN)
            | (1<<SSG_PIN) | (1<<SSP_PIN));
}


