#include "led.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "adc.h"
#include "remote.h"
#include<stdlib.h>



//ALIENTEK Mini STM32开发板范例代码11
//TFTLCD显示实验 
int ROWS = 20;
int COLS = 10;
int CELL_SIZE = 15;
int broad[20][10] = {0};
u16 colortable[9] = {0,0,RED,GREEN,BLUE,YELLOW,0x2345,0x8910,0x5410};



int arrSqare[2][4] = {0};
int nextarrSqare[2][4] = {0};


int nextnSqareID = -1;
int g_nextLine = -1;
int g_nextList = -1;
int nSqareID = -1;
int g_nLine = -1;
int g_nList = -1;
int Lines = 0;
int Score = 0;

int GameStart = 0;
int GamePause = 0;
int RankShowing = 0;
int scoreRankList[5] = {0};







void DrawBorderLine(){
	  POINT_COLOR=BLACK;
	  LCD_DrawRectangle(5,5,155,305);
	  LCD_DrawRectangle(165,5,225,65);
	  LCD_ShowString(165,75,100,12,12,"Score:");
	  LCD_ShowString(165,105,100,12,12,"Lines:");
	  LCD_ShowxNum(205,75,Score,4,12,0);//分数
	  LCD_ShowxNum(205,105,Lines,4,12,0);//分数
}

void StartScene(){
	 LCD_Clear(BLUE);
	 LCD_ShowString(10,65,200,24,24,"WELCOME TO TETRIS");
	 LCD_ShowString(10,125,220,16,16,"1.Press POWER to startgame");
	 LCD_ShowString(10,145,220,16,16,"2.Press ALIENTEK to ranklist");
}

void copyNextToNow(){
	int row = 0;
	int col = 0;
	for(row=0;row<2;row++){
		for(col=0;col<4;col++){
			arrSqare[row][col] = nextarrSqare[row][col];
		}
	}
	
	nSqareID = nextnSqareID;
	g_nLine = g_nextLine;
	g_nList = g_nextList;
	
}




/**
初始化画板
*/
void broad_Init(){
	int row = 0;
	int col = 0;
	for(row=0;row<ROWS;row++){
		for(col=0;col<COLS;col++){
			broad[row][col]=0;
		}
	}
}


