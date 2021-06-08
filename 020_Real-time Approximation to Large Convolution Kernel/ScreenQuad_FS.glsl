#version 430 core
in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D u_Albedo;
void main()
{
	vec3 Color = texture(u_Albedo, v2f_TexCoords).rgb;
	Color_ = vec4(Color, 1.0f);
}