#version 430 core
#pragma optionNV (unroll all)	

#define LOCAL_GROUP_SIZE 16
#define PI 3.1415926
#define InversePI 0.31830988618379

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;

layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputImage;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};
struct SPlane	//new
{
	vec4 P0;
	vec4 P1;
	vec4 P2;
	vec4 P3;
	vec4 Normal;
	vec4 ProportionAndUV;
	ivec4 Index;
};

layout (std430, binding = 1) buffer SPlanes
{
	SPlane u_Planes[];
};
layout (std430, binding = 2) buffer SVALs
{
	SPlane u_VALs[];
};
layout (std430, binding = 3) buffer SVALCount
{
	int u_VALCount;
};
uniform sampler2D u_LightDepthTexture;	
uniform mat4  u_LightVPMatrixMulInverseCameraViewMatrix;	//new
uniform mat4 u_InverseViewMatrix;
uniform vec3  u_CameraPosInWorldSpace;
uniform vec3  u_LightDir;
uniform vec3  u_LightColor;
uniform float u_Intensity;
uniform sampler2D u_FilteredLightTexture;
uniform sampler2D u_NormalAndDoubleRoughnessTexture;
uniform sampler2D u_PositionTexture;
uniform sampler2D u_AlbedoTexture;


void clipPolygonalLightByZPlane(inout vec3 vioPolygonalLightVertexPos[5], out int voPolygonalLightVertexNumAfterClipping)
{
	int Flag4HowManyVertexAboveZPlane = 0;
	if(vioPolygonalLightVertexPos[0].z > 0.0) Flag4HowManyVertexAboveZPlane += 1;
	if(vioPolygonalLightVertexPos[1].z > 0.0) Flag4HowManyVertexAboveZPlane += 2;
	if(vioPolygonalLightVertexPos[2].z > 0.0) Flag4HowManyVertexAboveZPlane += 4;
	if(vioPolygonalLightVertexPos[3].z > 0.0) Flag4HowManyVertexAboveZPlane += 8;

	voPolygonalLightVertexNumAfterClipping = 0;
	if(Flag4HowManyVertexAboveZPlane == 0)
	{
	}
	else if(Flag4HowManyVertexAboveZPlane == 1)
	{
		voPolygonalLightVertexNumAfterClipping = 3;
		vioPolygonalLightVertexPos[1] = -vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[0] + vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[1];
		vioPolygonalLightVertexPos[2] = -vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[0] + vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[3];
	}
	else if(Flag4HowManyVertexAboveZPlane == 2)
	{
		voPolygonalLightVertexNumAfterClipping = 3;
		vioPolygonalLightVertexPos[0] = -vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[1] + vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[0];
		vioPolygonalLightVertexPos[2] = -vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[1] + vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[2];
	}
	else if(Flag4HowManyVertexAboveZPlane == 3)
	{
		voPolygonalLightVertexNumAfterClipping = 4;
		vioPolygonalLightVertexPos[2] = -vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[1] + vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[2];
		vioPolygonalLightVertexPos[3] = -vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[0] + vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[3];
	}
	else if(Flag4HowManyVertexAboveZPlane == 4)
	{
		voPolygonalLightVertexNumAfterClipping = 3;
		vioPolygonalLightVertexPos[0] = -vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[2] + vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[3];
		vioPolygonalLightVertexPos[1] = -vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[2] + vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[1];
	}
	else if(Flag4HowManyVertexAboveZPlane == 5)
	{
		voPolygonalLightVertexNumAfterClipping = 0;
	}
	else if(Flag4HowManyVertexAboveZPlane == 6)
	{
		voPolygonalLightVertexNumAfterClipping = 4;
		vioPolygonalLightVertexPos[0] = -vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[1] + vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[0];
		vioPolygonalLightVertexPos[3] = -vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[2] + vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[3];
	}
	else if(Flag4HowManyVertexAboveZPlane == 7)
	{
		voPolygonalLightVertexNumAfterClipping = 5;
		vioPolygonalLightVertexPos[4] = -vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[0] + vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[3];
		vioPolygonalLightVertexPos[3] = -vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[2] + vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[3];
	}
	else if(Flag4HowManyVertexAboveZPlane == 8)
	{
		voPolygonalLightVertexNumAfterClipping = 3;
		vioPolygonalLightVertexPos[0] = -vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[3] + vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[0];
		vioPolygonalLightVertexPos[1] = -vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[3] + vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[2];
		vioPolygonalLightVertexPos[2] = vioPolygonalLightVertexPos[3];
	}
	else if(Flag4HowManyVertexAboveZPlane == 9)
	{
		voPolygonalLightVertexNumAfterClipping = 4;
		vioPolygonalLightVertexPos[1] = -vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[0] + vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[1];
		vioPolygonalLightVertexPos[2] = -vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[3] + vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[2];
	}
	else if(Flag4HowManyVertexAboveZPlane == 10)
	{
		voPolygonalLightVertexNumAfterClipping = 0;
	}
	else if(Flag4HowManyVertexAboveZPlane == 11)
	{
		voPolygonalLightVertexNumAfterClipping = 5;
		vioPolygonalLightVertexPos[4] = vioPolygonalLightVertexPos[3];
		vioPolygonalLightVertexPos[3] = -vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[3] + vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[2];
		vioPolygonalLightVertexPos[2] = -vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[1] + vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[2];
	}
	else if(Flag4HowManyVertexAboveZPlane == 12)
	{
		voPolygonalLightVertexNumAfterClipping = 4;
		vioPolygonalLightVertexPos[1] = -vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[2] + vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[1];
		vioPolygonalLightVertexPos[0] = -vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[3] + vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[0];
	}
	else if(Flag4HowManyVertexAboveZPlane == 13)
	{
		voPolygonalLightVertexNumAfterClipping = 5;
		vioPolygonalLightVertexPos[4] = vioPolygonalLightVertexPos[3];
		vioPolygonalLightVertexPos[3] = vioPolygonalLightVertexPos[2];
		vioPolygonalLightVertexPos[2] = -vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[2] + vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[1];
		vioPolygonalLightVertexPos[1] = -vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[0] + vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[1];
	}
	else if(Flag4HowManyVertexAboveZPlane == 14)
	{
		voPolygonalLightVertexNumAfterClipping = 5;
		vioPolygonalLightVertexPos[4] = -vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[3] + vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[0];
		vioPolygonalLightVertexPos[0] = -vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[1] + vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[0];
	}
	else if(Flag4HowManyVertexAboveZPlane == 15)
	{
		voPolygonalLightVertexNumAfterClipping = 4;
	}

	if(voPolygonalLightVertexNumAfterClipping == 3)
		vioPolygonalLightVertexPos[3] = vioPolygonalLightVertexPos[0];
	if(voPolygonalLightVertexNumAfterClipping == 4)
		vioPolygonalLightVertexPos[4] = vioPolygonalLightVertexPos[0];
}

