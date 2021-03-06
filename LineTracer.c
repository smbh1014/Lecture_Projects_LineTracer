#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>


#define MOTOR_PORT		PORTD
#define MOTOR_PORT_DDR	DDRD

#define SENSOR_PORT		PINC


void port_init(void)
{
 PORTA = 0x00;
 DDRA  = 0x00;
 PORTB = 0x00;
 DDRB  = 0x00;
 PORTC = 0x00;
 DDRC  = 0x00;
 PORTD = 0x00;
 DDRD  = 0xFF;
 PORTE = 0x00;
 DDRE  = 0x00;
 PORTF = 0x00;
 DDRF  = 0x00;
 PORTG = 0x00;
 DDRG  = 0x03;
}

void init_devices(void)
{
 cli(); //disable all interrupts
 XDIV  = 0x00; //xtal divider
 XMCRA = 0x00; //external memory
 port_init();

 MCUCR = 0x00;
 EICRA = 0x00; //extended ext ints
 EICRB = 0x00; //extended ext ints
 EIMSK = 0x00;
 TIMSK = 0x00; //timer interrupt sources
 ETIMSK = 0x00; //extended timer interrupt sources
 sei(); //re-enable interrupts
 //all peripherals are now initialized
}


/* Stepping Motor derive---------------------------*/
unsigned char  LEFTmotorOneClock(unsigned char step, char dir)
{	
	step = step & 0x0f;
	if(dir){
		switch(step){
			case 0x09: step=0x01; break;
			case 0x01: step=0x03; break;
			case 0x03: step=0x02; break;
			case 0x02: step=0x06; break;
			case 0x06: step=0x04; break;
			case 0x04: step=0x0c; break;
			case 0x0c: step=0x08; break;
			case 0x08: step=0x09; break;
			default: step=0x0c; break;
		}
	}else{
		switch(step){
			case 0x09: step=0x08; break;
			case 0x01: step=0x09; break;
			case 0x03: step=0x01; break;
			case 0x02: step=0x03; break;
			case 0x06: step=0x02; break;
			case 0x04: step=0x06; break;
			case 0x0c: step=0x04; break;
			case 0x08: step=0x0c; break;
			default: step=0x0c; break;
		}
	}
	return step;

}


/* Stepping Motor derive---------------------------*/
unsigned char  RIGHTmotorOneClock(unsigned char step, char dir)
{	
	step = step & 0xf0;
	if(dir){
		switch(step){
			case 0x90: step=0x10; break;
			case 0x10: step=0x30; break;
			case 0x30: step=0x20; break;
			case 0x20: step=0x60; break;
			case 0x60: step=0x40; break;
			case 0x40: step=0xc0; break;
			case 0xc0: step=0x80; break;
			case 0x80: step=0x90; break;
			default: step=0xc0; break;
		}
	}else{
		switch(step){
			case 0x90: step=0x80; break;
			case 0x10: step=0x90; break;
			case 0x30: step=0x10; break;
			case 0x20: step=0x30; break;
			case 0x60: step=0x20; break;
			case 0x40: step=0x60; break;
			case 0xc0: step=0x40; break;
			case 0x80: step=0xc0; break;
			default: step=0xc0; break;
		}
	}
	return step;
}


void delay(int n)
{
	volatile int i,j;
	for(i=1;i<n;i++)
	{
    	for(j=1;j<400;j++);
	}
}


void main(void)
{

 unsigned char sensor;
 volatile unsigned char stepleft=0, stepright=0;
 
 init_devices();
 	
 MOTOR_PORT_DDR = 0xff;
 
 while(1){
   		sensor = SENSOR_PORT & 0x0F;

		switch(sensor){
			case 0x0f:  
						stepright = RIGHTmotorOneClock(stepright, 1);
					    stepleft = LEFTmotorOneClock(stepleft, 0);
						delay(5);
						PORTB= 0x00;
						break;
			case 0x0b:
						stepleft = LEFTmotorOneClock(stepleft, 0);
					    delay(5);
					    PORTB= 0x02;
						break;
			case 0x0d:  
		    	 		stepright = RIGHTmotorOneClock(stepright, 1);
					    delay(5);
						PORTB= 0x01;
					    break;			
			default:    
						stepright = RIGHTmotorOneClock(stepright, 1);
					    stepleft = LEFTmotorOneClock(stepleft, 0);
						delay(5);
						PORTB= 0x00;
						break;
		}		
		MOTOR_PORT = stepleft|stepright;
 }
}

