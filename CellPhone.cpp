#include <stdlib.h>
#include <stdio.h>
#include "CellPhone.h"

CellPhone::CellPhone(){
	contactNumber = rand() % 1000000000 + 1;
	isMobile = true;
	isActive = true;
	initialize();
}

CellPhone::CellPhone(int number, bool mobile, bool active){
	contactNumber = number;
	isMobile = mobile;
	isActive = active;
	initialize();
}

CellPhone::CellPhone(int number, bool mobile, bool active, int bias){
	contactNumber = number;
	isMobile = mobile;
	isActive = active;
	initialize_biased(bias);
}

void CellPhone::default_initialize(){
	x_coordinate = 10.0; //placeholder
	y_coordinate = 10.0; //placeholder
}

void CellPhone::initialize(){
	x_coordinate = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1000));
	y_coordinate = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1000));
}

void CellPhone::initialize_biased(int bias){
	x_coordinate = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1000)) + bias;
	y_coordinate = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1000)) + bias;
}

float * CellPhone::getLocation(){
	float * pointer = new float[2];
	pointer[0] = this->x_coordinate;
	pointer[1] = this->y_coordinate;
	//cout << contactNumber << " is locating itself...\n";
	usleep(rand() % 5000);
	return pointer;
}

bool CellPhone::getStatus(){
	return isActive;
}

void CellPhone::debug_printStatus(){
	cout << "Number: " << contactNumber << "\t Location: " << this->x_coordinate << "\t" << this->y_coordinate  << "\tMobile: "<< isMobile << "\tActive: " << isActive << endl;
}

void CellPhone::move_position(){
	if(isMobile){
		this->x_coordinate += (rand() % 500 - 1000);
		this->y_coordinate += (rand() % 500 - 1000);
		//cout << "DEBUG: Phone Moved\n";
	}
}
