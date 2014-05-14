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
	usleep(backoff_time * 1000);//use usleep?
}

void MobileStation::waitAIFS(int accessClass){
	switch(accessClass){
		case BEST_EFFORT:
			usleep(AIFSN_BE);
			break;
		case BACKGROUND:
			usleep(AIFSN_BK);
			break;
		case VIDEO:
			usleep(AIFSN_VI);;
			break;
		case VOICE:
			usleep(AIFSN_VO);
			break;
		case DATA:
			usleep(AIFSN_DA);
			break;
	}
}

bool MobileStation::transmit(int accessClass){
	//Wait DIFS
	cout << "Station " << stationNumber << " waiting DIFS\n";
	usleep(DIFS);
	
	//Wait AIFS
	waitAIFS(accessClass);
	cout << "Station " << stationNumber << " waiting AIFS\n";
	//Sense Channel
	cout << "Station " << stationNumber << " Sensing Channel\n";
	bool status = baseStationName -> getChannelStatus(); 
	while(status==BUSY){
		cout << "Station " << stationNumber << " waiting for clear channel\n";
		usleep(50000); //wait until transmission
		status = baseStationName -> getChannelStatus();
	}
	//random wait for collisions?
	int wait = rand() % 300000;
	usleep(wait);
	
	
	//set to transmitting mode
	cout << "Station " << stationNumber << " attempting to transmit\n";
	bool attempt = baseStationName -> attemptTransmission(this);
	while(!attempt){
		cout << "Station " << stationNumber << " Backing off\n";
		backoff(accessClass);//back off
		cout << "Station " << stationNumber << " re-attempting\n";
		attempt = baseStationName -> attemptTransmission(this); //try again
	}
	//we got here sucessfully
	//pretend to transmit
	usleep(500000);
	//finish
	cout << "Station " << stationNumber << " Tx Sucess\n";
	baseStationName->finishTransmission();
	
	return true; //we tx successfully
}

void MobileStation::main_loop(){
	//pick packet if no new one
	//transmit loop
	
	for(int i = 0; i < 3; i++){
		transmit(BEST_EFFORT);
	}
	
	std::cout << "Station " << stationNumber << " Transmissions completed" << endl;
}

void MobileStation::debug_printStatus(){
	cout << "LOL" << endl;
}

