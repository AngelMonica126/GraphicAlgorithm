#version 430 core

in  vec3 v2f_FragPosInViewSpace;
in  vec2 v2f_TexCoords;
in  vec3 v2f_Normal;

layout (location = 0) out vec4 Albedo_;
layout (location = 1) out vec4 Normal_;
layout (location = 2) out vec4 Position_;

uniform sampler2D u_DiffuseTexture;

void main()
{
	vec3 Albedo = texture(u_DiffuseTexture, v2f_TexCoords).rgb;
	Albedo = pow(Albedo, vec3(2.2f));
	Albedo_ = vec4(Albedo, 1.0f);
	Normal_ = vec4(v2f_Normal, 1.0f);
	Position_ = vec4(v2f_FragPosInViewSpace, 1.0f);
}