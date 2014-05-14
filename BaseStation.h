#ifndef BASESTATION_H
#define BASESTATION_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "MobileStation.h"

#define CLEAR true
#define BUSY false

#define SUCCESS true
#define COLLISION false

class MobileStation;

class BaseStation{
	private:
		bool status;
		MobileStation* currentTransmitter;
		int idNumber;
	
	public:
		BaseStation();
		std::vector<MobileStation> connectedStations;
		bool getChannelStatus(void);
		bool attemptTransmission(MobileStation*);
		bool finishTransmission(void);
		void debug_printStatus(void);
		void addStation(MobileStation);
};
#endif
