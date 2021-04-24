#version 430 core

in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D u_DepthTexture;
uniform sampler2D u_PositionTexture;
uniform sampler2D u_NormalTexture;
uniform float u_Near = 0.1;
uniform float u_Far = 100.0f;
uniform float u_Fov;
uniform float u_WindowWidth;
uniform float u_WindowHeight;
float ViewSpaceZFromDepth(float d)
{
	d = d * 2.0 - 1.0;
	 //视线坐标系看向的z轴负方向，因此要求视觉空间的z值应该要把线性深度变成负值
	 return (2.0 * u_Near * u_Far) / (u_Far + u_Near - d * (u_Far - u_Near)); 
}

vec3 UVToViewSpace(vec2 uv, float z)
{
	uv = uv * 2.0 - 1.0;
	uv.x = uv.x * tan(u_Fov / 2.0) * u_WindowWidth / u_WindowHeight  * z ;
	uv.y = uv.y * tan(u_Fov / 2.0)  * z ;
	return vec3(uv, -z);
}

vec3 GetViewPos(vec2 uv)
{
	float z = ViewSpaceZFromDepth(texture(u_DepthTexture, uv).r);
	return UVToViewSpace(uv, z);
}
float Length2(vec3 V)
{
	return dot(V,V);
}

vec3 MinDiff(vec3 P, vec3 Pr, vec3 Pl)
{
    vec3 V1 = Pr - P;
    vec3 V2 = P - Pl;
    return (Length2(V1) < Length2(V2)) ? V1 : V2;
}
void main()
{
	float xOffset = 1.0 / u_WindowWidth;
	float yOffset = 1.0 / u_WindowHeight;
	vec3 P = GetViewPos(v2f_TexCoords);
	vec3 Pl = GetViewPos(v2f_TexCoords + vec2(-xOffset,0));
	vec3 Pr = GetViewPos(v2f_TexCoords + vec2(xOffset,0));
	vec3 Pu = GetViewPos(v2f_TexCoords + vec2(0,yOffset));
	vec3 Pd = GetViewPos(v2f_TexCoords + vec2(0,-yOffset));
	vec3 leftDir = MinDiff(P, Pr, Pl);
    vec3 upDir = MinDiff(P, Pu, Pd);
	vec3 Nomal = normalize(cross(leftDir,upDir));

	Color_ = vec4(normalize(Nomal), 1.0f);
//	Color_ = vec4(GetViewPos(v2f_TexCoords), 1.0f);
}