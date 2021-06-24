#version 430 core

in  vec2 v2f_TexCoords;
in  vec3 test;
layout (location = 0) out vec4 AlbedoAndMetallic_;

uniform mat4 u_TransposeInverseViewModelMatrix;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_NormalTexture;
uniform float u_Near = 0.1;
uniform float u_Far = 1000.0f;

void main()
{
	float gamma = 2.2;
	vec3 diffuseColor = pow(texture(u_DiffuseTexture, v2f_TexCoords).rgb, vec3(gamma));	
	vec3 mapped = vec3(1.0) - exp(-diffuseColor);
	mapped = pow(mapped, vec3(1.0f / 2.2f));
	AlbedoAndMetallic_ = vec4(mapped,1.0);
	float alpha = textureLod(u_DiffuseTexture, v2f_TexCoords,0).a;
	if(alpha != 1.0f)
		discard;
}