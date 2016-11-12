#include "mytm4c123gh6pm.h"
#include "LCD.H"
#include "init.h"

unsigned short xarray[100];
unsigned short yarray[100];
int xtotal = 0;
int ytotal = 0;

int i = 0;

void mywriteTS(unsigned char CMD) {
	// wait until FIFO is not busy and also FIFO is empty
	while((SSI0b->SR & 0x11) != 0x01);
	SSI0b->DR = CMD;
	SSI0b->DR = 0x0;
	SSI0b->DR = 0x0;
}

void mywriteCmd(unsigned char CMD) {
	// wait until FIFO is not busy and also FIFO is empty
	while((SSI0b->SR & 0x11) != 0x01);
	GPIOB->DATA = 0x0;  // command is 0
	SSI0b->DR = CMD;
}

void mywriteDat(unsigned char DAT) {
	// wait until FIFO is not busy and also FIFO is empty
	while((SSI0b->SR & 0x11) != 0x01);
	GPIOB->DATA = 0x1;  // data is 1
	SSI0b->DR = DAT;
}

void mywriteDat2(unsigned short DAT) {
	// wait until FIFO is not busy and also FIFO is empty
	while((SSI0b->SR & 0x11) != 0x01);
	GPIOB->DATA = 0x1;  // data is 1
	SSI0b->DR = DAT >> 8;
	while((SSI0b->SR & 0x11) != 0x01);
	SSI0b->DR = DAT;
}


#define SSI_SR_RNE 0x00000004 // SSI Rx FIFO Not Empty
#define SSI_SR_TFE 0x00000001 // SSI Tx FIFO Empty

// pulled from textbook, example 7.3
uint8_t sendAfterWaiting(uint8_t code) {
  while((SSI0b->SR & SSI_SR_TFE)==0){};  // wait until FIFO empty
	SSI0b->DR = code;
	while((SSI0b->SR & SSI_SR_RNE)==0){};  // wait until response received
	return SSI0b->DR;

}

void INIT_GPIOE(void) {
  SYSCTLb->RCGCGPIO |= 0x10;
	GPIOE->DIR |= 0xFD;  // PE[1] is input, PE[0] is output
	GPIOE->AFSEL &= 0x00;  
	GPIOE->PUR |= 0x1;   // put pullup on input touchscreen interrupt (active low) and TS_CS (high = deselct)
	GPIOE->DEN |= 0x3;    // 
	
	// enable interrupts
	GPIOE->IM |=0x2;             // allow pin[1] to interrupt
	NVIC->IP[4] = 0x40;          // interrupt priority 2
	NVIC->ISER[0] = 1 << 4;      // Enable interrupt 4 [GPIOE]
}

void getX(void) {
	// Temporary variables to store data as we read
	unsigned short data = 0;

	GPIOE->DATA &= 0xFE; // chip select low
  sendAfterWaiting(0xD0);  // read x-coord
  data = sendAfterWaiting(0) << 5;   // sends 16-bits of 0
	data += sendAfterWaiting(0) >> 3;   // sends 16-bits of 0
	GPIOE->DATA |= 0x1; // chip select high
	xarray[i%100] = data;
}

void getY(void) {
	// Temporary variables to store data as we read
	unsigned short data = 0;
	
	GPIOE->DATA &= 0xFE; // chip select low
  sendAfterWaiting(0x90);  // read x-coord
  data = sendAfterWaiting(0) << 5;   // sends 16-bits of 0
	data += sendAfterWaiting(0) >> 3;   // sends 16-bits of 0
	GPIOE->DATA |= 0x1; // chip select high
	yarray[i%100] = data;
}


// Triggers on touchscreen press
// PE1 == 0 causes this interrupt
void GPIOE_Handler(void) {
	// lightLED();
	int x = 0;
	int divideby = 0;

	// GET TOUCHED COORDINATES
  // while PENIRQ is low, continuously load touchscreen values into our array
	// GPIOE->DATA &= 0xFE;    // put CS low at start of Tx
	while ((GPIOE->DATA & 0x2) == 0x0) {  // polling PE[1], the interrupt
	  getX();   
	  getY();   
	  i++;     // array index for our read X and Y coordinates
	}
	// return chip select to high
	// because this means we have already released the touchscreen and should
	// be calculating a coordinate that was touched
	// GPIOE->DATA |= 0x1;    // put CS high at end of Tx
	
	// PROCESS TOUCH
	// sum all array values, divide by divideby variable
	
		// check if i > 100, if so, we change our divide by value when averaging the actual position
		// because this means someone touched and held for longer than 100 read cycles
		// in which case we need to keep filling array (loop around) and then average
		// the entire array, Ex: touch & hold & drag, then we want to read the final release coordinate
		// and not the initial touch point
	if (i > 100) divideby = 100;
		else divideby = i;
	
  if (divideby!=100) {
		for(x = 0; x < i; x++) {
				xtotal += xarray[x];
				ytotal += yarray[x];
	  }
		xtotal /= i;
		ytotal /= i;
	} else {
				for(x = 0; x < 100; x++) {
				xtotal += xarray[x];
				ytotal += yarray[x];
	  }
		xtotal /= 100;
		ytotal /= 100;
	}
	// at this point xtotal and ytotal represent the actual x and y coordinates
	
	// reset array index at the end of reading an actual coordinate
	// invalidates entire temporary xarray and yarray read tables
	i = 0;
	
	GPIOE->ICR |= 0x2;  // clear interrupt on pin [1]
}

int main(void)
{
	INIT_PLL();
	GPIO_INIT();
	INIT_SSI0();
	
	INIT_GPIOE();

	
	while(1);
}
