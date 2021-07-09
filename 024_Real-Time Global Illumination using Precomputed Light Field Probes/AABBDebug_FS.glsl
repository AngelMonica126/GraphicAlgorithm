#version 430 core

in vec3 v2f_FragPosInViewSpace;
in vec2 v2f_TexCoords;
in vec3 v2f_Normal;

out vec4 Color_;
uniform sampler2DArray u_OctRadianceTextures;
uniform vec3 u_LightPos;
uniform int u_Index = 0;


float signNotZero(in float k) {
    return k >= 0.0 ? 1.0 : -1.0;
}

vec2 signNotZero(in vec2 v) {
    return vec2( signNotZero(v.x), signNotZero(v.y) );
}

vec2 octDecode(vec3 vDirection)
{
	float NormalTotal = abs(vDirection.x)+abs(vDirection.y)+abs(vDirection.z);
	vec2 Result = vDirection.xy * (1.0 / NormalTotal);
	if(vDirection.z < 0.0)
		Result =(1.0 - abs(Result.yx)) * signNotZero(Result.xy);
	return Result;
}

void main()
{
	vec3 FragToLight = normalize(v2f_FragPosInViewSpace);
	vec2 UV = (octDecode(FragToLight) + 1) / 2.0;
	vec3 Texcoord = vec3(UV, u_Index);
	Color_ = vec4(texture(u_OctRadianceTextures, Texcoord).rgb,1);
}