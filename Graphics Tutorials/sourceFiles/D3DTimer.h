/*
 * File name:	D3DTimer.h
 * Description:	This is a header file for D3DTimer.cpp
 * Author: 		Alex J Davison
*/

#ifndef _D3D_TIMER_H
#define _D3D_TIMER_H

//For Windows
#include		<windows.h>

class D3DTimer
{
	//Variables
	private: 
	
		//Time elasped
		float	time;
		//Time the timer was started
		DWORD	dwTimeStart;
		//Current time 
		DWORD	dwTimeCur;

	//Methods
	public: 

		//Constructor and deconstructor 
		D3DTimer();
		~D3DTimer();

		//Update time
		VOID	update();
		//Reset timer
		VOID	resetTimer();
		//Set the start time of the timer
		VOID	setdwTimeStart(float value);
		//Get the time elsaped
		float	getTime();

};

#endif /* _D3D_TIMER_H */