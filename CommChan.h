#ifndef CommChan
#define CommChan

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

#define CLEAR true
#define BUSY false

#define SUCCESS true
#define COLLISION false

class CommChan{
	private:
	bool busy;
	MobileStation * currentTranmitter;
	
	public:
	CommChan();
}
#endif
