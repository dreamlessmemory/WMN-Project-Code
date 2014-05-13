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

void BaseStation::addStation(MobileStation station){
	connectedStations.push_back(station);
}

void BaseStation::debug_printStatus(){
	for(std::vector<MobileStation>::iterator it = connectedStations.begin(); it != connectedStations.end(); ++it) {
		it->debug_printStatus();
	}
}
