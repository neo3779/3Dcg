/*
 * File name:	D3DCamera.h
 * Description:	This is a header file for D3DCamera.cpp
 * Author: 		Alex J Davison
*/

#ifndef _D3D_CAMERA_H
#define _D3D_CAMERA_H

//For Windows
#include		<windows.h>
//For DirectX Graphics 10
#include		<d3dx10.h>

	class D3DCamera
	{
		//Variables
		private: 

			//For example, replace magic numbers that appear in setViewMatrix() by member variables
			//View matrix
			D3DXMATRIX			m_matView; 	
			//Projection matrix
			D3DXMATRIX			m_matProj;
			//Camera position
			D3DXVECTOR3			vCameraPos;
			//Camera look at point
			D3DXVECTOR3			vLookatPos;
			//Direction of the camera
			D3DXVECTOR3			vUpVec;

		//Functions
		public: 
			// Constructor and destructor
			D3DCamera();
			~D3DCamera();
		
			// Sets the view and projection transformation matrices
			VOID setMatrices();
			//Set where the camera is located
			VOID setvCameraPos(D3DXVECTOR3 vCameraPos);
			//Set where the camera should look at
			VOID setvLookatPos(D3DXVECTOR3 vLookatPos);
			//Set the direction of the camera
			VOID setvUpVec(D3DXVECTOR3 vUpVec);
			//Sets the view transformation matrix
			VOID setViewMatrix();
			//Sets the projection transformation matrix
			VOID setProjectionMatrix();
		
			//Get the camera position
			D3DXVECTOR3 getvCameraPos();
			//Get where th camera should look at
			D3DXVECTOR3 getvLookatPos();
			//Get the direction of the camera
			D3DXVECTOR3 getvUpVec();
			// Gets the view matrix
			D3DXMATRIX getViewMat();
			// Gets the projection matrix
			D3DXMATRIX getProjMat();		
	
	};

#endif /* _D3D_CAMERA_H */