vec3 integrateEdge(vec3 vVertex1, vec3 vVertex2)
{
	float CosTheta = dot(vVertex1, vVertex2);
	CosTheta = clamp(CosTheta, -0.9999, 0.9999);
	float Theta = acos(CosTheta);
    // 除以sin(theta)是因为v1，v2已经标准化
	return cross(vVertex1, vVertex2) * ((Theta / sin(Theta)));
}

struct SRay
{
    vec3 m_Origin;
    vec3 m_Dir;
};

bool rayPlaneIntersect(SRay vRay, vec4 vPlane, out float voDistanceFromRayOrigin2Plane)
{
    voDistanceFromRayOrigin2Plane = -dot(vPlane, vec4(vRay.m_Origin, 1.0)) / dot(vPlane.xyz, vRay.m_Dir);
    return voDistanceFromRayOrigin2Plane > 0.0;
}

vec3 fecthFilteredLightTexture(vec3 vPolygonalLightVertexPos[4], vec3 vLooupVector,vec2 vProportion,vec2 vUVBegin)
{
	vec3 V1 = vPolygonalLightVertexPos[1] - vPolygonalLightVertexPos[0];
	vec3 V2 = vPolygonalLightVertexPos[3] - vPolygonalLightVertexPos[0];
	vec3 PlaneOrtho = cross(V1, V2);
	float PlaneAreaSquared = dot(PlaneOrtho, PlaneOrtho);
	
	SRay Ray;
	Ray.m_Origin = vec3(0);
	Ray.m_Dir = vLooupVector;
	vec4 Plane = vec4(PlaneOrtho, -dot(PlaneOrtho, vPolygonalLightVertexPos[0]));
	float Distance2Plane;
	rayPlaneIntersect(Ray, Plane, Distance2Plane);
	
	vec3 P = Distance2Plane * Ray.m_Dir - vPolygonalLightVertexPos[0];

	float Dot_V1_V2 = dot(V1, V2);
	float Inv_Dot_V1_V1 = 1.0 / dot(V1, V1);
	vec3  V2_ = V2 - V1 * Dot_V1_V2 * Inv_Dot_V1_V1;
	vec2  UV;
	UV.y = dot(V2_, P) / dot(V2_, V2_);
	UV.x = dot(V1, P) * Inv_Dot_V1_V1 - Dot_V1_V2 * Inv_Dot_V1_V1 * UV.y;
	UV = vec2(1 - UV.y, 1 - UV.x);
	UV *= vProportion;
	UV += vUVBegin;
	float Distance = abs(Distance2Plane) / pow(PlaneAreaSquared, 0.25);
	float Lod = log(2048.0 * Distance) / log(3.0);
	Lod = min(Lod, 2.0);
	return textureLod(u_FilteredLightTexture, UV, Lod).rgb;
}

