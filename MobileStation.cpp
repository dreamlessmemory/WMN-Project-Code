#include "MobileStation.h"

using namespace std;

MobileStation::MobileStation(){
	stationNumber = rand() % 10 + 1;
	MobileStation::init();
}

MobileStation::MobileStation(int number, BaseStation * station){
	stationNumber = number;
	baseStationName = station;
	MobileStation::init();
}

void MobileStation::init(){
	x_coordinate = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1000));
	y_coordinate = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1000));
}

void MobileStation::backoff(int accessClass){
	int backoff_time;
	int cw_min = 0;
	int cw_max = 0;
	switch(accessClass){
		case BEST_EFFORT:
			backoff_time = rand() % (CWMAX_BE - CWMIN_BE) + CWMIN_BE;
			break;
		case BACKGROUND:
			backoff_time = rand() % (CWMAX_BK - CWMIN_BK) + CWMIN_BK;
			break;
		case VIDEO:
			cw_min = (CWMIN_VI + 1)/2 -1;
			backoff_time = rand() % (CWMAX_VI - cw_min) + cw_min;
			break;
		case VOICE:
			cw_min = (CWMIN_VO + 1)/4 -1;
			cw_min = (CWMIN_VO + 1)/2 -1; //MAX?
			backoff_time = rand() % (cw_max - cw_min) + cw_min;
			break;
		case DATA:
			cw_min = (CWMIN_DA + 1)/8 -1;
			cw_min = (CWMIN_DA + 1)/4 -1; //MAX?
			backoff_time = rand() % (cw_max - cw_min) + cw_min;
			break;
	}
	sleep(backoff_time);//use usleep?
}

void MobileStation::waitAIFS(int accessClass){
	switch(accessClass){
		case BEST_EFFORT:
			sleep(AIFSN_BE);
			break;
		case BACKGROUND:
			sleep(AIFSN_BK);
			break;
		case VIDEO:
			sleep(AIFSN_VI);;
			break;
		case VOICE:
			sleep(AIFSN_VO);
			break;
		case DATA:
			sleep(AIFSN_DA);
			break;
	}
}

bool MobileStation::transmit(int accessClass){
	//Wait DIFS
	sleep(DIFS);
	waitAIFS(accessClass);
	//Sense Channel
	bool ready = baseStationName -> getChannelStatus(); 
	while(!ready){
		usleep(500); //wait until transmission
		ready = baseStationName -> getChannelStatus();
	}	
	//set to transmitting mode
	bool attempt = baseStationName -> attemptTransmission(this);
	while(!attempt){
		backoff(accessClass);//back off
		attempt = baseStationName -> attemptTransmission(this); //try again
	}
	//we got here sucessfully
	baseStationName->finishTransmission();
	
	return true; //we tx successfully
}

void MobileStation::main_loop(){
	//pick packet if no new one
	//transmit loop
	
	std::cout << "Transmission completed" << endl;
}

void MobileStation::debug_printStatus(){
	cout << "LOL" << endl;
}

