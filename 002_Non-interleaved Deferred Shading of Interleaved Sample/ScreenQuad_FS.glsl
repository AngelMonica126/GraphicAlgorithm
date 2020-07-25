#version 430 core

in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D u_Texture2D;
uniform sampler2D u_Albedo;

void main()
{
	vec3 Albedo = texture(u_Albedo, v2f_TexCoords).rgb;
	Color_ = vec4(texture(u_Texture2D, v2f_TexCoords).rgb * Albedo + 0.1 * Albedo, 1.0f);
//	Color_ = vec4(texture(u_Texture2D, v2f_TexCoords).rgb,1.0f);
	//Color_ = texture(u_Texture2D, v2f_TexCoords);
}