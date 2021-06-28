#version 430 core

in  vec3 v2f_TexCoords;

layout (location = 0) out vec4 Albedo_;
uniform sampler2D u_EncodeTexture;

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
	Albedo_ = texture(u_EncodeTexture, (octDecode(v2f_TexCoords) + 1 ) / 2.0);
}