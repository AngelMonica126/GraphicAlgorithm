#version 430 core

in vec3 v2f_FragPosInViewSpace;
in vec2 v2f_TexCoords;
in vec3 v2f_Normal;

out vec4 Color_;
uniform samplerCube u_BakeAlbedoTextures;
uniform vec3 u_LightPos;

void main()
{
	vec3 FragToLight = v2f_FragPosInViewSpace - u_LightPos;
	Color_ = vec4(texture(u_BakeAlbedoTextures, FragToLight).rgb,1) ;
}