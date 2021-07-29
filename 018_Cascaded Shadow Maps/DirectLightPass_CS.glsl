#version 430 core
#pragma optionNV (unroll all)	//暂时不知道有没有起作用

#define LOCAL_GROUP_SIZE 32

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;
uniform sampler2D u_InputAlbedoTexture;
uniform sampler2D u_InputNormalTexture;
uniform sampler2D u_InputPositionTexture;
uniform sampler2DArray u_LightDepthTexture;	

layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputDirectIlluminationImage;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform mat4  u_LightVPMatrixMulInverseCameraViewMatrix[5];
uniform vec3  u_LightDir;	
uniform float  u_Intensity;	
uniform int u_SpiltNum = 5;
void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	vec4 Albedo = texelFetch(u_InputAlbedoTexture, FragPos, 0);
	vec4 Normal = texelFetch(u_InputNormalTexture, FragPos, 0);
	vec4 Position = texelFetch(u_InputPositionTexture, FragPos, 0);
	vec4 FragViewNormalAndDoubleRoughness = Normal;	//对vec3(0)不能做归一化
	vec3 FragViewNormal = normalize(FragViewNormalAndDoubleRoughness.xyz);
	vec4 FragAlbedoAndMetallic = Albedo;
	vec3 FragViewPos = Position.xyz;
	vec3 ViewDir = -normalize(FragViewPos);
	vec3 SpecularColor;
	float DirectIllumination;
	for(int i = 0; i < u_SpiltNum; i++)
	{
		vec4 FragPosInLightSpace = u_LightVPMatrixMulInverseCameraViewMatrix[i] * vec4(FragViewPos, 1);
		FragPosInLightSpace /= FragPosInLightSpace.w;
		FragPosInLightSpace.xyz = (FragPosInLightSpace.xyz + 1) / 2;
		vec3 LightDirInViewSpace = -normalize(vec3(vec4(u_LightDir, 0.0f)));	//这个负号不要忘了
		float Visibility4DirectLight = 0.0f;
		if(FragPosInLightSpace.x > 1.0f || FragPosInLightSpace.y > 1.0f || FragPosInLightSpace.x < 0.0f || FragPosInLightSpace.y < 0.0f)
			DirectIllumination = 0.1;
		else
		{
			vec3 FragNDCPos4Light = vec3(FragPosInLightSpace.xy,i);
			float ClosetDepth4Light = texture(u_LightDepthTexture, FragNDCPos4Light).r; 
			float Bias = 0.005f;
			Visibility4DirectLight = (FragPosInLightSpace.z - Bias <= ClosetDepth4Light) ? 1.0f : 0.0f;
			DirectIllumination = u_Intensity * max(dot(LightDirInViewSpace, FragViewNormal), 0) * Visibility4DirectLight + 0.1;
			break;
		}
	}

	imageStore(u_OutputDirectIlluminationImage, FragPos, vec4(DirectIllumination,DirectIllumination,DirectIllumination, 1) * Albedo);
}