vec3 integrateLTC(vec3 vNormal, vec3 vViewDir, vec3 vFragPos, vec3 vPolygonalLightVertexPos[4], vec2 vProportion,vec2 vUVBegin)
{	
	//着色点上的切线空间正交基
	vec3 Tangent = normalize(vViewDir - vNormal * dot(vViewDir, vNormal));
	vec3 Bitangent = cross(vNormal, Tangent);

	//将变换矩阵转换到切线空间
	mat3 vLTCMatrix = transpose(mat3(Tangent, Bitangent, vNormal));
	//多边形顶点(因为被平面z=0裁剪以后，四边形可能变成5个顶点)
	vec3 PolygonalLightVertexPosInTangentSpace[5], PolygonalLightVertexPosBeforeClipping[4];
	PolygonalLightVertexPosBeforeClipping[0] = PolygonalLightVertexPosInTangentSpace[0] = vLTCMatrix * (vPolygonalLightVertexPos[0] - vFragPos);
	PolygonalLightVertexPosBeforeClipping[1] = PolygonalLightVertexPosInTangentSpace[1] = vLTCMatrix * (vPolygonalLightVertexPos[1] - vFragPos);
	PolygonalLightVertexPosBeforeClipping[2] = PolygonalLightVertexPosInTangentSpace[2] = vLTCMatrix * (vPolygonalLightVertexPos[2] - vFragPos);
	PolygonalLightVertexPosBeforeClipping[3] = PolygonalLightVertexPosInTangentSpace[3] = vLTCMatrix * (vPolygonalLightVertexPos[3] - vFragPos);
	PolygonalLightVertexPosInTangentSpace[4] = PolygonalLightVertexPosInTangentSpace[3];

	int PolygonalLightVertexNumAfterClipping = 4;
	clipPolygonalLightByZPlane(PolygonalLightVertexPosInTangentSpace, PolygonalLightVertexNumAfterClipping);
	
	if(PolygonalLightVertexNumAfterClipping == 0)
		return vec3(0);
	
	//把裁剪后的多边形投影到球面上（也就是对每个顶点坐标向量归一化）
	PolygonalLightVertexPosInTangentSpace[0] = normalize(PolygonalLightVertexPosInTangentSpace[0]);
	PolygonalLightVertexPosInTangentSpace[1] = normalize(PolygonalLightVertexPosInTangentSpace[1]);
	PolygonalLightVertexPosInTangentSpace[2] = normalize(PolygonalLightVertexPosInTangentSpace[2]);
	PolygonalLightVertexPosInTangentSpace[3] = normalize(PolygonalLightVertexPosInTangentSpace[3]);
	PolygonalLightVertexPosInTangentSpace[4] = normalize(PolygonalLightVertexPosInTangentSpace[4]);

	//用累加边的公式来求解积分
	vec3 VSum = vec3(0.0);
	VSum += integrateEdge(PolygonalLightVertexPosInTangentSpace[0], PolygonalLightVertexPosInTangentSpace[1]);
	VSum += integrateEdge(PolygonalLightVertexPosInTangentSpace[1], PolygonalLightVertexPosInTangentSpace[2]);
	VSum += integrateEdge(PolygonalLightVertexPosInTangentSpace[2], PolygonalLightVertexPosInTangentSpace[3]);
	if(PolygonalLightVertexNumAfterClipping >= 4)
		VSum += integrateEdge(PolygonalLightVertexPosInTangentSpace[3], PolygonalLightVertexPosInTangentSpace[4]);
	if(PolygonalLightVertexNumAfterClipping == 5)
		VSum += integrateEdge(PolygonalLightVertexPosInTangentSpace[4], PolygonalLightVertexPosInTangentSpace[0]);
	float Sum =  max(VSum.z, 0.0);
	vec3 LookupVector = normalize(VSum);
	vec3 TextureLight = fecthFilteredLightTexture(PolygonalLightVertexPosBeforeClipping, LookupVector,vProportion,vUVBegin);
	return vec3(Sum) * TextureLight;
}
void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	vec3 FragViewNormal = texelFetch(u_NormalAndDoubleRoughnessTexture, FragPos, 0).xyz;
	vec3 FragWorldNormal = mat3(u_InverseViewMatrix) * FragViewNormal;
	vec3 FragPosInViewSpace = texelFetch(u_PositionTexture, FragPos, 0).xyz;
	vec3 Albedo = texelFetch(u_AlbedoTexture, FragPos, 0).xyz;
	vec3 FragPosInWorldSpace = vec3(u_InverseViewMatrix * vec4(FragPosInViewSpace,1.0));
	if((abs(FragWorldNormal.x) < 0.0001f) && (abs(FragWorldNormal.y) < 0.0001f) && (abs(FragWorldNormal.z) < 0.0001f))
	{
		imageStore(u_OutputImage, FragPos, vec4(0.52, 0.77, 1, 1));
		return;
	}
	FragWorldNormal= normalize(FragWorldNormal);
	vec3 ViewDir = normalize(u_CameraPosInWorldSpace - FragPosInWorldSpace);
	vec3 WorldLightDir = -normalize(u_LightDir);
	vec3 LightDirInViewSpace = -normalize(vec3(u_ViewMatrix * vec4(u_LightDir, 0.0f)));
	vec3 IndirectDiffuse = vec3(0);
	for(int i = 0 ; i < u_VALCount; i++)
	{
		vec3 Point[4] = {u_VALs[i].P0.xyz, u_VALs[i].P1.xyz, u_VALs[i].P2.xyz, u_VALs[i].P3.xyz};
		vec3 Diffuse =  u_VALs[i].Normal.w * integrateLTC(FragWorldNormal.xyz, ViewDir, FragPosInWorldSpace, Point, u_VALs[i].ProportionAndUV.xy, u_VALs[i].ProportionAndUV.zw);
		IndirectDiffuse += Diffuse;
	}
	vec3 ResultColor = u_Intensity * InversePI * 0.5 * (IndirectDiffuse);
	imageStore(u_OutputImage, FragPos, vec4(ResultColor, 1.0));
}