/*
 * File name:	Light.h
 * Description:	This is a header file for a light
 * Author: 		Alex J Davison
 */

#ifndef LIGHT_H
#define LIGHT_H

#include "d3dUtil.h"

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}

	//Position
	D3DXVECTOR3 position;
	//Padding float that isn't used
	float pad1;      
	//Direction
	D3DXVECTOR3 direction;
	//Padding float that isn't used
	float pad2;  
	//Ambient colour
	D3DXCOLOR ambient;
	//Diffused colour
	D3DXCOLOR diffuse;
	//Specular colour
	D3DXCOLOR specular;
	//Attenuation
	D3DXVECTOR3 attenuation;
	//Specular power
	float spotPow;
	//Range
	float range;
};

struct Material
{
	Material()
	{
		ZeroMemory(this, sizeof(Material));
	}

	//Ambient colour absorption
	D3DXCOLOR ambient;
	//Diffuse colour absorption
	D3DXCOLOR diffuse;
	//Specular colour absorption
	D3DXCOLOR specular;
	//Specular power of material
	float specPower;
};

#endif /* TIMER_H */