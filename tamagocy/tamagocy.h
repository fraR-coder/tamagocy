
#ifndef __TMGCY_H 
#define __TMGCY_H

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"

/* Private define ------------------------------------------------------------*/



#define  MAX_X  240
#define  MAX_Y  320   



/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

#define RGB565CONVERT(red, green, blue)\
(uint16_t)( (( red   >> 3 ) << 11 ) | \
(( green >> 2 ) << 5  ) | \
( blue  >> 3 ))


/* Private function prototypes -----------------------------------------------*/
void create_tamagochy(uint16_t Xpos,uint16_t Ypos, uint16_t r, uint16_t color);
void draw_tamagocy(uint16_t Xpos,uint16_t Ypos, uint16_t r,uint16_t color);

void draw_menu(void);
void draw_volume_icon(uint16_t color);
void draw_linguaccia(void);

void add_green_bar_happy(void);
void remove_green_bar_happy(void);
void add_green_bar_satiety(void);
void remove_green_bar_satiety(void);
void runaway_sequence(void);

void eat(void);
void cuddles_animation(void);
void delay_ms(uint16_t ms);

#endif 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
