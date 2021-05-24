#version 430 core

in  vec3 v2f_FragPosInViewSpace;
//in  vec2 v2f_TexCoords;
in  vec3 v2f_Normal;

layout(location = 0) out vec4 AlbedoAndMetallic_;
layout(location = 1) out vec4 NormalAndDoubleRoughness_;
layout(location = 2) out vec4 Position_;

//uniform sampler2D u_Texture;
//uniform sampler2D u_RoughnessTexture;
//uniform sampler2D u_MetallicTexture;
uniform vec3 u_DiffuseColor;

void main()
{
	//vec3 Albedo = texture(u_DiffuseTexture, v2f_TexCoords).rgb;
	//Albedo = pow(Albedo, vec3(2.2f));

	vec3 Albedo = u_DiffuseColor;
	//vec3 Albedo = texture(u_Texture, v2f_TexCoords).rgb;
//	if(v2f_TexCoords.y > 0.5)
//	Albedo *= 5;
	AlbedoAndMetallic_ = vec4(u_DiffuseColor, 0.0f);
	vec3 Normal = normalize(v2f_Normal);
	float Roughness = 0.2f;
	NormalAndDoubleRoughness_ = vec4(Normal, Roughness * Roughness);
	Position_ = vec4(v2f_FragPosInViewSpace, 1.0f);
}