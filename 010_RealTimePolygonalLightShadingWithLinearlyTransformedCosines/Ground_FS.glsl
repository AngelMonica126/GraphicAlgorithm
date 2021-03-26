#version 430 core

in  vec3 v2f_FragPosInWorldSpace;
in  vec2 v2f_TexCoords;
out vec4 FragColor_;

layout (std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform vec3  u_CameraPosInWorldSpace;
uniform vec3  u_DiffuseColor;
uniform vec3  u_SpecularColor;
uniform vec3  u_PolygonalLightVertexPos[4];

uniform float u_Roughness;
uniform float u_Intensity;
uniform bool  u_IsTwoSided = false;
uniform int u_Layer = 0;

uniform sampler2D u_LTC_MatrixTexture;
uniform sampler2D u_LTC_MagnitueTexture;
uniform sampler2DArray u_FilteredLightTexture;

const float LUT_SIZE = 64.0;
const float LUT_SCALE = (LUT_SIZE - 1.0) / LUT_SIZE;
const float LUT_BIAS = 0.5 / LUT_SIZE;
const float PI = 3.14159265;

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
	float Theta = acos(CosTheta);
	return cross(vVertex1, vVertex2) * ((Theta > 0.001) ? Theta / sin(Theta) : 1.0);
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

vec3 fecthFilteredLightTexture(vec3 vPolygonalLightVertexPos[4], vec3 vLooupVector)
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
	
	float Distance = abs(Distance2Plane) / pow(PlaneAreaSquared, 0.25);
	
	float Lod = log(2048.0 * Distance) / log(3.0);

	float LodA = floor(Lod);
	float LodB = ceil(Lod);
	float t = Lod - LodA;
	vec3 ColorA = texture(u_FilteredLightTexture, vec3(UV, LodA)).rgb;
	vec3 ColorB = texture(u_FilteredLightTexture, vec3(UV, LodB)).rgb;
	return mix(ColorA, ColorB, t);
}


vec3 integrateLTC(vec3 vNormal, vec3 vViewDir, vec3 vFragPos, mat3 vLTCMatrix, vec3 vPolygonalLightVertexPos[4], bool vTwoSided)
{	
	//着色点上的切线空间正交基
	vec3 Tangent = normalize(vViewDir - vNormal * dot(vViewDir, vNormal));
	vec3 Bitangent = cross(vNormal, Tangent);

	//将变换矩阵转换到切线空间
	vLTCMatrix = vLTCMatrix * transpose(mat3(Tangent, Bitangent, vNormal));
	//多边形顶点(因为被平面z=0裁剪以后，四边形可能变成5个顶点)
	vec3 PolygonalLightVertexPosInTangentSpace[5], PolygonalLightVertexPosBeforeClipping[4];
	PolygonalLightVertexPosBeforeClipping[0] = PolygonalLightVertexPosInTangentSpace[0] = vLTCMatrix * (vPolygonalLightVertexPos[0] - vFragPos);
	PolygonalLightVertexPosBeforeClipping[1] = PolygonalLightVertexPosInTangentSpace[1] = vLTCMatrix * (vPolygonalLightVertexPos[1] - vFragPos);
	PolygonalLightVertexPosBeforeClipping[2] = PolygonalLightVertexPosInTangentSpace[2] = vLTCMatrix * (vPolygonalLightVertexPos[2] - vFragPos);
	PolygonalLightVertexPosBeforeClipping[3] = PolygonalLightVertexPosInTangentSpace[3] = vLTCMatrix * (vPolygonalLightVertexPos[3] - vFragPos);
	PolygonalLightVertexPosInTangentSpace[4] = PolygonalLightVertexPosInTangentSpace[3];

	int PolygonalLightVertexNumAfterClipping;
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

	float Sum = vTwoSided ? abs(VSum.z) : max(VSum.z, 0.0);

	vec3 LookupVector = normalize(VSum);
	vec3 TextureLight = fecthFilteredLightTexture(PolygonalLightVertexPosBeforeClipping, LookupVector);

	return vec3(Sum) * TextureLight;
}

vec2 LTC_Coords(float vCosTheta, float vRoughness)
{
	float Theta = acos(vCosTheta);
    vec2 Coords = vec2(vRoughness, Theta/(0.5 * PI));
    const float LUT_SIZE = 32.0;
    // scale and bias coordinates, for correct filtered lookup
    Coords = Coords * (LUT_SIZE - 1.0)/LUT_SIZE + 0.5/LUT_SIZE;
    return Coords;
}

void main()
{
	vec3 GroundNormal = vec3(0, 1, 0);	//其他几何体的话应该由其法线乘以模型矩阵来算
	vec3 ViewDir = normalize(u_CameraPosInWorldSpace - v2f_FragPosInWorldSpace);

	vec2 UV = LTC_Coords(dot(GroundNormal, ViewDir), u_Roughness);

	vec4 LTCMatrixComponents = texture2D(u_LTC_MatrixTexture, UV);
	mat3 LTCMatrix = mat3
	(
		vec3(1, 0, LTCMatrixComponents.y),
		vec3(0, LTCMatrixComponents.z, 0),
		vec3(LTCMatrixComponents.w, 0, LTCMatrixComponents.x)
	);

	vec3 Diffuse = integrateLTC(GroundNormal, ViewDir, v2f_FragPosInWorldSpace, mat3(1), u_PolygonalLightVertexPos, u_IsTwoSided);
	vec3 Specular = integrateLTC(GroundNormal, ViewDir, v2f_FragPosInWorldSpace, LTCMatrix, u_PolygonalLightVertexPos, u_IsTwoSided);
	Specular *= texture2D(u_LTC_MagnitueTexture, UV).w;
	vec2 Schlick = texture2D(u_LTC_MagnitueTexture, UV).xy;
	Specular *= u_SpecularColor * Schlick.x + (1.0 - u_SpecularColor) * Schlick.y;

	vec3 ResultColor = u_Intensity * (Diffuse * u_DiffuseColor + Specular/10);
	//vec3 ResultColor = u_Intensity * Diffuse * u_DiffuseColor;
	ResultColor /=  PI;
//	ResultColor /= 2.0 * PI;
	FragColor_ = vec4(ResultColor, 1.0);
}