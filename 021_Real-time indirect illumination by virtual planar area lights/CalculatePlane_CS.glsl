#version 430 core
#pragma optionNV (unroll all)	//暂时不知道有没有起作用

#define LOCAL_GROUP_SIZE 1
#define PI 3.1415926
#define Sqrt2 1.414213562
#define Delta 0.001
#define FLT_MAX 100000000.0

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;

struct SPlane	
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
layout (std430, binding = 7) buffer Normals
{
	vec4 u_Normals[];
};
uniform int u_Block;
//uniform sampler2D u_InputFirstImage;
uniform sampler2D u_PlaneCentreTexture;
uniform int u_MaxLevel;
uniform int u_RSMResolution; 
uniform vec3 u_LightDir;
uniform vec2 u_Size;

void getPlane(ivec2 vUV,out vec3 vPiont[4],highp vec3 vNormal,out float vDistThresh)
{	
	float InverseBlock = 1.0f / u_Block;
	float width = u_Size.x * 0.5f * InverseBlock;
	float height = u_Size.y * 0.5f * InverseBlock;
	int size = u_RSMResolution / u_Block;		
	vec3 point = texelFetch(u_PlaneCentreTexture, ivec2(vUV),0).xyz;
//	highp vec3 first = texelFetch(u_InputFirstImage, ivec2(vUV), u_MaxLevel).xyz / (size * size);
	vec3 lightDir = normalize(u_LightDir);
	vec3 Up = vec3(0,1,0);
	vec3 lookUp = -lightDir;
//	if(abs(dot(lookUp,vNormal)) > 0.99)
//	{
//		lookUp +=vec3(0.5,0.5,0.5);
//		lookUp = normalize(lookUp);
//	}
	vec3 standard = normalize(cross(Up, lookUp));
	vec3 w = vNormal;
	vec3 u = normalize(cross(lookUp, w));
	if(dot(u,standard) < 0)
		u = -u;
	vec3 v = normalize(cross(u, w));
	mat4x4 matrix = mat4x4(
		u.x,			 w.x,			  v.x ,			   0,
		u.y,			 w.y,			  v.y ,			   0,
		u.z,			 w.z,			  v.z ,			   0,
		-dot(point , u), -dot(point , w), -dot(point , v) ,1
	);
	vec4 p0 = vec4(width,  0,  -height, 1);
	vec4 p1 = vec4(width,  0,  height, 1);
	vec4 p2 = vec4(-width, 0,  height, 1);
	vec4 p3 = vec4(-width, 0, -height, 1);
	mat4 inverseMatrix = inverse(matrix);
	vPiont[0] = vec3(inverseMatrix * p0);
	vPiont[1] = vec3(inverseMatrix * p1);
	vPiont[2] = vec3(inverseMatrix * p2);
	vPiont[3] = vec3(inverseMatrix * p3);
	vDistThresh = width  / max(abs(dot(vNormal,-lightDir)),0.1);
}
void main()
{
	ivec2 fragPos = ivec2(gl_GlobalInvocationID.xy);
	int fragIndex = (fragPos.y * u_Block + fragPos.x);
	int Temp = u_RSMResolution / u_Block;
	float InverseRSMResolution = 1.0f / u_RSMResolution;
	vec2 Size = vec2(Temp, Temp);
	vec2 Proportion = Size * InverseRSMResolution;
	vec2 UVSize = vec2(1.0f / u_Block);
	vec2 BeginUV = fragPos * UVSize;
	highp vec3 Point[4];
	vec3 normalPlane = normalize(u_Normals[fragIndex].xyz);
	float distThresh;
	getPlane(fragPos, Point, normalPlane, distThresh);
	u_Planes[fragIndex].P0 = vec4(Point[0],1.0);
	u_Planes[fragIndex].P1 = vec4(Point[1],1.0);
	u_Planes[fragIndex].P2 = vec4(Point[2],1.0);
	u_Planes[fragIndex].P3 = vec4(Point[3],1.0);
	u_Planes[fragIndex].Normal = vec4(normalPlane,distThresh);
	u_Planes[fragIndex].ProportionAndUV = vec4(Proportion,BeginUV);
	u_Planes[fragIndex].Index = ivec4(-1);
}