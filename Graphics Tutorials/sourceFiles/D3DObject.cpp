/*
* File name:	D3DObject.cpp
* Description:	The D3DObject class implements a 3D graphics object. 
* Author: 		Alex J Davison
*/

#include		"D3DObject.h"

//Construct and initialise a graphics object
D3DObject::D3DObject()
{
	m_pVertexLayout = NULL;
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	rot.x = 0.0f,rot.y = 0.0f,rot.x = 0.0f;
	tran.x = 0.0f, tran.y = 0.0f, tran.x = 0.0f;
	scal.x = 1.0f, scal.y= 1.0f, scal.z = 1.0f;
}

//Deconstructor
D3DObject::~D3DObject()
{
	relD3DObjects();
	return;	
}

//Release D3D objects
VOID D3DObject::relD3DObjects()
{
	//Release vertex buffer object
	if( m_pVertexBuffer )	{ m_pVertexBuffer->Release(); m_pVertexBuffer = NULL;}
	if( m_pIndexBuffer )	{m_pIndexBuffer->Release();  m_pIndexBuffer = NULL;}
	if( m_pVertexLayout )	{m_pVertexLayout->Release(); m_pVertexLayout = NULL;}
}

//Creates a cube
HRESULT D3DObject::createCube()
{
	HRESULT hr = S_OK;

	// Define the input layout
	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0 }
			
	};

	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

	//Create the input layout
	D3D10_PASS_DESC PassDesc;

	m_pTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	hr = m_pd3dDevice->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pVertexLayout );
	if( FAILED( hr ) )
		return hr;

	//Set the input layout
	m_pd3dDevice->IASetInputLayout( m_pVertexLayout );

	//Create vertex buffer
	SimpleVertex vertices[] =
	{	
		//Fill in the front face vertex data.
		{ D3DXVECTOR3( -0.5f, -0.5f, -0.5f), D3DXVECTOR3( 1.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 1.0f)},
		{ D3DXVECTOR3( -0.5f,  0.5f, -0.5f), D3DXVECTOR3( 1.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 0.0f)},
		{ D3DXVECTOR3( 0.5f,  0.5f, -0.5f), D3DXVECTOR3( 1.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 0.0f)},
		{ D3DXVECTOR3( 0.5f, -0.5f, -0.5f), D3DXVECTOR3( 1.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, -1.0f), D3DXVECTOR2( 1.0f, 1.0f)},

		//Fill in the back face vertex data.
		{ D3DXVECTOR3( -0.5f, -0.5f, 0.5f), D3DXVECTOR3( -1.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, 1.0f), D3DXVECTOR2( 1.0f, 1.0f)},
		{ D3DXVECTOR3(  0.5f, -0.5f, 0.5f), D3DXVECTOR3( -1.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, 1.0f), D3DXVECTOR2( 0.0f, 1.0f)},
		{ D3DXVECTOR3(  0.5f,  0.5f, 0.5f), D3DXVECTOR3( -1.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, 1.0f), D3DXVECTOR2( 0.0f, 0.0f)},
		{ D3DXVECTOR3( -0.5f,  0.5f, 0.5f), D3DXVECTOR3( -1.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, 1.0f), D3DXVECTOR2( 1.0f, 0.0f)},

		//Fill in the top face vertex data.
		{ D3DXVECTOR3( -0.5f, 0.5f, -0.5f), D3DXVECTOR3( 1.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 1.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f)},
		{ D3DXVECTOR3( -0.5f, 0.5f,  0.5f), D3DXVECTOR3( 1.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 1.0f, 0.0f), D3DXVECTOR2( 0.0f, 0.0f)},
		{ D3DXVECTOR3(  0.5f, 0.5f,  0.5f), D3DXVECTOR3( 1.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 1.0f, 0.0f), D3DXVECTOR2( 1.0f, 0.0f)},
		{ D3DXVECTOR3(  0.5f, 0.5f, -0.5f), D3DXVECTOR3( 1.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 1.0f, 0.0f), D3DXVECTOR2( 1.0f, 1.0f)},

		//Fill in the bottom face vertex data.
		{ D3DXVECTOR3( -0.5f, -0.5f, -0.5f), D3DXVECTOR3( -1.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, -1.0f, 0.0f), D3DXVECTOR2( 1.0f, 1.0f)},
		{ D3DXVECTOR3( 0.5f, -0.5f, -0.5f), D3DXVECTOR3( -1.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, -1.0f, 0.0f), D3DXVECTOR2( 0.0f, 1.0f)},
		{ D3DXVECTOR3(  0.5f, -0.5f,  0.5f), D3DXVECTOR3( -1.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, -1.0f, 0.0f), D3DXVECTOR2( 0.0f, 0.0f)},
		{ D3DXVECTOR3( -0.5f, -0.5f,  0.5f), D3DXVECTOR3( -1.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, -1.0f, 0.0f), D3DXVECTOR2( 1.0f, 0.0f)},

		//Fill in the left face vertex data.
		{ D3DXVECTOR3( -0.5f, -0.5f,  0.5f), D3DXVECTOR3( 0.0f, 0.0f, -1.0f), D3DXVECTOR3( -1.0f, 0.0f, 0.0f), D3DXVECTOR2( 0.0f, 1.0f)},
		{ D3DXVECTOR3( -0.5f,  0.5f,  0.5f), D3DXVECTOR3( 0.0f, 0.0f, -1.0f), D3DXVECTOR3( -1.0f, 0.0f, 0.0f), D3DXVECTOR2( 0.0f, 0.0f)},
		{ D3DXVECTOR3( -0.5f,  0.5f, -0.5f), D3DXVECTOR3( 0.0f, 0.0f, -1.0f), D3DXVECTOR3( -1.0f, 0.0f, 0.0f), D3DXVECTOR2( 1.0f, 0.0f)},
		{ D3DXVECTOR3( -0.5f, -0.5f, -0.5f), D3DXVECTOR3( 0.0f, 0.0f, -1.0f), D3DXVECTOR3( -1.0f, 0.0f, 0.0f), D3DXVECTOR2( 1.0f, 1.0f)},

		//Fill in the right face vertex data.
		{ D3DXVECTOR3(  0.5f, -0.5f, -0.5f), D3DXVECTOR3( 0.0f, 0.0f, 1.0f), D3DXVECTOR3( 1.0f, 0.0f, 0.0f), D3DXVECTOR2( 0.0f, 1.0f)},
		{ D3DXVECTOR3(  0.5f,  0.5f, -0.5f), D3DXVECTOR3( 0.0f, 0.0f, 1.0f), D3DXVECTOR3( 1.0f, 0.0f, 0.0f), D3DXVECTOR2( 0.0f, 0.0f)},
		{ D3DXVECTOR3(  0.5f,  0.5f,  0.5f), D3DXVECTOR3( 0.0f, 0.0f, 1.0f), D3DXVECTOR3( 1.0f, 0.0f, 0.0f), D3DXVECTOR2( 1.0f, 0.0f)},
		{ D3DXVECTOR3(  0.5f, -0.5f,  0.5f), D3DXVECTOR3( 0.0f, 0.0f, 1.0f), D3DXVECTOR3( 1.0f, 0.0f, 0.0f), D3DXVECTOR2( 1.0f, 1.0f)}
	};

	// Create index buffer
	DWORD indices[] =
	{
		0,1,2,
		0,2,3,

		4,5,6,
		4,6,7,

		8,9,10,
		8,10,11,

		12,13,14,
		12,14,15,

		16,17,18,
		16,18,19,

		20,21,22,
		20,22,23,
	};
		
	D3D10_BUFFER_DESC buffDesc;
	buffDesc.Usage			= D3D10_USAGE_DEFAULT;
	buffDesc.ByteWidth		= sizeof( SimpleVertex ) * 24;
	buffDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.MiscFlags		= 0;
	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem		= vertices;
	hr = m_pd3dDevice->CreateBuffer( &buffDesc, &InitData, &m_pVertexBuffer );
	if( FAILED( hr ) )
		return hr;

	// Set vertex buffer
	UINT stride = sizeof( SimpleVertex );
	UINT offset = 0;
	m_pd3dDevice->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );


	buffDesc.Usage			= D3D10_USAGE_DEFAULT;
	// 36 vertices needed for 12 triangles in a triangle list
	buffDesc.ByteWidth		= sizeof( DWORD ) * 36;        
	buffDesc.BindFlags		= D3D10_BIND_INDEX_BUFFER;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.MiscFlags		= 0;
	InitData.pSysMem		= indices;
	hr = m_pd3dDevice->CreateBuffer( &buffDesc, &InitData, &m_pIndexBuffer );
	if( FAILED( hr ) )
		return hr;

	// Set index buffer
	m_pd3dDevice->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	// Set primitive topology
	m_pd3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	return S_OK;
}

