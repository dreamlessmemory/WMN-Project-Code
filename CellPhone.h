#ifndef CELLPHONE
#define CELLPHONE

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

//#define NUMPHONES 25

using namespace std;

class CellPhone{
	private:
	int contactNumber;
	float x_coordinate, y_coordinate;
	bool isMobile, isActive;
	
	public:
		CellPhone();
		CellPhone(int, bool, bool);
		CellPhone(int, bool, bool, int);
		void default_initialize();
		void initialize();
		void initialize_biased(int);
		float * getLocation();
		void debug_printStatus();
		void move_position();
		bool getStatus();
};

#endif
