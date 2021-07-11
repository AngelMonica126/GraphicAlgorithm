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
const float MinThickness = 0.03; // meters
const float MaxThickness = 0.50; // meters

struct Ray
{
    vec3 m_Origin;
    vec3 m_Dir;
};

void minSwap(inout float a, inout float b) {
	float temp = min(a, b);
	b = max(a, b);
	a = temp;
}


void sort(inout vec3 v) {
	minSwap(v.x, v.y);
	minSwap(v.y, v.z);
	minSwap(v.x, v.y);
}

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

int nearestProbeIndices(vec3 vPosition) 
{
	float MinDis = 10.0f;
	int NearestIndex = gridCoordToProbeIndex(convertPointToGridIndex(vPosition));
	for(int i = 0; i < 8 ;i++)
	{
		ivec3 Offset = ivec3(i, i >> 1, i >> 2) & ivec3(1);
		ivec3 CellCoords = convertPointToGridIndex(vPosition) + Offset;
		int Index = gridCoordToProbeIndex(CellCoords);
		vec3 ProbePos = gridCoordToPosition(CellCoords);		vec3 ProbeToPoint = vPosition - ProbePos;
		float DistToProbe = length(ProbeToPoint);		if (DistToProbe < MinDis) {
			MinDis = DistToProbe;
			NearestIndex = i;
		}	}
	return NearestIndex;
}

ivec3 probeIndexToGridCoord(int vIndex) {
	ivec3 iPos;
	iPos.x = vIndex & (u_CellNum.x - 1);
	iPos.y = (vIndex & ((u_CellNum.x * u_CellNum.y) - 1)) >> findMSB(u_CellNum.x);
	iPos.z = vIndex >> findMSB(u_CellNum.x * u_CellNum.y);
	return iPos;
}

void computeRaySegments(vec3 vOrigin,vec3 vDirectionFrac,float tMin,float tMax,out float  vBoundaryTs[5]) 
{
	vBoundaryTs[0] = tMin;

	vec3 t = vOrigin * -vDirectionFrac;
	sort(t);
	for (int i = 0; i < 3; ++i) {
		vBoundaryTs[i + 1] = clamp(t[i], tMin, tMax);
	}
	vBoundaryTs[4] = tMax;
}

float distanceToIntersection(Ray R, vec3 v) {
	float numer;
	float denom = v.y * R.m_Dir.z - v.z * R.m_Dir.y;
	if (abs(denom) > 0.1) {
		numer = R.m_Origin.y * R.m_Dir.z - R.m_Origin.z * R.m_Dir.y;
	}
	else {
		numer = R.m_Origin.x * R.m_Dir.y - R.m_Origin.y * R.m_Dir.x;
		denom = v.x * R.m_Dir.y - v.y * R.m_Dir.x;
	}
	return numer / denom;
}

