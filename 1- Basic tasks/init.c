#include "init.h"
#include "#include "stm32f10x.h"


void CLK_init(void)
{	
// 01: HSE oscillator used as system clock 
//	RCC -> CFGR &= ((uint32_t) 0x1 << 16);
//	RCC -> CFGR |= ((uint32_t)0x1 << 0); //choooooooooose
//	RCC -> CFGR &= ~(((uint32_t)0x1) << 1);
	
	RCC -> CFGR &= ~((uint32_t) 0xffff);      //RESET
	RCC -> CFGR |= (uint32_t) 0x1  << 16 ;    //PLLSRC ==1, HSE is selected = 8MHZ
	RCC -> CFGR |= (uint32_t) 0x7  << 18 ;    //PLLMUL ==7, 8*9 =72MHZ
	RCC -> CFGR |= (uint32_t) 0x5  <<  8 ;    //APB1 Clock divided/2 = 36MHZ
	RCC -> CFGR |= ((uint32_t) 0x2 <<  0);    //PLL is used	
}
void RCC_init(void)
{
	RCC->APB2ENR |= ((uint32_t)0x1)<<0;  //Enable AFIO pin
	RCC->APB2ENR |= ((uint32_t)0x3)<<2;  //Enable ports A,B
	/*--------- new code ---------*/
	
	
	/*--------- new code ---------*/
	RCC->APB1ENR |= ((uint32_t)0x1)<<14; //ENABLE SPI2
	RCC->CFGR    &= ~((uint16_t)0x3 << 14); // Reset ADC prescaler
	RCC->CFGR    |=  ((uint16_t)0x2 << 14); // Set ADC prescaler to 6 (72MHZ/6 = 12 MHZ)
	RCC->APB2ENR |=  ((uint16_t)0x1 << 2 ); // Enable GPIOA
	RCC->APB2ENR |=  ((uint16_t)0x1 << 9 ); // Enable ADC1 Clock
}

void GPIO_init(void)
{
	GPIOA->CRL &= ~((uint32_t)0xf << 4); //PIN A1 : MODE : INPIT - CNF ANALOG
	
	GPIOB->CRH &= ~(((uint32_t) 0xf)  << 16) ;   //PB12 OUTPUT - AFIO - PUSH PULL
	GPIOB->CRH |=  ((uint32_t)  0xa)  << 16  ;   //PB12 OUTPUT - AFIO - PUSH PULL      //////NSS//////
	
	GPIOB->CRH &= ~(((uint32_t) 0xf)  << 20) ;   //PB13 OUTPUT - AFIO - PUSH PULL
	GPIOB->CRH |=  ((uint32_t)  0xa)  << 20  ;   //PB13 OUTPUT - AFIO - PUSH PULL      //////SCK//////
  
	GPIOB->CRH &= ~(((uint32_t)  0xf) << 24) ;   //PB14 OUTPUT - AFIO - PUSH PULL
	GPIOB->CRH |=  ((uint32_t)  0xa)  << 24  ;   //PB15 OUTPUT - AFIO - PUSH PULL      //////MISO//////
	
	GPIOB->CRH &= ~(((uint32_t) 0xf)  << 28) ;   //PB15 OUTPUT - AFIO - PUSH PULL
	GPIOB->CRH |=  ((uint32_t)  0xa)  << 28  ;   //PB15 OUTPUT - AFIO - PUSH PULL      //////MOSI//////

	GPIOA->CRH &= ~(((uint32_t) 0xf)  << 0) ;    //PA8 OUTPUT - GENERAL PURPUSE - PUSH PULL
	GPIOA->CRH |=  (((uint32_t) 0x2)  << 0) ;    //PA8 OUTPUT - GENERAL PURPUSE - PUSH PULL
	
	GPIOA->CRH &= ~(((uint32_t) 0xf)  << 4) ;    //PA9 OUTPUT - GENERAL PURPUSE - PUSH PULL
	GPIOA->CRH |=  (((uint32_t) 0x2)  << 4) ;    //PA9 OUTPUT - GENERAL PURPUSE - PUSH PULL
	
}

void SPI_init(void)
{ 
	SPI2->CR1  =  0x0000;
	SPI2->CR2 |= ((uint16_t)0x1)<<2;
	
	SPI2->CR1 |= ((uint16_t)0x01)<<3;    //divide by 16
	
	SPI2->CR1 |= ((uint16_t)0x1)<<2;    // Master mode
	SPI2->CR1 |= ((uint16_t)0x1)<<15;   // BIDIRECTION
 	SPI2->CR1 |= ((uint16_t)0x1)<<14;   // TRANSMIT ONLY
	SPI2->CR1 |= ((uint16_t)0x1)<<6;    // SPI1 enable
}

void delay(uint32_t max)
{
	uint32_t volatile count = 0 ;
	while( count < max )
	{
		count++;
	}
}


void wait(void)
{
	while ((*sr_ptr & ((uint16_t)0x1 << 1 ) ) != 0x2 );
}
