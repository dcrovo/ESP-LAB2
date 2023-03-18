
#if !defined (DISPLAY_H)
#define DIPLAY_H

#include "MKL46Z4.h"
#include "Time.h"
/*Pin board definition for sseg*/

#define SSA_PIN 6U
#define SSB_PIN 16U
#define SSC_PIN 3U
#define SSD_PIN 17U
#define SSE_PIN 18U
#define SSF_PIN 19U
#define SSG_PIN 31U
#define SSP_PIN 2U

void initDisplay();
void display(char input);
void displayOff();
void displayLowIntensity(Tm_Num PER_INT, char data, int *c);

#define GET_LSB(number, k, p) (((1 << k) - 1) & (number >> (p - 1)))
#endif
