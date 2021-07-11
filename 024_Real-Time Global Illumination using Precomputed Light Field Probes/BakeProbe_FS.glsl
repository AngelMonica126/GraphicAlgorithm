#version 430 core

in  vec3 v2f_FragPosInViewSpace;
in  vec2 v2f_TexCoords;
in  vec3 v2f_Normal;
//in vec3 v2f_Tangent;
layout (location = 0) out vec4 AlbedoAndMetallic_;
layout (location = 1) out vec4 Normal_;
layout (location = 2) out vec3 Chebyshevs_;

uniform sampler2D u_LightDepthTexture;	
uniform sampler2D u_DiffuseTexture;
uniform float u_Near = 0.1;
uniform float u_Far = 100.0f;

uniform mat4  u_LightVPMatrixMulInverseCameraViewMatrix;
uniform vec3  u_LightDir;	
uniform float  u_Intensity;	
uniform mat4 u_BakeProjectionMatrix;
uniform mat4 u_BakeViewMatrix;
uniform float u_Exposure = 2.0f;

float LinearizeDepth(float vDepth)
{
    float z = vDepth * 2.0 - 1.0; 
    return (2.0 * u_Near * u_Far) / (u_Far + u_Near - z * (u_Far - u_Near));    
}

void main()
{
	float Gamma = 2.2;
	vec3 DiffuseColor = pow(texture(u_DiffuseTexture, v2f_TexCoords).rgb, vec3(Gamma));	
	float Alpha = textureLod(u_DiffuseTexture, v2f_TexCoords,0).a;
	if(Alpha != 1.0f)
		discard;
	vec3 FragViewNormal = normalize(v2f_Normal);
	vec3 FragViewPos = v2f_FragPosInViewSpace;
	vec4 FragPosInLightSpace = u_LightVPMatrixMulInverseCameraViewMatrix * vec4(FragViewPos, 1);
	vec3 LightDirInViewSpace = -normalize(vec3(u_BakeViewMatrix * vec4(u_LightDir, 0.0f)));	//这个负号不要忘了
	float DirectIllumination;
	FragPosInLightSpace /= FragPosInLightSpace.w;
	FragPosInLightSpace.xyz = (FragPosInLightSpace.xyz + 1) / 2;
	float Visibility4DirectLight = 0.0f;
	if(FragPosInLightSpace.z < 0.0f || FragPosInLightSpace.x > 1.0f || FragPosInLightSpace.y > 1.0f || FragPosInLightSpace.x < 0.0f || FragPosInLightSpace.y < 0.0f )
		DirectIllumination = 0;
	else
	{
		vec2 FragNDCPos4Light = FragPosInLightSpace.xy;
		float ClosetDepth4Light = texture(u_LightDepthTexture, FragNDCPos4Light).r; 
		float Bias = max(0.00001 * (1.0 - dot(FragViewNormal, LightDirInViewSpace)), 0.00001);
		Visibility4DirectLight = (FragPosInLightSpace.z - Bias < ClosetDepth4Light) ? 1.0f : 0.0f;
		DirectIllumination = u_Intensity * max(dot(LightDirInViewSpace, FragViewNormal), 0) * Visibility4DirectLight;
	}
	vec3 TexelColor = DiffuseColor * (DirectIllumination* 0.8 + 0.2);
	vec3 Mapped = vec3(1.0) - exp(-TexelColor * u_Exposure);
	Mapped = pow(Mapped, vec3(1.0f / 2.2f));
	AlbedoAndMetallic_ = vec4(Mapped,1.0);
	Normal_ = vec4(FragViewNormal,0);
	float depth = (gl_FragCoord.z);
	Chebyshevs_ = vec3(depth,depth * depth,LinearizeDepth(depth));
}