//Initialises the mesh of the object 
HRESULT D3DObject::initialiseMesh()
{
	return createCube();
}

//Set the rendering device 
VOID D3DObject::setRenderingDev(ID3D10Device* pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
}

//Set the technique 
VOID D3DObject::setTechnique(ID3D10EffectTechnique* pTechnique)
{
	m_pTechnique = pTechnique;
}

//Set transformation pipeline and pass world through grahics pipeline as approriate
VOID D3DObject::setWorldMatrix()
{
	// Set / update world matrix. 
	// Parameters:
	// . orientation set as time-varying angle about the world y-axis.
	// . position defaults to zero units along the world x, y, and z axes.
	// . scale defaults to identity in the x, y, and z axes of world.

	
	//Reset matrixies to identy
	D3DXMatrixIdentity(&m_matScal);
	D3DXMatrixIdentity(&m_matRotX);
	D3DXMatrixIdentity(&m_matRotY);
	D3DXMatrixIdentity(&m_matRotZ);
	D3DXMatrixIdentity(&m_matTran);
	D3DXMatrixIdentity(&m_matWorld);
	//Set scale vector
	D3DXMatrixScaling(&m_matScal,scal.x,scal.y,scal.z);
	//Set rotation vector
	D3DXMatrixRotationX( &m_matRotX, rot.x);
	D3DXMatrixRotationY( &m_matRotY, rot.y);
	D3DXMatrixRotationZ( &m_matRotZ, rot.z);
	//Set translation vector
	D3DXMatrixTranslation(&m_matTran,tran.x,tran.y,tran.z);
	//Set the rotation within the world matrix		
	D3DXMatrixMultiply(&m_matWorld,&m_matWorld,&m_matRotX);
	D3DXMatrixMultiply(&m_matWorld,&m_matWorld,&m_matRotY);
	D3DXMatrixMultiply(&m_matWorld,&m_matWorld,&m_matRotZ);
	//Set the translation of the world matix
	D3DXMatrixMultiply(&m_matWorld,&m_matWorld,&m_matTran);
	//Set the scale matrix for the world
	D3DXMatrixMultiply(&m_matWorld,&m_matWorld,&m_matScal);
		
	// Alternative setting
	// Set / update world matrix. 
	// Parameters:
	// . orientation set as 30 degrees about the world y-axis.
	// . position defaults to zero units along the world x, y, and z axes.
	// . scale defaults to identity in the x, y, and z axes of world.
	//D3DXMatrixRotationY( &m_matWorld, D3DXToRadian(30.0f) );
	

}

