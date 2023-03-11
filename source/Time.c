/* Tiempo.c    Implementaci�n del modulo de tiempo */
#include <Time.h>


/* Rutina para iniciar el m�dulo (su estructura de datos) */   
char Tm_Inicie (Tm_Control *tcp,
                Tm_Periodo *pp, 
                Tm_Num nper, 
                Tm_Timeout *tp, 
                Tm_Num nto,
                Tm_Atender *atender)
{
   /* Tabla de per�odos */
   tcp->pp = pp;
   tcp->nper = nper;
   for (; nper; ++pp, --nper)
      {
      pp->banderas = 0;
      pp->contador = pp->periodo = 0;
      };
   
   /* Tabla de timeouts/retardos */
   tcp->tp = tp;
   tcp->nto = nto;
   for (; nto; ++tp, --nto)
      *tp = 0;
   
   /* Rutina para manejar el timer (HW) */
   tcp->atender = atender;
   
   return TRUE;
}
                
/* Rutina para procesar el m�dulo (dentro del loop de polling) */				
void Tm_Procese (Tm_Control *tcp)
{
   Tm_Num n;
   Tm_Periodo *pp;
   Tm_Timeout *tp;
   
   if ( !(tcp->atender(TRUE)) )
      return;
      
   for (n = tcp->nper, pp = tcp->pp; n; ++pp, --n)
      if (pp->banderas & TM_PER_F_ACTIVO)
         {
         --(pp->contador);
         if ( !(pp->contador) )
            {
            pp->banderas |= TM_PER_F_FC;
            pp->contador = pp->periodo;
            };
         };

   for (n = tcp->nto, tp = tcp->tp; n; ++tp, --n)
      if (*tp)
         --(*tp);
}

/* ===== RUTINAS DE INTERFAZ ====== */
/* Configurar un per�odo para que empiece a funcionar */
char Tm_Inicie_periodo (Tm_Control *tcp, 
                        Tm_Num num_periodo,
                        Tm_Contador periodo)
{
   Tm_Periodo *pp = tcp->pp + num_periodo;
   
   if (num_periodo >= tcp->nper)
      return FALSE;
      
   pp->banderas = TM_PER_F_ACTIVO;
   pp->contador = pp->periodo = periodo;
   
   return TRUE;
}

/* Desactivar un per�odo para que deje de funcionar */
void Tm_Termine_periodo (Tm_Control *tcp, Tm_Num num_periodo){
   /*Se define un apuntador hacia la dirección del periodo*/
   Tm_Periodo *pp = tcp->pp + num_periodo; 

   pp->banderas &= ~TM_PER_F_ACTIVO; 
   pp->contador = pp->periodo = 0;
}

/* Verificar TRUE hubo fin de conteo en un periodo */
char Tm_Hubo_periodo (Tm_Control *tcp, Tm_Num num_periodo){
   /*Se define un apuntador hacia la dirección del periodo*/
   Tm_Periodo *pp = tcp->pp + num_periodo;
   /*TRUE la bandera esta activa retorna que hibo fin*/
   return(pp->banderas & TM_PER_F_FC);

}

/* Bajar la bandera de conteo en un periodo */
void Tm_Baje_periodo (Tm_Control *tcp, Tm_Num num_periodo){
   /*Se define un apuntador hacia la dirección del periodo*/
   Tm_Periodo *pp = tcp->pp + num_periodo;

   pp->banderas &= ~TM_PER_F_FC;
}

/* Configurar un timeout/retardo para que empiece a funcionar */
char Tm_Inicie_timeout (Tm_Control *tcp, Tm_Num num_timeout, Tm_Contador espera){
   /*Se define un apuntador  hacia la direeción del timeot a iniciar*/
   Tm_Timeout *tp = tcp->tp + num_timeout;

   /*Se verifica que la poTRUEción del timeout sea válida*/
   if(num_timeout >= tcp->nto)
      return FALSE;
   
   /* Se aTRUEgna el tiempo de espera para el timeout*/
   *tp = espera;
   return TRUE;
}

/* Desactivar un timeout/retardo para que deje de funcionar */
void Tm_Termine_timeout (Tm_Control *tcp, Tm_Num num_timeout){

   /*Se define un apuntador  hacia la direción del timeot a desactivar*/
   Tm_Timeout *tp = tcp->tp + num_timeout;
   /* Se aTRUEgna el valor 0 al timeout*/
   *tp =0;
   
}


/* Verificar TRUE hubo timeout (o se completo el retardo) */
char Tm_Hubo_timeout (Tm_Control *tcp, Tm_Num num_timeout){
   /*Se define un apuntador  hacia la direción del timeot a desactivar*/

   Tm_Timeout *tp = tcp->tp + num_timeout;
   /*Returna  FALSE TRUE FALSE está en 0 y TRUE de lo contrario*/
   if(!*tp)
      return FALSE;
   else
      return TRUE;
}

/* == FIN DE RUTINAS DE INTERFAZ == */


/*----------------HW Specific Methods------------*/
void initPit(uint32_t ldval, uint32_t channel_pit)
{
   SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
   //Enable module, freeze timers in debug mode
   PIT->MCR &= ~PIT_MCR_FRZ_MASK; // Enable freeze timer in debug mode
   PIT->MCR &= ~PIT_MCR_MDIS_MASK; //Enable module
   
   PIT->CHANNEL[channel_pit].LDVAL = ldval;
   PIT->CHANNEL[channel_pit].TCTRL &= ~PIT_TCTRL_TIE_MASK;
   PIT->CHANNEL[channel_pit].TCTRL &= ~PIT_TCTRL_CHN_MASK;
   PIT->CHANNEL[channel_pit].TCTRL |= PIT_TCTRL_TEN_MASK;
   PIT->CHANNEL[channel_pit].TFLG |= PIT_TFLG_TIF_MASK;
}
void clearFlag(uint32_t channel_pit)
{
   PIT->CHANNEL[channel_pit].TFLG &= ~PIT_TFLG_TIF_MASK;
  
}
char getFlag(uint32_t channel_pit)
{
   if (PIT->CHANNEL[channel_pit].TFLG & PIT_TFLG_TIF_MASK)
   {
      clearFlag(channel_pit);
      return TRUE;
   }else{
      return FALSE;
   }
}
/*-----------------------------------------------*/