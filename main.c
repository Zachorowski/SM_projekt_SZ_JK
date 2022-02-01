#include "MKL05Z4.h"
#include "ADC.h"
#include "pit.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "motor.h"
#include "buttons.h"
#include "uart0.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

float volt_coeff = ((float)(((float)2.91) / 4095) );	
	
uint8_t wynik_ok=0;
uint16_t temp;
double	wynik;

void PIT_IRQHandler()
{
	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;		
	NVIC_ClearPendingIRQ(PIT_IRQn);
}

void ADC0_IRQHandler()
{	
	temp = ADC0->R[0];		
	if(!wynik_ok)					
	{
		wynik = temp;			
		wynik_ok=1;
	}
	NVIC_ClearPendingIRQ(ADC0_IRQn);
}
int main (void)
{
	uint8_t	kal_error;
	char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	LCD1602_Init();		 
	LCD1602_Backlight(TRUE);
	LCD1602_Print("---");		
	PIT_Init();

	kal_error=ADC_Init();	
	if(kal_error)
	{
		while(1);							
	}
	
	buttonsInitialize();
	motor_start();
	

	ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(0);		
	
	UART0_Init();	
	uint32_t i=0;
	while(1)
	{
		if(wynik_ok)
		{
			wynik=12.332*wynik*volt_coeff/129;
			wynik = pow((double)wynik,(double)-1.063) ;
			sprintf(display,"d=%.4fcm%c",wynik,0xd);
			//wynik=wynik*volt_coeff;
			//sprintf(display,"U=%.4fV%c",wynik,0xd);
			LCD1602_SetCursor(0,0);
			LCD1602_Print(display);
			wynik_ok=0;
			for(i=0;display[i]!=0;i++)
			{
				while(!(UART0->S1 & UART0_S1_TDRE_MASK));
				UART0->D = display[i];
			}
			
		}
	}
}
