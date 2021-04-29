#version 430 core

in  vec2 v2f_TexCoords;
in  vec3 v2f_WorldNormal;
in  vec3 v2f_WorldPos;

layout (location = 0) out vec4 RadiantFlux_;	
layout (location = 1) out vec4 NormalAndRoughness_;
layout (location = 2) out vec4 Position_;
layout (location = 3) out vec4 AlbedoAndMetallic_;	

uniform vec3  u_LightColor = vec3(1);
uniform int   u_RSMSize;
uniform int   u_VPLsCount;
uniform float u_RSMCameraAreaInWorldSpace;
uniform sampler2D u_DiffuseTexture;
uniform float u_Intensity;
void main()
{
	NormalAndRoughness_ = vec4(v2f_WorldNormal, 0);
	float gamma = 2.2;
	vec3 diffuseColor = pow(texture(u_DiffuseTexture, v2f_TexCoords,0).rgb, vec3(gamma));	
	AlbedoAndMetallic_ = vec4(diffuseColor,1.0);
	float alpha = textureLod(u_DiffuseTexture, v2f_TexCoords,0).a;
	if(alpha != 1.0f)
		discard;
	vec3 RadiantFlux = u_LightColor / u_VPLsCount * u_RSMCameraAreaInWorldSpace * u_Intensity * diffuseColor;	
	RadiantFlux_ = vec4(RadiantFlux, 1.0f);
	Position_ = vec4(v2f_WorldPos, 1.0f);
}