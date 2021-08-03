#version 430 core

in  vec3 v2f_FragPosInViewSpace;
in  vec2 v2f_TexCoords;
in  vec3 v2f_Normal;

layout (location = 0) out vec4 Albedo_;

const float PI = 3.1415926535897932384626433832795;
uniform vec3 u_Coef[16];
uniform vec3 u_DiffuseColor;
uniform sampler2D u_BRDFLut;


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

	float Basis[9];
	float x = v2f_Normal.x;
	float y = v2f_Normal.y;
	float z = v2f_Normal.z;
	float x2 = x * x;
	float y2 = y * y;
	float z2 = z * z;
    
    Basis[0] = 1.f / 2.f * sqrt(1.f / PI);
    Basis[1] = 2.0 / 3.0 * sqrt(3.f / (4.f * PI)) * z;
    Basis[2] = 2.0 / 3.0 * sqrt(3.f / (4.f * PI)) * y;
    Basis[3] = 2.0 / 3.0 * sqrt(3.f / (4.f * PI)) * x;
    Basis[4] = 1.0 / 4.0 * 1.f / 2.f * sqrt(15.f / PI) * x * z;
    Basis[5] = 1.0 / 4.0 * 1.f / 2.f * sqrt(15.f / PI) * z * y;
    Basis[6] = 1.0 / 4.0 * 1.f / 4.f * sqrt(5.f / PI) * (-x2 - z2 + 2 * y2);
    Basis[7] = 1.0 / 4.0 * 1.f / 2.f * sqrt(15.f / PI) * y * x;
    Basis[8] = 1.0 / 4.0 * 1.f / 4.f * sqrt(15.f / PI) * (x2 - z2);

	vec3 Diffuse = vec3(0,0,0);
	for (int i = 0; i < 9; i++)
		Diffuse += u_Coef[i] * Basis[i];


	vec3 F0 = vec3(0.2,0.2,0.2);
	float Roughness = 0.2;
	vec3 N = normalize(v2f_Normal);
	vec3 V = -normalize(v2f_FragPosInViewSpace);
	vec3 R = reflect(-V, N); 
	vec3 F        = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, Roughness);
	vec2 EnvBRDF  = texture(u_BRDFLut, vec2(max(dot(N, V), 0.0), Roughness)).rg;
	vec3 LUT = (F * EnvBRDF.x + EnvBRDF.y);

	x = R.x;
	y = R.y;
	z = R.z;
	x2 = x * x;
	y2 = y * y;
	z2 = z * z;
	Basis[0] = 1.f / 2.f * sqrt(1.f / PI);
    Basis[1] = sqrt(3.f / (4.f * PI)) * z;
    Basis[2] = sqrt(3.f / (4.f * PI)) * y;
    Basis[3] = sqrt(3.f / (4.f * PI)) * x;
    Basis[4] = 1.f / 2.f * sqrt(15.f / PI) * x * z;
    Basis[5] = 1.f / 2.f * sqrt(15.f / PI) * z * y;
    Basis[6] = 1.f / 4.f * sqrt(5.f / PI) * (-x2 - z2 + 2 * y2);
    Basis[7] = 1.f / 2.f * sqrt(15.f / PI) * y * x;
    Basis[8] = 1.f / 4.f * sqrt(15.f / PI) * (x2 - z2);

	vec3 Specular = vec3(0,0,0);
	for (int i = 0; i < 9; i++)
		Specular += u_Coef[i] * Basis[i];


	Albedo_ = vec4(1 * Diffuse + 1.0 * Specular * LUT,1.0);
}