/**
画一个方块
rownum 0 - 1
colnum 0 - 3
*/
void DrawBlock(int rownum,int colnum,u16 color){
	 int x1 = colnum * CELL_SIZE + 5;
	 int y1 = rownum * CELL_SIZE + 5;
	 LCD_Fill(x1,y1,x1 + CELL_SIZE,y1 + CELL_SIZE,color);
}
/**
画下一个方块
*/
void DrawNextBlock(int rownum,int colnum,u16 color){
	 int x1 = colnum * CELL_SIZE + 165;
	 int y1 = rownum * CELL_SIZE + 20;
	 LCD_Fill(x1,y1,x1 + CELL_SIZE,y1 + CELL_SIZE,color);
} 

 
//创建随机块
int CreateRandomSqare(){
	   int nIndex = (rand()%7)+2;
	   switch(nIndex){ //nIndex
			 case 2:
				 nextarrSqare[0][0] = 1,nextarrSqare[0][1] = 1,nextarrSqare[0][2] = 0,nextarrSqare[0][3] = 0;
			   nextarrSqare[1][0] = 0,nextarrSqare[1][1] = 1,nextarrSqare[1][2] = 1,nextarrSqare[1][3] = 0;
			   g_nextLine = 0; //行
         g_nextList = 3; //列
			 
				 break;
			 case 3:
				 nextarrSqare[0][0] = 0,nextarrSqare[0][1] = 1,nextarrSqare[0][2] = 1,nextarrSqare[0][3] = 0;
			   nextarrSqare[1][0] = 1,nextarrSqare[1][1] = 1,nextarrSqare[1][2] = 0,nextarrSqare[1][3] = 0;
			   g_nextLine = 0; //行
         g_nextList = 3; //列
			 
				 break;
			 case 4:
				 nextarrSqare[0][0] = 1,nextarrSqare[0][1] = 0,nextarrSqare[0][2] = 0,nextarrSqare[0][3] = 0;
			   nextarrSqare[1][0] = 1,nextarrSqare[1][1] = 1,nextarrSqare[1][2] = 1,nextarrSqare[1][3] = 0;
			   g_nextLine = 0; //行
         g_nextList = 3; //列
			 
			 
				 break;
			 case 5:
				 nextarrSqare[0][0] = 0,nextarrSqare[0][1] = 0,nextarrSqare[0][2] = 1,nextarrSqare[0][3] = 0;
			   nextarrSqare[1][0] = 1,nextarrSqare[1][1] = 1,nextarrSqare[1][2] = 1,nextarrSqare[1][3] = 0;

			   g_nextLine = 0; //行
         g_nextList = 3; //列
			 
				 break;
			 case 6:
				 nextarrSqare[0][0] = 0,nextarrSqare[0][1] = 1,nextarrSqare[0][2] = 0,nextarrSqare[0][3] = 0;
			   nextarrSqare[1][0] = 1,nextarrSqare[1][1] = 1,nextarrSqare[1][2] = 1,nextarrSqare[1][3] = 0;
			 	 g_nextLine = 0; //行
         g_nextList = 3; //列
			 
			 
				 break;
			 case 7:
				 nextarrSqare[0][0] = 0,nextarrSqare[0][1] = 1,nextarrSqare[0][2] = 1,nextarrSqare[0][3] = 0;
			   nextarrSqare[1][0] = 0,nextarrSqare[1][1] = 1,nextarrSqare[1][2] = 1,nextarrSqare[1][3] = 0;
			   g_nextLine = 0; //行
         g_nextList = 4; //列
			 
			 
				 break;
			 case 8:
				 nextarrSqare[0][0] = 1,nextarrSqare[0][1] = 1,nextarrSqare[0][2] = 1,nextarrSqare[0][3] = 1;
			   nextarrSqare[1][0] = 0,nextarrSqare[1][1] = 0,nextarrSqare[1][2] = 0,nextarrSqare[1][3] = 0;
			   g_nextLine = 0; //行
         g_nextList = 4; //列
			 
				 break;
		 }
		 
		 nextnSqareID = nIndex;
		 return nIndex;
}

void CopySqareToBack(){
	  int i = 0,j = 0;
	  for(i =0;i<2;i++){
			for(j = 0;j<4;j++){
				broad[i][j+3] = arrSqare[i][j];
			}
		}
	      
	
}
/**
画方块组合
*/
void draw_blocks(){
	int row = 0;
	int col = 0;
	for(row=0;row<ROWS;row++){
		for(col=0;col<COLS;col++){
			if(broad[row][col]==1){
				DrawBlock(row,col,colortable[nSqareID]);
			}
		}
	}
}

/**
画方块组合
*/
void draw_next_blocks(){
	int row = 0;
	int col = 0;
	for(row=0;row<2;row++){
		for(col=0;col<4;col++){
			if(nextarrSqare[row][col]==1){
				DrawNextBlock(row,col,colortable[nextnSqareID]);
			}
		}
	}
}



//画死方块
void draw_death_blocks(){
	int row = 0;
	int col = 0;
	for(row=0;row<ROWS;row++){
		for(col=0;col<COLS;col++){
			if(broad[row][col]>=2){
				DrawBlock(row,col,colortable[broad[row][col]]);
			}
		}
	}
}





void RePaint(){
	 LCD_Clear(WHITE);
	 DrawBorderLine();
	 draw_blocks();
	 draw_death_blocks();
	 //copyNextToNow()
	 draw_next_blocks();
}


