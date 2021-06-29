#version 430 core

in vec3 v2f_FragPosInWorldSpace;
layout(location = 0) out vec4 Color_;

uniform vec3 u_DiffuseColor;
uniform sampler2D u_DepthTexture;
uniform sampler2D u_AlbedoTexture;
uniform float u_Near = 0.1;
uniform float u_Far = 1000.0f;
uniform float u_WindowWidth;
uniform float u_WindowHeight;
uniform vec3  u_CameraPosInWorldSpace;
uniform float u_RayLength = 10000;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

float LinearizeDepth(float vDepth)
{
    float z = vDepth * 2.0 - 1.0; 
    return (2.0 * u_Near * u_Far) / (u_Far + u_Near - z * (u_Far - u_Near));    
}

struct Ray
{
    vec3 Origin;
    vec3 Direction;
};

struct Result
{
    bool IsHit;

    vec2 UV;
    vec3 Position;

    int IterationCount;
};

vec4 projectToScreenSpace(vec3 vPoint)
{
	return u_ProjectionMatrix * vec4(vPoint,1);
}

vec3 projectToViewSpace(vec3 vPointInViewSpace)
{
	return vec3(u_ViewMatrix * vec4(vPointInViewSpace,1));
}

float distanceSquared(vec2 A, vec2 B) 
{
    A -= B;
    return dot(A, A);
}
bool Query(vec2 z, vec2 uv)
{
    float depths = -LinearizeDepth(texture(u_DepthTexture, uv / vec2(u_WindowWidth,u_WindowHeight)).r);
//    return z.y < depths;
    return z.y < depths && z.x > depths;
}

Result RayMarching(Ray vRay)
{
	Result result;

	vec3 Begin = vRay.Origin;
	vec3 End = vRay.Origin + vRay.Direction * u_RayLength;

	vec3 V0 = projectToViewSpace(Begin);
	vec3 V1 = projectToViewSpace(End);

	vec4 H0 = projectToScreenSpace(V0);
	vec4 H1 = projectToScreenSpace(V1);

	float k0 = 1.0 / H0.w;
    float k1 = 1.0 / H1.w;

	vec3 Q0 = V0 * k0; 
    vec3 Q1 = V1 * k1;

	// NDC-space not Screen Space
    vec2 P0 = H0.xy * k0;
    vec2 P1 = H1.xy * k1;
	vec2 Size = vec2(u_WindowWidth,u_WindowHeight);
	//Screen Space
	P0 = (P0 + 1) / 2 * Size;
	P1 = (P1 + 1) / 2 * Size;

	P1 += vec2((distanceSquared(P0, P1) < 0.0001) ? 0.01 : 0.0);

	vec2 Delta = P1 - P0;

	bool Permute = false;
    if (abs(Delta.x) < abs(Delta.y)) 
	{ 
        Permute = true;
        Delta = Delta.yx; P0 = P0.yx; P1 = P1.yx; 
    }
	float StepDir = sign(Delta.x);
    float Invdx = StepDir / Delta.x;
	vec3  dQ = (Q1 - Q0) * Invdx;
    float dk = (k1 - k0) * Invdx;
    vec2  dP = vec2(StepDir, Delta.y * Invdx);
	float Stride = 1.0f;

    dP *= Stride; dQ *= Stride; dk *= Stride;

    P0 += dP; Q0 += dQ; k0 += dk;
	
	int Step = 0;
	int MaxStep = 5000;
	float k = k0;
	float EndX = P1.x * StepDir;
	vec3 Q = Q0;
	float prevZMaxEstimate = V0.z;

	for(vec2 P = P0;  Step < MaxStep;Step++,P += dP, Q.z += dQ.z, k += dk)
	{
		result.UV = Permute ? P.yx : P;
		vec2 Depths;
		Depths.x = prevZMaxEstimate;
		Depths.y = (dQ.z * 0.5 + Q.z) / (dk * 0.5 + k);
		prevZMaxEstimate = Depths.y;
		if(Depths.x < Depths.y)
			Depths.xy = Depths.yx;
		if(result.UV.x > u_WindowWidth || result.UV.x < 0 || result.UV.y > u_WindowHeight || result.UV.y < 0)
			break;
		result.IsHit = Query(Depths, result.UV);
		if (result.IsHit)
			break;
	}

	return result;
}

void main()
{
	vec3 OrginPoint = v2f_FragPosInWorldSpace;
	vec3 ViewDir = normalize(u_CameraPosInWorldSpace - OrginPoint);
	vec3 Normal = vec3(0,1,0);
	vec3 ReflectDir = normalize(reflect(-ViewDir,Normal));
	Ray ray;
	ray.Origin = OrginPoint;
	ray.Direction = ReflectDir;
	Result result = RayMarching(ray);
	vec3 End = OrginPoint;
	vec3 V1 = projectToViewSpace(End);
	vec4 H1 = projectToScreenSpace(V1);

    float k1 = 1.0 / H1.w;

    vec3 Q1 = V1 * k1;
	if(result.IsHit)
	{
		Color_ = vec4(texture(u_AlbedoTexture,result.UV / vec2(u_WindowWidth,u_WindowHeight)).xyz, 1);
	}
	else
	{
		vec4 PointInScreen = u_ProjectionMatrix * u_ViewMatrix * vec4(OrginPoint,1);
		PointInScreen.xy /= PointInScreen.w;
		PointInScreen.xy = PointInScreen.xy * 0.5 + 0.5;
		Color_ = vec4(texture(u_AlbedoTexture,PointInScreen.xy).xyz, 1);
	}
}