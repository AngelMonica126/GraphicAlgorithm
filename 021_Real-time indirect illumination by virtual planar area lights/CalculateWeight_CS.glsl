#version 430 core
#pragma optionNV (unroll all)

#define LOCAL_GROUP_SIZE 16
#define PI 3.1415926

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;

layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputImage;
struct SPlane	//newnew
{
	vec4 P0;
	vec4 P1;
	vec4 P2;
	vec4 P3;
	vec4 Normal;
	vec4 ProportionAndUV;
	ivec4 Index;
};
layout (std430, binding = 2) buffer SVALs
{
	SPlane u_VALs[];
};
layout (std430, binding = 3) buffer SVALCount
{
	int u_VALCount;
};
//layout (std430, binding = 1) buffer SPlanes
//{
//	SPlane u_Planes[];
//};
//uniform int u_VALCount;
uniform int u_Block;
uniform sampler2D u_RSMPositionTexture;
uniform sampler2D u_RSMAlbedoAndMetallicTexture;
uniform sampler2D u_RSMNormalTexture;
uniform int u_RSMResolution; 


void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	vec4 Albedo = texelFetch(u_RSMAlbedoAndMetallicTexture, FragPos, 0);
	vec3 Normal = texelFetch(u_RSMNormalTexture, FragPos, 0).xyz;
	vec3 Position = texelFetch(u_RSMPositionTexture, FragPos, 0).xyz;
	float weight = 0;
	for(int i = 0; i < u_VALCount; i++)
	{
		vec2 beginUV = u_VALs[i].ProportionAndUV.zw * u_RSMResolution;
		vec2 endUV = (u_VALs[i].ProportionAndUV.zw + u_VALs[i].ProportionAndUV.xy) * u_RSMResolution;
		if(FragPos.x >= beginUV.x && FragPos.x < endUV.x && FragPos.y >=beginUV.y && FragPos.y < endUV.y)
		{
			float angle =  acos(clamp(dot(u_VALs[i].Normal.xyz,normalize(Normal)),-0.999,0.999));
			if(angle < PI / 2.0)
				Albedo *= (1.0 - 0.5 * (1 - cos(angle) * cos(angle)));
			else
				Albedo *= (1.0 - 0.5 * (1 + cos(angle) * cos(angle)));
			
			break;
		}
	}
//	Albedo *= weight;
	imageStore(u_OutputImage, FragPos, vec4(Albedo.xyz,1.0));
}