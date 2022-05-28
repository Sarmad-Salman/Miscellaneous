	#include <stdint.h>
	#include "lm4f120h5qr.h"


	#define LED_RED   (1U << 1)
	#define LED_BLUE  (1U << 2)
	#define LED_GREEN (1U << 3)


	void DisableInterrupts(void);
	void EnableInterrupts(void);
	void WaitForInterrupt(void);

	volatile uint32_t Counts=0;

	const char lut_RGB[8]=
	{
		0xF1,	//		0001
		0xF3,	//R		0011
		0xF5,	//B		0101
		0xF7,	//RB	0111
		0xF9,	//G		1001
		0xFB,	//GR	1011
		0xFD,	//GB	1101
		0xFF	//GBR	1111
	};


	void SysTick_Init(uint32_t period)
	{
		 DisableInterrupts();
		
	 	 NVIC_ST_CTRL_R = 0;  // 0 ---> Disable control register

		 NVIC_ST_RELOAD_R = period-1; //Period = Clock Frequncy * (Desired ON Time)

	  	 NVIC_ST_CURRENT_R = 0; // Clears when any number is written/allotted

		 NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000;
		 
		 NVIC_ST_CTRL_R = 0x07;  // Enabling Control Bit, Reload Bit, Current Bit
					 // Reload Bit = 0 ---> Infinite loop
					 //Reload Bit = 1 ---> Exit from loop

		 EnableInterrupts();
	}


	void SysTick_Handler(void)
	{
	   GPIO_PORTF_DATA_R^= 0x04;
	   Counts = Counts + 1;
	}


	int main(void)
	{
	   SYSCTL_RCGCGPIO_R |= 0x20;  // activate port F
  	   Counts = 0;
	   GPIO_PORTF_DIR_R |= 0x04;   // make PF2 output (PF2 built-in LED)
	   GPIO_PORTF_DEN_R |= 0x04;   // enable digital I/O on PF2
	   SysTick_Init(16000000);     // initialize SysTick timer // reload value   Period Value=16MHz * desired ON time
	   EnableInterrupts();
	   while(1)
	   {                   
	     WaitForInterrupt();       // interrupts every 1ms, 500 Hz flash
	   }
	}


	void DisableInterrupts(void)
	{-+
		__asm ("    CPSID  I\n");
	}


	void EnableInterrupts(void)
	{
		__asm  ("    CPSIE  I\n");
	}


	void WaitForInterrupt(void)
	{
		__asm  ("    WFI\n");
	}
