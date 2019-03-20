/*
 * PneuFunctions.h
 *
 *  Created on: Jun 5, 2018
 *      Author: Xiaojiao Chen
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */
#ifndef USERINC_PNEUFUNCTIONS_H_
#define USERINC_PNEUFUNCTIONS_H_

#ifdef __cplusplus
 extern "C" {
#endif
#include "main.h"

/*****************************How to use PneuDrive***********************
 * 1. modify the MACROs  according to your hardware configuration
 * 2. implement the interface functions.
 * 3. useful ow-level APIs are provided.
 */

/***************************Configure control and display frequency********************/
#define CONTROL_FREQUENCY 1000	//The loop() frequency in Hz
#define DISPLAY_FREQUENCY 100	//The serialDisplay() frequency in Hz

/***************************Configure hardware extention********************/
#define ADBOARD_NUM 	0		//The quantity of AD7616 extension boards
#define DABOARD_NUM 	0		//The quantity of LTC2668 extension boards
#define PWMBOARDSPI_NUM 0		//The quantity of LT8500 extension boards
#define PWMBOARDI2C_NUM 0		//The quantity of PCA9685 extension boards
#define PWM_DIGITAL_OUTPUT_REVERSED	0 //Normally 0. Set to 1 if the power amplifier board needs reversed output.

/***************************Implement the following interface functions as needed********************/

void setup();			//called once at the beginning.
void loop();			//called at the frequency of CONTROL_FREQUENCY.
void serialDisplay();	//called at the frequency of DISPLAY_FREQUENCY.
void serialReceiveCallback(char *pSerialReceiveBuffer); //called when receive string with ending '\r\n'
void interruptCallback(int interrupt_Pin); //called when interrupt port has an uprising interrupt.

/*****************************Available low-levle APIs to use********************/
float AnalogRead(uint16_t num);
void AnalogWrite(uint16_t num,float voltage);

void DigitalRead(uint16_t num);
void DigitalWrite(uint16_t num, uint16_t state);
void DigitalToggle(uint16_t num);

void PWMWriteDuty(uint16_t num, float duty);
void PWMWriteFrequency(uint16_t num, float fre);


uint32_t millis();
uint32_t micros();
void delay_us(int32_t t);
void delay_ns(int32_t t);
/*******************************************************************/

/**************************Other interface functions to use*************/
void serialPort1Callback(char *pSerialPort1Buffer);
void serialPort4Callback(char *pSerialPort4Buffer);





#include <HardwareConfig.h>
#include "stm32f7xx_hal.h"
#include "arm_math.h"
#include "myUsartFunction.h"
#include "myPID.h"
#include "kalman.h"
#include "Controller.h"
#include "valveFlowFunc.h"

#ifdef __cplusplus
}
#endif

#endif /* USERINC_PNEUFUNCTIONS_H_ */
