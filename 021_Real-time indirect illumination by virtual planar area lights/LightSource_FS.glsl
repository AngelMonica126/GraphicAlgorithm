#version 430 core

in  vec2 v2f_TexCoords;
out vec4 Color_;

//uniform float u_Intensity = 1.0;
//uniform vec3 LightSourceColor = vec3(1, 1, 1);
uniform sampler2D u_tttttt;
uniform vec2 u_Proportion;
uniform vec2 u_UV;
void main()
{
	vec2 UV = v2f_TexCoords;
	UV *= u_Proportion;
	UV += u_UV;
	vec3 TexelColor = texture(u_tttttt, UV,5).xyz;
	vec3 mapped = vec3(1.0) - exp(-TexelColor * 2.0);
	mapped = pow(mapped, vec3(1.0f / 2.2f));
	Color_ = vec4(mapped, 1.0f);
}