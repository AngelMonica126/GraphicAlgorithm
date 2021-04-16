#version 460  core
#define PI 3.1415926
in  vec2 v2f_TexCoords;

layout (location = 0) out vec4 CoefficientOne_;
layout (location = 1) out vec4 CoefficientTwo_;

uniform sampler2D u_OpacityTexture;
uniform vec4 u_DiffuseColor;
uniform float u_NearPlane;
uniform float u_FarPlane;

float LinearizeDepth(float vDepth)
{
	float z = vDepth * 2.0 - 1.0; 
	return (2.0 * u_NearPlane * u_FarPlane) / (u_FarPlane + u_NearPlane - z * (u_FarPlane - u_NearPlane));
}
void main()
{
    float opaqueDepth = texelFetch(u_OpacityTexture, ivec2(gl_FragCoord.xy), 0).r;
	if (opaqueDepth != 0.0 && gl_FragCoord.z > opaqueDepth) discard;
	
	float a0 = u_DiffuseColor.a;
	float depth = LinearizeDepth(gl_FragCoord.z);

	float sin2, cos2, sin4, cos4, sin6, cos6, sin8, cos8, sin10, cos10, sin12, cos12, sin14, cos14;
	cos2 = cos(2 * PI * depth);
	sin2 = sin(2 * PI * depth);
	cos4 = cos2 * cos2 - sin2 * sin2;
	sin4 = 2 * cos2 * sin2;
	cos6 = cos4 * cos2 - sin4 * sin2;
	sin6 = sin4 * cos2 + cos4 * sin2;
	float a1 = a0 * cos2;
	float b1 = a0 * sin2;
	float a2 = a0 * cos4;
	float b2 = a0 * sin4;
	float a3 = a0 * cos6;
	float b3 = a0 * sin6;

	CoefficientOne_ = vec4(1,a0,a1,b1);
	CoefficientTwo_ = vec4(a2,b2,a3,b3);
}