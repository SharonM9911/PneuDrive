/*
 * CHAMBER.cpp
 *
 *  Created on: Jun 15, 2018
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#include <CHAMBER.h>
#include <PneuDriveLL.h>

static float myInterpolate(int num,float *x, float *y,float x0);
const float CHAMBER::effectiveRadius=0.027;
const float CHAMBER::effectiveArea=M_PI*effectiveRadius*effectiveRadius;
const float CHAMBER::pressureMin= 0;
const float CHAMBER::pressureMax=201325;

static float valveOpeningLimArray[30][4]={
		{-0.275,-0.26,0.18,0.23},//0
		{-0.235,-0.18,0.1,0.12},//1
		{-0.45,-0.29,0.1,0.12},//2
		{-0.425,-0.292,0.118,0.132},//3
		{-0.379,-0.262,0.1,0.12},//4

		{-0.245,-0.135,0.104,0.12},//5
		{-0.163,-0.135,0.1,0.12},//6
		{-0.45,-0.26,0.099,0.12},//7
		{-0.245,-0.135,0.099,0.12},//8
		{-0.245,-0.135,0.092,0.115},//9

		{-0.245,-0.135,0.099,0.12},//10
		{-0.245,-0.135,0.099,0.12},//11
		{-0.235,-0.13,0.099,0.12},//12
		{-0.245,-0.135,0.099,0.12},//13
		{-0.165,-0.111,0.099,0.12},//14

		{-0.245,-0.135,0.099,0.12},//15
		{-0.185,-0.125,0.099,0.12},//16
		{-0.221,-0.135,0.099,0.12},//17
		{-0.245,-0.135,0.099,0.12},//18
		{-0.245,-0.135,0.115,0.145},//19

		{-0.245,-0.135,0.098,0.12},//20
		{-0.245,-0.165,0.112,0.126},//21
		{-0.245,-0.135,0.106,0.125},//22
		{-0.245,-0.135,0.099,0.12},//23
		{-0.245,-0.135,0.099,0.12},//24

		{-0.245,-0.135,0.1,0.12},//25
		{-0.27,-0.16,0.1,0.12},//26
		{-0.245,-0.135,0.105,0.12},//27
		{-0.245,-0.135,0.09,0.11},//28
		{-0.245,-0.135,0.099,0.12}//29
};







CHAMBER::CHAMBER(int PWMPort1,int PWMPort2,int PressurePort):
valves{PWMPort1,PWMPort2},
pressureSensor(PressurePort),
pressureTable{0,27000,48600,65600,79000,90000,100000,109000,119000,130400,145000,163000,180000},
positionTable{0,10,20,30,40,50,60,70,80,90,100,110,120}
{

	length=0.3;
	lengthCommand = length;

	pressure=0;
	pressureFil=0;
	pressureCommand=pressure;
	pressureDeadZone = 2000;
	pressureMaxP=20000;
	pressureMinN=-20000;

	fulOpening=1;
	opening = 0;


	inflatingFlag=1;
	openingMinN = valveOpeningLimArray[PWMPort1/2][0];
	openingMaxN = valveOpeningLimArray[PWMPort1/2][1];
	openingMinP = valveOpeningLimArray[PWMPort1/2][2];
	openingMaxP = valveOpeningLimArray[PWMPort1/2][3];
	inflateVelocity=1;

	pressureController = NewPressureController(200000,0,CONTROLLDT,1e10,40000,2e-5,0,0,6e-5,0.3);
	curOpeningNum=0;
	endOpeningNum=0;
	inOpeningSequence = 0;
	memset(openingSequence,0,sizeof(openingSequence));

}


void CHAMBER::attach(int PWMPort1,int PWMPort2,int AnalogPort)
{
	valves[0].attach(PWMPort1);
	valves[1].attach(PWMPort2);
	pressureSensor.attach(AnalogPort);
}


void CHAMBER::attachPWM(int PWMPort1,int PWMPort2)
{
	valves[0].attach(PWMPort1);
	valves[1].attach(PWMPort2);
}
void CHAMBER::attachSensor(int AnalogPort)
{
	pressureSensor.attach(AnalogPort);
}

float CHAMBER::readPressure(){
	pressure = pressureSensor.read();
	pressureFil = stepKF(pressureController->pKalmanFilter,pressure);
	return pressure;
}


void CHAMBER::writePressure(float pNom)
{

	pressureCommand = CONSTRAIN(pNom,-100000,180000);
	readPressure();
	float pErr = pressureCommand-pressureFil;
	if(pErr>pressureDeadZone)
	{
		opening=MAPCONSTRAIN(pErr,pressureDeadZone,pressureMaxP*inflateVelocity,openingMinP,openingMaxP);
	}
	else if(pErr<-pressureDeadZone)
	{

		//opening=pressureController->controlPressure(pressureController,pressureFil,pressureCommand);

		opening=MAPCONSTRAIN(pErr,pressureMinN*inflateVelocity,-pressureDeadZone,openingMinN,openingMaxN);
	}
	else
	{
		opening=0;
	}

	writeOpening(opening);
}

void CHAMBER::writePressure(float pNom,float pNomDot)
{

	pressureCommand = CONSTRAIN(pNom,-100000,180000);
	readPressure();
	float pErr = pressureCommand-pressureFil;
	float pErrDot=pNomDot;
	if(pErrDot>0)
	{
		inflatingFlag=1;
		if(pErr>pressureDeadZone)
		{
			opening=MAPCONSTRAIN(pErr,pressureDeadZone,pressureMaxP*inflateVelocity,openingMinP,openingMaxP);
		}
		else if(pErr<-pressureDeadZone*10)
		{
			opening=MAPCONSTRAIN(pErr,-pressureMinN*inflateVelocity,-pressureDeadZone,openingMinN,openingMaxN);
		}
		else{
			//opening=pressureController->controlPressure(pressureController,pressureFil,pressureCommand);
			opening=MAPCONSTRAIN(pErr,pressureMinN*inflateVelocity,pressureDeadZone,0,openingMinP);
		}
	}
	else
	{
		inflatingFlag=0;
		if(pErr<-pressureDeadZone)
		{
			opening=MAPCONSTRAIN(pErr,-pressureMaxP*inflateVelocity,-pressureDeadZone,openingMinN,openingMaxN);
		}
		else if(pErr>pressureDeadZone*100)
		{
			//opening=mapconstrain(pErr,pressureDeadZone,pressureMaxP*inflateVelocity,openingMinP,openingMaxP);
		}
		else
		{
			//opening=pressureController->controlPressure(pressureController,pressureFil,pressureCommand);
			opening=MAPCONSTRAIN(pErr,-pressureDeadZone,0,openingMaxN,0);
		}
	}



	writeOpening(opening);
}

void CHAMBER::writeOpeningSequence()
{

	opening=openingSequence[curOpeningNum++];
	if(curOpeningNum>=endOpeningNum)
	{
		inOpeningSequence = 0;
		curOpeningNum = 0;
		opening=0;
	}
	writeOpening(opening);
}




float CHAMBER::readPosition()
{
	return length;
}
void CHAMBER::writePosition(float pos){
}

float CHAMBER::readOpening()
{
	return opening;
}
void CHAMBER::writeOpening(float op){

	if(op>0){
		opening=CONSTRAIN(op,0,1);
		valves[0].writeDuty(opening);
		valves[1].writeDuty(0);
	}
	else
	{
		opening=CONSTRAIN(op,-1,0);
		valves[0].writeDuty(0);
		valves[1].writeDuty(-opening);
	}

}
void CHAMBER::setSensorRange_GaugePa(float vmin,float vmax,float pmin,float pmax)
{
	pressureSensor.setSensorRange_GaugePa(vmin,vmax,pmin,pmax);
}


float CHAMBER::pressure2position(float pre){
	return myInterpolate(13,pressureTable,positionTable,pre);
}

float CHAMBER::position2pressure(float pos){
	return myInterpolate(13, positionTable,pressureTable,pos);
}

static float myInterpolate(int num,float *x, float *y,float x0){
	if(x0<x[0])
		return MAP(x0,x[0],x[1],y[0],y[1]);
	else if(x0>=x[num-1])
		return MAP(x0,x[num-2],x[num-1],y[num-2],y[num-1]);
	else{
		for(int i=1;i<num;i++)
			if (x0<x[i])
				return MAP(x0,x[i-1],x[i],y[i-1],y[i]);
	}
	return 0;//never reach
}
