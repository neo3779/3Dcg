/*
* File name:	D3DApplication.cpp
* Description:	The D3DApplication class implements a 3D graphics application object. 
* Author: 		Alex J Davison
*/

#include		"D3DApplication.h"

// Construct and initialise a graphics application object
D3DApplication::D3DApplication()
{
	//Set backgorund colour
	ClearColor[0] = 0.25f;
	ClearColor[1] = 0.25f;
	ClearColor[2] = 0.25f;
	ClearColor[3] = 1.0f; 

	//Create objects to NULL
	m_driverType = D3D10_DRIVER_TYPE_NULL;
	m_pd3dDevice = NULL;
	m_pSwapChain = NULL;
	m_pRenderTargetView = NULL;
	m_pTechnique = NULL;
	m_LightVar =NULL;
	mTextureRV = NULL;
	m_pEffect = NULL;

	//Set where the camera will start and end
	cameraStartPoint = 0;
	cameraEndPoint = 1;
	
	//Calulate camera position
	cameraPathGivenY(y,radius);

	//Set where the camera points
	m_camera.setvLookatPos(D3DXVECTOR3(0.0f,0.0f,0.0f));
	//Set the orination of the camera
	m_camera.setvUpVec(D3DXVECTOR3(0.0f,1.0f,0.0f));
	//Set the camera position
	m_camera.setvCameraPos(m_ViewPoint[0]);
	
	//Create the timers
	for(int i = 0; i < numberOfObjects + 1; i++)
	{
		timer[i] =  D3DTimer();
	} 

	//Create indentity matix for textures
	D3DXMatrixIdentity(&mIdentityTexMtx);
	
	//Set reflection of the surfaces
	mReflectNone = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	mReflectAll  = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	
	//Create lights
	initialiseLighting();
}

//Destructs a graphics application object
D3DApplication::~D3DApplication()
{
	//Release objects
	relD3DObjects();
}

//Release D3D objects
VOID D3DApplication::relD3DObjects()
{
	//Release effect object
	if( m_pEffect )	{m_pEffect->Release(); m_pEffect = NULL;}

	//Loop through objects
	for (int i = 0; i < numberOfObjects; i++)
	{
		//Release D3D objects owned by graphics object
		m_threeDobject[i].relD3DObjects();	
	}
	if( m_pd3dDevice )			m_pd3dDevice->ClearState();
	if( m_pRenderTargetView )	{m_pRenderTargetView->Release(); m_pRenderTargetView = NULL;}
	if( m_pSwapChain )			{m_pSwapChain->Release(); m_pSwapChain = NULL;}
	//Release rendering device object
	if( m_pd3dDevice )			{m_pd3dDevice->Release(); m_pd3dDevice = NULL;}	
}

//Initialises Direct3D
HRESULT D3DApplication::initialiseD3D( HWND hWnd )
{
	HRESULT hr = S_OK;

	// Create Direct3D device and swap chain
	createDevSwapChain( hWnd );
	//Create effects object
	createEffectObject();
	//Initalises the effect variable
	getEffectVariables();

	return S_OK;
}

