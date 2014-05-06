#include "BaseStation.h"
#include "MobileStation.h"

using namespace std;

bool getChannelStatus();
bool attemptTransmission(MobileStation *);
bool finishTransmission();

BaseStation::BaseStation(){
	idNumber = 0;
}

bool BaseStation::getChannelStatus(void){
	return busy;
}

bool BaseStation::attemptTransmission(MobileStation * station){
	if(busy == BUSY){
		return COLLISION;
	} else {
		currentTransmitter = station;
		busy = BUSY;
		return SUCESS;
	}
}

bool BaseStation::finishTransmission(){
	busy = CLEAR;
	currentTransmitter = NULL;
	return CLEAR;
}
		
