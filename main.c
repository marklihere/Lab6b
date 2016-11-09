#include "tm4c123gh6pm.h"

unsigned short xarray[100];
unsigned short yarray[100];

int i = 0;


void INIT_GPIOE(void) {
  SYSCTL->RCGCGPIO = 0x10;
	GPIOE->AFSEL &= 0x00;  
	GPIOE->DIR &= 0xFE;  // PE[0] is input
	GPIOE->DEN |= 0x1;    // 
}

void getX(void) {
	unsigned short data;
	unsigned short realdata;
  writeCmd(0xD0);  // read x-coord
  writeData2(0);   // sends 16-bits of 0
  data = SSI0->DR;  // get data
  realdata = (data & 0x7FFF) >> 3;
	xarray[i] = realdata;
}

void getY(void) {
}


void GPIOE_Handler(void) {
  //enable touchscreen chip select
  
	while ((GPIOE->DATA & 0x1) == 0x0) {
	  getX();
	  getY();
	  i++;
	}
	
	// sum all array values, divide by i
	// FINISH ME
	
	GPIOE->ICR |= 0x1;  // clear interrupt
}

int main(void)
{
	INIT_GPIOE();

	
	while(1);
}
