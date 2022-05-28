int main(void) {
    *((unsigned int *)0x400FE608U) = 0x20U; //Clock Gating
    *((unsigned int *)0x40025400U) = 0x0EU; //GPIO In or Out
    *((unsigned int *)0x4002551CU) = 0x0EU; //Analog or Digital 

    while (1) {
        *((unsigned int *)0x400253FCU) = 0x04U; //GPIO Data enable Bit

        int volatile counter = 0;
        while (counter < 1000000) {
            ++counter;
        }

        *((unsigned int *)0x400253FCU) = 0x00U;
        counter = 0;
        while (counter < 1000000) {
            ++counter;
        }

    }
    //return 0;
}