int highResolutionTraceOneRaySegment(Ray vProbeSpaceRay,vec2 vStartTexCoord,vec2 vEndTexCoord,int vProbeIndex,out float tMin,out float tMax,out vec2 vHitProbeTexCoord) {

	vec2 TexCoordDelta = vEndTexCoord - vStartTexCoord;
	float TexCoordDistance = length(TexCoordDelta);
	vec2 TexCoordDirection = TexCoordDelta * (1.0 / TexCoordDistance);
	float TexCoordStep = 1.0f / u_BakeResolution;
//	float TexCoordStep = 1,0f / u_BakeResolution * (TexCoordDistance / maxComponent(abs(texCoordDelta)));

	vec3 DirectionFromProbeBefore = octDecode(vStartTexCoord * 2.0 - 1.0);
	float DistanceFromProbeToRayBefore = max(0.0, distanceToIntersection(vProbeSpaceRay, DirectionFromProbeBefore));

	for (float d = 0.0f; d <= TexCoordDistance; d += TexCoordStep) 
	{
		vec2 TexCoord = (TexCoordDirection * min(d + TexCoordStep * 0.5, TexCoordDistance)) + vStartTexCoord;
		float DistanceFromProbeToSurface = texelFetch(u_OutputOctChebyshevsImage,ivec3(u_BakeResolution * TexCoord, vProbeIndex), 0).z;
		vec3 DirectionFromProbe = octDecode(TexCoord * 2.0 - 1.0);

		vec2 texCoordAfter = (TexCoordDirection * min(d + TexCoordStep, TexCoordDistance)) + vStartTexCoord;
		vec3 DirectionFromProbeAfter = octDecode(texCoordAfter * 2.0 - 1.0);
		float DistanceFromProbeToRayAfter = max(0.0, distanceToIntersection(vProbeSpaceRay, DirectionFromProbeAfter));
		float MaxDistFromProbeToRay = max(DistanceFromProbeToRayBefore, DistanceFromProbeToRayAfter);

		if (MaxDistFromProbeToRay >= DistanceFromProbeToSurface) 
		{
			float MinDistFromProbeToRay = min(DistanceFromProbeToRayBefore, DistanceFromProbeToRayAfter);
			float DistanceFromProbeToRay = (MinDistFromProbeToRay + MaxDistFromProbeToRay) * 0.5;

			vec3 ProbeSpaceHitPoint = DistanceFromProbeToSurface * DirectionFromProbe;
			float DistAlongRay = dot(ProbeSpaceHitPoint - vProbeSpaceRay.m_Origin, vProbeSpaceRay.m_Dir);
			vec3 Normal = octDecode(texelFetch(u_OutputOctNormalImage, ivec3(u_BakeResolution * TexCoord, vProbeIndex), 0).xy  * 2.0 - 1.0);
			float SurfaceThickness = MinThickness
				+ (MaxThickness - MinThickness) *
				max(dot(vProbeSpaceRay.m_Dir, DirectionFromProbe), 0.0) *
				(2 - abs(dot(vProbeSpaceRay.m_Dir, Normal))) *
				clamp(DistAlongRay * 0.1, 0.05, 1.0);
			if ((MinDistFromProbeToRay < DistanceFromProbeToSurface + SurfaceThickness) && (dot(Normal, vProbeSpaceRay.m_Dir) < 0)) {
				tMax = DistAlongRay;
				vHitProbeTexCoord = TexCoord;
				return TRACE_RESULT_HIT;
			}
			else {
				vec3 ProbeSpaceHitPointBefore = DistanceFromProbeToRayBefore * DirectionFromProbeBefore;
				float DistAlongRayBefore = dot(ProbeSpaceHitPointBefore - vProbeSpaceRay.m_Origin, vProbeSpaceRay.m_Dir);
				tMin = max(tMin, min(DistAlongRay, DistAlongRayBefore));
				return TRACE_RESULT_UNKNOWN;
			}
		}
		DistanceFromProbeToRayBefore = DistanceFromProbeToRayAfter;
	} 
	return TRACE_RESULT_MISS;
}

int traceOneRaySegment(Ray vProbeSpaceRay,float t0,float t1,int vProbeIndex,out float tMin,out float tMax,out vec2 vHitProbeTexCoord) {

	vec3 ProbeSpaceStartPoint = vProbeSpaceRay.m_Origin + vProbeSpaceRay.m_Dir * (t0 + RayBumpEpsilon);
	vec3 ProbeSpaceEndPoint = vProbeSpaceRay.m_Origin + vProbeSpaceRay.m_Dir * (t1 - RayBumpEpsilon);

	if (dot(ProbeSpaceStartPoint,ProbeSpaceStartPoint) < 0.001) 
		ProbeSpaceStartPoint = vProbeSpaceRay.m_Dir;

	vec2 StartOctCoord = octEncode(normalize(ProbeSpaceStartPoint));
	vec2 EndOctCoord = octEncode(normalize(ProbeSpaceEndPoint));

	vec2 Texcoord = StartOctCoord * 0.5 + 0.5;
	vec2 SegmentEndTexCoord = EndOctCoord * 0.5 + 0.5;

	return highResolutionTraceOneRaySegment(vProbeSpaceRay, Texcoord, SegmentEndTexCoord, vProbeIndex, tMin, tMax, vHitProbeTexCoord);
}


