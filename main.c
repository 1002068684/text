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
	GPIO_ResetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12); //亮灯
	Exti_K3Init();
	while(1)//为了程序不结束
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

	//GPIO_ResetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12); //亮灯
	//GPIO_SetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12); //灭灯
	//GPIO_WriteBit(GPIOH,GPIO_Pin_10 , 1); //设置单个灯


/**********
灯泡初始化
*/
void LED_init(){
	GPIO_InitTypeDef  GPIO_InitStruct;
	//PH10 11 12
	//1.使能GPIO时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	//2.配置GPIO引脚
		//设置要修改的引脚位置
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
		//设置引脚输出模式
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
		//设置引脚输出频率,就先写50MHZ
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
		// 推挽输出 GPIO_OType_PP  
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
		//上拉和下拉是指GPIO输出高电位(上拉)还是低电位(下拉)。
		//上拉就是输入高电平，然后接一个上拉电阻（起保护作用），
		//知道上拉就表示该端口在默认情况下输入为高电平。
		//下拉就相反了，指输入低电平，然后接一个下拉电阻。
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_DOWN;
		//函数是IO引脚的初始化函数
	GPIO_Init(GPIOH, &GPIO_InitStruct);
	
	//GPIO_ResetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12); //亮灯
	//GPIO_SetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12); //灭灯
	//GPIO_WriteBit(GPIOH,GPIO_Pin_10 , 1); //设置单个灯
}


/**********
按钮初始化
//GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)用于获得按钮的输入值
*/
void Key_init(){  //按键初始化
	GPIO_InitTypeDef GPIO_InitStruct;
	//PA0
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN; //输入模式
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**********
蜂鸣器初始化
*/
void Buzzer_init(){
	GPIO_InitTypeDef  GPIO_InitStruct;
	//PH10 11 12
	//1.使能GPIO时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//2.配置GPIO引脚
		//设置要修改的引脚位置
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
		//设置引脚输出模式
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
		//设置引脚输出频率,就先写50MHZ
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
		// 推挽输出 GPIO_OType_PP  
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_DOWN;

	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//默认不响
	//GPIO_ResetBits(GPIOB,GPIO_Pin_5); //设置蜂鸣器  响
	GPIO_SetBits(GPIOB,GPIO_Pin_5); //设置蜂鸣器  不响

}

/*
*功    能：K3的外部中断初始化
*参    数：无
*返 回 值：无
*引    脚：PC13
*/

void  Exti_K3Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	EXTI_InitTypeDef  EXTI_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	//1.时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//2.GPIO引脚配置
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN; //输入模式
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	//3.选择中断源
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
	
	//4.外部中断参数配置
	EXTI_InitStruct.EXTI_Line=EXTI_Line13;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	//5.中断控制器初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
	
}

//中断服务程序
//硬件调用 无返回值  无传参
void EXTI15_10_IRQHandler(void)
{
	//确认中断是不是真的产生
	if( EXTI_GetITStatus( EXTI_Line13 )==SET )
	{
		//中断任务
		GPIO_SetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
		//GPIO_ResetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12); 
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
	
	}
	EXTI_ClearITPendingBit(EXTI_Line13 );
}
