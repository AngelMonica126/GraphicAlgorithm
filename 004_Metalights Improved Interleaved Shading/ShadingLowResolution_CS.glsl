#version 430 core

#define LOCAL_GROUP_SIZE 16

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;
uniform sampler2D u_AlbedoTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_PositionTexture;

uniform int u_LowWidth;
uniform int u_LowHeight;
uniform int u_BlockX;
uniform int u_BlockY;
struct SPointLight
{
	vec4  Position;
	vec4  ColorAndRadius;
	vec4 Importance;
};

layout (std430, binding = 0) buffer PointLights
{
	SPointLight u_PointLights[];
};

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

vec3 calcPointLight(vec3 vFragViewPos, vec3 vNormal, vec3 vLightViewPos, vec3 vLightColor, float vRadius)
{
	vec3 LightDir = vLightViewPos - vFragViewPos;
	float Distance = length(LightDir);
	LightDir = normalize(LightDir);
	float Attenuation = max(1.0f - Distance / vRadius, 0.0f);
	return max(dot(LightDir, vNormal), 0.0f) * vLightColor * Attenuation;
}

void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	int index = int(u_BlockX * gl_GlobalInvocationID.y + gl_GlobalInvocationID.x);
	vec3 sum = vec3(0);
	SPointLight PointLight = u_PointLights[index];

	for(int i = 0 ; i < u_LowWidth ; i++)
		for(int j = 0 ; j < u_LowHeight ; j++)
		{	
			ivec2 TexCoord = ivec2(i,j);
			vec3 Normal = texelFetch(u_NormalTexture, TexCoord, 0).xyz;
			vec3 Albedo = texelFetch(u_AlbedoTexture, TexCoord, 0).xyz;
			vec3 FragViewPos = texelFetch(u_PositionTexture, TexCoord, 0).xyz;
			vec4 LightViewPos = u_ViewMatrix * PointLight.Position;
			sum += calcPointLight(FragViewPos, Normal, LightViewPos.xyz, PointLight.ColorAndRadius.xyz, PointLight.ColorAndRadius.w);
		}
//	sum /= u_LowHeight*u_LowWidth;
	u_PointLights[index].Importance = vec4(dot(sum,sum),1,0,0);
}