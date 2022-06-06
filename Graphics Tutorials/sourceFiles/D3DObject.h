/*
 * File name:	D3DObject.h
 * Description:	This is a header file for D3DObject.cpp
 * Author: 		Alex J Davison
 */

#ifndef _D3D_OBJECT_H
#define _D3D_OBJECT_H

#include		<windows.h>

//For DirectX Graphics 10
#include		<d3dx10.h>

// Vertex structure: contains vertex position, surface tangent, normal and texture cordinates
struct SimpleVertex
{   
    D3DXVECTOR3 pos;
	D3DXVECTOR3 surTan;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texC;
};

class D3DObject
{

	//Variables
	private: 

		//Translation vector
		D3DXVECTOR3		tran;
		//Scale vector
		D3DXVECTOR3		scal;
		//Rototation vector
		D3DXVECTOR3		rot;
		//World matrix
		D3DXMATRIXA16	m_matWorld;		
		//World matrix rotation in X
		D3DXMATRIXA16	m_matRotX;
		//World matrix rotation in Y
		D3DXMATRIXA16	m_matRotY;
		//World matrix rotation in Z
		D3DXMATRIXA16	m_matRotZ;
		//World matrix for tranlation
		D3DXMATRIXA16	m_matTran;
		//World matrix for scaling
		D3DXMATRIXA16	m_matScal;		
		//D3D10 API objects used by the application
		//Pointer to rendering device object
		ID3D10Device*	m_pd3dDevice;	
		
		ID3D10EffectTechnique*	m_pTechnique; 

		ID3D10InputLayout*		m_pVertexLayout;
		//Pointer to vertex buffer object
		ID3D10Buffer*			m_pVertexBuffer;			
		
		ID3D10Buffer*           m_pIndexBuffer;
		
		D3D10_TECHNIQUE_DESC	techDesc;
		
		D3DXVECTOR3 u;
		
		D3DXVECTOR3 v;

    //Functions
    public: 
		
		// Constructor and destructor
		D3DObject();
		~D3DObject();

		//Release D3D objects
		VOID relD3DObjects();
		//Passes the object through the graphics pipeline, and draws it
		VOID render();							 
		//Set the rendering device
		VOID setRenderingDev(ID3D10Device* pd3dDevice); 	

		VOID setTechnique(ID3D10EffectTechnique* pTechnique);		
		//Set the trainslation of the object, where the position of the object is in the world
		VOID setTranslation(float x, float y, float z);
		//Set the rotation of the object
		VOID setRotation(float x, float y, float z);
		//Set the scale of the object
		VOID setScaling(float x, float y, float z);
		//Sets the world transformation matrix
		VOID setWorldMatrix();

		//Initialises the object's mesh
		HRESULT initialiseMesh();				
		// Gets world matrix
		D3DXMATRIXA16 getWorldMat();			
		//Get the translation of the object				
		D3DXVECTOR3 getTranslation();
		//Get the rotation of the object
		D3DXVECTOR3 getRotation();
		//Get the scale of the object
		D3DXVECTOR3 getScaling();					

	private:

		//Creates a cube
		HRESULT createCube();
		
	};

#endif /* _D3D_OBJECT_H */




