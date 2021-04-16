#version 460  core
in  vec2 v2f_TexCoords;

layout (location = 0) out vec4 Albedo_;


uniform sampler2D u_DiffuseTexture;
uniform vec4 u_DiffuseColor;

void main()
{
	vec3  Albedo = texture(u_DiffuseTexture, v2f_TexCoords).xyz;
	Albedo_ = vec4(Albedo,1.0);

}