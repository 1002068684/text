#include "stm32f4xx.h"


void Key_init();
void LED_init();
void Buzzer_init();
void  Exti_K3Init(void);
void EXTI15_10_IRQHandler(void);

int main(void)
{
	LED_init();
	Key_init();
	Buzzer_init();
	GPIO_ResetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12); //����
	Exti_K3Init();
	while(1)//Ϊ�˳��򲻽���
	{
		
		if( GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 1 )
		{
		GPIO_ResetBits(GPIOB,GPIO_Pin_5); 
			GPIO_ResetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
			//GPIO_ResetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12); 
		
	}
		//if( GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) != 1 )
		else
	{
		
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
			//GPIO_ResetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12); 
	}
}
	
	return 0;
}

	//GPIO_ResetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12); //����
	//GPIO_SetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12); //���
	//GPIO_WriteBit(GPIOH,GPIO_Pin_10 , 1); //���õ�����


/**********
���ݳ�ʼ��
*/
void LED_init(){
	GPIO_InitTypeDef  GPIO_InitStruct;
	//PH10 11 12
	//1.ʹ��GPIOʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	//2.����GPIO����
		//����Ҫ�޸ĵ�����λ��
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
		//�����������ģʽ
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
		//�����������Ƶ��,����д50MHZ
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
		// ������� GPIO_OType_PP  
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
		//������������ָGPIO����ߵ�λ(����)���ǵ͵�λ(����)��
		//������������ߵ�ƽ��Ȼ���һ���������裨�𱣻����ã���
		//֪�������ͱ�ʾ�ö˿���Ĭ�����������Ϊ�ߵ�ƽ��
		//�������෴�ˣ�ָ����͵�ƽ��Ȼ���һ���������衣
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_DOWN;
		//������IO���ŵĳ�ʼ������
	GPIO_Init(GPIOH, &GPIO_InitStruct);
	
	//GPIO_ResetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12); //����
	//GPIO_SetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12); //���
	//GPIO_WriteBit(GPIOH,GPIO_Pin_10 , 1); //���õ�����
}


/**********
��ť��ʼ��
//GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)���ڻ�ð�ť������ֵ
*/
void Key_init(){  //������ʼ��
	GPIO_InitTypeDef GPIO_InitStruct;
	//PA0
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN; //����ģʽ
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**********
��������ʼ��
*/
void Buzzer_init(){
	GPIO_InitTypeDef  GPIO_InitStruct;
	//PH10 11 12
	//1.ʹ��GPIOʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//2.����GPIO����
		//����Ҫ�޸ĵ�����λ��
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
		//�����������ģʽ
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
		//�����������Ƶ��,����д50MHZ
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
		// ������� GPIO_OType_PP  
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_DOWN;

	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//Ĭ�ϲ���
	//GPIO_ResetBits(GPIOB,GPIO_Pin_5); //���÷�����  ��
	GPIO_SetBits(GPIOB,GPIO_Pin_5); //���÷�����  ����

}

/*
*��    �ܣ�K3���ⲿ�жϳ�ʼ��
*��    ������
*�� �� ֵ����
*��    �ţ�PC13
*/

void  Exti_K3Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	EXTI_InitTypeDef  EXTI_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	//1.ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//2.GPIO��������
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN; //����ģʽ
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	//3.ѡ���ж�Դ
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
	
	//4.�ⲿ�жϲ�������
	EXTI_InitStruct.EXTI_Line=EXTI_Line13;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	//5.�жϿ�������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
	
}

//�жϷ������
//Ӳ������ �޷���ֵ  �޴���
void EXTI15_10_IRQHandler(void)
{
	//ȷ���ж��ǲ�����Ĳ���
	if( EXTI_GetITStatus( EXTI_Line13 )==SET )
	{
		//�ж�����
		GPIO_SetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
		//GPIO_ResetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12); 
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
	
	}
	EXTI_ClearITPendingBit(EXTI_Line13 );
}
