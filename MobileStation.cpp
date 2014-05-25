#include "MobileStation.h"

using namespace std;

/******************************* Constructor ***********************************************/
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

/********************************** CSMA **************************************************/
void MobileStation::backoff(int accessClass){
	int backoff_time;

	//int cw_min = 0;
	//int cw_max = 0;

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
	bool tx_done;
	collision_flag = false;	

	//Wait for open channel
	waitStandard(accessClass);

	//random wait for collisions?
	int wait = rand() % 100;
	usleep(wait);
	
	//Attempt to Secure Channel
	cout << "Station " << stationNumber << " attempting to transmit" << endl;
	bool attempt = baseStationName -> attemptTransmission(this);

	clock_t start;
    	start = clock();

	while(!attempt){
		cout << "Station " << stationNumber << " Backing off" << endl;

		backoff(accessClass);//back off
		waitStandard(accessClass);

		//cout << "Station " << stationNumber << " re-attempting" << endl;
		cout << "Station " << stationNumber << " re-attempting" << endl;
		attempt = baseStationName -> attemptTransmission(this); //try again
	}

	delay_time += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

	//pretend to transmit

	tx_done = pretendTransmitting(accessClass);

	//finish
	if(tx_done){
		cout << "Station " << stationNumber << " Tx Sucess" << endl;
	}
	else{
		collision_count++;
		cout << "Station " << stationNumber << " Tx Failed - Collision" << endl;
	}

	baseStationName->finishTransmission();	
	
	return tx_done; //we tx successfully
}

void MobileStation::debug_printStatus(){
	cout << "LOL" << endl;
}

bool MobileStation::pretendTransmitting(int accessClass){
	//int packetSize = 0;
	bool tx_finish = true;	

	switch(accessClass){
		case BEST_EFFORT:
			tx_finish = transmitting(5000);
			break;
		case BACKGROUND:
			tx_finish = transmitting(3000);
			break;
		case VIDEO:
			tx_finish = transmitting(7000);
			break;
		case VOICE:
			tx_finish = transmitting(7000);
			break;
		case DATA:
			tx_finish = transmitting(10000);
			break;
	}

	return tx_finish;
}

bool MobileStation::transmitting(int tx_time){
	int count = 0;

	while((count < tx_time) && !(collision_flag)){
		count += SLOT_TIME;
		usleep(SLOT_TIME);
	}
	
	if(count != tx_time)
	{
		return false;
	}

	return true;
}

void MobileStation::waitStandard(int accessClass){
	bool status = baseStationName -> getChannelStatus();
	clock_t start;
	start = clock();

	do {
		//cout << "Station " << stationNumber << " waiting DIFS" << endl;
		usleep(DIFS);//Wait DIFS
		waitAIFS(accessClass); //Wait AIFS

		//cout << "Station " << stationNumber << " waiting AIFS" << endl;
		//Sense Channel
		//cout << "Station " << stationNumber << " Sensing Channel" << endl;
		status = baseStationName -> getChannelStatus();
	} while(status==BUSY);

	wait_time += ( clock() - start ) / (double) CLOCKS_PER_SEC;
}

/******************************** Operations ************************************************/

void MobileStation::main_loop(){
	
	clock_t start;
	double duration;
	start = std::clock();
	
	collision_count = 0;

	ofstream log;//writing
	stringstream ss;//create a stringstream

	ss << stationNumber;//add number to the stream
	string log_name = testScenario + "_" + std::string("Station_") + ss.str() + string(".txt");

#ifdef IEEE_STANDARD
	log_name = "IEEE_Standard_" + log_name;
#endif

	//cout << "log name: " << log_name << endl;
	log.open(log_name.c_str(), fstream::in | fstream::out | fstream::trunc); //problem

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
		if(transmit(newPacket)){
			newPacket = pickPacket();
		}
		else{
			cout << "Station " << stationNumber << " re-transmitting" << endl;
		}
	}

	cout << "Station " << stationNumber << " Transmissions completed" << endl;

	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	cout << "Total time: " << duration << " seconds" << endl;
	cout << "Number of collision during transmission: " << collision_count << endl;
	
	log << "Number of collision during transmission: " << collision_count << endl;
	log << "Total time: " << duration << " seconds" << endl;
	log << "Wait time: " << wait_time << " seconds" << endl;
	log << "Delay time: " << delay_time << " seconds" << endl;
	log.close();
}

int MobileStation::pickPacket(void){
	//check if empty
	if((packets[0] + packets[1] + packets[2] + packets[3] + packets[4]) == 0){
		return -1;
	}

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
