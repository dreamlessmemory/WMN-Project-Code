#include "MobileStationQueue.h"
#include "MobileStation.h"
#include "BaseStation.h"

using namespace std;

void startMobileStation(MobileStation station){
	station.main_loop();
}

int main( int argc, const char* argv[] ){
	
	if(!argc == 1){
		cout << "Specifify input file\n";
		return 0;
	}
	
	//BS setup
	BaseStation * base = new BaseStation();
	
	//MS setup
	std::ifstream infile(argv[0]);
	int stationNumber, be, bk, vi, vo, da;
	while(infile >> stationNumber >> be >> bk >> vi >> vo >> da){
		MobileStation * temp = new MobileStation(stationNumber, base);
		temp->packets[0] = be;
		temp->packets[1] = bk;
		temp->packets[2] = vi;
		temp->packets[3] = vo;
		temp->packets[4] = da;
		base->addStation(*temp);
	}
	return 0;
}
