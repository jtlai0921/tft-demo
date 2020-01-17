#include "Nano100Series.h"
#include <stdio.h>
extern const uint16_t data[1576];
extern const uint16_t data2[482];
//--------------- Systick ------------------
uint32_t tick=0;
void SysTick_Handler(void){
    tick++;
}
 
void delay_ms(uint32_t ms){
    ms += tick;
    while( ms != tick){}
}
//----------------- TIMER0 --------------------
void init_TIMER0(int freq){
  SYS_UnlockReg();
    //--- CLK
    CLK_EnableModuleClock(TMR0_MODULE);
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HIRC, 0);
    //--- PIN
  SYS_LockReg();
    //--- OPEN
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, freq);
    TIMER_Start(TIMER0);
    //--- NVIC
    TIMER_EnableInt(TIMER0);
    NVIC_EnableIRQ(TMR0_IRQn);
}
    //--- ISR
uint16_t run=1;
uint16_t nn=0, d0=0, d1=0;
char bf[64];
void TMR0_IRQHandler(void){
	d1=d0; d0=data[nn++];
	if(run){	printf("%d\n", d0); }
	if(nn>1576-1){ nn=0; }
    TIMER_ClearIntFlag( TIMER0 );
}
void TMR0_IRQHandler1(void){
	d1=d0; d0=data2[nn++];
	if(run){	printf("%d.%d\n", d0/100,d0%100); }
	if(nn>481){ nn=0; }
    TIMER_ClearIntFlag( TIMER0 );
}
//----------- UART1 --------------
void init_UART1(void){
  SYS_UnlockReg();
    //--- CLK
    CLK_EnableModuleClock(UART1_MODULE);
    CLK_SetModuleClock( UART1_MODULE, CLK_CLKSEL1_UART_S_HIRC, CLK_UART_CLK_DIVIDER(1) );
    //--- PIN
    SYS->PB_L_MFP &=~ SYS_PB_L_MFP_PB4_MFP_Msk;
    SYS->PB_L_MFP |=  SYS_PB_L_MFP_PB4_MFP_UART1_RX;
    SYS->PB_L_MFP &=~ SYS_PB_L_MFP_PB5_MFP_Msk;
    SYS->PB_L_MFP |=  SYS_PB_L_MFP_PB5_MFP_UART1_TX;
  SYS_LockReg();
    //--- OPEN
    UART_Open(UART1, 9600);
    //--- NVIC
    UART_ENABLE_INT(UART1, UART_IER_RDA_IE_Msk);
    NVIC_EnableIRQ(UART1_IRQn);
}
  //--- IRQ
void UART1_IRQHandler(void){
	char c;
    c = UART1->RBR;
	if(c=='1'){ run=1; }
	if(c=='0'){ run=0; }
}
void init(void){
	d0=data[0];
    SysTick_Config( SystemCoreClock /1000);
	init_UART1();
	init_TIMER0(120);
}

int main(void){
	init();
	while(1){
	}
}
