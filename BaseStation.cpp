#include "BaseStation.h"

using namespace std;

BaseStation::BaseStation(){
	status = CLEAR;
	idNumber = 0;
}

bool BaseStation::getChannelStatus(void){
	//cout << "Base Station Clear = " << status << endl;
	return status;
}

bool BaseStation::attemptTransmission(MobileStation* station){
	if(status == BUSY){
		currentTransmitter->collision_flag = true;
		return COLLISION;
	} else {
		currentTransmitter = station;
		status = BUSY;
		return SUCCESS;
	}
}

bool BaseStation::finishTransmission(){
	status = CLEAR;
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
