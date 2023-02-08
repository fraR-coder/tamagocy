/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../timer/timer.h"
#include "../tamagocy/tamagocy.h"


/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/


volatile int volume=3;
unsigned short AD_current;   
unsigned short AD_last = 0;     /* Last converted value               */
extern int sound;

void ADC_IRQHandler(void) {
  	NVIC_DisableIRQ(RIT_IRQn);
		NVIC_DisableIRQ(TIMER0_IRQn);
		NVIC_DisableIRQ(TIMER1_IRQn);
		
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);// Read Conversion Result             
  if(AD_current != AD_last){
		
		if(AD_current==0x0FFF){
			draw_volume_icon(White);
			volume=3;
			draw_volume_icon(Black);
		}
		else if(AD_current==0){
			draw_volume_icon(White);
			volume=0;
		}
		else if(AD_current>0 && AD_current<=0x0800 && volume!=1 ){
			draw_volume_icon(White);
			volume=1;
			draw_volume_icon(Black);
		}
		else if(AD_current>0x0800 && AD_current<0x0FFF && volume!=2 ){
			draw_volume_icon(White);
			volume=2;
			draw_volume_icon(Black);
		}
			
		
		AD_last = AD_current;
  }	
	NVIC_EnableIRQ(RIT_IRQn);
	NVIC_EnableIRQ(TIMER0_IRQn);
	NVIC_EnableIRQ(TIMER1_IRQn);
	
	
}
