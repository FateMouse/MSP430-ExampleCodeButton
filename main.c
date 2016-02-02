#include  <msp430x14x.h>

unsigned char key,flag;
unsigned int add;

void delay(unsigned int timer)
{
   for(;timer>0;timer--);
}

void Clock_init()                         // 始终初始化函数
{
 
   BCSCTL1 = RSEL0 + RSEL1 + RSEL2;        // XT2on, max RSEL
   BCSCTL1&=~XT2OFF;                       //打开XT2振荡器
   do
   {
      IFG1 &= ~OFIFG;                      // 清除振荡器失效标志
      delay(255);                          // 延时，等待XT2起振
   }
    while ((IFG1 & OFIFG) != 0);           // 判断XT2是否起振
   
   BCSCTL2 |= SELS+SELM_2;                 //SMCLK ,MCLK 时钟源选择XT2       
  
} 
void timerA_init(void)                     //定时器初始化函数
{
   CCTL0 = CCIE;                           // CCR0 中断使能
   CCR0 = 8000;                            // 晶振8M   
   TACTL = TASSEL_2 + MC_1+TACLR;          // SMCLK, 增计数模式，清TAR
}


void main(void)
{
   WDTCTL = WDTPW + WDTHOLD;                  // Stop WDT
 
   Clock_init();
   timerA_init();
   
   P4DIR=0xff;
   P4OUT=0xff;
   
   P1SEL&=0XC7;   
   
   _BIS_SR(GIE);                           // 开总中断 
  while(1)
  {
     key=P1IN&0x38;                              // 取键值
     switch(key)
     {
       
       case 0x18:
                  delay(100);
                  if((P1IN&0x38)==0x18)
                   {
                    P4OUT=0Xdf;
                    flag=1;
                   }
                  break;                   //P1.5   key19
       case 0x28:
                  delay(100);
                  if((P1IN&0x38)==0x28)
                  {
                    P4OUT=0Xef;
                    flag=2;
                  }
                  break;                  //P1.4  key14
       case 0x30:
                  delay(100);
                  if((P1IN&0x38)==0x30)
                  {
                    P4OUT=0Xf7;
                    flag=3;
                  }
                  break;                  //P1.3  key9
         default :break;           
                 
     }
     
  }

}

#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
 add++;
 if(add>500)
 {
    add=0;                    // 0.5s
    switch(flag)
    {
      case 1:
             if(P4OUT&BIT5)
               P4OUT&=~(BIT5);   // P1.5 闪烁
             else
              P4OUT|=BIT5;
             break;
      case 2:
             if(P4OUT&BIT4)
               P4OUT&=~(BIT4);  // P1.4 闪烁
             else
              P4OUT|=BIT4;
              break;
      case 3:
             if(P4OUT&BIT3)
               P4OUT&=~(BIT3);  // P1.3 闪烁
             else
              P4OUT|=BIT3;
             break;
      default:break;
    }
 }
} 





