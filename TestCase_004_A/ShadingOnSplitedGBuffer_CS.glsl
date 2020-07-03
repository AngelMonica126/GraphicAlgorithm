#version 430 core

#define LOCAL_GROUP_SIZE 16

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;
uniform sampler2D u_AlbedoTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_PositionTexture;
layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputImage;

uniform int u_WindowWidth;
uniform int u_WindowHeight;
uniform int u_SubBufferNumX;
uniform int u_SubBufferNumY;
uniform int u_TotalLightNum;

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
	//vec2  TexCoord = vec2(float(FragPos.x + 0.5f) / u_WindowWidth, float(FragPos.y + 0.5f) / u_WindowHeight);
	ivec2 SubBufferSize = ivec2(u_WindowWidth / u_SubBufferNumX, u_WindowHeight / u_SubBufferNumY);
	int   SubBufferNum = u_SubBufferNumX * u_SubBufferNumY;
	int   LightNumPerSubBuffer = (u_TotalLightNum + SubBufferNum - 1) / SubBufferNum;

	ivec2 SubBufferIndex2 = ivec2(FragPos.x / SubBufferSize.x, FragPos.y / SubBufferSize.y);
	int   SubBufferIndex = SubBufferIndex2.y * u_SubBufferNumX + SubBufferIndex2.x;
	int   LightStartIndex = SubBufferIndex * LightNumPerSubBuffer;
	int   LightEndIndex = min(LightStartIndex + LightNumPerSubBuffer, u_TotalLightNum);

//	vec3 Normal = texture(u_NormalTexture, TexCoord).xyz;
//	vec3 Albedo = texture(u_AlbedoTexture, TexCoord).xyz;
//	vec3 FragViewPos = texture(u_PositionTexture, TexCoord).xyz;
	vec3 Normal = texelFetch(u_NormalTexture, FragPos, 0).xyz;
	vec3 Albedo = texelFetch(u_AlbedoTexture, FragPos, 0).xyz;
	vec3 FragViewPos = texelFetch(u_PositionTexture, FragPos, 0).xyz;

	vec3 FragColor = vec3(0);// = Albedo * 0.1;
	for(int i = LightStartIndex; i < LightEndIndex; ++i)
	{
		SPointLight PointLight = u_PointLights[i];
		vec4 LightViewPos = u_ViewMatrix * PointLight.Position;
		FragColor += calcPointLight(FragViewPos, Normal, LightViewPos.xyz, PointLight.ColorAndRadius.xyz, PointLight.ColorAndRadius.w);
	}
	FragColor *= u_SubBufferNumX * u_SubBufferNumY;

	//imageStore(u_OutputImage, FragPos, vec4(FragColor.x, LightStartIndex, LightEndIndex, 1.0f));
	imageStore(u_OutputImage, FragPos, vec4(FragColor, 1.0f));
}