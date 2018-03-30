
#ifndef _LCD_H_
#define _LCD_H_

#include "stm32f10x.h"
#include <stdint.h>

#define width   84
#define height  48
#define LCD_SIZE 504 //(84*48)/8

#define func_set              (0x20)
#define vertical_addr         (0x02)


#define SPACE             LCD_data(0x00)
#define NRES              GPIOA->ODR |=  ((((uint16_t)  0x1)  << 8))
#define RES               GPIOA->ODR &=  (~(((uint16_t) 0x1)  << 8))
#define SET_DC            GPIOA->ODR |=  ((((uint16_t)  0x1)  << 9))
#define RESET_DC          GPIOA->ODR &=  (~(((uint16_t) 0x1)  << 9))

//flags
#define done                                       0
#define out_of_boundary                            1

//actions to take if it "indx" out of boundary
#define nothing                                    0
#define clearScreen                                1
#define addAtBottom                                2

/*    LCD CONFIGURATION    */
#define whenOutOfBoundaryDo                        addAtBottom

extern void wait(void);
void LCD_init(void);
char setX(uint8_t x);
char setY(uint8_t y);
void LCD_cmd(uint8_t cmd);
void LCD_data(uint8_t data);
char pixel(uint8_t x , uint8_t y); //DRAWS PIXEL
void LCD_clear(void);
void filldown(uint8_t x1 , uint8_t x2 , uint8_t y1);
void fillup(uint8_t x1 , uint8_t x2 , uint8_t y1);
void fill(uint8_t x1 , uint8_t x2 , uint8_t y1 , uint8_t y2);
char fillsegment(uint8_t y , uint8_t x1 , uint8_t x2);
void line(uint8_t x1 , uint8_t y1 , uint8_t x2 , uint8_t y2);

void px_vline(uint8_t x , uint8_t y1 , uint8_t y2); 
void vline(uint8_t x1 , uint8_t x2 , uint8_t y1 , uint8_t y2);
char square(uint8_t x1 , uint8_t y1 ,uint8_t x2 , uint8_t y2 );



/*------------------------------- New Functions -------------------------------*/

void display(void);        //uploads data to the LCD
void clearLCD(void);
void fillLCD(void);
uint8_t character(uint16_t chr);
uint8_t space(void);
uint8_t displayCursor(void);
void write(char* chr);
uint8_t newLine(void);
void setPixel(uint8_t x , uint8_t y);

#endif
