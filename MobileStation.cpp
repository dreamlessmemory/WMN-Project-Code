#include "MobileStation.h"

using namespace std;

MobileStation::MobileStation(){
	stationNumber = rand() % 10 + 1;
}

MobileStation::MobileStation(int number, BaseStation * station){
	stationNumber = number;
	baseStationName = station;
	collided = false;
	
	delay_time = 0.0;
	wait_time = 0.0;
}

void MobileStation::backoff(int accessClass){
	int backoff_time;
	switch(accessClass){
		case BEST_EFFORT:
			backoff_time = rand() % (CWMAX_BE - CWMIN_BE) + CWMIN_BE;
			break;
		case BACKGROUND:
			backoff_time = rand() % (CWMAX_BK - CWMIN_BK) + CWMIN_BK;
			break;
		case VIDEO:
			backoff_time = rand() % (CWMAX_VI - CWMIN_VI) + CWMIN_VI;
			break;
		case VOICE:
			backoff_time = rand() % (CWMAX_VO - CWMIN_VO) + CWMIN_VO;
			break;
		case DATA:
			backoff_time = rand() % (CWMAX_DA - CWMIN_DA) + CWMIN_DA;
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
	int wait = rand() % 100;
	usleep(wait);
	
	//Attempt to Secure Channel
	cout << "Station " << stationNumber << " attempting to transmit" << endl;
	bool attempt = baseStationName -> attemptTransmission(this);
	std::clock_t start;
    start = std::clock();
	while(!attempt){
		cout << "Station " << stationNumber << " Backing off" << endl;
		backoff(accessClass);//back off
		waitStandard(accessClass);
		cout << "Station " << stationNumber << " re-attempting" << endl;
		attempt = baseStationName -> attemptTransmission(this); //try again
	}
	delay_time += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

	//pretend to transmit
	bool finished = pretendTransmitting(accessClass);
	while(!finished){
		cout << "Station " << stationNumber << " Collided" << endl;
		attempt = baseStationName -> attemptTransmission(this);
		if(!attempt)
			continue;
		cout << "Station " << stationNumber << " re-tx" << endl;
		finished = pretendTransmitting(accessClass);
	}
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

bool MobileStation::pretendTransmitting(int accessClass){
	//int packetSize = 0;
	int sleepTime = 0;
	switch(accessClass){
		case BEST_EFFORT:
			sleepTime = 5000;
			break;
		case BACKGROUND:
			sleepTime = 3000;
			break;
		case VIDEO:
			sleepTime = 7000;
			break;
		case VOICE:
			sleepTime = 7000;
			break;
		case DATA:
			sleepTime = 10000;
			break;
	}
	while (sleepTime >= 0){
		usleep(50);
		if(collided){
			collided = false;
			return COLLISION;
		}
		sleepTime -= 50;
	}
	return SUCCESS;
}

void MobileStation::waitStandard(int accessClass){
	bool status = baseStationName -> getChannelStatus();
	std::clock_t start;
    start = std::clock();
	do {
		//cout << "Station " << stationNumber << " waiting DIFS" << endl;
		usleep(DIFS);//Wait DIFS
		waitAIFS(accessClass); //Wait AIFS
		//cout << "Station " << stationNumber << " waiting AIFS" << endl;
		//Sense Channel
		//cout << "Station " << stationNumber << " Sensing Channel" << endl;
		status = baseStationName -> getChannelStatus();
	} while(status==BUSY);
	wait_time += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
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
	log << "Wait time: " << wait_time << " seconds" << endl;
	log << "Delay time: " << delay_time << " seconds" << endl;
	log.close();
}
