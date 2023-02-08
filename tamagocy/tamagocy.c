/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           funct_joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        High level joystick management functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

#include <math.h>
#include <stdio.h>
#include "../GLCD/GLCD.h"
#include "tamagocy.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"
#include "../adc/adc.h"

uint16_t bar_lenght = 82;
uint16_t bar_height = 30;
extern int dead;
extern int meal;
extern int snack;
extern int cuddles;
volatile uint16_t happines_x_pos, satiety_x_pos, bars_y_pos;

volatile uint16_t radius, xc, yc;

extern int volume;

volatile int eating = 0;
volatile int hungry = 0;
extern int flag;
volatile int lingua = 0;
uint16_t happy_bar_start;
uint16_t happy_bar_end;
uint16_t satiety_bar_start;
uint16_t satiety_bar_end;

void delay_ms(uint16_t ms)
{
	uint16_t i, j;
	for (i = 0; i < ms; i++)
	{
		for (j = 0; j < 1141; j++)
			;
	}
}

void cuddles_animation(void){
	
	int i,j;
	int times=3;
	uint16_t r=40;
	uint16_t dim=50;
	volatile double distance=0;
	draw_tamagocy(xc,yc,radius,White);
	
	cuddles=1;
	draw_tamagocy(xc,yc,dim,Black);
	draw_tamagocy(xc,yc,dim,White);
  
	
	
	
	do{
     for(i=xc-r;i<=xc+r;i++){
			for(j=yc;j<=(yc+r);j++){
        distance=((i-xc)*(i-xc))+((yc-j)*(yc-j));
        if(distance==r*r){
            draw_tamagocy(i,j,dim,Black);
						delay_ms(500);
						draw_tamagocy(i,j,dim,White);
        }
				}
		}
		for(i=xc+r;i>=xc-r;i--){
			for(j=yc;j>=yc-r;j--){
        distance=((i-xc)*(i-xc))+((yc-j)*(yc-j));
        if(distance==r*r){
             draw_tamagocy(i,j,dim,Black);
						delay_ms(500);
						draw_tamagocy(i,j,dim,White);
        }
				}
		}
		times--;
	}while(times>0);
		
	
	
		cuddles=0;
		draw_tamagocy(xc,yc,radius,Black);
		add_green_bar_happy();
	
	
}

void draw_snack(uint16_t xi, int yi, uint16_t color)
{

	LCD_DrawRectangle(20 + xi, 230 + yi, 20, 10, color);
	LCD_DrawTriangle(10 + xi, 227 + yi, 10 + xi, 243 + yi, 20 + xi, 235 + yi, color);
	LCD_DrawTriangle(50 + xi, 227 + yi, 50 + xi, 243 + yi, 40 + xi, 235 + yi, color);
}

void draw_meal(uint16_t xi, int yi, uint16_t color)
{

	if (color == White)
	{
		LCD_DrawCircle(30 + xi, 240 + yi, 20, color);
	}
	else
	{

		LCD_DrawCircle(30 + xi, 240 + yi, 20, Red);
		LCD_MyDrawLine(10 + xi, 240 + yi, 50 + xi, 240 + yi, Black);
		LCD_MyDrawLine(30 + xi, 220 + yi, 30 + xi, 260 + yi, Black);
		//	LCD_MyDrawLine(15+xi,226+yi,44+xi,254+yi,Black);
		//	LCD_MyDrawLine(15+xi,254+yi,44+xi,226+yi,Black);
		//
		LCD_DrawCircle(20 + xi, 250 + yi, 3, White);
		LCD_DrawCircle(20 + xi, 230 + yi, 3, White);
		LCD_DrawCircle(40 + xi, 250 + yi, 3, White);
		LCD_DrawCircle(40 + xi, 230 + yi, 3, White);
	}
}

void draw_food(uint16_t xi, int yi, uint16_t color)
{

	if (meal == 1)
	{
		draw_meal(xi, yi, color);
	}
	if (snack == 1)
	{
		draw_snack(xi, yi, color);
	}
}

void draw_linguaccia()
{

	if (lingua == 0)
	{
		lingua = 1;
		LCD_DrawSemiCircle(xc, yc + 45 + 1, 25, Red, 1);
		LCD_DrawLine(xc, yc + 45 + 1, xc, yc + 10 + 51, Black);
	}
	else
	{
		LCD_DrawSemiCircle(xc, yc + 45 + 1, 25, White, 1);
		lingua = 0;
	}
}

