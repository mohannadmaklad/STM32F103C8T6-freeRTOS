#include "stm32f10x.h"                  // Device header
#include "lcd.h"
#include <string.h>
#include <math.h>
#include "fonts.h"
#include "ds.h"

static uint8_t LCD[LCD_SIZE];
extern int abs(int);

uint16_t topMarker ;
uint16_t bottomMarker;
uint16_t indx = 0; // Cursor position


void LCD_init()
{
	RESET_DC ;
	RES;
	NRES;
	setX(0x00);
	setY(0x00);
}


void LCD_cmd(uint8_t cmd)
{
	RESET_DC;
	SPI2->DR = (cmd);
	wait();
}

void LCD_data(uint8_t data)
{
	SET_DC;
	SPI2->DR = (data);
	wait();
	
}


char setX(uint8_t x)
{
	if(x > 83) return 1;
	else
	{
		LCD_cmd(((uint8_t)0x1<<7) | x );
		return 0 ;
	}
}

char setY(uint8_t y)
{
	if(y > 5) return 1;
	else
	{
		LCD_cmd(((uint8_t)0x1<<6) | y );
		return 0 ;
	}
}

char pixel(uint8_t x , uint8_t y)
{
	uint8_t row ;
	if(x > 83) return 1 ;
	if(y > 47) return 1 ;
	
	else
	{
		row = y / 8 ; // 0,1,2,3,4,5
	}
	
	setX(x);
	setY(row);
	LCD_data(0x00 | (1 << (y%8)));
	return 0 ;
}

void LCD_clear(void)
{
	setX(0x00);
	setY(0x00);
	for (uint16_t i = 0 ; i < 504 ; i++)
	{
		LCD_data(0x00);
	}
}

void line(uint8_t x1 , uint8_t y1 , uint8_t x2 , uint8_t y2)
{
	
	// case1 -1 < m < 1
	// note that x,y coordinated start at the top of the screen. so, (0,0) is the top left , (83,47) at the lower right
	
	float m ;
	signed char adjust ;
	uint32_t offset = 0 ;
	uint32_t thresh ;
	uint8_t thresh_delta ;
	
	
	signed char rise , run ;
	uint8_t delta ;
	
	rise = y2 - y1 ;
	run  = x2 - x1 ;
	m =  (float)rise / run ;
	
	if (m >= 0) adjust = 1 ;
	else adjust = -1 ;
	
	//case 1
	if(m >= -1 && m <= 1 )
	{
		delta         = 2 * abs(rise);
		thresh        =     abs(run) ;
		thresh_delta  = 2 * abs(run) ;
		
		for(uint8_t x = x1,y = y1 ; x <= abs(x2-x1) ; x++)
		{
			setPixel(x,(y)); // !!to make y start from the bottom
			offset += delta ;
			
			if (offset >= thresh)
			{
				y+= adjust ;
				thresh += thresh_delta ;
			}
		}
	}
	else
	{
		delta         = 2 * abs(run);
		thresh        =     abs(rise) ;
		thresh_delta  = 2 * abs(rise) ;
		
		
		for(uint8_t y = y1 , x = x1 ; y <= abs(y2-y1) ; y++)
		{
			setPixel(x,(y) ); // !!to make y start from the bottom
			offset += delta ;	
			if (offset >= thresh)
			{
				x+= adjust ;
				thresh += thresh_delta ;
			}
		}
	}

}

char fillsegment(uint8_t y , uint8_t x1 , uint8_t x2)
{
	// y ranges from 0-5
	//Use this function as (0,0) lies at the lower left corner i.e fill(0,0,83) fills the bottom row
	if (y > 5) return 1 ;
	setX(x1);
	setY(5-y);
	for(uint8_t i = 0 ; i <= (x2-x1) ; i++)
	{
		LCD_data(0xff);
	}
	return done ;
}	




void fill(uint8_t x1 , uint8_t x2 , uint8_t y1 , uint8_t y2)
{
	
	//y1 and y2 belong to the same segment
	//Use this function as (0,0) lies at the lower left corner
	
	uint8_t top_row ;
	uint8_t top_ypixel;
	uint8_t lower_ypixel ;
	uint8_t data ;
	
	y1 = 47 - y1 ;
	y2 = 47 - y2 ;
	
	top_row     =  y1 / 8 ; 
	top_ypixel  =  y2 % 8 ;
	lower_ypixel = y1 % 8 ;
	setX(x1);
	setY(top_row);
	data = ((0xff << top_ypixel)&(0xff >> (7-lower_ypixel))) ;
	for(uint8_t i =0 ; i <= (x2-x1) ; i++)
	{
		LCD_data(data);
	}
}