//Creates Direct3D device and swap chain
HRESULT D3DApplication::createDevSwapChain( HWND hWnd )
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect( hWnd, &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
	
	#ifdef _DEBUG
		createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
	#endif

	D3D10_DRIVER_TYPE driverTypes[] =
	{
		D3D10_DRIVER_TYPE_HARDWARE,
		D3D10_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

	DXGI_SWAP_CHAIN_DESC scDesc;
	ZeroMemory( &scDesc, sizeof( scDesc ) );
	scDesc.BufferCount							= 1;
	scDesc.BufferDesc.Width						= width;
	scDesc.BufferDesc.Height					= height;
	scDesc.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferDesc.RefreshRate.Numerator		= 60;
	scDesc.BufferDesc.RefreshRate.Denominator	= 1;
	scDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.OutputWindow							= hWnd;
	scDesc.SampleDesc.Count						= 1;
	scDesc.SampleDesc.Quality					= 0;
	scDesc.Windowed								= TRUE;

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		m_driverType = driverTypes[driverTypeIndex];
		hr = D3D10CreateDeviceAndSwapChain( NULL, m_driverType, NULL, createDeviceFlags,
											D3D10_SDK_VERSION, &scDesc, &m_pSwapChain, &m_pd3dDevice );
		if ( SUCCEEDED( hr ) )
			break;
	}
	if ( FAILED( hr ) )
		return hr;

	// Create a render target view
	hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), ( LPVOID* )&pBuffer );
	if ( FAILED( hr ) )
		return hr;

	hr = m_pd3dDevice->CreateRenderTargetView( pBuffer, NULL, &m_pRenderTargetView );
	pBuffer->Release();

	if ( FAILED( hr ) )
		return hr;

	depthStencilDesc.Width     = width;
	depthStencilDesc.Height    = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count   = 1; // multisampling must match
	depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
	depthStencilDesc.Usage          = D3D10_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D10_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	m_pd3dDevice->CreateTexture2D(&depthStencilDesc, 0, &pBuffer);
	m_pd3dDevice->CreateDepthStencilView(pBuffer, 0, &mDepthStencilView);


	// Bind the render target view and depth/stencil view to the pipeline.

	m_pd3dDevice->OMSetRenderTargets(1, &m_pRenderTargetView, mDepthStencilView);

	// Setup the viewport
	D3D10_VIEWPORT viewport;
	viewport.Width		= width;
	viewport.Height		= height;
	viewport.MinDepth	= 0.0f;
	viewport.MaxDepth	= 1.0f;
	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	m_pd3dDevice->RSSetViewports( 1, &viewport );
	
	return S_OK;
}

//Initialises the world
HRESULT D3DApplication::initialiseWorld()
{

	HRESULT hr = S_OK;

	//Load textures
	loadResources();

	//Initialise effects 
	m_pTechnique = m_pEffect->GetTechniqueByName( "NormalMapTech" );

	//Initialise 3D object 
	for (int i = 0; i < numberOfObjects; i++)
	{
		m_threeDobject[i].setRenderingDev(m_pd3dDevice);
		m_threeDobject[i].setScaling(2.0f,2.0f,2.0f);
		m_threeDobject[i].setTechnique(m_pTechnique);

		hr = m_threeDobject[i].initialiseMesh();
		if(hr != S_OK)
		{
			break;
		}
	}

	//Cube count
	int cubeCount = 0;
	//Set loaction of the cubes
	for(int x = -3; x < 6; x+=3)
	{
		for(int y = -3; y < 6; y+=3)
		{
			for(int z = -3; z < 6; z+=3)
			{
				m_threeDobject[cubeCount].setTranslation((float)x,(float)y,(float)z);
				cubeCount++;
			}
		}
	}

	return hr;
}

//Initialises the camera 
HRESULT D3DApplication::initialiseCamera()
{
	// Initialise camera 
	m_camera.setMatrices();	
	return S_OK; 
}

