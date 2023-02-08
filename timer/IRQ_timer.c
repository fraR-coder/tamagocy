/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h"

#include "../tamagocy/tamagocy.h"
#include "../touchpanel/touchpanel.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
const int freqs[8]={3200,3000,2900,2800,2700,2600,2400,2200};

const int freqs_for_dead[8]={3200,3200,2000,3200,3200,2000,3000,3000};
const int freqs_for_cuddle[8]={2000,1800,2200,2000,2000,1800,2200,2000};
//const int freqs[8]={2120,1890,1684,1592,1417,1263,1125,1062};

volatile int seconds = 0;
volatile int minutes = 0;
volatile int hours = 0;

extern int cuddles;
extern int time;
extern int dead;
extern int volume;

extern int number_of_notes;
extern int starting_note;
extern int duration;


uint16_t SinTable[45] =                                       
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

void TIMER0_IRQHandler(void)
{
	
	// Match register 0 interrupt service routine
	if (LPC_TIM0->IR & 01)
	{


		if (seconds > 59)
		{
			seconds = 0;

			if (minutes > 59)
			{
				minutes = 0;

				hours++;
			}
			minutes++;
		}
		seconds++;
		time = 1;

	
		
	}
	LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler(void)
{
	
	 if(getDisplayPoint(&display, Read_Ads7846(), &matrix )){
		if(display.y <= 240 && display.y>=80 && display.x <= 200 && display.x>=40){
			
			cuddles=1;
			
		}
		}		
	
	else{
		//do nothing if touch returns values out of bounds
	}
	
	LPC_TIM1->IR = 1; /* clear interrupt flag */

	return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler(void)
{
	
	static int ticks=0;
	static int n=0;
	//if(note_played<number_of_notes){
		/* DAC management */	
		LPC_DAC->DACR = SinTable[ticks]<<6;
		ticks++;
		
		if(ticks==45){
			ticks=0;
			n++;
		}
		if(n==duration){
			disable_timer(2);
			NVIC_EnableIRQ(TIMER3_IRQn);
			n=0;
			
		}
	//}
	
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  
	return;
}
void TIMER3_IRQHandler(void)
{	
	static int num=0;
	NVIC_DisableIRQ(TIMER3_IRQn);
	reset_timer(2);
	if(dead){
		init_timer(2,0,0,3,freqs_for_dead[(starting_note)]);
	}
	else if(cuddles){
		init_timer(2,0,0,3,freqs_for_cuddle[(starting_note)]);
	}
	else{
	init_timer(2,0,0,3,freqs[(starting_note)]);
	}
	enable_timer(2);
	starting_note=(starting_note+1)%8;
	num++;
	if(num>number_of_notes){
		num=0;
		disable_timer(2);
		disable_timer(3);
	}
  LPC_TIM3->IR = 1;			/* clear interrupt flag */
  
	return;
}
/******************************************************************************
**                            End Of File
******************************************************************************/
