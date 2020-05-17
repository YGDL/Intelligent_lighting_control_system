#include "stm32f10x.h"

void delay_ms(vu32 ms)
{
	vu32 clk=0;
	for(; ms!=0 ; ms--)
		for(clk=72000 ; clk!=0 ; clk--);
}

void delay_us(vu8 us)
{
	vu8 clk=0;
	for(; us!=0 ; us--)
		for(clk=72 ; clk!=0 ; clk--);
}
