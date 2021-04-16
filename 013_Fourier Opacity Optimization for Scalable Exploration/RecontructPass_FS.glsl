#version 460  core
#define PI 3.1415926
in  vec2 v2f_TexCoords;

layout (location = 0) out vec4 Color_;

uniform sampler2D u_CoefficientOneTex;
uniform sampler2D u_CoefficientTwoTex;
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
	
	float depth = LinearizeDepth(gl_FragCoord.z);

	vec4 fourierOne = texelFetch(u_CoefficientOneTex, ivec2(gl_FragCoord.xy), 0);
	vec4 fourierTwo = texelFetch(u_CoefficientTwoTex, ivec2(gl_FragCoord.xy), 0);

	float a0 = fourierOne.y;
	float a1 = fourierOne.z;
	float b1 = fourierOne.w;
	float a2 = fourierTwo.x;
	float b2 = fourierTwo.y;
	float a3 = fourierTwo.z;
	float b3 = fourierTwo.w;

	float sin2, cos2, sin4, cos4, sin6, cos6, sin8, cos8, sin10, cos10, sin12, cos12, sin14, cos14;
	cos2 = cos(2 * PI * depth);
	sin2 = sin(2 * PI * depth);
	cos4 = cos2 * cos2 - sin2 * sin2;
	sin4 = 2 * cos2 * sin2;
	cos6 = cos4 * cos2 - sin4 * sin2;
	sin6 = sin4 * cos2 + cos4 * sin2;

	float opticalDepth = 0.5 * a0 * depth;
	opticalDepth += (a1 / (2 * PI)) * sin2;
	opticalDepth += (b1 / (2 * PI)) * (1 - cos2);
	opticalDepth += (a2 / (4 * PI)) * sin4 ;
	opticalDepth += (b2 / (4 * PI)) * (1 - cos4);
	opticalDepth += (a3 / (6 * PI)) * sin6;
	opticalDepth += (b3 / (6 * PI)) * (1 - cos6);
	float transmittance = exp(-opticalDepth);
	Color_ = vec4(transmittance * u_DiffuseColor.xyz * u_DiffuseColor.a / (1.0 - u_DiffuseColor.a), 
	              transmittance * u_DiffuseColor.a / (1.0 - u_DiffuseColor.a));
}