#version 430 core

in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D u_ShadingTexture;

void main()
{
	//Color_ = texture(u_Texture2D, v2f_TexCoords);
	vec3 Albedo = texture(u_ShadingTexture, v2f_TexCoords).rgb;
	Color_ = vec4(Albedo, 1.0f);
}