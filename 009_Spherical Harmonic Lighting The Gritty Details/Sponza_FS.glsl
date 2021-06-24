#version 430 core

in  vec3 v2f_FragPosInViewSpace;
in  vec2 v2f_TexCoords;
in  vec3 v2f_Normal;

layout (location = 0) out vec4 Albedo_;

const float PI = 3.1415926535897932384626433832795;
uniform vec3 u_Coef[16];
uniform vec3 u_DiffuseColor;
uniform sampler2D u_BRDFLut;

uniform samplerCube u_Texture1;
uniform samplerCube u_Texture2;
uniform samplerCube u_Texture3;


vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}  

void main()
{	
	if((abs(v2f_Normal.x) < 0.0001f) && (abs(v2f_Normal.y) < 0.0001f) && (abs(v2f_Normal.z) < 0.0001f))
	{
		Albedo_ = vec4(0, 0, 0, 1);
		return;
	}
	vec3 F0 = vec3(0.2,0.2,0.2);
	float roughness = 0.2;
	vec3 N = normalize(v2f_Normal);
	vec3 V = -normalize(v2f_FragPosInViewSpace);
	vec3 F        = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	vec2 envBRDF  = texture(u_BRDFLut, vec2(max(dot(N, V), 0.0), roughness)).rg;
	vec3 specular = (F * envBRDF.x + envBRDF.y);

	vec3 Basis1 = texture(u_Texture1,N).xyz;
	vec3 Basis2 = texture(u_Texture2,N).xyz;
	vec3 Basis3 = texture(u_Texture3,N).xyz;

	float Basis[9];
	Basis[0]  = Basis1.x / PI;
	Basis[1]  = Basis1.y / PI;
	Basis[2]  = Basis1.z / PI;
	Basis[3]  = Basis2.x / PI;
	Basis[4]  = Basis2.y / PI;
	Basis[5]  = Basis2.z / PI;
	Basis[6]  = Basis3.x / PI;
	Basis[7]  = Basis3.y / PI;
	Basis[8]  = Basis3.z / PI;

	vec3 color = vec3(0,0,0);
	for (int i = 0; i < 9; i++)
		color += u_Coef[i] * Basis[i];

	Albedo_ = vec4(1 * color + 0.0 * specular,1.0);
}