//Update the world
VOID D3DApplication::update()
{
	//Update timers
	for(int i = 0; i < numberOfObjects + 1; i++)
	{
		timer[i].update();
	}

	//Calulcate point to point for camera 
	p2pvec = vectorEquation(m_ViewPoint[cameraStartPoint],m_ViewPoint[cameraEndPoint]);
	//Calucate point to camera 
	p2cvec =  vectorEquation(m_ViewPoint[cameraStartPoint],m_camera.getvCameraPos());
	//Magntudie between point and point
	p2pmag = mag(p2pvec);
	//Magntudie between point and camera
	p2cmag = mag(p2cvec);
	//Check to see if camera is passed the point
	if(p2cmag > p2pmag)
	{
		//Incease points
		cameraStartPoint++;
		cameraEndPoint++;
		//If last point rest to first point
		if(cameraEndPoint > numberOfViewingPoints-1)
		{
			cameraEndPoint = 0;
		}
		//If last point rest to first point
		if(cameraStartPoint > numberOfViewingPoints-1)
		{
			cameraStartPoint = 0;
		}
		//Rest timer
		timer[3].setdwTimeStart(0.0f);
		timer[3].update();
	}

	//Set camera position
	m_camera.setvCameraPos(lineEquation(m_ViewPoint[cameraStartPoint],m_ViewPoint[cameraEndPoint],timer[3].getTime()));
	
	//Update lighting
	updateLighting();

	//Rotate cubes 
	for(int i = 0; i < 9 ; i ++)
	{
		m_threeDobject[i].setRotation(timer[1].getTime(),timer[1].getTime(),timer[1].getTime());
	}

	//Check scale of the cubes and rest if too big
	if(m_threeDobject[9].getScaling().x > 3 )
	{
		timer[0].resetTimer();
	}

	//Update scale of cube
	for(int i = 9; i < 18; i++)
	{
		m_threeDobject[i].setScaling(timer[0].getTime(),timer[0].getTime(),timer[0].getTime());	
	}

	//Rest cubes position if it has moved to far
	if(m_threeDobject[18].getTranslation().x > 6 )
	{
		timer[2].resetTimer();
	}

	//Move cubes
	for(int i = 18; i < 27; i++)
	{
		m_threeDobject[i].setTranslation(timer[2].getTime() ,m_threeDobject[i].getTranslation().y,m_threeDobject[i].getTranslation().z);
	}
}

//Renders a frame: Passes the specification of the world through the graphics pipeline, and draws the scene
VOID D3DApplication::render()
{
	m_pd3dDevice->OMSetDepthStencilState(0, 0);
	m_pd3dDevice->ClearRenderTargetView( m_pRenderTargetView, ClearColor );
	m_pd3dDevice->ClearDepthStencilView(mDepthStencilView, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);
	//Set camera matrix
	m_camera.setMatrices();
	for (int i = 0; i < numberOfObjects; i++)
	{
		//Set update matrices of transformation pipeline
		setTransfPipe(&m_threeDobject[i]);
		
		//Render graphics object 
		m_threeDobject[i].render();
	}
	
	// Present back buffer to front buffer
	m_pSwapChain->Present( 0, 0 );
}

//Sets the transformation pipeline (world, view, and projection transformation matrices).
VOID D3DApplication::setTransfPipe(D3DObject *Object)
{
	//Set world matix of object
	Object->setWorldMatrix();
	//Set effect
	setEffect(Object);
}

// Create the effect
HRESULT D3DApplication::createEffectObject()
{
	HRESULT hr;

	// Create the effect
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

	#if defined( DEBUG ) || defined( _DEBUG )
		// Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
		dwShaderFlags |= D3D10_SHADER_DEBUG;
	#endif

	// Gather effect compilation errors
	ID3D10Blob *	pFXcompileErrors;

	//Load effect file
	hr = D3DX10CreateEffectFromFile( /*L*/".\\Effects\\normalmap.fx", NULL, NULL, "fx_4_0", dwShaderFlags, 0, m_pd3dDevice, NULL, NULL, &m_pEffect, &pFXcompileErrors, NULL );
	
	if( FAILED( hr ) )
	{
		if( hr == D3D10_ERROR_FILE_NOT_FOUND )
		{
			MessageBox( NULL, /*L*/"The FX file cannot be located. Please run this executable from the parent of the directory (named 'Effects') which contains the FX file.", /*L*/"Error", MB_OK );
		}
		else
		{
			MessageBox( NULL, /*L*/"Failed to compile the FX file.  Please debug the FX code.", /*L*/"Error", MB_OK );

			#if defined( DEBUG ) || defined( _DEBUG )
			// Display effect compile errors (if any) on debugging console
			if ( pFXcompileErrors != 0 && pFXcompileErrors->GetBufferPointer() != 0 )
			{
				OutputDebugString( "\n--------------------------------------------------------------------------\n" );
				OutputDebugString( "                           Effect Compilation Errors\n" );
				OutputDebugString( "--------------------------------------------------------------------------\n\n" );
				OutputDebugString( (char *)pFXcompileErrors->GetBufferPointer() );
				OutputDebugString( "\n--------------------------------------------------------------------------\n\n" );
			}
			#endif // defined( DEBUG ) || defined( _DEBUG )
		}
		return hr;
	}
	return S_OK; 
}

