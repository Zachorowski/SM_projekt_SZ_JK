#include "MKL05Z4.h"                   
#include "Motor.h"	

typedef enum{ DIR = 6, STEP = 7} motorType;
typedef enum{ ON = 1, OFF = 0 } motorStateType;
void ledCntrl (motorType led, motorStateType state);
static int disance = 30; 

void motorCntrl (motorType motor, motorStateType state) {
	if( state == ON)
		FPTB->PCOR |= (1 << motor);
	if( state == OFF)
		FPTB->PSOR |= (1 << motor);
}
void motor_speed(){
volatile int delay;
	for(delay=0; delay<10000; delay++);
}
void motor_start(){
	 SIM->SCGC5 |=  SIM_SCGC5_PORTB_MASK;     
  PORTB->PCR[DIR] = PORT_PCR_MUX(1);        
  PORTB->PCR[STEP] = PORT_PCR_MUX(1);      
	FPTB->PDDR |= (1 << DIR) | (1 << STEP);          
	

};

void motor_right(){
	motorCntrl(DIR, ON);
	volatile int steps;
	for(steps=0; steps<disance; steps++){
		motorCntrl(STEP, ON);
		motor_speed();
		motorCntrl(STEP, OFF);
		motor_speed();
	}
}

void motor_left(){
		motorCntrl(DIR, OFF);
	volatile int steps;
	for(steps=0; steps<disance; steps++){
		motorCntrl(STEP, ON);
		motor_speed();
		motorCntrl(STEP, OFF);
		motor_speed();
	}
}

