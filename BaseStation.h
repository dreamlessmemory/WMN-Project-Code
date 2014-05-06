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

using namespace std;

//class MobileStation;

class BaseStation{
	private:
		bool busy;
		MobileStation * currentTranmitter;
		int idNumber;
	
	public:
		BaseStation();
		bool getChannelStatus(void);
		bool attemptTransmission(MobileStation * station);
		bool finishTransmission(void);
};
#endif
