#include "BaseStation.h"

using namespace std;

BaseStation::BaseStation(){
	idNumber = 0;
}

bool BaseStation::getChannelStatus(void){
	return busy;
}

bool BaseStation::attemptTransmission(MobileStation* station){
	if(busy == BUSY){
		return COLLISION;
	} else {
		currentTransmitter = station;
		busy = BUSY;
		return SUCCESS;
	}
}

bool BaseStation::finishTransmission(){
	busy = CLEAR;
	currentTransmitter = NULL;
	return CLEAR;
}
		
int main(){
	return 0;
}