int traceOneProbeOct(int vIndex,  Ray vRay, inout float tMin, inout float tMax, inout vec2 vHitProbeTexCoord) 
{
	const float degenerateEpsilon = 0.001; 

	vec3 ProbeOrigin = gridCoordToPosition(probeIndexToGridCoord(vIndex));

	Ray ProbeSpaceRay;
	ProbeSpaceRay.m_Origin = vRay.m_Origin - ProbeOrigin;
	ProbeSpaceRay.m_Dir = vRay.m_Dir;

	float BoundaryTs[5];
	computeRaySegments(ProbeSpaceRay.m_Origin, vec3(1.0) / ProbeSpaceRay.m_Dir, tMin, tMax, BoundaryTs);

	// for each open interval (t[i], t[i + 1]) that is not degenerate
	for (int i = 0; i < 4; ++i) {
		if (abs(BoundaryTs[i] - BoundaryTs[i + 1]) >= degenerateEpsilon) 
		{
			int result = traceOneRaySegment(ProbeSpaceRay, BoundaryTs[i], BoundaryTs[i + 1], vIndex, tMin, tMax, vHitProbeTexCoord);
			switch (result) {
			case TRACE_RESULT_HIT:
				// Hit!            
				return TRACE_RESULT_HIT;

			case TRACE_RESULT_UNKNOWN:
				// Failed to find anything conclusive
				return TRACE_RESULT_UNKNOWN;
			} // switch
		} // if 
	} // For each segment

	return TRACE_RESULT_MISS;
}

int trace(Ray vRay,inout float tMax, out vec2 vHitProbeTexCoord, out int vHitProbeIndex, bool vFillHoles) 
{
	vHitProbeIndex = -1;
	int BaseIndex = gridCoordToProbeIndex(convertPointToGridIndex(vRay.m_Origin));
	int i = nearestProbeIndices(vRay.m_Origin);
	int ProbesLeft = 8;
	float tMin = 0.0f;
	int Result = TRACE_RESULT_UNKNOWN;
	for(int i = 0; i < 8 ;i++)
	{
		ivec3 Offset = ivec3(i, i >> 1, i >> 2) & ivec3(1);
		ivec3 CellCoords = convertPointToGridIndex(vRay.m_Origin) + Offset;
		int Index = gridCoordToProbeIndex(CellCoords);
		Result = traceOneProbeOct(Index,vRay, tMin, tMax, vHitProbeTexCoord);
	
		if (Result != TRACE_RESULT_UNKNOWN) {

			if (Result == TRACE_RESULT_HIT) {
				vHitProbeIndex = Index;
			}
			break;
		}
	}
	if((vHitProbeIndex == -1) && vFillHoles) 
	{
		vec3 ignore;
		vHitProbeIndex = BaseIndex;
		vHitProbeTexCoord = octEncode(vRay.m_Dir) * 0.5 + 0.5;

		float ProbeDistance = texelFetch(u_OutputOctChebyshevsImage, ivec3(ivec2(vHitProbeTexCoord * u_BakeResolution), vHitProbeIndex), 0).r;
		if (ProbeDistance < 10000) {
//			vec3 hitLocation = probeLocation(lightFieldSurface, hitProbeIndex) + worldSpaceRay.direction * probeDistance;
//			tMax = length(worldSpaceRay.origin - hitLocation);
			return TRACE_RESULT_HIT;
		}
	}
	return Result;
}


vec3 computeGlossyRay(vec3 vPosition, vec3 ViewDir, vec3 vNormal) {
	vec3 wi = normalize(reflect(-ViewDir, vNormal));
	Ray WorldSpaceRay = Ray(vPosition + wi * RayBumpEpsilon, wi);

	float   HitDistance = 10000;
	vec2  HitProbeTexCoord;
	int  ProbeIndex;
	int Result = trace(WorldSpaceRay, HitDistance, HitProbeTexCoord, ProbeIndex, true);
	if (Result == TRACE_RESULT_UNKNOWN) {
		return vec3(1,0,0);
	}
	else {
		return texture(u_OutputOctRadianceImage, vec3(HitProbeTexCoord, ProbeIndex), 0).rgb;
	}

}



void main()
{

	vec3 Albedo = u_DiffuseColor;
	vec3 Position = v2f_FragPosInWorldSpace;
	vec3 Normal = normalize(v2f_Normal);
	vec3 Diffuse = computePrefilteredIrradiance(Position,Normal);
	vec3 FragInViewSpace = vec3(u_ViewMatrix * vec4(Position,1));
	vec3 ViewDir = -normalize(FragInViewSpace);
	vec3 Specular = computeGlossyRay(Position,ViewDir,Normal);
	vec3 Color = (Diffuse + Specular) * Albedo;
	vec3 Mapped = vec3(1.0) - exp(-Color * u_Exposure);
	Mapped = pow(Mapped, vec3(1.0f / 2.2f));
	Color_ = vec4(Mapped, 1.0f);

}