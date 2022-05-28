#include <stdint.h>
#include "lm4f120h5qr.h"


#define LED_RED   (1U << 1)
#define LED_BLUE  (1U << 2)
#define LED_GREEN (1U << 3)


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode

volatile uint32_t FallingEdges = 0;
const char lut_RGB[8]={
	0xF1,	//		0001
	0xF3,	//R		0011
	0xF5,	//B		0101
	0xF7,	//RB	0111
	0xF9,	//G		1001
	0xFB,	//GR	1011
	0xFD,	//GB	1101
	0xFF	//GBR	1111
};

void EdgeCounter_Init(void){                          
  	DisableInterrupts();
	SYSCTL_RCGCGPIO_R |= 0x00000020; // (a) activate clock for port F
  	FallingEdges = 0;
	GPIO_PORTF_DIR_R |= 0x0E;    // (c) make PF4 in (built-in button)
	GPIO_PORTF_DEN_R |= 0x1E;     //     enable digital I/O on PF4   
  	GPIO_PORTF_PUR_R |= 0x10;     //     enable weak pull-up on PF4
	GPIO_PORTF_IS_R &= ~0x10;     // (d) PF4 is edge-sensitive
	GPIO_PORTF_IBE_R &= ~0x10;    //     PF4 is not both edges
	GPIO_PORTF_IEV_R &= ~0x10;    //     PF4 falling edge event
	GPIO_PORTF_ICR_R = 0x10;      // (e) clear flag4
	GPIO_PORTF_IM_R |= 0x10;      // (f) arm interrupt on PF4 *** No IME bit as mentioned in Book ***
	NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
	NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
	EnableInterrupts();           // (i) Clears the I bit
}

void GPIOF_Handler(void)
{
	GPIO_PORTF_ICR_R = 0x10;      // acknowledge flag4
 	FallingEdges = FallingEdges + 1;
}

void delay(unsigned long value)
{
	unsigned long i;
	for(i=0;i<value;i++);
}

void DisplayRGB(void)
{
	if (FallingEdges>7) FallingEdges=0;//waitforSW1press,polllow
	GPIO_PORTF_DATA_R|=lut_RGB[FallingEdges];//REDon,makePF1high
	delay(1000000);
	GPIO_PORTF_DATA_R&=lut_RGB[0];//REDoff,makePF1low
	delay(1000000);
}

int main(void)
{
  EdgeCounter_Init();           // initialize GPIO Port F interrupt
  while(1)
  {
	DisplayRGB();
  }
}

void DisableInterrupts(void)
{
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
