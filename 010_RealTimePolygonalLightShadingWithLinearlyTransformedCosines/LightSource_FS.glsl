#version 430 core

in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform float u_Intensity = 1.0;
uniform sampler2D u_LightSourceTexture;
uniform vec3 LightSourceColor = vec3(1, 1, 1);

void main()
{
	//Color_ = vec4(u_Intensity * LightSourceColor, 1.0);
	//Color_ = u_Intensity * texture(u_LightSourceTexture, vec2(v2f_TexCoords.x, 1 - v2f_TexCoords.y));
	Color_ = u_Intensity * texture(u_LightSourceTexture, v2f_TexCoords);
}