//Set the effect variables links
VOID D3DApplication::getEffectVariables()
{
	//Set the light
	mfxLightVar          = m_pEffect->GetVariableByName("gLight");
	//Set the camera position
	mfxEyePosVar         = m_pEffect->GetVariableByName("gEyePosW");
	//Set world, view and project matrix
	mfxWVPVar            = m_pEffect->GetVariableByName("gWVP")->AsMatrix();
	//Set world variables
	mfxWorldVar          = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
	//Set texture matix
	mfxTexMtxVar         = m_pEffect->GetVariableByName("gTexMtx")->AsMatrix();
	//Set reflection matrix
	mfxReflectMtrlVar    = m_pEffect->GetVariableByName("gReflectMtrl")->AsVector();
	//Set diffused texture
	mfxDiffuseMapVar     = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
	//Set specuclar texture
	mfxSpecMapVar        = m_pEffect->GetVariableByName("gSpecMap")->AsShaderResource();
	//Set normal texture
	mfxNormalMapVar      = m_pEffect->GetVariableByName("gNormalMap")->AsShaderResource();
}

// Set the parameters of the effect
VOID D3DApplication::setEffect(D3DObject *Object )
{
	//Set camera position
	mfxEyePosVar->SetRawValue(&m_camera.getvCameraPos(), 0, sizeof(D3DXVECTOR3));
		
	//Apply all lights
	if(lightType == 4)
	{
		for(int i = 0; i < numberOfLights; i ++)
		{
			mfxLightVar->SetRawValue(&lights[i], 0, sizeof(Light));
		}
	}
	else
	{
		//Apply just one light
		mfxLightVar->SetRawValue(&lights[lightType], 0, sizeof(Light));
	}
	//Set world and view and project matirx
	D3DXMATRIX WVP = Object->getWorldMat() * m_camera.getViewMat() * m_camera.getProjMat();
	mfxWVPVar->SetMatrix((float*)&WVP);
	//Set world matrix
	mfxWorldVar->SetMatrix((float*)&Object->getWorldMat());
	//Set texture matrix
	mfxTexMtxVar->SetMatrix((float*)&mIdentityTexMtx);
	//Set the diffuse map
	mfxDiffuseMapVar->SetResource(mTextureRV);
	//Set the normal map
	mfxNormalMapVar->SetResource(mTextureBump);
	//Set specular map
	mfxSpecMapVar->SetResource(mTextureSpec);
	//Set refective 
	mfxReflectMtrlVar->SetFloatVector((float*)&mReflectAll);

}

//Vector between two points
D3DXVECTOR3 D3DApplication::vectorEquation(D3DXVECTOR3 p1, D3DXVECTOR3 p2)
{		
	return p2- p1;
}

//Point on a line between two points
D3DXVECTOR3 D3DApplication::lineEquation(D3DXVECTOR3 p1, D3DXVECTOR3 p2,float t)
{
	D3DXVECTOR3 point;
		
	point = p1 + (t * vectorEquation(p1,p2));
		
	return point;
}

//Calucate magntude
float D3DApplication::mag(D3DXVECTOR3 p1)
{
	float f = 0.0f;

	f = sqrt((p1.x*p1.x)+(p1.y*p1.y)+(p1.z*p1.z));
		
	return f;
}

