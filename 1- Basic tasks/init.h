#ifndef _INIT_H_
#define  _INIT_H_

void CLK_init (void);
void RCC_init (void);
void GPIO_init(void);
void SPI_init (void);
void delay(uint32_t max);
void wait(void);


#endif