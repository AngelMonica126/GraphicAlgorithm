#version 430 core

in  vec3 v2f_FragPosInViewSpace;
in  vec2 v2f_TexCoords;
in  vec3 v2f_Normal;
layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};
layout (location = 0) out vec4 Color_;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_RoughnessTexture;
uniform sampler2D u_MetallicTexture;
uniform sampler2D u_DepthTexture;
uniform vec3 u_LightDir;
uniform vec3 u_ViewPos;
uniform vec3 u_LightColor = vec3(1);
uniform float u_Width;
uniform float u_Height;

uniform int u_Layer;

void main()
{
	const float SpecularStrength = 0.5f;
	const float Bias = 1e-4;
	vec2 Deptc = vec2(gl_FragCoord.x / u_Width, gl_FragCoord.y / u_Height);
	if(u_Layer != 0 && gl_FragCoord.z <= (texture(u_DepthTexture,Deptc).r) + Bias)
		discard ;
	vec3 Albedo = texture(u_DiffuseTexture, v2f_TexCoords).rgb;
	vec3 L = -mat3(u_ViewMatrix) * u_LightDir;
    vec3 N = normalize(v2f_Normal);
    vec3 Diffuse = max(dot(N, L), 0.0)  * u_LightColor;
    vec3 Specular = SpecularStrength * pow(max(dot(L, N), 0.0), 16)  * u_LightColor;
	Color_ = vec4((Diffuse + Specular) * Albedo + 0.1 * Albedo, 1) ; 
}