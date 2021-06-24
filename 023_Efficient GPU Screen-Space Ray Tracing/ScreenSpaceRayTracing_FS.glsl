#version 430 core

in vec3 v2f_Normal;
in vec3 v2f_FragPosInViewSpace;
in vec2 uv;
layout(location = 0) out vec4 Color_;

uniform vec3 u_DiffuseColor;
uniform sampler2D u_DepthTexture;
uniform sampler2D u_AlbedoTexture;
uniform float u_Near = 0.1;
uniform float u_Far = 1000.0f;
uniform vec2 u_DepthSize;
uniform mat4 _ScreenSpaceProjectionMatrix;
uniform int _MaximumIterationCount = 400;
uniform int _MaximumMarchDistance = 200;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};


  struct Ray
    {
        vec3 origin;
        vec3 direction;
    };

    struct Segment
    {
        vec3 start;
        vec3 end;

        vec3 direction;
    };

    struct Result
    {
        bool isHit;

        vec2 uv;
        vec3 position;

        int iterationCount;
    };

float LinearizeDepth(float vDepth)
{
    float z = vDepth * 2.0 - 1.0; 
    return (2.0 * u_Near * u_Far) / (u_Far + u_Near - z * (u_Far - u_Near));    
}

float DistanceSquared(vec2 a, vec2 b) {
    a -= b;
    return dot(a, a);
}

float getSquaredDistance(vec2 first, vec2 second)
{
    first -= second;
    return dot(first, first);
}

vec4 projectToScreenSpace(vec3 position)
{
	vec4 FragPosInProSpace =  u_ProjectionMatrix * vec4(position.x, position.y, position.z, 1);
//	FragPosInProSpace /= FragPosInProSpace.w;
	FragPosInProSpace.xyz = (FragPosInProSpace.xyz + 1) / 2;
	FragPosInProSpace.xy *= u_DepthSize;
	return FragPosInProSpace;
}

bool query(vec2 z, vec2 uv)
{
    float depths = -LinearizeDepth(texelFetch(u_DepthTexture, ivec2(uv),0).r);
    return z.y < depths && z.x > depths;
}

Result march(in Ray ray)
{
    Result result;

    result.isHit = false;

    result.uv = vec2(0);
    result.position = vec3(0);

    result.iterationCount = 0;

    Segment segment;

    segment.start = ray.origin;

    float end = ray.origin.z + ray.direction.z * _MaximumMarchDistance;
    float magnitude = _MaximumMarchDistance;

    if (end > -u_Near)
        magnitude = (-u_Near - ray.origin.z) / ray.direction.z;

    segment.end = ray.origin + ray.direction * magnitude;

    vec4 r = projectToScreenSpace(segment.start);
    vec4 q = projectToScreenSpace(segment.end);

    const vec2 homogenizers = 1. / vec2(r.w, q.w);

    segment.start *= homogenizers.x;
    segment.end *= homogenizers.y;

    vec4 endPoints = vec4(r.xy, q.xy) * homogenizers.xxyy;
//    endPoints.zw += step(getSquaredDistance(endPoints.xy, endPoints.zw), .0001) * max(u_DepthSize.x, u_DepthSize.y);

    vec2 displacement = endPoints.zw - endPoints.xy;

    bool isPermuted = false;

    if (abs(displacement.x) < abs(displacement.y))
    {
        isPermuted = true;

        displacement = displacement.yx;
        endPoints.xyzw = endPoints.yxwz;
    }

    float direction = sign(displacement.x);
    float normalizer = direction / displacement.x;

    segment.direction = (segment.end - segment.start) * normalizer;
    vec4 derivatives = vec4(vec2(direction, displacement.y * normalizer), (homogenizers.y - homogenizers.x) * normalizer, segment.direction.z);

    float stride = 2. - min(1., -segment.start.z * .1);

    derivatives *= stride;
    segment.direction *= stride;

    vec2 z = vec2(0);
    vec4 tracker = vec4(endPoints.xy, homogenizers.x, segment.start.z);

    for(; result.iterationCount < _MaximumIterationCount; ++result.iterationCount)
    {
        tracker += derivatives;

        z.x = z.y;
        z.y = tracker.w + derivatives.w * .5;
        z.y /= tracker.z + derivatives.z * .5;

        if (z.y > z.x)
        {
            float k = z.x;
            z.x = z.y;
            z.y = k;
        }

        result.uv = tracker.xy;
        if (isPermuted)
            result.uv = result.uv.yx;

        result.isHit = query(z, result.uv);

        if (result.isHit)
            break;
    }

    segment.start.xy += segment.direction.xy * float( result.iterationCount);
    segment.start.z = tracker.w;

    result.position = segment.start / tracker.z;

    return result;
}

void main()
{
	vec3 orginPoint = v2f_FragPosInViewSpace;
	vec3 normal = normalize(v2f_Normal);
	vec3 viewDir = normalize(orginPoint);
	vec3 reflectDir = normalize(reflect(viewDir,normal));
	 Ray ray;
     ray.origin = orginPoint;

     ray.direction = normalize(reflect(normalize(ray.origin), normal));
	 
     Result result = march(ray);
     if (result.isHit)
     {
         Color_ =  texelFetch(u_AlbedoTexture, ivec2(result.uv),0);
     }
	 else
		Color_ = vec4(1,0,0,1);
}