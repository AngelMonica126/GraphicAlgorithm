#version 430 core
#pragma optionNV (unroll all)	
#define NUM 300
#define LOCAL_GROUP_SIZE 16

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;

layout(rgba32f ,binding = 0) uniform writeonly image2DArray u_OutputOctRadianceImage;
layout(rgba32f ,binding = 1) uniform writeonly image2DArray u_OutputOctNormalImage;
layout(rg32f ,binding = 2) uniform writeonly image2DArray u_OutputOctChebyshevsImage;

uniform int u_BakeResolution;
uniform int u_ImageWidthNum;
uniform int u_ImageHeightNum;
uniform samplerCubeArray u_RadianceImages;
uniform samplerCube u_NormalImage[288];
uniform samplerCube u_ChebyshevsImage[288];
uniform int u_Index;
float signNotZero(in float k) {
    return k >= 0.0 ? 1.0 : -1.0;
}

vec2 signNotZero(in vec2 v) {
    return vec2( signNotZero(v.x), signNotZero(v.y));
}

int getImageIndex(ivec2 vPos, out vec2 vUV)
{
	ivec2 Index = vPos / u_BakeResolution;
	vUV = vec2((vPos.x % u_BakeResolution) / float(u_BakeResolution),(vPos.y % u_BakeResolution) / float(u_BakeResolution));
	return Index.y * u_ImageWidthNum + Index.x;
}

vec3 octEncode(vec2 vTextCoord)
{
	vec3 Direction = vec3(vTextCoord.x, vTextCoord.y, 1.0 - abs(vTextCoord.x) - abs(vTextCoord.y));
    if (Direction.z < 0.0) {
        Direction.xy = (1.0 - abs(Direction.yx)) * signNotZero(Direction.xy);
    }
    return normalize(Direction);
}


void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	vec2 UV;
	int Index = getImageIndex(FragPos, UV);
	vec3 Dir = octEncode(UV);
	imageStore(u_OutputOctRadianceImage, ivec3(FragPos,u_Index), vec4(texture(u_RadianceImages,vec4(Dir,0)).xyz, 1.0));
	imageStore(u_OutputOctNormalImage, ivec3(FragPos,u_Index), vec4(texture(u_NormalImage[Index],Dir).xyz, 1.0));
	imageStore(u_OutputOctChebyshevsImage, ivec3(FragPos,u_Index), vec4(texture(u_ChebyshevsImage[Index],Dir).xy,0,0));
}