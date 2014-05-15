#include "MobileStation.h"

using namespace std;

MobileStation::MobileStation(){
	stationNumber = rand() % 10 + 1;
}

MobileStation::MobileStation(int number, BaseStation * station){
	stationNumber = number;
	baseStationName = station;
	
	//log setup
	/*std::stringstream ss;//create a stringstream
	ss << stationNumber;//add number to the stream
	string log_name = std::string("Station_") + ss.str() + std::string(".txt");
	cout << "log name: " << log_name << endl;
	log->open(log_name.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc); //problem
	cout << "pass\n";*/
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
	
	//Wait for open channel
	waitStandard(accessClass);
	//random wait for collisions?
	int wait = rand() % 30000;
	usleep(wait);
	
	
	//set to transmitting mode
	cout << "Station " << stationNumber << " attempting to transmit" << endl;
	bool attempt = baseStationName -> attemptTransmission(this);
	while(!attempt){
		cout << "Station " << stationNumber << " Backing off" << endl;
		backoff(accessClass);//back off
		waitStandard(accessClass);
		//cout << "Station " << stationNumber << " re-attempting" << endl;
		attempt = baseStationName -> attemptTransmission(this); //try again
	}
	//we got here sucessfully
	//pretend to transmit
	pretendTransmitting(accessClass);
	//finish
	cout << "Station " << stationNumber << " Tx Sucess" << endl;
	baseStationName->finishTransmission();
	
	return true; //we tx successfully
}

void MobileStation::debug_printStatus(){
	cout << "LOL" << endl;
}

int MobileStation::pickPacket(void){
	//check if empty
	if((packets[0] + packets[1] + packets[2] + packets[3] + packets[4]) == 0)
		return -1;
	int picked = -1;
	while (picked == -1){
		int check = rand() % 5;
		if(packets[check] > 0){
			picked = check;
			packets[check]--;
		}
	}
	return picked;
}

void MobileStation::pretendTransmitting(int accessClass){
	//int packetSize = 0;
	switch(accessClass){
		case BEST_EFFORT:
			usleep(5000);
			break;
		case BACKGROUND:
			usleep(3000);
			break;
		case VIDEO:
			usleep(7000);
			break;
		case VOICE:
			usleep(7000);
			break;
		case DATA:
			usleep(10000);
			break;
	}
}

void MobileStation::waitStandard(int accessClass){
	bool status = baseStationName -> getChannelStatus();
	do {
		//cout << "Station " << stationNumber << " waiting DIFS" << endl;
		usleep(DIFS);//Wait DIFS
		waitAIFS(accessClass); //Wait AIFS
		//cout << "Station " << stationNumber << " waiting AIFS" << endl;
		//Sense Channel
		//cout << "Station " << stationNumber << " Sensing Channel" << endl;
		status = baseStationName -> getChannelStatus();
	} while(status==BUSY);
}

void MobileStation::main_loop(){
	
	std::clock_t start;
    double duration;
    start = std::clock();
    
    std::ofstream log;//writing
    std::stringstream ss;//create a stringstream
	ss << stationNumber;//add number to the stream
	string log_name = std::string("Station_") + ss.str() + std::string(".txt");
	//cout << "log name: " << log_name << endl;
	log.open(log_name.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc); //problem
    
    //Keep Track of what gets tx
    log << log_name << endl;
    log << "Transmitting:" << endl;
    log << "Best Effort\t-\t" << packets[0] << endl;
    log << "Background\t-\t" << packets[1] << endl;
    log << "Video\t\t-\t" << packets[2] << endl;
    log << "Voice\t\t-\t" << packets[3] << endl;
    log << "Data\t\t-\t" << packets[4] << endl;
    
    //Transmit loop
	int newPacket = pickPacket();
	while(newPacket != -1){
		transmit(newPacket);
		newPacket = pickPacket();
	}
	
	std::cout << "Station " << stationNumber << " Transmissions completed" << endl;
	
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	std::cout << "Total time: " << duration << " seconds" << endl;
	log << "Total time: " << duration << " seconds" << endl;
	log.close();
}