//Calulate the position of the camera
VOID D3DApplication::cameraPathGivenY(float y, float radius)
{
	//Radion
	float increments = 360 / numberOfViewingPoints;
	float num = 0;
	for (int i = 0; i < numberOfViewingPoints; i++)
	{
		num = increments * i;
		num = D3DXToRadian(num);
		m_ViewPoint[i].x = radius * cosf(num);
		m_ViewPoint[i].y = y;
		m_ViewPoint[i].z = radius * sinf(num);
	}
}

//Set up light
VOID D3DApplication:: initialiseLighting()
{
	lightType = 0;
	
	//Parallel light
	lights[0].position = D3DXVECTOR3(9.0f,9.0f,9.0f);
	lights[0].direction = D3DXVECTOR3(0.57735f,-0.57735f,0.57735f);
	lights[0].ambient = D3DXCOLOR(0.1f,0.1f,0.1f,1.0f);
	lights[0].diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	lights[0].specular = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	//Point light
	lights[1].position = D3DXVECTOR3(-9.0f,-9.0f,-9.0f);
	lights[1].ambient = D3DXCOLOR(0.4f,0.4f,0.4f,1.0f);
	lights[1].diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	lights[1].specular = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	lights[1].attenuation.x = 0.0f;
	lights[1].attenuation.y = 0.1f;
	lights[1].attenuation.z = 0.0f;
	lights[1].range = 50.0f;

	//Spotlight
	lights[3].position = D3DXVECTOR3(9.0f,-9.0f,9.0f);
	lights[2].direction = D3DXVECTOR3(0.0f,0.0f,0.0f);
	lights[2].ambient = D3DXCOLOR(0.25f,0.25f,0.25f,0.3f);
	lights[2].diffuse = D3DXCOLOR(0.5f,0.5f,0.5f,0.3f);
	lights[2].specular = D3DXCOLOR(0.0f,0.0f,0.7f,0.3f);
	lights[2].attenuation.x = 0.0f;
	lights[2].attenuation.y = 0.2f;
	lights[2].attenuation.z = 0.0f;
	lights[2].spotPow = 34.0f;
	lights[2].range = 9.0f;

}

