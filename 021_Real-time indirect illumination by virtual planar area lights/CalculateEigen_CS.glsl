#version 430 core
#pragma optionNV (unroll all)	//暂时不知道有没有起作用

#define LOCAL_GROUP_SIZE 1
#define PI 3.14159265358979323846264338327950288
#define EPSINON 0.00001
#define Delta 0.001
#define FLT_MAX 100000000.0

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;
uniform int u_Block;
uniform sampler2D u_InputFirstImage;
uniform sampler2D u_InputSecondImage;
uniform sampler2D u_RSMPositionTexture;
uniform int u_MaxLevel;
uniform vec3 u_LightDir;
layout (std430, binding = 7) buffer Normals
{
	vec4 u_Normals[];
};

highp float getEigenvalues(mat3x3 vMatrix)
{
	highp float b1 = -vMatrix[0][0] - vMatrix[1][1] - vMatrix[2][2];
	highp float c1 = vMatrix[0][0] * vMatrix[1][1] + vMatrix[0][0] * vMatrix[2][2] + vMatrix[1][1] * vMatrix[2][2] - vMatrix[0][1] * vMatrix[0][1] - vMatrix[0][2] * vMatrix[0][2] - vMatrix[1][2] * vMatrix[1][2];
	highp float d1 = -determinant(vMatrix);
	highp float M = max(max(max(1.0, abs(b1)), pow(abs(c1), 0.5)), pow(abs(d1), 1.0 / 3.0));
	highp float b = b1 / M;
	highp float c = c1 / (M * M);
	highp float d = d1 / (M * M * M);
	highp float p = (3 * c - b * b) / 3.0;
	highp float q = (2 * pow(b, 3) - 9 * b * c + 27 * d) / 27.0;
	highp float D = q * q / 4.0 + pow(p, 3) / 27.0;
	highp float T = asin(pow(27.0 * D / pow(p, 3), 0.5));
	highp float theta = (q > 0) ? PI - T : T;
	highp float x1, x2, x3;
	if (D >= EPSINON && D <= EPSINON)
	{
		x1 = 2 * pow(-(q / 2.0), 1.0 / 3.0) - b / 3.0;
		x2 = x3 = pow((q / 2.0), 1.0 / 3.0) - b / 3.0;
	}
	else
	{
		x1 = 2 * pow(-(p / 3.0), 0.5) * cos(theta / 3.0) - b / 3.0;
		x2 = 2 * pow(-(p / 3.0), 0.5) * cos((2 * PI + theta) / 3.0) - b / 3.0;
		x3 = 2 * pow(-(p / 3.0), 0.5) * cos((4 * PI + theta) / 3.0) - b / 3.0;
	}
	return max(max(x1 * M, x2 * M) ,x3 * M);
}
vec3 getEigenvector(mat3x3 vMatrix)
{
	double d[3][3];
	for(int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			d[i][j] = vMatrix[i][0] * vMatrix[j][0] + vMatrix[i][1] * vMatrix[j][1] + vMatrix[i][2] * vMatrix[j][2];
	double S[2];
	for (int k = 1; k < 3; k++)
		S[k - 1] = pow(vMatrix[0][0] * vMatrix[1][k] - vMatrix[1][0] * vMatrix[0][k], 2) + pow(vMatrix[0][0] * vMatrix[2][k] - vMatrix[2][0] * vMatrix[0][k], 2) + pow(vMatrix[1][0] * vMatrix[2][k] - vMatrix[2][0] * vMatrix[1][k], 2);
	double x, y, z;
	vec3 vec;
	if (S[0] >= S[1])
	{
		mat2 mat = mat2(
			d[0][0], d[0][1],
			d[0][1], d[1][1]
		);
		mat = inverse((mat));
		vec2 res = mat * vec2(-d[0][2], -d[1][2]);
		vec = vec3(res.x, res.y, 1);
	}
	else
	{ 
		mat2 mat = mat2(
			d[0][0], d[2][0],
			d[0][2], d[2][2]
		);
		mat = inverse((mat));
		vec2 res = mat * vec2(-d[0][1], -d[1][2]);
		vec = vec3(res.x, 1, res.y);
	}
	vec3 lightDir = normalize(u_LightDir);
	vec = dot(lightDir,normalize(vec)) > 0 ? -vec : vec;
	return vec;
}
void main()
{
	ivec2 fragPos = ivec2(gl_GlobalInvocationID.xy);
	int fragIndex = (fragPos.y * u_Block + fragPos.x);
//	fragPos = ivec2(fragPos.y,fragPos.x);
	vec3 first = vec3(0); 
	vec3 second = vec3(0); 
//	for(int i = 0 ; i < u_Block; i++)
	{
		first  = texelFetch(u_InputFirstImage, ivec2(fragPos), u_MaxLevel).xyz;
		second = texelFetch(u_InputSecondImage, ivec2(fragPos), u_MaxLevel).xyz;
	}
	mat3x3 normalMatrix = mat3x3(
		first.x, first.y , first.z,
		first.y, second.x, second.y,
		first.z, second.y, second.z
	);
	float eigenvalue = getEigenvalues(normalMatrix);
	vec3 normal = getEigenvector(normalMatrix - mat3x3(eigenvalue));
	if(isnan(normal.x))
	{
		vec3 vec = normalize(vec3(sqrt(first.x),sqrt(second.x),sqrt(second.z)));
		vec3 lightDir = normalize(u_LightDir);
		vec = dot(lightDir,vec) > 0 ? -vec : vec;
		u_Normals[fragIndex] = vec4(vec,1);

	}
	else
		u_Normals[fragIndex] = vec4(normal,1);
	
}