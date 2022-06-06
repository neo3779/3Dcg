#include		"D3DTimer.h"

//Constructor
D3DTimer::D3DTimer()
{
	dwTimeStart = 0;
	time = 0;
}

//Deconstructor
D3DTimer::~D3DTimer()
{

}

//Update the timer 
VOID D3DTimer::update()
{
	//Set up the double word with current time
	dwTimeCur = GetTickCount();
	
	//Check if time started equals zero
	if( dwTimeStart == 0 )
	{
		dwTimeStart = dwTimeCur;
	}

	//Calucate time
	time = ( dwTimeCur - dwTimeStart ) / 1000.0f;
}

//Get time, time being the time between updates
float D3DTimer::getTime()
{
	return time;
}

//Set the time that the timer was started
void D3DTimer::setdwTimeStart(float value)
{
	this->dwTimeStart = value;
}

//Reset the timer
void D3DTimer::resetTimer()
{
	dwTimeStart = 0;
	time = 0;
}