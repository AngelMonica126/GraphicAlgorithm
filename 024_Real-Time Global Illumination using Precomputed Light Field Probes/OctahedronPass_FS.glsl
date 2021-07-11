#version 430 core
#extension GL_NV_shader_atomic_float : require
#extension GL_NV_shader_atomic_fp16_vector : require
#extension GL_NV_gpu_shader5 : require
in  vec2 v2f_TexCoords;
layout(rgba16f ,binding = 0) uniform writeonly image2DArray u_OutputOctRadianceImage;
layout(rgba16f ,binding = 1) uniform writeonly image2DArray u_OutputOctNormalImage;
layout(rgba16f ,binding = 2) uniform writeonly  image2DArray u_OutputOctChebyshevsImage;
layout(rgba16f ,binding = 3) uniform writeonly image2DArray u_OutputOctIrradianceImage;

uniform samplerCube u_BakeRadianceTextures;
uniform samplerCube u_BakeNormalTextures;
uniform samplerCube u_BakeChebyshevsTextures;
uniform int u_Index;
uniform int u_BakeResolution;
uniform float u_Weight[9] = float[] (0.0947416,0.118318,0.0947416,0.118318,0.147761,0.118318,0.0947416,0.118318,0.0947416);


int getIndex(vec3 vDir)
{
	float ax = abs(vDir.x);
	float ay = abs(vDir.y);
	float az = abs(vDir.z);
	if (ax >= ay && ax >= az)	// x face
		return vDir.x >= 0 ? 0 : 1;
	else if (ay >= az)	// y face
		return vDir.y >= 0 ? 2 : 3;
	else // z face
		return vDir.z >= 0 ? 4 : 5;
}

vec3 filpDir(vec3 vDir)
{
	int Index = getIndex(vDir);
	switch(Index)
	{
		case 0:
			vDir.y *= -1;
			return vDir;
		case 1:
			vDir.y *= -1;
			return vDir;
		case 2:
			vDir.x *= -1;
			return vDir;
		case 3:
			vDir.x *= -1;
			return vDir;
		case 4:
			vDir.xy *= -1;
			return vDir;
		case 5:
			vDir.xy *= -1;
			return vDir;

	}
	return vec3(0);
}

float signNotZero(float f)
{
	return (f >= 0.0) ? 1.0:-1.0;
}

vec2 signNotZero(vec2 v)
{
	return vec2(signNotZero(v.x),signNotZero(v.y));
}
vec3 octDecode(vec2 vTextCoord)
{
	vec3 Direction = vec3(vTextCoord.x, vTextCoord.y, 1.0 - abs(vTextCoord.x) - abs(vTextCoord.y));
    if (Direction.z < 0.0) {
        Direction.xy = (1.0 - abs(Direction.yx)) * signNotZero(Direction.xy);
    }
    return normalize(Direction);
}
vec3 CubeUV2XYZ(vec2 vUV,int vIndex)
{
	float u = vUV.x * 2.f - 1.f;
	float v = vUV.y * 2.f - 1.f;
	switch (vIndex)
	{
	case 0: return vec3( 1,  v, -u ); 	// +x
	case 1: return vec3( -1,  v,  u); 	// -x
	case 2: return vec3( u,  1, -v );  // +y
	case 3: return vec3( u, -1,  v );	// -y
	case 4: return vec3( u,  v,  1 );  // +z
	case 5: return vec3( -u,  v, -1);;	// -z
	}
	return vec3(0);
}
vec3 getIrradince(vec3 vDir)
{
	vec3 irradiance = vec3(0);
	for(int x = 0; x < u_BakeResolution; x++)
	{
		for(int y = 0; y < u_BakeResolution; y++)
		{
			for(int k = 0; k < 6; k++)
			{
				vec3 w = normalize(CubeUV2XYZ(vec2(x,y),k));
				w = filpDir(w);
				irradiance +=  max(dot(w,vDir),0) * texture(u_BakeRadianceTextures,w).xyz;
			}
		}
	}

	return irradiance * 4.0f  / u_BakeResolution / u_BakeResolution / 6;
}

ivec2 XYZ2CubeUV(vec3 vDir)
{
	float ax = abs(vDir.x);
	float ay = abs(vDir.y);
	float az = abs(vDir.z);
	vec2 c;
	if (ax >= ay && ax >= az)	// x face
	{
		c = vec2(-vDir.z / vDir.x, vDir.y / ax);
	}
	else if (ay >= az)	// y face
	{
		c = vec2(vDir.x / ay, -vDir.z / vDir.y);
	}
	else
	{
		c = vec2(vDir.x / vDir.z, vDir.y / az);
	}
	c.x = (c.x * 0.5f + 0.5f) * u_BakeResolution;
	c.y = (c.y * 0.5f + 0.5f) * u_BakeResolution;
	return ivec2(c);
}

vec3 getFilterChebyshevs(ivec2 FragPos)
{
	vec3 Dir = octDecode(FragPos  * 2.0f / u_BakeResolution - 1.0);
	Dir = filpDir(Dir);
	float Distance = texture(u_BakeChebyshevsTextures,Dir).z;
	ivec2 Pos = FragPos;
	int BeginX = max(0,Pos.x - 1);
	int BeginY = max(0,Pos.y - 1);
	int EndX = min(u_BakeResolution,Pos.x + 1);
	int EndY = min(u_BakeResolution,Pos.y + 1);
	vec2 Result = vec2(0);
	int WeightIndex = 0;
	for(int x = BeginX; x <= EndX; x++)
	{
		for(int y = BeginY; y <= EndY; y++)
		{
			ivec2 Texcoord = ivec2(x,y);
			vec3 Dir = octDecode(Texcoord  * 2.0f / u_BakeResolution - 1.0);
			Dir = filpDir(Dir);
			Result += texture(u_BakeChebyshevsTextures,Dir).xy * u_Weight[WeightIndex++];
		}
	}
	return vec3(Result,Distance);
}

void main()
{
	ivec2 FragPos = ivec2(v2f_TexCoords * u_BakeResolution);
	vec3 Dir = octDecode(v2f_TexCoords * 2.0 - 1.0);
	Dir = filpDir(Dir);
	imageStore(u_OutputOctRadianceImage, ivec3(FragPos,u_Index), vec4(texture(u_BakeRadianceTextures,Dir).xyz, 1.0));
	imageStore(u_OutputOctNormalImage, ivec3(FragPos,u_Index), vec4(texture(u_BakeNormalTextures,Dir).xyz, 1.0));
	imageStore(u_OutputOctChebyshevsImage, ivec3(FragPos,u_Index), vec4(getFilterChebyshevs(FragPos),0));
	imageStore(u_OutputOctIrradianceImage, ivec3(FragPos,u_Index), vec4(getIrradince(Dir), 1.0));
}