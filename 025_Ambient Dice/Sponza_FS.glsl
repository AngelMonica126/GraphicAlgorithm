#version 430 core
#define kT 0.618034f 
in  vec3 v2f_FragPosInViewSpace;
in  vec2 v2f_TexCoords;
in  vec3 v2f_Normal;

layout (location = 0) out vec4 Albedo_;

const float PI = 3.1415926535897932384626433832795;
uniform vec3 u_Coef[12];
uniform vec3 u_DiffuseColor;
uniform sampler2D u_BRDFLut;
const vec3 m_Vertices[12] = {
							normalize(vec3(1.0, kT, 0.0)),
							normalize(vec3(-1.0, kT, 0.0)),
							normalize(vec3(1.0, -kT, -0.0)),
							normalize(vec3(-1.0, -kT, 0.0)),
							normalize(vec3(0.0, 1.0, kT)),
							normalize(vec3(-0.0, -1.0, kT)),
							normalize(vec3(0.0, 1.0, -kT)),
							normalize(vec3(0.0, -1.0, -kT)),
							normalize(vec3(kT, 0.0, 1.0)),
							normalize(vec3(-kT, 0.0, 1.0)),
							normalize(vec3(kT, -0.0, -1.0)),
							normalize(vec3(-kT, -0.0, -1.0))
						};
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
	vec3 Normal = normalize(v2f_Normal);
	vec3 Diffuse = vec3(0,0,0);
	for(int i = 0; i < 12; i++)
	{
		float Cos = dot(Normal, m_Vertices[i]);
		Diffuse += (0.05981 + 0.12918 * Cos + 0.07056 * Cos * Cos) * u_Coef[i];
	}

	vec3 F0 = vec3(1);
	float Roughness = 0.02;
	vec3 N = normalize(v2f_Normal);
	vec3 V = -normalize(v2f_FragPosInViewSpace);
	vec3 R = normalize(reflect(-V, N)); 
	vec3 F        = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, Roughness);
	vec2 EnvBRDF  = texture(u_BRDFLut, vec2(max(dot(N, V), 0.0), Roughness)).rg;
	vec3 LUT = (F * EnvBRDF.x + EnvBRDF.y);
	vec3 Specular = vec3(0,0,0);
	for(int i = 0; i < 12; i++)
	{
		float Cos = dot(R, m_Vertices[i]);
		if (Cos < 0)
			continue;
		Cos *= Cos;
		Specular += (0.7f * 0.5f * Cos + 0.3f * 5.0f / 6.0f * Cos * Cos) * u_Coef[i];
	}

	Albedo_ = vec4( Diffuse + Specular * LUT,1.0);
}