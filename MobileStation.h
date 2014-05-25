#ifndef MOBILESTATION_H
#define MOBILESTATION_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <sstream>
#include "BaseStation.h"


// Access Classes
#define BEST_EFFORT 0
#define BACKGROUND 1
#define VIDEO 2
#define VOICE 3
#define DATA 4

//Parameters
//Best Effort
#define CWMIN_BE 15
#define CWMAX_BE 1023
#define AIFSN_BE 500

// Background
#define CWMIN_BK 15
#define CWMAX_BK 1023
#define AIFSN_BK 700

// Video
#define CWMIN_VI 7
#define CWMAX_VI 15
#define AIFSN_VI 300

// Voice
#define CWMIN_VO 3
#define CWMAX_VO 7
#define AIFSN_VO 300

// Data
#define CWMIN_DA 15
#define CWMAX_DA 20
#define AIFSN_DA 300

// Frame time
#define DIFS 500 //Placeholder
#define SIFS 200 //Placeholder

//802.11 
#define SLOT_TIME	50

#define CLEAR true
#define BUSY false

#define SUCCESS true
#define COLLISION false

class BaseStation;

class MobileStation{
	private:
		int stationNumber;
		BaseStation* baseStationName;
	
	public:
		//tracking
		int packets[5];
		double delay_time;
		double wait_time;
		bool collision_flag;
		int collision_count;
		std::string testScenario; 
		
		//Flags
		bool collided;
		
		//constructors
		MobileStation();
		MobileStation(int, BaseStation *);
		
		//CSMA
		void waitAIFS(int);
		void backoff(int);
		bool transmit(int);
		void debug_printStatus(void);
		bool pretendTransmitting(int);
		void waitStandard(int);
		bool transmitting(int);		

		//operations
		void main_loop();
		int pickPacket(void);
};

#endif