//方块下落
void SqureDown(){
	int i=0,
	    j=0;
	for(i = 18;i>=0;i--){
		for(j = 0;j<10;j++){
			 if(1 == broad[i][j]){
				 broad[i+1][j] = broad[i][j];
				 broad[i][j] = 0;
				 
			 }
		}
	}
}

//方块左移
void SqureLeft(){
	int i=0,
	    j=0;
	for(i = 0;i < 20;i++){
		for(j = 0;j<10;j++){
			 if(1 == broad[i][j]){
				 broad[i][j-1] = broad[i][j];
				 broad[i][j] = 0;
				 
			 }
		}
	}
}
//方块左移结束
int CanSqureLeft(){
	int i= 0,j=0;
	 for(i=0;i<20;i++){
		 if(1 == broad[i][0]){
			  return 0;
		 }
	 }
	for(i = 0;i<20;i++){
		for(j = 0;j<10;j++){
			 if(1 == broad[i][j]){
				 if(broad[i][j-1] >= 2){
				    return 0;
			   }
			 }
		}
	}
	 
	 return 1;
}

//方块右移
void SqureRight(){
	int i=0,
	    j=0;
	for(i = 0;i < 20;i++){
		for(j = 9;j>=0;j--){
			 if(1 == broad[i][j]){
				 broad[i][j+1] = broad[i][j];
				 broad[i][j] = 0;
				 
			 }
		}
	}
	
}

//方块右移结束
int CanSqureRight(){
	int i= 0,j=0;
	 for(i=0;i<20;i++){
		 if(1 == broad[i][9]){
			  return 0;
		 }
	 }
	for(i = 0;i<20;i++){
		for(j = 9;j>=0;j--){
			 if(1 == broad[i][j]){
				 if(broad[i][j+1]>=2){
				    return 0;
			   }
			 }
		}
	}
	 
	 return 1;
	
}

int CanLongChange(){
	  int i=0,j=0;
	  for(i=1;i<4;i++){ //右
			  if(broad[g_nLine][g_nList+i]>=2 || g_nList+i>9){
					 break;
				}
		}
	  for(j=1;j<4;j++){ //右
			  if(broad[g_nLine][g_nList-j]>=2 || g_nList-j<0){
					 break;
				}
		}
		
		if(i+j<5){
			return 0;
		}
	  return 1;
}

void ChangeSqureLong(){
	 if(g_nLine==0 || g_nList==0 || g_nList==9 ||g_nList==8){
		 return;
		 
	 }
	 if(broad[g_nLine][g_nList-1]==1){  //横着
		 
		   //清零
		   broad[g_nLine][g_nList-1] = 0;
		   broad[g_nLine][g_nList+1] = 0;
		   broad[g_nLine][g_nList+2] = 0;
		 
		   if(broad[g_nLine+1][g_nList] >=2){
			 	   //赋值
		       broad[g_nLine-1][g_nList] = 1;
		       broad[g_nLine-2][g_nList] = 1;
		       broad[g_nLine-3][g_nList] = 1;
			 }
		   else if(broad[g_nLine+2][g_nList] >=2){
				 //赋值
		       broad[g_nLine+1][g_nList] = 1;
		       broad[g_nLine-1][g_nList] = 1;
		       broad[g_nLine-2][g_nList] = 1;
				 
			 }
		 
		 
		 
		 
		   //赋值
		   broad[g_nLine-1][g_nList] = 1;
		   broad[g_nLine+1][g_nList] = 1;
		   broad[g_nLine+2][g_nList] = 1;
	 }else{
		   if( broad[g_nLine][g_nList+1]>=2 || broad[g_nLine][g_nList+2]>=2 || broad[g_nLine][g_nList-1]>=2 ){
				 
			 }else{
				   
		   //清零
		   broad[g_nLine-1][g_nList] = 0;
		   broad[g_nLine+1][g_nList] = 0;
		   broad[g_nLine+2][g_nList] = 0;
		   //赋值
		   broad[g_nLine][g_nList-1] = 1;
		   broad[g_nLine][g_nList+1] = 1;
		   broad[g_nLine][g_nList+2] = 1;
			 }
	 }
	
}


