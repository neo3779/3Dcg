/*
 * File name:	D3DCamera.cpp 
 * Description:	The D3DCamera class implements a 3D camera object. 
 * Author: 		Alex J Davison
 */

#include		"D3DCamera.h"

//Constructor
D3DCamera::D3DCamera()
{

}

//Deconstructo
D3DCamera::~D3DCamera()
{
	return;	
}

//Set the camera matix
VOID D3DCamera::setMatrices()
{
	setViewMatrix();
	setProjectionMatrix();
}

//Set the camera position
VOID D3DCamera::setvCameraPos(D3DXVECTOR3 vCameraPos)
{
	this->vCameraPos = vCameraPos;
}

//Set where the camera should look at
VOID D3DCamera::setvLookatPos(D3DXVECTOR3 vLookatPos)
{
	this->vLookatPos = vLookatPos;
}

//Set the direction of the camera
VOID D3DCamera::setvUpVec(D3DXVECTOR3 vUpVec)
{
	this->vUpVec = vUpVec;
}

//Geth position of the camera
D3DXVECTOR3 D3DCamera::getvCameraPos()
{
	return vCameraPos;
}

//Get where the camera is looking at
D3DXVECTOR3 D3DCamera::getvLookatPos()
{
	return vLookatPos;
}

//Get the direction of the camera
D3DXVECTOR3 D3DCamera::getvUpVec()
{
	return vUpVec;
}

//Set the view matrix of the camera
VOID D3DCamera::setViewMatrix()
{
	// Set / update view matrix.
	// Parameters:
	// . viewpoint set to zero units along the world x-axis, three units up along 
	//   the world y-axis, five units back along the world z-axis.
	// . look-at point set as origin of world.
	// . view-up direction defined approximately, as in y-direction of world.

	D3DXMatrixLookAtLH( &m_matView, &vCameraPos, &vLookatPos, &vUpVec );

	// Alternative settings
	//D3DXVECTOR3 vEyePt( 0.0f, 1.0f, -5.0f );
	//D3DXVECTOR3 vLookatPt( 0.0f, 1.0f, 0.0f );
	//D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	//D3DXMatrixLookAtLH( &m_matView, &vEyePt, &vLookatPt, &vUpVec );
}

//Set the projection matirix of the camera
VOID D3DCamera::setProjectionMatrix()
{
	// Set / update projection matrix. 
	// Parameters:
	// . projection type set as perspective transform from 3D view space to 
	//   2D viewport space.
	// . field of view defined by: 
	//	 * angle along vertical set as pi/4,
	//	 * aspect ratio set as 1
	//	 * near and far clipping planes (resp.) set as 1 and 100 units from viewpoint.

	// Alternative setting
	//D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );

	// Initialize the projection matrix
	D3DXMatrixPerspectiveFovLH( &m_matProj, ( float )D3DX_PI * 0.5f, 1.0f, 0.1f, 100.0f );
}

//Gets the view transformation matrix
D3DXMATRIX D3DCamera::getViewMat()
{
	return m_matView;
}

//Gets the projection transformation matrix
D3DXMATRIX D3DCamera::getProjMat()
{
	return m_matProj;
}