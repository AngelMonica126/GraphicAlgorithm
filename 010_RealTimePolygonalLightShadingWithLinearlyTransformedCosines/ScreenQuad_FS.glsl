#version 430 core

in  vec2 v2f_TexCoord;
out vec4 Color_; 

uniform sampler2D u_HDRTexture;

void main()
{
	//Color_ = texture(u_HDRTexture, v2f_TexCoord);
	//Color_ = vec4(0, 1, 0, 1);
	vec3 HDRColor = texture(u_HDRTexture, v2f_TexCoord).rgb;
	// -------------------------------ReinhardÉ«µ÷Ó³Éä---------------------------------
	vec3 ReinhardMappedColor = HDRColor / (HDRColor + vec3(1.0));

	vec3 GammaedColor = pow(ReinhardMappedColor, vec3(1.0 / 2.2));
	Color_ = vec4(GammaedColor, 1.0);
}