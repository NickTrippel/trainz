/**
* Title: TrainSystem.cc
* Date: November 7, 2012
* Author(s): Robert O'Connor/Nick Trippel
* Description: represents the system of trains
*/

using namespace std;

#define TRAINSYSTEM_SRC

#include <assert.h>
#include <string>
#include <iostream>
#include <fstream>
#include "ABCList.hpp"
#include "LinkedList.hpp"
#include "Queue.hpp"
#include "Train.h"
#include "Passenger.h"
#include "Station.h"
#include "Line.h"
#include "TrainSystem.h"


void TrainSystem::Start(int trains, int hour, int min)
{
	fout.open ("passlog.txt") ;
	startHour= hour;
	startMinute= min;

	numberOfTrains= trains ;

	int systemCloseTime= 60*(endHour - startHour) + (endMinute - startMinute);

	redRoute= new CLine (RED);
	redRoute->Initialize (this) ;

	blueRoute = new CLine (BLUE);
	blueRoute->Initialize (this) ;

	// ROBERT 
	// you can only display one train route per simulation
	// UNcomment one of the following to display trains on one route
	//  blueRoute->ToggleDisplay() ;
	// redRoute->ToggleDisplay() ;

	assert(systemCloseTime > 0);
	Initialize () ; // create routes/stations

	int i;
	for (i= 0; i < systemCloseTime;  i++)
	{
		NotifyLines ();
		timeCounter++ ;
	}

}

void TrainSystem::Initialize()
{

	// initialize MultiLineStation for inclusion in both RED and BLUE lists
	MultiLineStation* ts= new MultiLineStation ();
	enum Line lineID1= redRoute->GetLineID() ;
	enum Line lineID2= blueRoute->GetLineID() ;

	// fill up RED route list with stations
	
	Station *s= new Station();
	s->Initialize(this, lineID1, 'A', 1);  // this 
	redRoute->AddStation (s);
	
	s= new Station();
	s->Initialize(this, lineID1, 'A', 2);
	redRoute->AddStation (s);
	
	s= new Station();
	s->Initialize(this, lineID1, 'A', 3);
	redRoute->AddStation (s);
	
	s= new Station();
	s->Initialize(this, lineID1, 'A', 4);
	redRoute->AddStation (s);
	
	// fifth station on RED route is transfer station
	// at this point, we call isTransfer() which sets xferPos to the Cline list pos
	ts->Initialize(this, lineID1, lineID2, 'B', 3);
	redRoute->AddStation (ts);
	
	s= new Station();
	s->Initialize(this, lineID1, 'B', 4);
	redRoute->AddStation (s);
	
	s= new Station();
	s->Initialize(this, lineID1, 'B', 5);
	redRoute->AddStation (s);

	// fill up blue route list with stations
	
	Station *t= new Station();
	t->Initialize(this, lineID2, 'B', 1);  // this 
	blueRoute->AddStation (t);
	
	t = new Station();
	t->Initialize(this, lineID2, 'B', 2);
	blueRoute->AddStation (t);
	
	// third station on BLUE route is transfer station
	blueRoute->AddStation (ts);
	
	t = new Station();
	t->Initialize(this, lineID2, 'A', 5);
	blueRoute->AddStation (t);

	// ROBERT - train placement is critical (UNcomment line above for red- or blueRoute->ToggleDisplay() ;
	// to monitor train movement through the system
	// outbound is the positive direction (pos increases) from top of list
	// top of red  route (list) starts at station A1 with positions from 1 through 7 - transfer pos 5
	// top of blue route (list) starts at station B1 with positions from 1 through 4 - transfer pos 3

	// probably best not to PlaceTrain back to back at start of Cline
	// Example Given
	// redRoute->PlaceTrain(1, INBOUND);
	// redRoute->PlaceTrain(1, OUTBOUND);

	// place trains on RED route

	redRoute->PlaceTrain(4, INBOUND);
	redRoute->PlaceTrain(1, OUTBOUND);
	redRoute->PlaceTrain(7, INBOUND);
	redRoute->PlaceTrain(4, OUTBOUND);
		
	// place trains on BLUE route

	blueRoute->PlaceTrain(3, INBOUND);
	blueRoute->PlaceTrain(1, OUTBOUND);
	blueRoute->PlaceTrain(4, INBOUND);
	blueRoute->PlaceTrain(2, OUTBOUND); 


}
void TrainSystem::NotifyLines() 
{
	redRoute->NotifyStations() ;
	blueRoute->NotifyStations() ; 
}

void TrainSystem::FindDestination(enum Line lineOfOrigin, int stationPosOfOrigin, 
	enum Line lineOfDestination, int stationPosOfDestination,bool &bInbound, bool &xfer )
{//stub
	
	if (lineOfOrigin == lineOfDestination)
		xfer= false ;
	else
		xfer= true ;

	CLine* curLine= NULL ;

	if (lineOfOrigin == RED)
		curLine= redRoute;
	else 
		curLine= blueRoute ;
	
	if (xfer == true)
	{
		if (stationPosOfOrigin < curLine->xferPos)
			bInbound= false ;
		else
			bInbound= true ;
	}
	else
	{
		if (stationPosOfOrigin < stationPosOfDestination)
			bInbound= false ;
		else
			bInbound= true ;
	}
	
}


