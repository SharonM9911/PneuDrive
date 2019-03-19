/*
 * UserConfig.h
 *
 *  Created on: Jan 3, 2019
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#ifndef FRAMEWORK_FRAMEINC_USERCONFIG_H_
#define FRAMEWORK_FRAMEINC_USERCONFIG_H_
#ifdef __cplusplus
 extern "C" {
#endif



/******************Modify according to your hardware configuration******************/



#define ADBOARD_NUM 	0		//The quantity of AD7616 Boards

#define DABOARD_NUM 	0		//The quantity of LTC2668 Boards

#define PWMBOARDSPI_NUM 0		//The quantity of LT8500 Boards

#define PWMBOARDI2C_NUM 0		//The quantity of PCA9685 Boards

#define CONTROL_FREQUENCY 1000	//The loop() frequency in Hz

#define DISPLAY_FREQUENCY 100	//The serialDisplay() frequency in Hz

#define PWM_DIGITAL_OUTPUT_REVERSE	0 //According to the power amplifier board

/********************************End of modification****************************/








/*********************************************************************************************************
*******************Do not modify the following contents, unless you are aware what you are doing*********
 ********************************************************************************************************/
#define ETHERCAT_ENABLED 0		//Enable EtherCAT at SPI1 at the sacrifice of the last 4 PWMports.
#define AD7616_CHANNELNUM 	16
#define LT8500_CHANNELNUM  	48
#define LTC2668_CHANNELNUM 	16
#define PCA9685_CHANNELNUM 	16

#define ADBOARD_CHANNELNUM 		(AD7616_CHANNELNUM*ADBOARD_NUM)
#define DABOARD_CHANNELNUM 		(LTC2668_CHANNELNUM*DABOARD_NUM)
#define PWMBOARDSPI_CHANNELNUM  (LT8500_CHANNELNUM*PWMBOARDSPI_NUM)
#define PWMBOARDI2C_CHANNELNUM  (PCA9685_CHANNELNUM*PWMBOARDI2C_NUM)

#define EXTERNAL_ANA_IN_NUM 	ADBOARD_CHANNELNUM
#define EXTERNAL_ANA_OUT_NUM 	DABOARD_CHANNELNUM
#define EXTERNAL_PWM_NUM 		(PWMBOARDSPI_CHANNELNUM+PWMBOARDI2C_CHANNELNUM)

#define BUILTIN_ANA_IN_NUM 		16
#define BUILTIN_DIT_OUT_NUM 	20
#define BUILTIN_DIT_IN_NUM 		4
#if PWMBOARDSPI_NUM>0 || ETHERCAT_ENABLED !=0
#define BUILTIN_PWM_NUM 		8
#else
#define BUILTIN_PWM_NUM 		26
#endif
#define BUILTIN_EXTI_NUM 		8

#define ALL_ANA_IN_NUM 		(BUILTIN_ANA_IN_NUM+EXTERNAL_ANA_IN_NUM)
#define ALL_ANA_OUT_NUM 	EXTERNAL_ANA_OUT_NUM
#define ALL_DIG_IN_NUM 		BUILTIN_DIT_IN_NUM
#define ALL_DIG_OUT_NUM 	BUILTIN_DIT_OUT_NUM
#define ALL_PWM_NUM 		(BUILTIN_PWM_NUM+EXTERNAL_PWM_NUM)
#define ALL_EXTI_NUM 		BUILTIN_EXTI_NUM


#define  BUILTIN_PWM_FREMIN  			0.03		//108000000/2^16/2^16
#define  BUILTIN_PWM_FREMAX  			27000000
#define  EXTERNAL_PWMBOARDSPI_FREMIN  	0.00001
#define  EXTERNAL_PWMBOARDSPI_FREMAX  	5000
#define  EXTERNAL_PWMBOARDI2C_FREMIN  	40
#define  EXTERNAL_PWMBOARDI2C_FREMAX  	1000

#define CONTROLLDT 			(1.0/CONTROL_FREQUENCY)
#define SENDDT 				(1.0/DISPLAY_FREQUENCY)

#define HIGH 1
#define LOW 0

#define MAP(x,fromLow,fromHigh,toLow,toHigh)  ((toLow)+((x)-(fromLow))*((toHigh)-(toLow))/((float)((fromHigh)-(fromLow))))
#define CONSTRAIN(x, min, max)	 (((x) < (min)) ? (min) :  (((x) > (max)) ? (max) : (x)))
#define MAPCONSTRAIN(x,fromLow,fromHigh,toLow,toHigh) MAP(CONSTRAIN(x, fromLow, fromHigh),fromLow,fromHigh,toLow,toHigh)



#include "main.h"
#include "stdlib.h"
#include "stdio.h"

#ifdef __cplusplus
}
#endif
#endif /* FRAMEWORK_FRAMEINC_USERCONFIG_H_ */
