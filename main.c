#include  <msp430x14x.h>

unsigned char key,flag;
unsigned int add;

void delay(unsigned int timer)
{
   for(;timer>0;timer--);
}

void Clock_init()                         // ʼ�ճ�ʼ������
{
 
   BCSCTL1 = RSEL0 + RSEL1 + RSEL2;        // XT2on, max RSEL
   BCSCTL1&=~XT2OFF;                       //��XT2����
   do
   {
      IFG1 &= ~OFIFG;                      // �������ʧЧ��־
      delay(255);                          // ��ʱ���ȴ�XT2����
   }
    while ((IFG1 & OFIFG) != 0);           // �ж�XT2�Ƿ�����
   
   BCSCTL2 |= SELS+SELM_2;                 //SMCLK ,MCLK ʱ��Դѡ��XT2       
  
} 
void timerA_init(void)                     //��ʱ����ʼ������
{
   CCTL0 = CCIE;                           // CCR0 �ж�ʹ��
   CCR0 = 8000;                            // ����8M   
   TACTL = TASSEL_2 + MC_1+TACLR;          // SMCLK, ������ģʽ����TAR
}


void main(void)
{
   WDTCTL = WDTPW + WDTHOLD;                  // Stop WDT
 
   Clock_init();
   timerA_init();
   
   P4DIR=0xff;
   P4OUT=0xff;
   
   P1SEL&=0XC7;   
   
   _BIS_SR(GIE);                           // �����ж� 
  while(1)
  {
     key=P1IN&0x38;                              // ȡ��ֵ
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
               P4OUT&=~(BIT5);   // P1.5 ��˸
             else
              P4OUT|=BIT5;
             break;
      case 2:
             if(P4OUT&BIT4)
               P4OUT&=~(BIT4);  // P1.4 ��˸
             else
              P4OUT|=BIT4;
              break;
      case 3:
             if(P4OUT&BIT3)
               P4OUT&=~(BIT3);  // P1.3 ��˸
             else
              P4OUT|=BIT3;
             break;
      default:break;
    }
 }
} 





