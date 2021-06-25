#version 430 core

in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D u_IndirectTexture;
uniform sampler2D u_DirectTexture;
uniform sampler2D u_AlbedoTexture;
uniform float u_Exposure = 2.0f;
void main()
{
	vec3 Albedo = texture(u_AlbedoTexture, v2f_TexCoords).rgb;
	if(equal(Albedo,vec3(0,0,0)) == bvec3(1,1,1))
		Albedo = vec3(0.52, 0.77, 1);
	vec3 TexelColor = (texture(u_DirectTexture, v2f_TexCoords).rgb * 0.8f + 0.2f) * Albedo;
	vec3 Mapped = vec3(1.0) - exp(-TexelColor * u_Exposure);
	Mapped = pow(Mapped, vec3(1.0f / 2.2f));
	Color_ = vec4(Mapped, 1.0f);
}