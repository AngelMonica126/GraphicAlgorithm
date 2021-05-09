#version 430 core

in  vec3 v2f_FragPosInViewSpace;
in  vec2 v2f_TexCoords;
in  vec3 v2f_Normal;
//in vec3 v2f_Tangent;
layout (location = 0) out vec4 AlbedoAndMetallic_;
layout (location = 1) out vec4 NormalAndDoubleRoughness_;
layout (location = 2) out vec4 Position_;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_NormalTexture;
uniform float u_Near = 0.1;
uniform float u_Far = 100.0f;


void main()
{
	float gamma = 2.2;
	vec3 diffuseColor = pow(texture(u_DiffuseTexture, v2f_TexCoords).rgb, vec3(gamma));	
	AlbedoAndMetallic_ = vec4(diffuseColor,1.0);
	float alpha = textureLod(u_DiffuseTexture, v2f_TexCoords,0).a;
	if(alpha != 1.0f)
		discard;
	NormalAndDoubleRoughness_ = vec4(v2f_Normal,0);
	Position_ = vec4(v2f_FragPosInViewSpace, 1);
}