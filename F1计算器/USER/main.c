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
	if(p!=0)//С����
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
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
 	while(1)
	{
		if(flag==1)//˵������������
			{
				/*******************************sin**********************************/
				if(buffer[0]=='s')//sin����
				{
					for(i=4;i<30;i++)//ǰ�ĸ��϶�����')'
					if(buffer[i]==')')
					{
						fuhao_pos1 = i;
						fuhao = 's';
					}
					if(buffer[4]!='-')//����
					{
						for(i=4;i<fuhao_pos1;i++)
						{
							Num1_Buf[i-4]=buffer[i];
						}
						Num1_Length=fuhao_pos1-4;
						Num1 = TF(Num1_Buf,Num1_Length);
					}
					else//����
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
				else if(buffer[0]=='c')//cos����
				{
					for(i=4;i<30;i++)//ǰ�ĸ��϶�����')'
					if(buffer[i]==')')
					{
						fuhao_pos1 = i;
						fuhao = 'c';
					}
					if(buffer[4]!='-')//����
					{
						for(i=4;i<fuhao_pos1;i++)
						{
							Num1_Buf[i-4]=buffer[i];
						}
						Num1_Length=fuhao_pos1-4;
						Num1 = TF(Num1_Buf,Num1_Length);
					}
					else//����
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
				/*******************************��**********************************/
				else if(buffer[0]=='-')//����
				{
					for(i=0;i<29;i++)//��һ���ǰ���ǰ��ĸ���ȥ������Ϊ����
					{
						buffer[i]=buffer[i+1];
					}
					for(i=0;;i++)//�ó�����λ��
					{
						if(buffer[i]=='+'||buffer[i]=='-'||buffer[i]=='*'||buffer[i]=='/')
						{fuhao_pos1 = i;
							fuhao = buffer[i];}//��¼����ʲô����
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
					Num1 = -Num1;//�����˼���ǰ��ĸ���
					for(i=fuhao_pos1+1;i<fuhao_pos2;i++)
					{
						Num2_Buf[i-fuhao_pos1-1]=buffer[i];
					}
					Num2_Length=fuhao_pos2-fuhao_pos1-1;
					Num2 = TF(Num2_Buf,Num2_Length);
				}
				/*******************************��**********************************/
				else//��һ��������
				{
					for(i=0;;i++)//�ó�����λ��
					{
						if(buffer[i]=='+'||buffer[i]=='-'||buffer[i]=='*'||buffer[i]=='/')
						{fuhao_pos1 = i;
							fuhao = buffer[i];}//��¼����ʲô����
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
				//������ϣ����ڴ���
				flag = 0;//�´�while(1)��Ҫ�����㣬ֱ����������
				for(i=0;i<30;i++)//Buffer���㣬�����´β���Ӱ��
				buffer[i]=0;
			}
	}	 
 }