void draw_mouth(uint16_t Xpos, uint16_t Ypos, uint16_t color)
{
	if (lingua == 1)
	{
		draw_linguaccia();
	}

	if (hungry == 1)
	{
		LCD_DrawSemiCircle_empty(Xpos, Ypos + 10, radius - 40, color, 1);
	}
	else if (eating == 1)
	{
		LCD_DrawCircle_empty(Xpos, Ypos + 40, 30, color);
	}
	else if(cuddles){
		LCD_DrawSemiCircle_empty(Xpos, Ypos + 10, 20, color, 1);
	}
	else
	{
		LCD_MyDrawLine(Xpos - 40, Ypos + 45, Xpos + 40, Ypos + 45, color);
	}
}
void draw_eye(uint16_t Xpos, uint16_t Ypos, uint16_t lenght, uint16_t height, uint16_t color)
{

	if (eating == 1)
	{
		LCD_DrawSemiCircle_empty(Xpos - 20, Ypos - 30, 20, color, 2);
		LCD_DrawSemiCircle_empty(Xpos + 24, Ypos - 30, 20, color, 2);
	}
	else if (dead == 1)
	{
		LCD_DrawTriangle(Xpos - 50, Ypos - 10, Xpos - 50, Ypos - 40, Xpos - 10, Ypos - 10, color);
		LCD_DrawTriangle(Xpos + 10, Ypos - 10, Xpos + 50, Ypos - 40, Xpos + 50, Ypos - 10, color);
	}
	else if(cuddles){
		LCD_DrawSemiCircle_empty(Xpos - 20, Ypos - 20, 10, color, 2);
		LCD_DrawSemiCircle_empty(Xpos + 20, Ypos - 20, 10, color, 2);
	}									
	else
	{
		LCD_DrawRectangle_empty(Xpos - 50, Ypos - 50, lenght, height, color);
		LCD_DrawCircle(Xpos - 30, Ypos - 27, 5, color);

		LCD_DrawRectangle_empty(Xpos + 10, Ypos - 50, lenght, height, color);
		LCD_DrawCircle(Xpos + 30, Ypos - 27, 5, color);
	}
}

void eat(void)
{

	uint16_t i;
	uint16_t max_x = 50;
	draw_food(0, 0, Blue);
	draw_tamagocy(xc, yc, radius, White);
	hungry = 1;

	draw_tamagocy(xc, yc, radius, Black);

	draw_tamagocy(xc, yc, radius, White);

	for (i = 1; i < max_x; i += 5)
	{
		draw_tamagocy(xc - i, yc, radius, Black);
		delay_ms(1000);
		draw_tamagocy(xc - i, yc, radius, White);
	}
	draw_food(0, 0, Blue);
	hungry = 0;
	eating = 1;
	draw_tamagocy(xc - max_x, yc, radius, Black);

	draw_food(0, 0, White);

	for (i = 0; i <= 40; i +=5)
	{
		draw_food(i, -i, Blue);
		delay_ms(1000);
		draw_food(i, -i, White);
		
		draw_tamagocy(xc - max_x, yc, radius, Black);
	}

	draw_food(40, -40, Blue);
	delay_ms(1000);
	draw_food(40, -40, White);
	draw_tamagocy(xc - max_x, yc, radius, White);
	eating = 0;
	hungry = 1;
	for (i = 1; i < 50; i += 5)
	{
		draw_tamagocy(xc - max_x + i, yc, radius, Black);
		delay_ms(1000);
		draw_tamagocy(xc - max_x + i, yc, radius, White);
	}
	hungry = 0;
	draw_tamagocy(xc, yc, radius, Black);
}

void draw_tamagocy(uint16_t Xpos, uint16_t Ypos, uint16_t r, uint16_t color)
{
	if(cuddles){
		LCD_DrawCircle_empty(Xpos, Ypos, r, color);
	}
	else{
	LCD_DrawCircle_empty(Xpos, Ypos, r, color);
	// LCD_DrawSemiCircle(Xpos,Ypos,radius,Red);
	}
	draw_mouth(Xpos, Ypos, color);
	// eyes
	draw_eye(Xpos, Ypos, 40, 30, color);
}

void draw_volume_icon(uint16_t color){
	
	int i,k;
	LCD_DrawRectangle(10,10,5,10,Black);
		
	for(i=0;i<=5;i++){
			LCD_MyDrawLine(15+i,10-i,15+i,20+i,Black);
		
		}
	
	for(i=0;i<volume;i++){
			LCD_DrawCircle(26+(i*10),15,4,color);
	}
		
	
}