//Update lighting
VOID D3DApplication:: updateLighting()
{
	//Set light state

	//Parallel light
	if(GetAsyncKeyState('1') & 0x8000)
	{
		lightType = 0;
	}
	//Point light
	if(GetAsyncKeyState('2') & 0x8000)
	{
		lightType = 1;
	}
	//Spotlight
	if(GetAsyncKeyState('3') & 0x8000)
	{
		lightType = 2;
	}
	//Set lights on
	if(GetAsyncKeyState('4') & 0x8000)
	{
		lightType = 3;
	}

	//Set to zero
	if(GetAsyncKeyState('Z') & 0x8000)
	{
		lights[lightType].ambient = D3DXCOLOR(0.0f,0.0f,0.0f,0.f);
		lights[lightType].diffuse = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
		lights[lightType].specular = D3DXCOLOR(0.0f,0.0f,0.0f,.0f);
	}

	//Ambient light

	//Red plus
	if(GetAsyncKeyState('W') & 0x8000)
	{
		if(lights[lightType].ambient.r < 1.0f )
		{
			lights[lightType].ambient.r += 0.003f;
		}
	}
	//Red negative
	if(GetAsyncKeyState('S') & 0x8000)
	{
		if(lights[lightType].ambient.r > 0.0f )
		{
			lights[lightType].ambient.r -= 0.003f;
		}
	}
	//Green plus
	if(GetAsyncKeyState('E') & 0x8000)
	{
		if(lights[lightType].ambient.g < 1.0f)
		{
			lights[lightType].ambient.g += 0.003f;
		}
	}
	//Green negative
	if(GetAsyncKeyState('D') & 0x8000)
	{
		if(lights[lightType].ambient.g > 0.0f )
		{
			lights[lightType].ambient.g -= 0.003f;
		}
	}
	//Blue plus
	if(GetAsyncKeyState('R') & 0x8000)
	{
		if(lights[lightType].ambient.b < 1.0f)
		{
			lights[lightType].ambient.b += 0.003f;
		}
	}
	//Blue negative
	if(GetAsyncKeyState('F') & 0x8000)
	{
		if(lights[lightType].ambient.b > 0.0f )
		{
			lights[lightType].ambient.b -= 0.003f;
		}
	}

	//Diffuse light

	//Red plus
	if(GetAsyncKeyState('T') & 0x8000)
	{
		if(lights[lightType].diffuse.r < 1.0f )
		{
			lights[lightType].diffuse.r += 0.003f;
		}
	}
	//Red negative
	if(GetAsyncKeyState('G') & 0x8000)
	{
		if(lights[lightType].diffuse.r > 0.0f )
		{
			lights[lightType].diffuse.r -= 0.003f;
		}
	}
	//Green plus
	if(GetAsyncKeyState('Y') & 0x8000)
	{
		if(lights[lightType].diffuse.g < 1.0f)
		{
			lights[lightType].diffuse.g += 0.003f;
		}
	}
	//Green negative
	if(GetAsyncKeyState('H') & 0x8000)
	{
		if(lights[lightType].diffuse.g > 0.0f )
		{
			lights[lightType].diffuse.g -= 0.003f;
		}
	}
	//Blue plus
	if(GetAsyncKeyState('U') & 0x8000)
	{
		if(lights[lightType].diffuse.b < 1.0f)
		{
			lights[lightType].diffuse.b += 0.003f;
		}
	}
	//Blue negative
	if(GetAsyncKeyState('J') & 0x8000)
	{
		if(lights[lightType].diffuse.b > 0.0f )
		{
			lights[lightType].diffuse.b -= 0.003f;
		}
	}

	//Specular light

	//Red plus
	if(GetAsyncKeyState('I') & 0x8000)
	{
		if(lights[lightType].specular.r < 1.0f )
		{
			lights[lightType].specular.r += 0.003f;
		}
	}
	//Red negative
	if(GetAsyncKeyState('K') & 0x8000)
	{
		if(lights[lightType].specular.r > 0.0f )
		{
			lights[lightType].specular.r -= 0.003f;
		}
	}
	//Green plus
	if(GetAsyncKeyState('O') & 0x8000)
	{
		if(lights[lightType].specular.g < 1.0f)
		{
			lights[lightType].specular.g += 0.003f;
		}
	}
	//Green negative
	if(GetAsyncKeyState('L') & 0x8000)
	{
		if(lights[lightType].specular.g > 0.0f )
		{
			lights[lightType].specular.g -= 0.003f;
		}
	}
	//Blue plus
	if(GetAsyncKeyState('P') & 0x8000)
	{
		if(lights[lightType].specular.b < 1.0f)
		{
			lights[lightType].specular.b += 0.003f;
		}
	}
	//Blue negative
	if(GetAsyncKeyState(':') & 0x8000)
	{
		if(lights[lightType].specular.b > 0.0f )
		{
			lights[lightType].specular.b -= 0.003f;
		}
	}
}

//load textures
VOID D3DApplication:: loadResources()
{
	D3DX10CreateShaderResourceViewFromFile(m_pd3dDevice, "face.jpg", 0, 0, &mTextureRV, 0);
	D3DX10CreateShaderResourceViewFromFile(m_pd3dDevice, "face_normalmap_256x256.bmp", 0, 0, &mTextureBump, 0);
	D3DX10CreateShaderResourceViewFromFile(m_pd3dDevice, "face_SPEC.png", 0, 0, &mTextureSpec, 0);
}