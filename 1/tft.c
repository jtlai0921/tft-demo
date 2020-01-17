#include "Nano100Series.h"
#include <stdio.h>
extern const uint32_t data2[];
extern int im;
extern const char Font16_Table[];

#define	RS	PB6
#define	RST	PD8
#define	BL	PD9

void delay_ms(uint32_t ms);

//--- TFT ---------------------------------------------------
void WriteComm(uint8_t CMD)
{
	RS=0;
	*(__IO uint8_t *) (EBI_BASE_ADDR) = CMD;
}
void WriteData(uint8_t tem_data)
{	
	RS=1;	
	*(__IO uint8_t *) (EBI_BASE_ADDR) = tem_data;
}
//--------init_TFT
void init_TFT(void){
	
	//--- LCD_Rst();
    RST=0;          // Clr_Rst;
    delay_ms(100);  //Delay(10000);				   
    RST=1;          //Set_Rst;		 	 
    delay_ms(100);  //Delay(10000);	
	//---
	//S6D05A1
	
WriteComm(0x13);
WriteComm(0x35);
WriteData(0x00);
WriteComm(0x36);
WriteData(0x48);
WriteComm(0xD0);
WriteData(0x00);
WriteData(0x05);

WriteComm(0xEF);
WriteData(0x07);


WriteComm(0xF2);
WriteData(0x1B);
WriteData(0x16);
WriteData(0x0F);
WriteData(0x08);
WriteData(0x08);
WriteData(0x08);
WriteData(0x08);
WriteData(0x10);
WriteData(0x00);
WriteData(0x1C);
WriteData(0x16);

WriteComm(0xF3);
WriteData(0x01);
WriteData(0x41);
WriteData(0x15);
WriteData(0x0D);
WriteData(0x33);
WriteData(0x63);
WriteData(0x46);
WriteData(0x10);


WriteComm(0xF4);
WriteData(0x5B);
WriteData(0x5B);
WriteData(0x55);
WriteData(0x55);
WriteData(0x44);

WriteComm(0xF5);
WriteData(0x12);
WriteData(0x11);
WriteData(0x06);
WriteData(0xF0);
WriteData(0x00);
WriteData(0x1F);

WriteComm(0xF6);
WriteData(0x80);
WriteData(0x10);
WriteData(0x00);

WriteComm(0xFD);
WriteData(0x11);
WriteData(0x1D);
WriteData(0x00);


WriteComm(0xF8); //Positive Gamma Control
WriteData(0x00);
WriteData(0x15);
WriteData(0x01);
WriteData(0x08);
WriteData(0x15);
WriteData(0x22);
WriteData(0x25);
WriteData(0x28);
WriteData(0x14);
WriteData(0x13);
WriteData(0x10);
WriteData(0x11);
WriteData(0x09);
WriteData(0x24);
WriteData(0x28);

WriteComm(0xF9); //Positive Gamma Control
WriteData(0x00);
WriteData(0x15);
WriteData(0x01);
WriteData(0x08);
WriteData(0x15);
WriteData(0x22);
WriteData(0x25);
WriteData(0x28);
WriteData(0x14);
WriteData(0x13);
WriteData(0x10);
WriteData(0x11);
WriteData(0x09);
WriteData(0x24);
WriteData(0x28);

WriteComm(0xFC); //Positive Gamma Control
WriteData(0x00);
WriteData(0x15);
WriteData(0x01);
WriteData(0x08);
WriteData(0x15);
WriteData(0x22);
WriteData(0x25);
WriteData(0x28);
WriteData(0x14);
WriteData(0x13);
WriteData(0x10);
WriteData(0x11);
WriteData(0x09);
WriteData(0x24);
WriteData(0x28);


WriteComm(0x36);//Memory Data Access Control
WriteData(0x40);

WriteComm(0x3A);//SET 65K Color
WriteData(0x55);

WriteComm(0x11); 
delay_ms(120);  //Delay(120);

WriteComm(0x29);//Display on
WriteComm(0x2C);//Write GRAM	

delay_ms(10);   //Delay(10);
	
BL=1;           //Lcd_Light_ON;
}
//--------BlockWrite
void BlockWrite(uint16_t X0,uint16_t X1,uint16_t Y0,uint16_t Y1) 
{
	//R61581

	WriteComm(0x2a);   
	WriteData(X0>>8);
	WriteData(X0&0xff);
	WriteData(X1>>8);
	WriteData(X1&0xff);
	WriteComm(0x2b);   
	WriteData(Y0>>8);
	WriteData(Y0&0xff);
	WriteData(Y1>>8);
	WriteData(Y1&0xff);
	WriteComm(0x2c);
}
//---------------------------------
uint16_t FG = 0xFFFF;
uint16_t BG = 0x001F;
uint16_t RED =0xF800;
uint16_t blue =0xe800;
void tft_line(uint16_t y0, uint16_t y1, uint16_t x){	
	BlockWrite(x, x, y0, y1);
	for(uint16_t i=1; i<2; i++)
	{
		WriteData(RED>>8); WriteData(RED);
	}
}
//--- 填色
void tft_color(uint16_t color){
	BlockWrite(0, 320-1, 0, 240-1);
	for(int i=0; i<3; i++){
		{
			WriteData(color>>8); WriteData(color);
		}
	}
}
void line1(uint16_t x,uint16_t y,uint16_t a,uint16_t b){
	BlockWrite(0, 320-1, 0, 240-1);
	for(int i=0; i<320; i++){
		for(int j=0; j<240; j++){
			WriteData(blue>>8); WriteData(blue);
		}
	}
}
//--- 填色
void tft_box(uint16_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	BlockWrite(x, x+w-1, y, y+h-1);
	for(int i=0; i<w; i++){
		for(int j=0; j<h; j++){
			WriteData(color>>8); WriteData(color);
		}
	}

}
//--- 印字
void tft_test(void){	
	
//	tft_color(0xfFF0);
	for(int a=1;a<100;a++)
	line1(a,a,a,a);

	//---
	WriteComm(0x36); WriteData(0x00);
	//---

	//---
	
		for(int k=105;k<262;k++){
		tft_line(data2[k]/20, data2[k]/20, k*2);
			delay_ms(50);
			tft_box(k,k,k,1,1);delay_ms(100);
		}	
		
	//---
	for(int b=1;b<100;b++)
		{
		{BlockWrite(20, 20+16-1, 10, 10+16-1);
		char c = b- ' ';
		const char *p = &Font16_Table[c*16*2];
	//---
	for(int i=0;i<16;i++){
	char pt;
		pt = *p++;
		for(int j=0;j<8;j++){
			if(pt&0x80){ WriteData(FG>>8); WriteData(FG); }
			else{ WriteData(BG>>8); WriteData(BG); }
			pt <<= 1;
		}
		//---
		pt = *p++;
		for(int j=0;j<8;j++){
			if(pt&0x80){ WriteData(FG>>8); WriteData(FG); }
			else{ WriteData(BG>>8); WriteData(BG); }
			pt <<= 1;
		}
		//---
	}
}
	}	
}