void filldown(uint8_t x1 , uint8_t x2 , uint8_t y1)
{
	//Use this function as (0,0) lies at the lower left corner
	uint8_t row ;
	uint8_t top_ypixel ;
	uint8_t data ;
	
	y1 = 47 - y1 ;	
	row     =  y1 / 8 ; 
	top_ypixel = y1 % 8 ;
	
	setX(x1);
	setY(row);
	
	data = (0xff << top_ypixel) ;
	for(uint8_t i =0 ; i <= (x2-x1) ; i++)
	{
		LCD_data(data);
	}
}


void fillup(uint8_t x1 , uint8_t x2 , uint8_t y1)
{
	//Use this function as (0,0) lies at the lower left corner
	//fillup(0,83,2) fills the lowest y segment from 2 to 7
	
	uint8_t row ;
	uint8_t lower_ypixel;
	uint8_t data ;
	
	y1 = 47 - y1 ;
	
	row     = y1 / 8 ; 
	lower_ypixel  = y1 % 8 ;
	setX(x1);
	setY(row);
	data = (0xff >> (7-lower_ypixel)) ;
	for(uint8_t i =0 ; i <= (x2-x1) ; i++)
	{
		LCD_data(data);
	}
}

void px_vline(uint8_t x , uint8_t y1 , uint8_t y2)
{
	//Use this function as (0,0) lies at the lower left corner

	for(uint8_t i = y1 ; i <= y2 ; i++ )
	{
		fillsegment(i , x , x+1);
	}
	setX(0); //any command
}

void vline(uint8_t x1 , uint8_t x2 , uint8_t y1 , uint8_t y2)
{
	
	//Use this function as (0,0) lies at the lower left corner

	for(uint8_t i = y1 ; i <= y2 ; i++ )
	{
		fillsegment(i , x1 , x2);
	}
	//setX(0); //any command
}

/*------------------------------- New Functions -------------------------------*/
/*------------------------------- New Functions -------------------------------*/
/*------------------------------- New Functions -------------------------------*/


void display(void) //uploads data to the LCD
{
	setX(0x00);
	setY(0x00);
	uint16_t i = 0;
	for(  ; i <= LCD_SIZE ; i++)
	{
		LCD_data(LCD[i]);
	}
}
	
void clearLCD(void)
{
	memset(LCD , 0x00 , LCD_SIZE );
	uint16_t i ;
	
	for(i = 0 ; i < LCD_SIZE ; i++)
	{
		LCD_data(LCD[i]);
	}
	setX(0x00);
	setY(0x00);
}

void fillLCD(void)
{
	memset(LCD , 0xff , LCD_SIZE );
	uint16_t i;

	for(i=0 ; i < LCD_SIZE ; i++)
	{
		LCD_data(LCD[i]);
	}
	setX(0x00);
	setY(0x00);
}
	

uint8_t character(uint16_t chr)
{
	if (indx >= LCD_SIZE-1) return out_of_boundary ;
	for(uint8_t i = 0 ; i<5 ; i++)
	{
		LCD[indx++] = FontLookup[chr-32][i] ;
	}
	indx++;
	return done ;
}

uint8_t space(void)
{
	if (indx >= LCD_SIZE-1) return out_of_boundary ;
	LCD[indx++] = 0;
	return done ;
}

uint8_t displayCursor(void)
{
	uint16_t tmpindx = indx ;
	if (tmpindx >= LCD_SIZE-1) return out_of_boundary ;
	for(uint8_t i = 0 ; i<5 ; i++)
	{
		LCD[tmpindx++] = FontLookup['_'-32][i] ;
	}
	return done ;
}

void write(char* chr)
{
	uint8_t status, i = 0, shiftby = 1 ;
	while(chr[i] != '\0')
	{
		status = character(*chr);
			if(status == out_of_boundary)
			{
				#if (whenOutOfBoundaryDo == clearScreen)
				{
					indx = 0 ;
					clearLCD();
				}
				#elif (whenOutOfBoundaryDo == addAtBottom)
					indx -= shiftby*84 ;
					lshift(LCD , LCD_SIZE , shiftby*84 , 1) ;
					
				#else
					break ;
				#endif
			}
		else chr++ ; 
	}
	//displayCursor();
}

uint8_t newLine(void)
{
	uint8_t currentLine = indx/84;
	if(currentLine > 5) return out_of_boundary ;
	indx = (currentLine+1)*84; 
	return done ;
}


void setPixel(uint8_t x , uint8_t y) //sets any pixel on the screen on ; doesn't depend on indx
{
	uint8_t row = y/8 ;//row + x
	uint16_t position = row*84 + y%8 ;
	LCD[row*84+x] |= (0x00 | (1 << (y%8)));
}
	
	