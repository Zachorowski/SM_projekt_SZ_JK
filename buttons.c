#include "MKL05Z4.h"                   
#include "buttons.h"
#include "motor.h"


/* Buttons connected to port B */
typedef enum{ BUT1 = 1, BUT2 = 2 } ButtonType;

static const IRQn_Type myPORT_IRQn = 31; 

void PORTB_IRQHandler(void){		

	if( PORTB->ISFR & (1 << BUT1) ){        
		motor_right();
     while((FPTB->PDIR&(1<<BUT1))==0);				
		 PORTB->PCR[BUT1] |= PORT_PCR_IRQC_MASK; 
  }
	
	if( PORTB->ISFR & (1 << BUT2) ){          
		motor_left();
    while((FPTB->PDIR&(1<<BUT2))==0);					
		 PORTB->PCR[BUT2] |= PORT_PCR_IRQC_MASK; 

  }
}

void buttonsInitialize(void){
	SIM->SCGC5 |=  SIM_SCGC5_PORTB_MASK; 				
	PORTB->PCR[BUT1] |= PORT_PCR_MUX(1);      	
	PORTB->PCR[BUT2] |= PORT_PCR_MUX(1);      	
	
	
	PORTB->PCR[BUT1] |=  PORT_PCR_PE_MASK |		
											 PORT_PCR_PS_MASK;	   
	PORTB->PCR[BUT1] |= 	PORT_PCR_IRQC(10);		
	PORTB->PCR[BUT2] |=  PORT_PCR_PE_MASK |		
											 PORT_PCR_PS_MASK;			
	PORTB->PCR[BUT2] |=  PORT_PCR_IRQC(10);	  

	NVIC_ClearPendingIRQ(myPORT_IRQn);			
	NVIC_EnableIRQ(myPORT_IRQn);							
	
	NVIC_SetPriority (myPORT_IRQn, 1);			

}



