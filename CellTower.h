#ifndef CELLTOWER
#define CELLTOWER
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include "CellPhone.h"

#define NUMPHONES 25

using namespace std;

class CellTower{
	unsigned int identifier;
	bool emergencyMode;
	map<int, CellPhone> phonesInArea;
	
	public:
		CellTower();
		CellTower(unsigned int);
		bool callPhone(int);
		float * getPhoneLocation(int);
		map<int, float *> locatePhones(vector<int>);
		bool setEmergencyMode(bool);
		void debug_locatePhones(void);
		bool getStatus();
};
#endif
