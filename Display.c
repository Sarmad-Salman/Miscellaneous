
#define SYSCTL_RCGCGPIO_R	(*((volatile unsigned long*)0x400FE608C))

#define GPIO_PORTB_DATA_R	(*((volatile unsigned long*)0x400053FC))
#define GPIO_PORTB_DIR_R	(*((volatile unsigned long*)0x40005400))
#define GPIO_PORTB_AFSEL_R	(*((volatile unsigned long*)0x40005420))
#define GPIO_PORTB_DEN_R	(*((volatile unsigned long*)0x4000551C))
#define GPIO_PORTB_PCTL_R	(*((volatile unsigned long*)0x4000552C))

#define GPIO_PORTA_DATA_R	(*((volatile unsigned long*)0x400043FC))
#define GPIO_PORTA_DIR_R	(*((volatile unsigned long*)0x40004400))

#define GPIO_PORTA_AFSEL_R	(*((volatile unsigned long*)0x40004420))
#define GPIO_PORTA_DEN_R	(*((volatile unsigned long*)0x4000451C))
#define GPIO_PORTA_PCTL_R	(*((volatile unsigned long*)0x4000452C))

#define GPIO_PORTF_DATA_R	(*((volatile unsigned long*)0x400253FC))
#define GPIO_PORTF_DIR_R	(*((volatile unsigned long*)0x40025400))
#define GPIO_PORTF_AFSEL_R	(*((volatile unsigned long*)0x40025420))
#define GPIO_PORTF_DEN_R	(*((volatile unsigned long*)0x4002551C))
#define GPIO_PORTF_PCTL_R	(*((volatile unsigned long*)0x4002552C))
#define GPIO_PORTF_PUR_R	(*((volatile unsigned long*)0x40025510))


#define SEG1	0xFB
#define SEG2	0xF7
#define SEG3	0xEF
#define SEG4	0xDF

void init_gpio(void);
void wait_for_key(void);
void display_uofs(void);
void display_hello(void);
void delay(unsigned long value);

//PortBpins:	76543210
//	1edcbafg

//LookuptableforUOFS
const char lut_uofs[4]={0xC1,	//U	11000001
	0xC0,	//O	11000000
	0x8E,	//F	10001110
	0x92	//S	10010010
	};


//LookuptableforHELLO
const char lut_hello[5]={0x89,	//H	10001001
	0x86,	//E	10000110
	0xC7,	//L	11000111
	0xC7,	//L	11000111
	0xC0	//O	11000000
	};

//Initializationfunctionforports
void init_gpio(void){
	volatile unsigned long delay_clk; //delay for clock, must have 3 sys clockdelay
  SYSCTL_RCGCGPIO_R |= 0x23U;
	delay_clk=SYSCTL_RCGCGPIO_R;

	GPIO_PORTB_PCTL_R &=0x00000000;
	GPIO_PORTB_AFSEL_R &= ~0xFF;
	GPIO_PORTB_DIR_R|=0xFF;
	GPIO_PORTB_DEN_R|=0xFF;

	GPIO_PORTA_PCTL_R&=0x00000000;
	GPIO_PORTA_AFSEL_R &= ~0x3C;
	GPIO_PORTA_DIR_R|=0x3C;
	GPIO_PORTA_DEN_R|=0x3C;
	GPIO_PORTF_PCTL_R&=0xFFF0FF0F;
	GPIO_PORTA_AFSEL_R &= ~0x12;
	GPIO_PORTF_DEN_R|=0x12;
	GPIO_PORTF_DIR_R|=0x02;
	GPIO_PORTF_PUR_R|=0x10;

}

//otherfunctions
void wait_for_key(void){

	while(GPIO_PORTF_DATA_R&0x10);//waitforSW1press,polllow
	GPIO_PORTF_DATA_R|=0x02;//REDon,makePF1high
	delay(1000000);
	GPIO_PORTA_AFSEL_R &= ~0x2;//REDoff,makePF1low
	delay(1000000);
}

//displays UOFS on display board

void display_uofs(void){
	while(GPIO_PORTF_DATA_R&0x10)
	{

	GPIO_PORTA_DATA_R=0xFF;
	GPIO_PORTB_DATA_R=lut_uofs[0];
	GPIO_PORTA_DATA_R=SEG1;
	delay(10000);


	GPIO_PORTA_DATA_R=0xFF;
	GPIO_PORTB_DATA_R=lut_uofs[1];
	GPIO_PORTA_DATA_R=SEG2;
	delay(10000);


	GPIO_PORTA_DATA_R=0xFF;
	GPIO_PORTB_DATA_R=lut_uofs[2];
	GPIO_PORTA_DATA_R=SEG3;
	delay(10000);

	GPIO_PORTA_DATA_R=0xFF;
	GPIO_PORTB_DATA_R=lut_uofs[3];
	GPIO_PORTA_DATA_R=SEG4;
	delay(10000);
	}
}

//displaysHELOondisplayboard

void display_helo(void){
	int i=0;
	while(i<100)
	{
	GPIO_PORTA_DATA_R=0xFF;
	GPIO_PORTB_DATA_R=lut_hello	[0];
	GPIO_PORTA_DATA_R=SEG1;
	delay(10000);

	GPIO_PORTA_DATA_R=0xFF;
	GPIO_PORTB_DATA_R=lut_hello	[1];
	GPIO_PORTA_DATA_R=SEG2;
	delay(10000);

	GPIO_PORTA_DATA_R=0xFF;
	GPIO_PORTB_DATA_R=lut_hello	[2];
	GPIO_PORTA_DATA_R=SEG3;
	delay(10000);

	GPIO_PORTA_DATA_R=0xFF;
	GPIO_PORTB_DATA_R=lut_hello	[4];
	GPIO_PORTA_DATA_R=SEG4;
	delay(10000);
	i++;
	}
}

void delay(unsigned long value){
	unsigned long i;
	for(i=0;i<value;i++);
}

int main(void){

	init_gpio();

	while(1){
	display_uofs();
	wait_for_key();//pollingSW1(PF4)
	display_helo();
	}
}



