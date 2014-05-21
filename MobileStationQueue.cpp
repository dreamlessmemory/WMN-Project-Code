#include "MobileStationQueue.h"
#include "MobileStation.h"
#include "BaseStation.h"
#include <boost/thread.hpp>

using namespace std;

void startMobileStation(MobileStation);
void testThread(void);

void startMobileStation(MobileStation station){
	station.main_loop();
}

int main( int argc, const char* argv[] ){
	
	//BS setup
	BaseStation * base = new BaseStation();
	
	//MS setup
	ifstream infile(argv[1]);

	if(!infile){
		cout << "Specifify input file." << endl;
		return 0;
	}

	int stationNumber, be, bk, vi, vo, da;

	while(infile >> stationNumber >> be >> bk >> vi >> vo >> da){
		//cout << "CheckpointX" << endl;
		MobileStation * temp = new MobileStation(stationNumber, base);

		temp->packets[0] = be;
		temp->packets[1] = bk;
		temp->packets[2] = vi;
		temp->packets[3] = vo;

#ifdef IEEE_STANDARD
		//if IEEE standardm make data packets as best effort class.
		temp->packets[0] = temp->packets[0] + da;
		temp->packets[4] = 0;
#else
		temp->packets[4] = da;
#endif

		base->addStation(*temp);
	}
	
	//startup MS
	//cout << "Checkpoint" << endl;
	boost::thread_group mobileThreads;

	for(vector<MobileStation>::iterator it = base->connectedStations.begin(); it != base->connectedStations.end(); ++it) {
		mobileThreads.add_thread(new boost::thread(startMobileStation, *it));
	}

	mobileThreads.join_all();
	
	return 0;
}
