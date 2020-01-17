#include "Nano100Series.h"
#include <stdio.h>

//--- USB HID
void init_USB(void);
int HID_output(char *src);
uint8_t* HID_input(void);

//---
#define	RS	PB6
#define	RST	PD8
#define	BL	PD9

//--- tft.c
void init_TFT(void);
void tft_test(void);
void tft_box(uint16_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void BlockWrite(uint16_t X0,uint16_t X1,uint16_t Y0,uint16_t Y1);
void WriteData(uint8_t tem_data);

void init_EBI(void){
	SYS_UnlockReg();
	//--- CLK
	CLK_EnableModuleClock(EBI_MODULE);
	CLK_SetModuleClock(EBI_MODULE, 0, 0);
	//--- PIN
	SYS->PA_H_MFP=(SYS->PA_H_MFP &~ SYS_PA_H_MFP_PA10_MFP_Msk)|SYS_PA_H_MFP_PA10_MFP_EBI_NWE;
  SYS->PA_H_MFP=(SYS->PA_H_MFP &~ SYS_PA_H_MFP_PA11_MFP_Msk)|SYS_PA_H_MFP_PA11_MFP_EBI_NRE;
  SYS->PB_L_MFP=(SYS->PB_L_MFP &~ SYS_PB_L_MFP_PB7_MFP_Msk)|SYS_PB_L_MFP_PB7_MFP_EBI_NCS;
  SYS->PB_H_MFP=(SYS->PB_H_MFP &~ SYS_PB_H_MFP_PB12_MFP_Msk)|SYS_PB_H_MFP_PB12_MFP_EBI_AD0;
  SYS->PB_H_MFP=(SYS->PB_H_MFP &~ SYS_PB_H_MFP_PB13_MFP_Msk)|SYS_PB_H_MFP_PB13_MFP_EBI_AD1;
  SYS->PC_H_MFP=(SYS->PC_H_MFP &~ SYS_PC_H_MFP_PC14_MFP_Msk)|SYS_PC_H_MFP_PC14_MFP_EBI_AD2;
  SYS->PC_H_MFP=(SYS->PC_H_MFP &~ SYS_PC_H_MFP_PC15_MFP_Msk)|SYS_PC_H_MFP_PC15_MFP_EBI_AD3;
  SYS->PC_L_MFP=(SYS->PC_L_MFP &~ SYS_PC_L_MFP_PC6_MFP_Msk)|SYS_PC_L_MFP_PC6_MFP_EBI_AD4;
  SYS->PC_L_MFP=(SYS->PC_L_MFP &~ SYS_PC_L_MFP_PC7_MFP_Msk)|SYS_PC_L_MFP_PC7_MFP_EBI_AD5;
  SYS->PA_L_MFP=(SYS->PA_L_MFP &~ SYS_PA_L_MFP_PA7_MFP_Msk)|SYS_PA_L_MFP_PA7_MFP_EBI_AD6;
  SYS->PA_L_MFP=(SYS->PA_L_MFP &~ SYS_PA_L_MFP_PA6_MFP_Msk)|SYS_PA_L_MFP_PA6_MFP_EBI_AD7;
  SYS->PA_L_MFP=(SYS->PA_L_MFP &~ SYS_PA_L_MFP_PA5_MFP_Msk)|SYS_PA_L_MFP_PA5_MFP_EBI_AD8;
  SYS->PA_L_MFP=(SYS->PA_L_MFP &~ SYS_PA_L_MFP_PA4_MFP_Msk)|SYS_PA_L_MFP_PA4_MFP_EBI_AD9;
  SYS->PA_L_MFP=(SYS->PA_L_MFP &~ SYS_PA_L_MFP_PA3_MFP_Msk)|SYS_PA_L_MFP_PA3_MFP_EBI_AD10;
  SYS->PA_L_MFP=(SYS->PA_L_MFP &~ SYS_PA_L_MFP_PA2_MFP_Msk)|SYS_PA_L_MFP_PA2_MFP_EBI_AD11;
  SYS->PA_L_MFP=(SYS->PA_L_MFP &~ SYS_PA_L_MFP_PA1_MFP_Msk)|SYS_PA_L_MFP_PA1_MFP_EBI_AD12;
  SYS->PA_H_MFP=(SYS->PA_H_MFP &~ SYS_PA_H_MFP_PA12_MFP_Msk)|SYS_PA_H_MFP_PA12_MFP_EBI_AD13;
  SYS->PA_H_MFP=(SYS->PA_H_MFP &~ SYS_PA_H_MFP_PA13_MFP_Msk)|SYS_PA_H_MFP_PA13_MFP_EBI_AD14;
  SYS->PA_H_MFP=(SYS->PA_H_MFP &~ SYS_PA_H_MFP_PA14_MFP_Msk)|SYS_PA_H_MFP_PA14_MFP_EBI_AD15;
	SYS_LockReg();
	//---	
	GPIO_SetMode(PB, BIT6, GPIO_PMD_OUTPUT);    //RS
	GPIO_SetMode(PD, BIT8, GPIO_PMD_OUTPUT);    //RST
	GPIO_SetMode(PD, BIT9, GPIO_PMD_OUTPUT);    //BL
	RS=1; RST=1; BL=1;
	//--- OPEN
	EBI_Open(0, EBI_BUSWIDTH_8BIT, EBI_TIMING_FASTEST, 0, 0);
}
//--- systick ----------------------------------------------
uint32_t tick=0;
void SysTick_Handler(void){
	tick++;
}

void delay_ms(uint32_t ms){
	ms += tick;
	while(ms != tick){}
}


int cnt=0;
uint16_t cc=0x1042, sta=0;
uint32_t wh=0;
void usb_test(uint8_t *p){
	if(sta==0){
		uint16_t x,y,w,h;
		x = p[1]*256+p[0];
		y = p[3]*256+p[2];
		w = p[5]*256+p[4];
		h = p[7]*256+p[6];
		wh = w*h;
		BlockWrite(x, x+w-1, y, y+h-1);
		sta=1;
	}
	if(sta==1){
		for(int i=0;i<64;i++){
			WriteData(*p++);
			wh--;
			if(wh==0){ sta=0; break;}
		}
	}
}

int main(void){
	init_USB();
	SysTick_Config(SystemCoreClock/1000);
	init_EBI();
	init_TFT();
	tft_test();
	while(1){
		uint8_t *p = HID_input();
		if(p){ HID_output((char *)p); usb_test(p);	}
	}
}
