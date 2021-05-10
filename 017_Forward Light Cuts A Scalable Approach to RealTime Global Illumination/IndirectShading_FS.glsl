#version 430 core
#define PI 3.1415926
in  vec2 v2f_TexCoords;
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


layout (std430, binding = 1) buffer SkSSBO
{
	float u_Sk[];
};


uniform sampler2D u_InputNormalTexture;
uniform sampler2D u_InputPositionTexture;
uniform float u_Intensity;
vec3 calcVPLIrradiance(vec3 vVPLRadiance, vec3 vVPLNormal, vec3 vVPLPos, vec3 vFragPos, vec3 vFragNormal)
{
	vec3 VPL2Frag = vFragPos - vVPLPos;
	float SquareDistance = dot(VPL2Frag, VPL2Frag);
	return vVPLRadiance.xyz * max(dot(vVPLNormal.xyz, VPL2Frag), 0) * max(dot(vFragNormal, -VPL2Frag), 0) / SquareDistance;
}

float getF(float d,int level)
{
	if(level == 0) return 1;
	float DkMinus = sqrt(u_Sk[level - 1]);
	float Dk = sqrt(u_Sk[level]);
	float DkPlus = sqrt(u_Sk[level + 1]);
	if(d >= DkMinus && d <= Dk)
		return (d - DkMinus) / (Dk - DkMinus);
	else if(d >= Dk && d <= DkPlus)
		return (DkPlus - d) / (DkPlus - Dk);
	else return 0;
}

void main()
{
	
	int VPLNum = int(atomicCounter(uTriangleID));
	vec3 Normal = texture(u_InputNormalTexture, v2f_TexCoords).xyz;
	vec3 Position = texture(u_InputPositionTexture, v2f_TexCoords).xyz;
	vec3 IndirectIllumination = vec3(0);
	for(int i = 0; i < VPLNum; i++)
	{
		vec3 VPLPos = u_Triangles[i].Center.xyz;
		vec3 VPLNormal = normalize(u_Triangles[i].NormalandLevel.xyz);
		vec3 VPLRadiance = u_Triangles[i].ColorAndArea.xyz * 1.5 / PI;
		vec3 VPL2Frag = Position - VPLPos;
		float SquareDistance = dot(VPL2Frag, VPL2Frag);
		int level = int(u_Triangles[i].NormalandLevel.w);
		float d = sqrt(SquareDistance) / dot(VPLNormal, VPL2Frag);
		IndirectIllumination += u_Sk[level] *
								getF(d,level) * 
								VPLRadiance * max(dot(VPLNormal, VPL2Frag), 0) * max(dot(Normal, -VPL2Frag), 0) / SquareDistance;
	}
	Color_ = vec4(u_Intensity * IndirectIllumination / PI,1);
}