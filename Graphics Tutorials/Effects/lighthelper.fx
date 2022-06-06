//Light information
struct Light
{
	//Position of the light
	float3 pos;
	//Direction of the light
	float3 dir;
	//Ambient light colour
	float4 ambient;
	//Diffused light  colour
	float4 diffuse;
	//Specular light colour
	float4 spec;
	//Attenuation of spec light
	float3 att;
	//Specular light power
	float  spotPower;
	//Range of the light
	float  range;
};

//Surface information
struct SurfaceInfo
{
	//Position of surface
	float3 pos;
    //Normal of surface
	float3 normal;
	//Diffuse light colour
    float4 diffuse;
	//Specular light colour
    float4 spec;
};

//Calucation for a parllel light
float3 ParallelLight(SurfaceInfo v, Light L, float3 eyePos)
{
	//Create pix colour
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
 
	//Calucate light vector which aims the opposite direction the light rays travel
	float3 lightVec = -L.dir;
	
	//Add the ambient lighting to colour
	litColor += v.diffuse * L.ambient;	
	
	//Add diffuse and specular term, provided to the surface that is in line of site of the light
	float diffuseFactor = dot(lightVec, v.normal);

	//Chech that the diffuse factor is greater than zero
	[branch]
	if( diffuseFactor > 0.0f )
	{
		//Get max specular power
		float specPower   = max(v.spec.a, 1.0f);
		//Work out the vector from eye to surface
		float3 toEye      = normalize(eyePos - v.pos);
		//Work out refecletive value
		float3 reflectVec = reflect(-lightVec, v.normal);
		//Work out specfator
		float specFactor  = pow(max(dot(reflectVec, toEye), 0.0f), specPower);
					
		//Work out the diffuse and specular
		litColor += diffuseFactor * v.diffuse * L.diffuse;
		litColor += specFactor * v.spec * L.spec;
	}
	
	//Return colour
	return litColor;
}

//Calucation for a point light
float3 PointLight(SurfaceInfo v, Light L, float3 eyePos)
{
	//Create pix colour
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	
	//Calucation for the vector from the surface to the light.
	float3 lightVec = L.pos - v.pos;
		
	//The distance from surface to light.
	float d = length(lightVec);
	
	//If distance is out of the range of the light the return a black pixel
	if( d > L.range )
		return float3(0.0f, 0.0f, 0.0f);
		
	//Normalize the light vector.
	lightVec /= d; 
	
	//Add the ambient light to the diffused
	litColor += v.diffuse * L.ambient;	
	
	//Add diffuse and specular term, provided to the surface that is in line of site of the light
	float diffuseFactor = dot(lightVec, v.normal);

	//Chech that the diffuse factor is greater than zero
	[branch]
	if( diffuseFactor > 0.0f )
	{
		//Get max specular power
		float specPower   = max(v.spec.a, 1.0f);
		//Work out the vector from eye to surface
		float3 toEye      = normalize(eyePos - v.pos);
		//Work out refecletive value
		float3 reflectVec = reflect(-lightVec, v.normal);
		//Work out specfator
		float specFactor  = pow(max(dot(reflectVec, toEye), 0.0f), specPower);
	
		//Work out the diffuse and specular
		litColor += diffuseFactor * v.diffuse * L.diffuse;
		litColor += specFactor * v.spec * L.spec;
	}
	
	//Return colour with attenuate
	return litColor / dot(L.att, float3(1.0f, d, d*d));
}

//Calulcation for a spotlight
float3 Spotlight(SurfaceInfo v, Light L, float3 eyePos)
{
	//Create pix colour
	float3 litColor = PointLight(v, L, eyePos);
	
	//Caluculate the vector from the surface to the light
	float3 lightVec = normalize(L.pos - v.pos);
	
	//Calulcat the specular light
	float s = pow(max(dot(-lightVec, L.dir), 0.0f), L.spotPower);
	
	//Scale color by spotlight factor.
	return litColor*s;
}