//Set the scale of the object
VOID D3DObject::setScaling(float x,float y, float z)
{
	scal.x = x;
	scal.y = y;
	scal.z = z;
}

//Set the translation of the object
VOID D3DObject::setTranslation(float x,float y, float z)
{	
	tran.x = x;
	tran.y = y;
	tran.z = z;
}

//Set the rotation of the object
VOID D3DObject::setRotation(float x,float y, float z)
{
	rot.x = x;
	rot.y = y;
	rot.z = z;
}

//Get the scale of the object
D3DXVECTOR3  D3DObject::getScaling()
{
	return scal;
}

//Get the translation of the object
D3DXVECTOR3  D3DObject::getTranslation()
{
	return tran;
}

//Get the rotation of the object
D3DXVECTOR3  D3DObject::getRotation()
{
	return rot;
}

//Gets the view transformation matrix.
D3DXMATRIXA16 D3DObject::getWorldMat()
{
	return m_matWorld;
}

//Passes the object through the graphics pipeline, and draws it.
VOID D3DObject::render()
{
	// Renders the object
	m_pTechnique->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; ++p )
	{
		m_pTechnique->GetPassByIndex( p )->Apply( 0 );
		//36 vertices needed for 12 triangles in a triangle list
		m_pd3dDevice->DrawIndexed( 36, 0, 0 ); 
	}
}