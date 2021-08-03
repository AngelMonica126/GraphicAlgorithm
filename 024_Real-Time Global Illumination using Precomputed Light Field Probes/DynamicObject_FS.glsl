#version 430 core

#define TRACE_RESULT_MISS    0
#define TRACE_RESULT_HIT     1
#define TRACE_RESULT_UNKNOWN 2

in  vec3 v2f_FragPosInWorldSpace;
in  vec3 v2f_Normal;
layout(location = 0) out vec4 Color_;
layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform sampler2D u_BRDFLut;
uniform vec3 u_DiffuseColor;
uniform sampler2D u_PositionTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_AlbedoTexture;
uniform sampler2DArray u_OutputOctRadianceImage;
uniform sampler2DArray u_OutputOctNormalImage;
uniform sampler2DArray u_OutputOctChebyshevsImage;
uniform sampler2DArray u_OutputOctIrradianceImage;
uniform float u_Exposure = 2.0f;
uniform mat4 u_InverseViewMat;
uniform float u_CellSize;
uniform vec3 u_MinAABB;
uniform ivec3 u_CellNum;
uniform int u_BakeResolution = 128;
const float RayBumpEpsilon = 0.01;
const float MinThickness = 0.03;
const float MaxThickness = 0.50;


ivec3 convertPointToGridIndex(vec3 vPos) {
	return ivec3((vPos - u_MinAABB) / u_CellSize);
}

vec3 gridCoordToPosition(vec3 vCellCoords)
{
	return vCellCoords * u_CellSize + u_MinAABB;
}

float signNotZero(in float k) {
    return k >= 0.0 ? 1.0 : -1.0;
}

vec2 signNotZero(in vec2 v) {
    return vec2( signNotZero(v.x), signNotZero(v.y) );
}

vec2 octEncode(vec3 vDirection)
{
	float NormalTotal = abs(vDirection.x)+abs(vDirection.y)+abs(vDirection.z);
	vec2 Result = vDirection.xy * (1.0 / NormalTotal);
	if(vDirection.z < 0.0)
		Result =(1.0 - abs(Result.yx)) * signNotZero(Result.xy);
	return Result;
}
vec3 octDecode(vec2 vTextCoord)
{
	vec3 Direction = vec3(vTextCoord.x, vTextCoord.y, 1.0 - abs(vTextCoord.x) - abs(vTextCoord.y));
    if (Direction.z < 0.0) {
        Direction.xy = (1.0 - abs(Direction.yx)) * signNotZero(Direction.xy);
    }
    return normalize(Direction);
}

int gridCoordToProbeIndex(ivec3 vCellCoords)
{
	return vCellCoords.x * u_CellNum.y * u_CellNum.z + vCellCoords.y * u_CellNum.z + vCellCoords.z;
}

vec3 computePrefilteredIrradiance(vec3 vPosition, vec3 vNormal)
{
	ivec3 CellGridIndex = convertPointToGridIndex(vPosition);
	vec3 Diffuse  = vec3(0);
	vec3 CellBasePos = vec3(convertPointToGridIndex(vPosition)) * u_CellSize +  u_MinAABB;
	vec3 alpha = clamp((vPosition - CellBasePos) / u_CellSize, vec3(0), vec3(1));
	float SumWeight = 0;
	for(int i = 0; i < 8 ;i++)
	{
		ivec3 Offset = ivec3(i, i >> 1, i >> 2) & ivec3(1);
		ivec3 CellCoords = convertPointToGridIndex(vPosition) + Offset;
		int Index = gridCoordToProbeIndex(CellCoords);
		vec3 Trilinear = mix (1 - alpha, alpha, Offset);
		float Weight = Trilinear.x * Trilinear.y * Trilinear.z;		vec3 ProbePos = gridCoordToPosition(CellCoords);		vec3 ProbeToPoint = vPosition - ProbePos;
		vec3 Dir = normalize(-ProbeToPoint);		vec2 Texcoord = (octEncode(-Dir) * 0.5 + 0.5);		Weight *= max(0.05, dot(Dir, vNormal));		vec2 VSM = texture(u_OutputOctChebyshevsImage,vec3(Texcoord,Index)).xy;		float Mean = VSM.x;
		float Variance = abs(VSM.y - Mean * Mean);		float DistToProbe = length(ProbeToPoint);		float SubMean = DistToProbe - Mean;
		float Chebychev = Variance / (Variance + SubMean * SubMean);		Weight *= ((DistToProbe <= Mean) ? 1.0 : max(Chebychev, 0.0));				Weight = max(0.0002, Weight);		SumWeight += Weight;		Diffuse += Weight  * texture(u_OutputOctIrradianceImage,vec3((octEncode(vNormal) * 0.5 + 0.5),Index)).xyz;
	}
	return Diffuse;
}

vec3 computeGlossyRay(vec3 vPosition, vec3 ViewDir, vec3 vNormal) {
	vec3 wi = normalize(reflect(-ViewDir, vNormal));
	for(int i = 0; i < 8 ;i++)
	{
		ivec3 Offset = ivec3(i, i >> 1, i >> 2) & ivec3(1);
		ivec3 CellCoords = convertPointToGridIndex(vPosition) + Offset;
		int Index = gridCoordToProbeIndex(CellCoords);
		vec2 Texcoord = (octEncode(wi) * 0.5 + 0.5);
		vec3 Normal = texture(u_OutputOctNormalImage, vec3(Texcoord, Index), 0).xyz;
		if(dot(Normal,wi) < 0)
			return texture(u_OutputOctRadianceImage, vec3(Texcoord, Index), 0).rgb;
	}
	ivec3 CellCoords = convertPointToGridIndex(vPosition);
	int Index = gridCoordToProbeIndex(CellCoords);
	vec2 Texcoord = (octEncode(vNormal) * 0.5 + 0.5);
	return texture(u_OutputOctRadianceImage, vec3(Texcoord, Index), 0).rgb;
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}  

void main()
{
	vec3 Albedo = vec3(1);
	vec3 Position = v2f_FragPosInWorldSpace;
	vec3 Normal = normalize(v2f_Normal);
	vec3 Diffuse = computePrefilteredIrradiance(Position,Normal);
	vec3 FragInViewSpace = vec3(u_ViewMatrix * vec4(Position,1));
	vec3 ViewDir = -normalize(FragInViewSpace);
	vec3 Specular = computeGlossyRay(Position,ViewDir,Normal);
	vec3 F0 = vec3(0.2,0.2,0.2);
	float Roughness = 0.2;
	vec3 F        = FresnelSchlickRoughness(max(dot(Normal, ViewDir), 0.0), F0, Roughness);
	vec2 EnvBRDF  = texture(u_BRDFLut, vec2(max(dot(Normal, ViewDir), 0.0), Roughness)).rg;
	vec3 LUT = (F * EnvBRDF.x + EnvBRDF.y);


	vec3 Color = (Diffuse + Specular * LUT) * Albedo;
	vec3 Mapped = vec3(1.0) - exp(-Color * u_Exposure);
	Mapped = pow(Mapped, vec3(1.0f / 2.2f));
	Color_ = vec4(Mapped, 1.0f);
}