void ChangeSqureNormal(){
	
	 int i=0,j=0;
	 int marrSqare[3][3] = {0};
	 //复制背景块
	 for(i = 0;i<3;i++){
		 for(j = 0;j<3;j++){
			 marrSqare[i][j] = broad[g_nLine+i][g_nList+j];
		 }
	 }
	//变型后复制回去
	 for(i = 0;i<3;i++){
		 for(j = 0;j<3;j++){
			  broad[g_nLine+i][g_nList+j] = marrSqare[2-j][i];
		 }
	 }
	
}
//判断方块是否可以变形
int CanSqureChangeShape(){
	   int i =0,j=0;
	   for(i=0;i<3;i++){
			 for(j=0;j<3;j++){
				 if(broad[g_nLine+i][g_nList+j]>=2){
					 return 0;
				 }		 
			 }
			 
		 }
		 
		 if(g_nList < 0 ){
			    g_nList = 0;
		 }else if(g_nList+2 > 9){
			    g_nList = 7;
		 }
		 
		 
		 
		 
	return 1;
	
}


void DestoryOneLine(){
	 int i = 0,j=0;
	 int nTempi=0;
	 int isfull = 1; //默认为full
	
	 for(i=19;i>=0;i--){
		  for(j=0;j<10;j++){
				 if(broad[i][j]<2){
					 isfull=0;
					 break;
				 }
			}
			if(isfull == 1){
				//消除一行
				for(nTempi=i-1;nTempi>=0;nTempi--){
					 for(j=0;j<10;j++){
						 broad[nTempi+1][j]=broad[nTempi][j];
					 }
				}
				Score+=10;
				Lines+=1;
				//回到底部
				i=20;
			}
		  isfull = 1;
	 } 
	 
	
}


//上键变化
void ChangeSqure(){
	  switch(nSqareID){//nSqareID
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:  //普通变形
				if(1 == CanSqureChangeShape()){
					   ChangeSqureNormal();
				}else{
					   return;
				}
			
				break;
			case 7:  //正方形
				return;
			case 8:  //长条
				if(1 == CanLongChange()){
					  ChangeSqureLong();
				}

				break;
			
			
		}
	
}








//方块到底停下
int CanSqureDown(){
	 int i= 0;
	 for(i=0;i<10;i++){
		 if(1 == broad[19][i]){
			  return 0;
		 }
	 }
	 return 1;
}


int CanSqureDown2(){
	int i=0,
	    j=0;
	for(i = 18;i>=0;i--){
		for(j = 0;j<10;j++){
			 if(1 == broad[i][j]){
				 if(broad[i+1][j] >= 2){
				    return 0;
			   }
			 }
		}
	}
	return 1;
	
}


void Change1To2(){
	int i=0,
	    j=0;
	for(i = 0;i<20;i++){
		for(j = 0;j<10;j++){
			 if(1 == broad[i][j]){
				 broad[i][j] = nSqareID;
			 }
		}
	}
}


void showTitle(){
	LCD_ShowString(30,20,50,24,24,"Rank");
	LCD_ShowString(150,20,50,24,24,"Score");
}
void shiftRank(int start){
	int i;
	for(i =4;i>start;i--){
		scoreRankList[i] = scoreRankList[i-1];
	}
}
void addRankList(int score){
	int i;
	for(i =0; i<5;i++){
		if(scoreRankList[i]<score){
			shiftRank(i);
			scoreRankList[i] =score;
			break;
		}
	}
}
void showThisScore(int rank,int score){
		int rankX = 55;
		int scoreX = 150;
		int baseY = 50;
		int offset = (rank-1)*54;
		LCD_ShowxNum(rankX,baseY+offset,rank,1,24,0);
		LCD_ShowxNum(scoreX,baseY+offset,score,5,24,0);
}
void showRankScene(){
	int i;
	
	showTitle();
	for(i = 0;i<5;i++){
		if(scoreRankList[i]!=0){
			showThisScore(i+1,scoreRankList[i]);
		}
	}
}


