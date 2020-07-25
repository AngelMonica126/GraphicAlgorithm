#version 430 core

in  vec3 v2f_FragPosInViewSpace;
in  vec2 v2f_TexCoords;
in  vec3 v2f_Normal;

layout (location = 0) out vec3 Flux_;;
layout (location = 1) out vec3 Normal_;
layout (location = 2) out vec3 Position_;

uniform sampler2D u_DiffuseTexture;
uniform vec3 u_LightColor = vec3(1);

void main()
{
	vec3 TexelColor = texture(u_DiffuseTexture, v2f_TexCoords).rgb;
	//TexelColor = pow(TexelColor, vec3(2.2f));
	vec3 VPLFlux = u_LightColor * TexelColor;
	Flux_ = VPLFlux;
	Normal_ = v2f_Normal;
	Position_ = v2f_FragPosInViewSpace;
}