#include "led.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"

//ALIENTEK Mini STM32开发板范例代码11
//TFTLCD显示实验 
int ROWS = 20;
int COLS = 10;
int CELL_SIZE = 15;
 
void LCD_Fill_Circle(u16 x0,u16 y0,u8 r){
	   u8 i=1;
	   LCD_DrawPoint(x0,y0);
	  while(i<=r){
			LCD_Draw_Circle(x0,y0,i);
			i++;
		}
	
}

void DrawABlock(){
	  LCD_Fill(5,5,155,305,RED);
}
/**
rownum 0 - 19
colnum 0 - 9
*/
void DrawBlock(int rownum,int colnum,u16 color){
	 int x1 = colnum * CELL_SIZE + 5;
	 int y1 = rownum * CELL_SIZE + 5;
	 LCD_Fill(x1,y1,x1 + CELL_SIZE,y1 + CELL_SIZE,color);
}


 

 int main(void)
 { 
	 
	 u8 y=0;
	 int start = 0;

	 
	delay_init();	    	 //延时函数初始化	
	uart_init(9600);	 	
	LED_Init();		  		//初始化与LED连接的硬件接口
 	LCD_Init();
	POINT_COLOR=RED; 
	
	//LED0 = 1;
	 
	 while(start < ROWS)
	 {
		 LCD_Clear(WHITE);
		 DrawBlock(start,0,RED);
		 delay_ms(1000);
		 start++;
	 }
	 
	 
	 
  	
	 
		//POINT_COLOR=RED;	 
   
		//三角形
		
   // POINT_COLOR=BLACK;
		//LCD_DrawLine(30,300,120,140);
		//LCD_DrawLine(120,140,210,300);
		//LCD_DrawLine(210,300,30,300);
		//POINT_COLOR=GREEN;
		//LCD_Fill_Circle(120,250,40);
	return 0;
}