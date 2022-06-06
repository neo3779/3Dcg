//Include lighting to work out the lighting
#include "lighthelper.fx"
 
//Textures
Texture2D gDiffuseMap;
Texture2D gSpecMap;
Texture2D gNormalMap;
TextureCube gCubeMap;

//Texture sampling information
SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

//Light and camera information
cbuffer cbPerFrame
{
	Light gLight;
	float3 gEyePosW;
};

//Object information
cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWVP; 
	float4x4 gTexMtx;
	float4 gReflectMtrl;
};

//Vector in information
struct VS_IN
{
	float3 posL     : POSITION;
	float3 tangentL : TANGENT;
	float3 normalL  : NORMAL;
	float2 texC     : TEXCOORD;
};

//Vector out information
struct VS_OUT
{
	float4 posH     : SV_POSITION;
    float3 posW     : POSITION;
    float3 tangentW : TANGENT;
    float3 normalW  : NORMAL;
    float2 texC     : TEXCOORD;
};

// Geometry Shader
[MaxVertexCount(3)] 
void GS_T6P1(triangle VS_OUT inVert[3], inout TriangleStream<VS_OUT> output) 
{ 
	//Calculate triangle normal using cross product of triangle edges in world space 
	float3 wTrNormal = cross((inVert[1].posW - inVert[0].posW), (inVert[2].posW - inVert[0].posW)); 

 	//Calculate eye vector
	float4 eyeVect = float4(gEyePosW,1.0f) - float4(inVert[0].posW, 1.0f);
	
	//If dot product of eye vector and triangle normal is not positive, cull triangle (i.e. do not output it towards rasterizer) 
	if (dot(eyeVect.xyz, wTrNormal) <= 0.0f) 
		return; 
	else 
	{ 
		//Output front-facing triangle 
		VS_OUT outVert = (VS_OUT)0; 
		//Output each vertex of triangle 
		for (int i = 0; i < 3; i++) 
		{
			//Ouput device-space vertex position and world-space vertex normal 
			outVert.posH = inVert[i].posH; 
			outVert.posW = inVert[i].posW; 
			outVert.normalW = inVert[i].normalW; 
			outVert.texC = inVert[i].texC;
			outVert.tangentW = inVert[i].tangentW;
			//Send triangle vertices to list which will be sent for interpolation by rasterizer  
			output.Append(outVert);
		} 
	} 
} 

//Vertex shader
VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	
	//Transform to world space space.
	vOut.posW     = mul(float4(vIn.posL, 1.0f), gWorld);
	vOut.tangentW = mul(float4(vIn.tangentL, 0.0f), gWorld);
	vOut.normalW  = mul(float4(vIn.normalL, 0.0f), gWorld);
		
	//Transform to homogeneous clip space.
	vOut.posH = mul(float4(vIn.posL, 1.0f), gWVP);
	
	//Output vertex attributes for interpolation across triangle.
	vOut.texC = mul(float4(vIn.texC, 0.0f, 1.0f), gTexMtx);
	
	return vOut;
}

//Pixel shader
float4 PS(VS_OUT pIn) : SV_Target
{
	//Sample for the pixel for the pixel colour
	float4 diffuse = gDiffuseMap.Sample( gTriLinearSam, pIn.texC );
	//Cancel transparent pixels
	clip(diffuse.a - 0.15f);

	//Sample for the pixel specular value
	float4 spec    = gSpecMap.Sample( gTriLinearSam, pIn.texC );

	//Sample for the pixel normal value
	float3 normalT = gNormalMap.Sample( gTriLinearSam, pIn.texC );
	
	//Convert map for 0 to 1 to 0 to 256
	spec.a *= 256.0f;
	
	//Uncompress pixel normal
	normalT = 2.0f*normalT - 1.0f;
	
	//Create the orthonormal basis normal, tangent and binormal
	float3 N = normalize(pIn.normalW);
	float3 T = normalize(pIn.tangentW - dot(pIn.tangentW, N)*N);
	float3 B = cross(N,T);
	
	//Create the orthonormal matrix
	float3x3 TBN = float3x3(T, B, N);
	
	//Convert transform from tangent space to world space.
	float3 bumpedNormalW = normalize(mul(normalT, TBN));
    
	//Compute the color for this pixel.
    SurfaceInfo v = {pIn.posW, bumpedNormalW, diffuse, spec};
	float3 litColor = PointLight(v, gLight, gEyePosW);
	
    
	//Return pixel colour
    return float4(litColor, diffuse.a);
}

//Rasterizer State
RasterizerState rasterizerState { FillMode = SOLID; CullMode = BACK;};

//Technique
technique10 NormalMapTech
{
    pass P0
    {
		SetRasterizerState(rasterizerState);
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GS_T6P1() ) );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
		
	}
}

