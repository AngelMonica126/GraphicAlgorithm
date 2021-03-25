#version 430 core

in  vec3 v2f_FragPosInViewSpace;
in  vec2 v2f_TexCoords;
in  vec3 v2f_Normal;

layout (location = 0) out vec4 Albedo_;

const float PI = 3.1415926535897932384626433832795;
uniform vec3 u_Coef[16];
uniform vec3 u_DiffuseColor;

void main()
{	
	if((abs(v2f_Normal.x) < 0.0001f) && (abs(v2f_Normal.y) < 0.0001f) && (abs(v2f_Normal.z) < 0.0001f))
	{
		Albedo_ = vec4(0, 0, 0, 1);
		return;
	}
	vec3 Normal = normalize(v2f_Normal);
	
	float basis[16];
	float x = Normal.x;
	float y = Normal.y;
	float z = Normal.z;
	float x2 = x*x;
	float y2 = y*y;
	float z2 = z*z;
    
    basis[0] = 1.f / 2.f * sqrt(1.f / PI);
    basis[1] = sqrt(3.f / (4.f*PI))*z;
    basis[2] = sqrt(3.f / (4.f*PI))*y;
    basis[3] = sqrt(3.f / (4.f*PI))*x;
    basis[4] = 1.f / 2.f * sqrt(15.f / PI) * x * z;
    basis[5] = 1.f / 2.f * sqrt(15.f / PI) * z * y;
    basis[6] = 1.f / 4.f * sqrt(5.f / PI) * (-x2 - z2 + 2 * y2);
    basis[7] = 1.f / 2.f * sqrt(15.f / PI) * y * x;
    basis[8] = 1.f / 4.f * sqrt(15.f / PI) * (x2 - z2);
    basis[9] = 1.f / 4.f*sqrt(35.f / (2.f*PI))*(3 * x2 - z2)*z;
    basis[10] = 1.f / 2.f*sqrt(105.f / PI)*x*z*y;
    basis[11] = 1.f / 4.f*sqrt(21.f / (2.f*PI))*z*(4 * y2 - x2 - z2);
    basis[12] = 1.f / 4.f*sqrt(7.f / PI)*y*(2 * y2 - 3 * x2 - 3 * z2);
    basis[13] = 1.f / 4.f*sqrt(21.f / (2.f*PI))*x*(4 * y2 - x2 - z2);
    basis[14] = 1.f / 4.f*sqrt(105.f / PI)*(x2 - z2)*y;
    basis[15] = 1.f / 4.f*sqrt(35.f / (2 * PI))*(x2 - 3 * z2)*x;

	vec3 color = vec3(0,0,0);
	for (int i = 0; i < 16; i++)
		color += u_Coef[i] * basis[i];

	Albedo_ = vec4(color,1.0);
}