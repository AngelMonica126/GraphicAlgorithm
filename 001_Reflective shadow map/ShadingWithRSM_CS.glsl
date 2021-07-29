#version 430 core
#pragma optionNV (unroll all)	//暂时不知道有没有起作用

#define LOCAL_GROUP_SIZE 16
#define VPL_NUM 32

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;

layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputImage;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

layout(std140, binding = 1) uniform VPLsSampleCoordsAndWeights
{
	vec4 u_VPLsSampleCoordsAndWeights[VPL_NUM];
};

uniform sampler2D u_AlbedoTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_PositionTexture;
uniform sampler2D u_RSMFluxTexture;
uniform sampler2D u_RSMNormalTexture;		
uniform sampler2D u_RSMPositionTexture;		
uniform mat4  u_LightVPMatrixMulInverseCameraViewMatrix;
uniform float u_MaxSampleRadius;
uniform int   u_RSMSize;
uniform int   u_VPLNum;
uniform vec3  u_LightDirInViewSpace;

vec3 calcVPLIrradiance(vec3 vVPLFlux, vec3 vVPLNormal, vec3 vVPLPos, vec3 vFragPos, vec3 vFragNormal, float vWeight)
{
	vec3 VPL2Frag = normalize(vFragPos - vVPLPos);
	return vVPLFlux * max(dot(vVPLNormal, VPL2Frag), 0) * max(dot(vFragNormal, -VPL2Frag), 0) * vWeight;
}

void main()
{
	if(u_VPLNum != VPL_NUM)
		return;

	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	vec3 FragViewNormal = normalize(texelFetch(u_NormalTexture, FragPos, 0).xyz);
	vec3 FragAlbedo = texelFetch(u_AlbedoTexture, FragPos, 0).xyz;
	vec3 FragViewPos = texelFetch(u_PositionTexture, FragPos, 0).xyz;

	vec4 FragPosInLightSpace = u_LightVPMatrixMulInverseCameraViewMatrix * vec4(FragViewPos, 1);
	FragPosInLightSpace /= FragPosInLightSpace.w;
	vec2 FragNDCPos4Light = (FragPosInLightSpace.xy + 1) / 2;
	float RSMTexelSize = 1.0 / u_RSMSize;
	vec3 DirectIllumination;
	if(FragPosInLightSpace.z < 0.0f || FragPosInLightSpace.x > 1.0f || FragPosInLightSpace.y > 1.0f || FragPosInLightSpace.x < 0.0f || FragPosInLightSpace.y < 0.0f )
		DirectIllumination = vec3(0.1) * FragAlbedo;
	else
		DirectIllumination = FragAlbedo * max(dot(-u_LightDirInViewSpace, FragViewNormal), 0.1);
	vec3 IndirectIllumination = vec3(0);
	for(int i = 0; i < u_VPLNum; ++i)
	{
		vec3 VPLSampleCoordAndWeight = u_VPLsSampleCoordsAndWeights[i].xyz;
		vec2 VPLSamplePos = FragNDCPos4Light + u_MaxSampleRadius * VPLSampleCoordAndWeight.xy * RSMTexelSize;
		vec3 VPLFlux = texture(u_RSMFluxTexture, VPLSamplePos).xyz;
		vec3 VPLNormalInViewSpace = normalize(texture(u_RSMNormalTexture, VPLSamplePos).xyz);
		vec3 VPLPositionInViewSpace = texture(u_RSMPositionTexture, VPLSamplePos).xyz;

		IndirectIllumination += calcVPLIrradiance(VPLFlux, VPLNormalInViewSpace, VPLPositionInViewSpace, FragViewPos, FragViewNormal, VPLSampleCoordAndWeight.z);
	}
	IndirectIllumination *= FragAlbedo;

	vec3 Result = DirectIllumination  + IndirectIllumination / u_VPLNum;

	imageStore(u_OutputImage, FragPos, vec4(Result, 1.0));
}