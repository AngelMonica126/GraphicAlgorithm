#version 430 core

in  vec3 v2f_TexCoords;

layout (location = 0) out vec4 Albedo_;
uniform sampler2D u_EncodeTexture;

vec2 signNotZero(vec2 v) {
	return vec2((v.x >= 0.0) ? +1.0 : -1.0, (v.y >= 0.0) ? +1.0 : -1.0);
}

vec2 float32x3_to_oct(in vec3 v) {
	vec2 p = v.xy * (1.0 / (abs(v.x) + abs(v.y) + abs(v.z)));
	return (v.z <= 0.0) ? ((1.0 - abs(p.yx)) * signNotZero(p)) : p;
}

void main()
{	
	Albedo_ = texture(u_EncodeTexture, (float32x3_to_oct(v2f_TexCoords) + 1 ) / 2.0);
}