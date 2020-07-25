#version 430 core

in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D u_BlendTexture;
uniform sampler2D u_ColorTexture;
uniform float u_BlendFactor = 0.8;
void main()
{
	Color_ = vec4(texture(u_BlendTexture,v2f_TexCoords).rgb * u_BlendFactor + (1 - u_BlendFactor) * texture(u_ColorTexture,v2f_TexCoords).rgb , 1) ; 
}