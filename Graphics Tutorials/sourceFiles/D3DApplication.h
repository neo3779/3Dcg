/*
* File name:	D3DApplication.h
* Description:	This is a header file for D3DApplication.cpp
* Author: 		Alex J Davison
*/


#ifndef _D3D_APPL_H
#define _D3D_APPL_H

#include		<windows.h>
// For DirectX Graphics 10
#include		<d3dx10.h>
#include		"Light.h"
#include		"D3DTimer.h"
#include		"D3DObject.h"
#include		"D3DCamera.h"

class D3DApplication
	{

	private: 

		static const int			numberOfObjects = 27;
		static const int			numberOfViewingPoints = 15;
		static const int			y = 2;
		static const int			radius = 15;
		static const int			numberOfLights = 3;

		int							cameraStartPoint;
		int							cameraEndPoint;
		
		int							lightType;

		Light						lights[numberOfLights];

		//D3D10 API objects used by the application
		D3D10_DRIVER_TYPE			m_driverType; 
		//Pointer to rendering device object
		ID3D10Device*				m_pd3dDevice; 
		IDXGISwapChain*				m_pSwapChain; 
		ID3D10RenderTargetView*		m_pRenderTargetView; 
		ID3D10EffectTechnique*		m_pTechnique; 

		//Textures
		ID3D10ShaderResourceView*	mTextureRV;
		ID3D10ShaderResourceView*	mTextureBump;
		ID3D10ShaderResourceView*	mTextureSpec;
		
		//Number of points for a camera to pass through
		D3DXVECTOR3					m_ViewPoint[numberOfViewingPoints];
		//Graphics object and camera used by the application
		//3D graphics object
		D3DObject					m_threeDobject[numberOfObjects] ;	
		//Camera
		D3DCamera					m_camera;
		//Pointer to effect object
		ID3D10Effect*				m_pEffect;		
		//Point to point vector
		D3DXVECTOR3					p2pvec; 
		//Point to camera
		D3DXVECTOR3					p2cvec;
		//Point to point magntued
		float						p2pmag;
		//Point to camer magnatued
		float						p2cmag;
		//Timer object
		D3DTimer					timer[numberOfObjects + 1];
		// Clear the back buffer
		float						ClearColor[4];
		ID3D10EffectVariable*		m_LightVar;
		
		D3DXVECTOR4					mReflectNone;
		D3DXVECTOR4 mReflectAll;

		D3DXMATRIX mIdentityTexMtx;

		D3D10_TEXTURE2D_DESC		depthStencilDesc;
		ID3D10Texture2D*			pBuffer;

		ID3D10DepthStencilView*		mDepthStencilView;

		ID3D10EffectVariable*		mfxLightVar;
		ID3D10EffectVariable*		mfxEyePosVar;
		ID3D10EffectMatrixVariable* mfxWVPVar;
		ID3D10EffectMatrixVariable* mfxWorldVar;
		ID3D10EffectMatrixVariable* mfxTexMtxVar;
		ID3D10EffectVectorVariable* mfxReflectMtrlVar;

		ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
		ID3D10EffectShaderResourceVariable* mfxSpecMapVar;
		ID3D10EffectShaderResourceVariable* mfxNormalMapVar;
		
	//Functions
	public: 

		// Constructor and destructor
		D3DApplication();
		~D3DApplication();

		//Effect obect creator
		HRESULT createEffectObject();			
		//Initialises Direct3D
		HRESULT initialiseD3D( HWND hWnd );
		//Creates Direct3D device and swap chain
		HRESULT createDevSwapChain( HWND hWnd );
		//Initialises the world
		HRESULT initialiseWorld();	
		//Initialises the camera
		HRESULT initialiseCamera();	
		//Get effect variables
		VOID getEffectVariables();	
		//Set the effects
		VOID setEffect(D3DObject *Object);		
		//Passes the specification of the world through the graphics pipeline, and draws the scene
		VOID render();
		//Releases D3D objects
		VOID relD3DObjects();
		//Work out the path of the camera
		VOID cameraPathGivenY(float y, float radius);
		//Update scene
		VOID update();
		
	private: 

		D3DXVECTOR3 lineEquation(D3DXVECTOR3 p1, D3DXVECTOR3 p2,float t);
		D3DXVECTOR3 vectorEquation(D3DXVECTOR3 p1, D3DXVECTOR3 p2);
		float mag(D3DXVECTOR3 p1);
		//Sets the transformation pipeline (world, view, and projection transformation matrices)
		VOID setTransfPipe(D3DObject *Object);	
		//Load resources
		VOID loadResources();
		//Initialise lighting
		VOID initialiseLighting();
		//Update lighting
		VOID updateLighting();

	};

#endif /* _D3D_APPL_H */