void startGame(){
	 broad_Init();
	 DrawBorderLine();
	 CreateRandomSqare();
	 copyNextToNow(); //给now赋值
	 CopySqareToBack();
	 CreateRandomSqare();//初始化下一个
	 draw_next_blocks();
	 draw_blocks();
	 Lines = 0;
   Score = 0;
	 delay_ms(400);
}

int CanGameOver(){
	  
	int i = 0;
	for(i = 0;i<10;i++){
		 if(broad[0][i] >= 2){
			   return 1; //游戏结束
		 }
	}
	return 0;
}


void drawYouDead(){
	  LCD_Clear(BLUE);
	  LCD_ShowString(10,65,200,24,24,"YOU DEAD");
	
}



 int main(void)
 { 
	 u8 key;
	 u16 iSeed=0;
	 int delaytime = 500;
   
	 //srand(9);
	delay_init();	    	 //延时函数初始化	
	uart_init(9600);	 	
	LED_Init();		  		//初始化与LED连接的硬件接口
 	LCD_Init();
	Adc_Init();//ADC初始化
	iSeed=Get_Adc_Average(ADC_Channel_14,3);
	srand(iSeed);//随机数种子
	 
	Remote_Init();	
	
	 
	 
	StartScene();
	 while(1){
		 key=Remote_Scan(delaytime);	
		 if(key)
		 {	 
			switch(key)
			{	   

				case 226:        //外星人键
				  if(GameStart==0){
						if(RankShowing ==0){
						 LCD_Clear(BLUE);
						 showRankScene();
						}else{
							StartScene();
						}
						RankShowing=!RankShowing;
						delay_ms(200);
					}
					break;
				
				case 162:        //电源键
					GameStart = GameStart==0?1:0;
				  if(GameStart==1){
						 LCD_Clear(WHITE);
						 startGame();
					}else{
						 StartScene();
					}
				
				break;
				case 98: 
					 if(GameStart==1){
						    ChangeSqure();
					 }
				
				break;	  //上  	  
				case 194: 
					
				if(GameStart==1){
						if(1 == CanSqureRight()){
						SqureRight(); 
						g_nList++;
					}
				}
				
				
				  break;	  //右
				case 34: 

          if(GameStart==1){
						  if(1 == CanSqureLeft()){
						SqureLeft(); 
						g_nList--;
					}
					}
				
				  break;		  //左
				case 168: 
					 if(GameStart==1){
						 	delaytime = 50; 
					 }
				
				break;		  //下
			}
		 }else{
			  delaytime = 500;
			  delay_ms(50);
		 }
		 
		 
		
		
		 
		   if(GameStart==1){
				 
				 if(1 == CanSqureDown() && 1 == CanSqureDown2()){
			   SqureDown();
				 g_nLine++;
		    }else{
			   //1 to 2
			   Change1To2();
				 DestoryOneLine();
				//游戏结束判断
				 if(CanGameOver()==1){
					  drawYouDead();
					  GameStart=0;
					  addRankList(Score);

					  delay_ms(1000);
					  StartScene();
					  continue;
				 }
				 copyNextToNow(); //给now赋值
				 CopySqareToBack();
			   CreateRandomSqare();//初始化下一个
		    }

		    RePaint();
		    delay_ms(delaytime);
		    LED1=!LED1;
				 
			 }
		  
	 }
	 
	 
	 
	 
	 
  	
	 
		//POINT_COLOR=RED;	 
   
		
		
  // POINT_COLOR=BLACK;
		//LCD_DrawLine(30,300,120,140);
		//LCD_DrawLine(120,140,210,300);
		//LCD_DrawLine(210,300,30,300);
		//POINT_COLOR=GREEN;
		//LCD_Fill_Circle(120,250,40);
	
}
