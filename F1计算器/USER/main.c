#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "math.h"


double Num1,Num2,t_i,Result;
u8 Num1_Buf[10],Num2_Buf[10],Num1_Length,Num2_Length;
u8 fuhao,fuhao_pos1,fuhao_pos2,t_u;
double TF(u8 buf[],u8 L)
{
	u8 i,p=0;
	double result;
	for(i=0;i<L;i++)
	{
		if(buf[i]=='.')
		{
			p=i;
			break;
		}
	}
	if(p!=0)//小数点
	{
	for(i=0;i<p;i++)
		result = result*10 + (buf[i]-0x30);
	for(i=p+1;i<L;i++)
		result = result*10 + (buf[i]-0x30);
	result = result/pow(10.0,(double)(L-p-1));
	}
	else
	{
		for(i=0;i<L;i++)
		result = result*10 + (buf[i]-0x30);
	}
	return result;
}
 int main(void)
 {
	u8 i;	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
 	while(1)
	{
		if(flag==1)//说明有数进来了
			{
				/*******************************sin**********************************/
				if(buffer[0]=='s')//sin函数
				{
					for(i=4;i<30;i++)//前四个肯定不是')'
					if(buffer[i]==')')
					{
						fuhao_pos1 = i;
						fuhao = 's';
					}
					if(buffer[4]!='-')//正数
					{
						for(i=4;i<fuhao_pos1;i++)
						{
							Num1_Buf[i-4]=buffer[i];
						}
						Num1_Length=fuhao_pos1-4;
						Num1 = TF(Num1_Buf,Num1_Length);
					}
					else//负数
					{
						for(i=5;i<fuhao_pos1;i++)
						{
							Num1_Buf[i-5]=buffer[i];
						}
						Num1_Length=fuhao_pos1-5;
						Num1 = TF(Num1_Buf,Num1_Length);
						Num1 = -Num1;
					}
				}
				/*******************************cos**********************************/
				else if(buffer[0]=='c')//cos函数
				{
					for(i=4;i<30;i++)//前四个肯定不是')'
					if(buffer[i]==')')
					{
						fuhao_pos1 = i;
						fuhao = 'c';
					}
					if(buffer[4]!='-')//正数
					{
						for(i=4;i<fuhao_pos1;i++)
						{
							Num1_Buf[i-4]=buffer[i];
						}
						Num1_Length=fuhao_pos1-4;
						Num1 = TF(Num1_Buf,Num1_Length);
					}
					else//负数
					{
						for(i=5;i<fuhao_pos1;i++)
						{
							Num1_Buf[i-5]=buffer[i];
						}
						Num1_Length=fuhao_pos1-5;
						Num1 = TF(Num1_Buf,Num1_Length);
						Num1 = -Num1;
					}
				}
				/*******************************负**********************************/
				else if(buffer[0]=='-')//负数
				{
					for(i=0;i<29;i++)//这一步是把最前面的负号去掉。化为正数
					{
						buffer[i]=buffer[i+1];
					}
					for(i=0;;i++)//得出符号位置
					{
						if(buffer[i]=='+'||buffer[i]=='-'||buffer[i]=='*'||buffer[i]=='/')
						{fuhao_pos1 = i;
							fuhao = buffer[i];}//记录下是什么符号
						if(buffer[i]=='=')
						{fuhao_pos2 = i;
						break;}
					}
					for(i=0;i<fuhao_pos1;i++)
					{
						Num1_Buf[i]=buffer[i];
					}
					Num1_Length=fuhao_pos1;
					Num1 = TF(Num1_Buf,Num1_Length);
					Num1 = -Num1;//别忘了加上前面的负号
					for(i=fuhao_pos1+1;i<fuhao_pos2;i++)
					{
						Num2_Buf[i-fuhao_pos1-1]=buffer[i];
					}
					Num2_Length=fuhao_pos2-fuhao_pos1-1;
					Num2 = TF(Num2_Buf,Num2_Length);
				}
				/*******************************正**********************************/
				else//第一个是正数
				{
					for(i=0;;i++)//得出符号位置
					{
						if(buffer[i]=='+'||buffer[i]=='-'||buffer[i]=='*'||buffer[i]=='/')
						{fuhao_pos1 = i;
							fuhao = buffer[i];}//记录下是什么符号
						if(buffer[i]=='=')
						{fuhao_pos2 = i;
						break;}
					}
					for(i=0;i<fuhao_pos1;i++)
					{
						Num1_Buf[i]=buffer[i];
					}
					Num1_Length=fuhao_pos1;
					Num1 = TF(Num1_Buf,Num1_Length);
					for(i=fuhao_pos1+1;i<fuhao_pos2;i++)
					{
						Num2_Buf[i-fuhao_pos1-1]=buffer[i];
					}
					Num2_Length=fuhao_pos2-fuhao_pos1-1;
					Num2 = TF(Num2_Buf,Num2_Length);
				}
				switch(fuhao)
				{
					case '+': Result = Num1 + Num2; break;
					case '-': Result = Num1 - Num2; break;
					case '*': Result = Num1 * Num2; break;
					case '/': Result = Num1 / Num2; break;
					case 's': Result = sin(Num1);break;
					case 'c': Result = cos(Num1);break;
					default :  break; 
				}
				if(fuhao == 's'||fuhao == 'c')
				{
					switch(fuhao)
					{
						case 's': printf("The equation: sin(%lf)\r\n",Num1);
											printf("The result  : %lf\r\n",Result);break;
						case 'c': printf("The equation: cos(%lf)\r\n",Num1);
											printf("The result  : %lf\r\n",Result);break;
						default :  break; 
					}
					
				}
				else
				{
					printf("The equation: %lf %c %lf\r\n",Num1,fuhao,Num2);
					printf("The result  : %lf\r\n",Result);
				}
				//运算完毕，后期处理
				flag = 0;//下次while(1)不要再运算，直到有新数据
				for(i=0;i<30;i++)//Buffer清零，不对下次产生影响
				buffer[i]=0;
			}
	}	 
 }

