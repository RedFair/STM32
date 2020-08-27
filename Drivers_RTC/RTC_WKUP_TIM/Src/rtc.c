/**
  ******************************************************************************
  * File Name          : RTC.c
  * Description        : This file provides code for the configuration
  *                      of the RTC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* RTC init function */
void MX_RTC_Init(void)
{
  LL_RTC_InitTypeDef RTC_InitStruct = {0};

  /* Peripheral clock enable */
  LL_RCC_EnableRTC();

  /** Initialize RTC and set the Time and Date 
  */
  RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
  RTC_InitStruct.AsynchPrescaler = 127;
  RTC_InitStruct.SynchPrescaler = 255;
  LL_RTC_Init(RTC, &RTC_InitStruct);

}

/* USER CODE BEGIN 1 */
void rtc_INT_init(uint16_t ms)
{
	uint16_t WaitTime=1000;
	LL_RCC_EnableRTC();
	
	LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  /**/
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_20;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
  LL_EXTI_Init(&EXTI_InitStruct);

	/* (1) �� RTC �Ĵ���ִ��д���� */
	/* (2) ��ֹ���Ѷ�ʱ���Զ�������޸� */
	/* (3) �ȴ������޸Ļ�������ֵ */
	/* (4) �޸Ļ���ֵ���ؼ������Ա�ÿ 1 Hz ����һ�� */
	/* (5) ʹ�ܻ��Ѽ������ͻ����ж� */
	/* (6) ��ֹд���� */
	
	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_WAKEUP_Disable(RTC);					//ʧ�ܻ��Ѷ�ʱ��
	
	while(LL_RTC_IsActiveFlag_WUTW(RTC)) /* (3) */
	{
			WaitTime--;
			if(WaitTime==0)
			{
				printf("RTC Init ERROR\r\n");
				break;
			}	
	}
	LL_RTC_WAKEUP_SetClock(RTC, LL_RTC_WAKEUPCLOCK_DIV_4);//4��Ƶ,Ҳ��ʱ��Ƶ��Ϊ9.25K,��һ����Ϊ0.1ms													
	LL_RTC_WAKEUP_SetAutoReload(RTC, 10*ms);//�����õ����ֵΪ32767
	
	NVIC_SetPriority(RTC_IRQn, 0);
  NVIC_EnableIRQ(RTC_IRQn);
	
	LL_RTC_EnableIT_WUT(RTC);//ʹ�ܻ��Ѷ�ʱ���ж�
	LL_RTC_WAKEUP_Enable(RTC);//ʹ�ܻ��Ѷ�ʱ��
	LL_RTC_EnableWriteProtection(RTC);
}




void RTC_IRQHandler(void)
{
	if(LL_RTC_IsActiveFlag_WUT(RTC))
	{
		printf("IRQn WKUP Timer Enter...\r\n");
		LL_RTC_ClearFlag_WUT(RTC);
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_20);
	}
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