void draw_menu()
{
	char a[200];

	bars_y_pos = 41;
	happines_x_pos = 20;
	satiety_x_pos = 140;

	happy_bar_end = happines_x_pos - 2 + bar_lenght;
	happy_bar_start = happines_x_pos;
	satiety_bar_end = satiety_x_pos - 2 + bar_lenght;
	satiety_bar_start = satiety_x_pos;

	// coomands
	LCD_DrawRectangle_empty(0, 270, 120, 50, Black);
	GUI_Text(15, 280, (uint8_t *)" MEAL  ", Blue, White);
	LCD_DrawRectangle_empty(120, 270, 240, 50, Black);
	GUI_Text(135, 280, (uint8_t *)" SNACK  ", Blue, White);

	GUI_Text(happines_x_pos, 25, (uint8_t *)"Happiness  ", Blue, White);
	LCD_DrawRectangle_empty(happines_x_pos - 1, bars_y_pos - 1, bar_lenght, bar_height, Green);
	add_green_bar_happy();
	add_green_bar_happy();
	add_green_bar_happy();
	add_green_bar_happy();

	GUI_Text(satiety_x_pos, 25, (uint8_t *)"Satiety  ", Blue, White);
	LCD_DrawRectangle_empty(satiety_x_pos - 1, bars_y_pos - 1, bar_lenght, bar_height, Green);
	add_green_bar_satiety();
	add_green_bar_satiety();
	add_green_bar_satiety();
	add_green_bar_satiety();

	sprintf(a, "  Age: %02d:%02d:%02d", 0, 0, 0);

	GUI_Text(60, 0, (uint8_t *)a, Blue, White);
	
	draw_volume_icon(Black);
	
}

void create_tamagochy(uint16_t Xpos, uint16_t Ypos, uint16_t r, uint16_t color)
{
	xc = Xpos;
	yc = Ypos;
	radius = r;
	
	// body
	draw_tamagocy(xc, yc, r, color);

	// menu
	draw_menu();
}

void runaway_sequence(void)
{
	int i,j;
	//char a[200]="a";
	// animazione per l'uscita

	// delete_tamagocy(xc,yc,radius,White);
	
	LCD_DrawRectangle(xc - radius, yc - radius, radius * 2 + 1, radius * 2 + 1, White);

	for (i = 0; i < MAX_X + radius; i += 20)
	{

		draw_tamagocy(xc + i, yc, radius, Black);
		delay_ms(1500);
		draw_tamagocy(xc + i, yc, radius, White);
	}

	// LCD_DrawRectangle(0,40,240,320-40,White);

	LCD_Clear(White);
	for (i = MAX_X + radius-20; i >=0; i -= 20)
	{

		draw_tamagocy(xc + i, yc, radius, Black);
		delay_ms(1500);
		draw_tamagocy(xc + i, yc, radius, White);
	}

	draw_tamagocy(xc, yc, radius, Black);

	LCD_DrawRectangle_empty(139, bars_y_pos - 1, bar_lenght, bar_height, Red);
	GUI_Text(175, 50, (uint8_t *)"!  ", Red, White);

	LCD_DrawRectangle_empty(19, bars_y_pos - 1, bar_lenght, bar_height, Red);
	GUI_Text(55, 50, (uint8_t *)"!  ", Red, White);

	LCD_DrawRectangle_empty(0, 270, 240, 50, Red);
	GUI_Text(100, 280, (uint8_t *)" RESET  ", Black, White);

	/*sprintf(a, "  Age: %02d:%02d:%02d", 0, 0, 0);

	GUI_Text(60, 0, (uint8_t *)a, Blue, White);
	*/

	GUI_Text(70, 220, (uint8_t *)"My friend is gone...       Press here to try again ", Red, White);
}

void add_green_bar_happy()
{

	uint16_t lenght = 20;
	uint16_t height = 28;

	if (happines_x_pos < happy_bar_end)
	{
		LCD_DrawRectangle(happines_x_pos, bars_y_pos, lenght, height, Green);
		happines_x_pos += lenght;
	}
}
void remove_green_bar_happy()
{

	uint16_t lenght = 20;
	uint16_t height = 28;

	if (happines_x_pos > happy_bar_start)
	{
		happines_x_pos -= lenght;
		LCD_DrawRectangle(happines_x_pos, bars_y_pos, lenght, height, White);
	}
	if (happines_x_pos == happy_bar_start)
	{
		dead = 1;
	}
}

void add_green_bar_satiety()
{

	uint16_t lenght = 20;
	uint16_t height = 28;

	if (satiety_x_pos < satiety_bar_end)
	{
		LCD_DrawRectangle(satiety_x_pos, bars_y_pos, lenght, height, Green);
		satiety_x_pos += lenght;
	}
}
void remove_green_bar_satiety()
{

	uint16_t lenght = 20;
	uint16_t height = 28;

	if (satiety_x_pos > satiety_bar_start)
	{
		satiety_x_pos -= lenght;
		LCD_DrawRectangle(satiety_x_pos, bars_y_pos, lenght, height, White);
	}

	if (satiety_x_pos == satiety_bar_start)
	{
		dead = 1;
	}
}
