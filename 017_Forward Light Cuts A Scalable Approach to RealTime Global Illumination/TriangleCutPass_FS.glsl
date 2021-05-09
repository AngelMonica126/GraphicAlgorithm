#version 430 core

out vec4 Color_;

struct STriangleCut
{
	vec4 Center;
	vec4 NormalandLevel;
	vec4 ColorAndArea;
};

layout (std430, binding = 0) buffer Triangles
{
	STriangleCut u_Triangles[];
};
layout (binding = 0, offset = 0) uniform atomic_uint uTriangleID;

struct STriangle
{
	vec3 mPositions[3];
	vec3 mNormals;
	vec2 mTexCoords;
	vec2 mAreaAndLevel;
};

in GS_FS_VERTEX
{
	STriangle TriangleInfo;
}fragment_out;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_LightDepthTexture;	
uniform mat4  u_LightVPMatrix;
uniform vec3  u_LightDir;	

void main()
{
	vec3 DiffuseColor = texture(u_DiffuseTexture, fragment_out.TriangleInfo.mTexCoords).rgb;
	vec3 Center = (fragment_out.TriangleInfo.mPositions[0] + 
				  fragment_out.TriangleInfo.mPositions[1] +
				  fragment_out.TriangleInfo.mPositions[2]) / 3.0;
	vec4 FragPosInLightSpace = u_LightVPMatrix * vec4(Center, 1);
	FragPosInLightSpace /= FragPosInLightSpace.w;
	FragPosInLightSpace.xyz = (FragPosInLightSpace.xyz + 1) / 2;
	if(FragPosInLightSpace.z < 0.0f || FragPosInLightSpace.x > 1.0f || FragPosInLightSpace.y > 1.0f || FragPosInLightSpace.x < 0.0f || FragPosInLightSpace.y < 0.0f)
		discard;
	else
	{
		vec2 FragNDCPos4Light = FragPosInLightSpace.xy;
		float ClosetDepth4Light = texture(u_LightDepthTexture, FragNDCPos4Light).r; 
		float Bias = max(0.00001 * (1.0 - dot(fragment_out.TriangleInfo.mNormals, u_LightDir)), 0.00001);
		float Visibility4DirectLight = (FragPosInLightSpace.z - Bias < ClosetDepth4Light) ? 1.0f : 0.0f;
		if(Visibility4DirectLight < 0.5) discard;
	}

	int CurID = int(atomicCounterIncrement(uTriangleID)); 
	u_Triangles[CurID].Center = vec4(Center, 1);
	u_Triangles[CurID].NormalandLevel = vec4(fragment_out.TriangleInfo.mNormals,fragment_out.TriangleInfo.mAreaAndLevel.y);
	u_Triangles[CurID].ColorAndArea = vec4(DiffuseColor,fragment_out.TriangleInfo.mAreaAndLevel.x);
	Color_ = vec4(DiffuseColor,1);
}