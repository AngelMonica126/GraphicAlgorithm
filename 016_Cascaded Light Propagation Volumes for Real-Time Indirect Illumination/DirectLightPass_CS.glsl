#version 430 core
#pragma optionNV (unroll all)	//暂时不知道有没有起作用

#define LOCAL_GROUP_SIZE 32

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;
uniform sampler2D u_InputAlbedoTexture;
uniform sampler2D u_InputNormalTexture;
uniform sampler2D u_InputPositionTexture;
uniform sampler2D u_LightDepthTexture;	

layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputDirectIlluminationImage;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform mat4  u_LightVPMatrixMulInverseCameraViewMatrix;
uniform vec3  u_LightDir;	
uniform float  u_Intensity;	

void main()
{
		ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	vec4 Normal = texelFetch(u_InputNormalTexture, FragPos, 0);
	vec4 Position = texelFetch(u_InputPositionTexture, FragPos, 0);
	vec3 Albedo = texelFetch(u_InputAlbedoTexture, FragPos, 0).xyz;
	if((abs(Normal.x) < 0.0001f) && (abs(Normal.y) < 0.0001f) && (abs(Normal.z) < 0.0001f))
	{
		imageStore(u_OutputDirectIlluminationImage, FragPos, vec4(0, 0, 0, 1));
		return;
	}
	vec3 FragViewNormal = normalize(Normal.xyz);
	vec3 FragViewPos = Position.xyz;
	vec4 FragPosInLightSpace = u_LightVPMatrixMulInverseCameraViewMatrix * vec4(FragViewPos, 1);
	vec3 LightDirInViewSpace = -normalize(vec3(u_ViewMatrix * vec4(u_LightDir, 0.0f)));	//这个负号不要忘了
	float DirectIllumination;
	FragPosInLightSpace /= FragPosInLightSpace.w;
	FragPosInLightSpace.xyz = (FragPosInLightSpace.xyz + 1) / 2;
	float Visibility4DirectLight = 0.0f;
	if(FragPosInLightSpace.z < 0.0f || FragPosInLightSpace.x > 1.0f || FragPosInLightSpace.y > 1.0f || FragPosInLightSpace.x < 0.0f || FragPosInLightSpace.y < 0.0f )
		DirectIllumination = 0;
	else
	{
		vec2 FragNDCPos4Light = FragPosInLightSpace.xy;
		float ClosetDepth4Light = texture(u_LightDepthTexture, FragNDCPos4Light).r; 
		float Bias = max(0.00001 * (1.0 - dot(FragViewNormal, LightDirInViewSpace)), 0.00001);
		Visibility4DirectLight = (FragPosInLightSpace.z - Bias < ClosetDepth4Light) ? 1.0f : 0.0f;
		DirectIllumination = u_Intensity * max(dot(LightDirInViewSpace, FragViewNormal), 0) * Visibility4DirectLight;
	}

	imageStore(u_OutputDirectIlluminationImage, FragPos, vec4(DirectIllumination, DirectIllumination, DirectIllumination, 1));
}