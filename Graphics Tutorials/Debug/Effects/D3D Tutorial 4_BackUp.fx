//--------------------------------------------------------------------------------------
// File: Tutorial04.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
matrix World;
matrix View;
matrix Projection;
matrix WorldInvTrans;

//Eye pos
float3 gEyePosW;

//Light pos
float3 gLightVecW;

//Diffused lighting
float4 gDiffuseMtrl;
float4 gDiffuseLight;

//Ambient lighting
float4 gAmbientMtrl;
float4 gAmbientLight;

//Specular light
float4 gSpecularMtrl;
float4 gSpecularLight;
float  gSpecularPower;

//Texture
Texture2D gTexture;
Texture2D gTextureSpec;
Texture2D gTextureBump;

SamplerState gAnisoSampler
{
	Filter = ANISOTROPIC;
};

SamplerState gBumpSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Clamp; 
    AddressV = Clamp;
};

struct VS_IN
{
	float4 PosL    : POSITION;
	float3 TangentL  : TANGENT;
	float3 NormalL : NORMAL;
	float2 TexC	   : TEXCOORD;
};

//--------------------------------------------------------------------------------------
struct VS_OUT
{
	float4 Pos : SV_POSITION;
    float3 PosW : POSITION;
    float3 Normal  : NORMAL0;
	float3 TangentW : TANGENT0;
	float2 TexC : TEXCOORD;
	
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUT VS(VS_IN vIn)
{
    VS_OUT output = (VS_OUT)0;

	output.TangentW = vIn.TangentL;

    output.PosW = mul( vIn.PosL, World );
	output.Normal = mul(float4(vIn.NormalL, 0.0f), WorldInvTrans).xyz;
	output.Normal = normalize(output.Normal);

	output.Pos = mul( vIn.PosL, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );

	output.TexC = vIn.TexC;
	
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUT pIn) : SV_Target
{
	float4 Colour;

	float4 norm = gTextureBump.Sample(gBumpSampler,pIn.TexC);
	norm.xyz = (norm.xyz *2) -1;

	float3 N = normalize(pIn.Normal);
	float3 T = normalize(pIn.TangentW - dot(pIn.TangentW,N)*N);
	float3 B = cross (N,T);

	float3x3 tangentSpace = float3x3(T,B,N);

	float3 bumpedNormalW = normalize (mul(norm,tangentSpace));

	float lightVec = normalize(gLightVecW - pIn.PosW);
	lightVec = mul(lightVec,tangentSpace);

	float cameraVec = normalize(gEyePosW - pIn.PosW);
	cameraVec = mul(cameraVec,tangentSpace);

	
	// Compute Colour
	// Compute the reflection vector.
	float3 r = reflect(-lightVec, norm);

	// Determine how much (if any) specular light makes it
	// into the eye.
	float t = pow(max(dot(r, cameraVec),0.0f),gSpecularPower);

	// Determine the diffuse light intensity that strikes the vertex.
	float s = max(dot(lightVec, norm), 0.0f);


	float4 specMtrl = gTextureSpec.Sample(gAnisoSampler, pIn.TexC);
	float4 diffMtrl = gTexture.Sample(gAnisoSampler, pIn.TexC);

	// Compute the ambient, diffuse, and specular terms separately.
	float3 spec = t *(specMtrl * gSpecularLight).rgb;
	float3 diffuse = s * (diffMtrl * gDiffuseLight).rgb;
	float3 ambient = diffMtrl * gAmbientLight;
	Colour.rgb = saturate(ambient + diffuse + spec);

	// Sum all the terms together and copy over the diffuse alpha
	Colour.a = saturate(gDiffuseMtrl.a);
	
	
	// Compute Colour
	return Colour;
}

//--------------------------------------------------------------------------------------
// Geometry Shader
//--------------------------------------------------------------------------------------

[MaxVertexCount(3)] 
void GS_T6P1(triangle VS_OUT inVert[3], inout TriangleStream<VS_OUT> output) 
{ 

	// Calculate triangle normal using cross product of triangle edges in world space 
	float3 wTrNormal = cross((inVert[1].PosW - inVert[0].PosW), (inVert[2].PosW - inVert[0].PosW)); 

 	// Calculate eye vector
	float4 eyeVect = float4(gEyePosW,1.0f) - float4(inVert[0].PosW, 1.0f);
	
	// if dot product of eye vector and triangle normal is not positive, 
	// cull triangle (i.e. do not output it towards rasterizer) 
	if (dot(eyeVect.xyz, wTrNormal) <= 0.0f) 
		return; 
	else 
	{ 
		// output front-facing triangle 
		VS_OUT outVert = (VS_OUT)0; 
		// output each vertex of triangle 
		for (int i = 0; i < 3; i++) 
		{
			// Ouput device-space vertex position and world-space vertex normal 
			outVert.Pos = inVert[i].Pos; 
			outVert.PosW = inVert[i].PosW; 
			outVert.Normal = inVert[i].Normal; 
			outVert.TexC = inVert[i].TexC;
			outVert.TangentW = inVert[i].TangentW;
			// Send triangle vertices to list which will be sent for interpolation by rasterizer  
			output.Append(outVert);
		} 
	} 
} 

//--------------------------------------------------------------------------------------
RasterizerState rasterizerState { FillMode = SOLID;
								CullMode = BACK;};

technique10 Render
{
    pass P0
    {
		SetRasterizerState(rasterizerState);
		SetGeometryShader( CompileShader( gs_4_0, GS_T6P1() ) );
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
				
    }
}

