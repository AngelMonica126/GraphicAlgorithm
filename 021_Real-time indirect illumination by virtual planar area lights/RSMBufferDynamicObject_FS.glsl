#version 430 core

//in  vec2 v2f_TexCoords;
in  vec3 v2f_WorldNormal;
in  vec3 v2f_WorldPos;

layout(location = 0) out vec4 RadiantFlux_;	//RadiantFlux其实没有必要存到纹理里，因为整张纹理都是一样的值，用uniform传进去就可以了
layout(location = 1) out vec4 NormalAndRoughness_;
layout(location = 2) out vec4 Position_;
layout(location = 3) out vec4 AlbedoAndMetallic_;	//newnew，由于GGX BRDF中辐射通过和Albedo不是线性关系，所以需要分开存储

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform vec3  u_LightColor = vec3(1);
uniform vec3  u_LightDir;
uniform int   u_RSMSize;
uniform int   u_VPLsCount;
uniform float u_RSMCameraAreaInWorldSpace;
//uniform sampler2D u_Texture;
uniform vec3  u_DiffuseColor;
//uniform sampler2D u_DiffuseTexture;
//uniform sampler2D u_RoughnessTexture;
//uniform sampler2D u_MetallicTexture;
uniform float u_Intensity;
void main()
{
	vec3 Normal = normalize(v2f_WorldNormal);
	float Roughness = 0.2f;
	NormalAndRoughness_ = vec4(Normal, Roughness);
	AlbedoAndMetallic_ = vec4(u_DiffuseColor, 0.0f);
	vec3 RadiantFlux = u_LightColor / u_VPLsCount * u_RSMCameraAreaInWorldSpace * u_Intensity;	
	RadiantFlux_ = vec4(RadiantFlux, 1.0f);
	Position_ = vec4(v2f_WorldPos, 1.0f);
}