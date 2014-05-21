
WMN-Project-Code
================

ELEN 6951 - Xusheng/Misha

Compiling command: 

g++ -Wall -pthread MobileStationQueue.cpp MobileStation.cpp BaseStation.cpp -lboost_system -lboost_thread -o e.out

Compiling command for IEEE standard: 

g++ -Wall -pthread MobileStationQueue.cpp MobileStation.cpp BaseStation.cpp -lboost_system -lboost_thread -o i.out -DIEEE_STANDARD

Assume:

Equidistant Stations transmitting at equal power and not moving
All stations in range are active
No RTS/CTS


File Format:

Station Number - BE - BK - VI - VO - DA

