#ifndef MOBILESTATION_H
#define MOBILESTATION_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
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
#define AIFSN_BE 500000

// Background
#define CWMIN_BK 15
#define CWMAX_BK 1023
#define AIFSN_BK 700000

// Video
#define CWMIN_VI 7
#define CWMAX_VI 15
#define AIFSN_VI 300000

// Voice
#define CWMIN_VO 3
#define CWMAX_VO 7
#define AIFSN_VO 300000

// Data
#define CWMIN_DA 15
#define CWMAX_DA 20
#define AIFSN_DA 300000

// Frame time
#define DIFS 500 //Placeholder
#define SIFS 200 //Placeholder

#define CLEAR true
#define BUSY false

#define SUCCESS true
#define COLLISION false

class BaseStation;

class MobileStation{
	private:
		int stationNumber;
		float x_coordinate, y_coordinate;
		BaseStation* baseStationName;
	
	public:
		int packets[5];
		
		MobileStation();
		MobileStation(int, BaseStation *);
		
		void init();
		void waitAIFS(int);
		void backoff(int);
		bool transmit(int);
		void main_loop();
		void debug_printStatus(void);
};

#endif
