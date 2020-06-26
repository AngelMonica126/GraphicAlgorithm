#version 430 core
#pragma optionNV (unroll all)	//暂时不知道有没有起作用

#define LOCAL_GROUP_SIZE 16
#define PI 3.1415926

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;

layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputImage;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

struct SVPL
{
	vec4 PositionInViewSpace;
	vec4 NormalInViewSpaceAndAlphai;
	vec4 RadianceIntensityAndRadius;
};

layout (std430, binding = 0) buffer VPLs
{
	SVPL u_VPLs[];
};

layout (std430, binding = 1) buffer RandomNumbers
{
	float u_RandomNumbers[];
};

uniform sampler2D u_AlbedoTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_PositionTexture;
uniform sampler2D u_LightDepthTexture;		//光源空间下的NDC深度
uniform mat4  u_LightVPMatrixMulInverseCameraViewMatrix;
uniform int   u_RSMSize;
uniform vec3  u_LightDir;
uniform int   u_VPLsCount;

uniform bool  u_EnableIndirectIllumination;

vec3 calcVPLIrradiance(vec4 vVPLRadianceIntensityAndRadius, vec4 vVPLNormalAndAlphai, vec3 vVPLPos, vec3 vFragPos, vec3 vFragNormal, float vXi4VPL)
{
	vec3 VPL2Frag = vFragPos - vVPLPos;
	float SquareDistance = dot(VPL2Frag, VPL2Frag);
	float fil = 1.0f / SquareDistance;
	float pil = min(fil/vVPLNormalAndAlphai.w, 1.0f);
	float Atten;
	if(pil > vXi4VPL)
		Atten = max(vVPLNormalAndAlphai.w, fil);
	else
		Atten = 0;
	return vVPLRadianceIntensityAndRadius.xyz * max(dot(vVPLNormalAndAlphai.xyz, VPL2Frag), 0) * max(dot(vFragNormal, -VPL2Frag), 0) / SquareDistance * Atten;
}

void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	vec3 FragViewNormal = texelFetch(u_NormalTexture, FragPos, 0).xyz;	//对vec3(0)不能做归一化
	if((abs(FragViewNormal.x) < 0.0001f) && (abs(FragViewNormal.y) < 0.0001f) && (abs(FragViewNormal.z) < 0.0001f))
	{
		imageStore(u_OutputImage, FragPos, vec4(0, 0, 0, 1));
		return;
	}
	FragViewNormal = normalize(FragViewNormal);
	vec3 FragAlbedo = texelFetch(u_AlbedoTexture, FragPos, 0).xyz;
	vec3 FragViewPos = texelFetch(u_PositionTexture, FragPos, 0).xyz;
	vec4 FragPosInLightSpace = u_LightVPMatrixMulInverseCameraViewMatrix * vec4(FragViewPos, 1);
	FragPosInLightSpace /= FragPosInLightSpace.w;
	FragPosInLightSpace.xyz = (FragPosInLightSpace.xyz + 1) / 2;
	vec3 LightDirInViewSpace = vec3(u_ViewMatrix * vec4(u_LightDir, 0.0f));
	vec3 DirectIllumination;
	if(FragPosInLightSpace.z < 0.0f || FragPosInLightSpace.x > 1.0f || FragPosInLightSpace.y > 1.0f || FragPosInLightSpace.x < 0.0f || FragPosInLightSpace.y < 0.0f)
	{
		DirectIllumination = vec3(0);
	}
	else
	{
		vec2 FragNDCPos4Light = FragPosInLightSpace.xy;
		float ClosetDepth4Light = texture(u_LightDepthTexture, FragNDCPos4Light).r; 
		float Bias = max(0.0001 * (1.0 - dot(FragViewNormal, -LightDirInViewSpace)), 0.00001);
		float Visibility4DirectLight = (FragPosInLightSpace.z - Bias < ClosetDepth4Light) ? 1.0f : 0.0f;
		DirectIllumination = FragAlbedo * max(dot(-LightDirInViewSpace, FragViewNormal), 0);// * Visibility4DirectLight;
	}
	vec3 IndirectIllumination = vec3(0);
	for(int i = 0; i < u_VPLsCount; ++i)
	{
		vec4 VPLRadianceIntensityAndRadius = u_VPLs[i].RadianceIntensityAndRadius;
		vec4 VPLNormalInViewSpaceAndAlphai = u_VPLs[i].NormalInViewSpaceAndAlphai;
		vec3 VPLPositionInViewSpace = u_VPLs[i].PositionInViewSpace.xyz;

		IndirectIllumination += calcVPLIrradiance(VPLRadianceIntensityAndRadius, VPLNormalInViewSpaceAndAlphai, VPLPositionInViewSpace, FragViewPos, FragViewNormal, u_RandomNumbers[i]);
	}

	IndirectIllumination *= FragAlbedo;

	vec3 Result= DirectIllumination + IndirectIllumination;
	imageStore(u_OutputImage, FragPos, vec4(Result, 1.0));
}