#version 430 core

in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D u_Texture;
uniform float u_Exposure = 2.0f;

void main()
{
	vec3 mapped = vec3(1.0) - exp(-(texture(u_Texture, v2f_TexCoords).rgb) * u_Exposure);
	mapped = pow(mapped, vec3(1.0f / 2.2f));
	Color_ = vec4(mapped,1);
}
