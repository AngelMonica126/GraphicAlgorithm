#version 430 core

in vec3 v2f_Normal;
in vec3 v2f_FragPosInViewSpace;

layout(location = 0) out vec4 Color_;

uniform vec3 u_DiffuseColor;
uniform sampler2D u_DepthTexture;
uniform sampler2D u_AlbedoTexture;
uniform float u_Near = 0.1;
uniform float u_Far = 1000.0f;
uniform vec2 u_DepthSize;

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

void Swap(in out float a, in out float b) {
     float temp = a;
     a = b;
     b = temp;
}

float DistanceSquared(vec2 a, vec2 b) {
    a -= b;
    return dot(a, a);
}

bool TraceScreenSpaceRay
   (vec3          csOrigin, 
    vec3         csDirection,
    float           csZThickness,
    float			stride,
    float           jitterFraction,
    float           maxSteps,
    in float        maxRayTraceDistance,
    out vec2      hitPixel,
	out vec3		csHitPoint) {
    
    // Clip ray to a near plane in 3D (doesn't have to be *the* near plane, although that would be a good idea)
    float rayLength = ((csOrigin.z + csDirection.z * maxRayTraceDistance) > u_Near) ?
                        (u_Near - csOrigin.z) / csDirection.z :
                        maxRayTraceDistance;
	vec3 csEndPoint = csDirection * rayLength + csOrigin;

    // Project into screen space
    vec4 H0 = u_ProjectionMatrix * vec4(csOrigin, 1.0);
    vec4 H1 = u_ProjectionMatrix * vec4(csEndPoint, 1.0);

    float k0 = 1.0 / H0.w;
    float k1 = 1.0 / H1.w;

    vec3 Q0 = csOrigin * k0; 
    vec3 Q1 = csEndPoint * k1;

    vec2 P0 = H0.xy * k0;
    vec2 P1 = H1.xy * k1;

    // [Optional clipping to frustum sides here]

    // Initialize to off screen
    hitPixel = vec2(-1.0, -1.0);
    P1 += vec2((DistanceSquared(P0, P1) < 0.0001) ? 0.01 : 0.0);

    vec2 delta = P1 - P0;

    // Permute so that the primary iteration is in x to reduce
    // large branches later
    bool permute = false;
	if (abs(delta.x) < abs(delta.y)) {
		// More-vertical line. Create a permutation that swaps x and y in the output
		permute = true;

        // Directly swizzle the inputs
		delta = delta.yx;
		P1 = P1.yx;
		P0 = P0.yx;        
	}
    
	// From now on, "x" is the primary iteration direction and "y" is the secondary one

    float stepDirection = sign(delta.x);
    float invdx = stepDirection / delta.x;
    vec2 dP = vec2(stepDirection, invdx * delta.y);

    // Track the derivatives of Q and k
    vec3 dQ = (Q1 - Q0) * invdx;
    float   dk = (k1 - k0) * invdx;

    // Scale derivatives by the desired pixel stride
	dP *= stride; dQ *= stride; dk *= stride;

    // Offset the starting values by the jitter fraction
	P0 += dP * jitterFraction; Q0 += dQ * jitterFraction; k0 += dk * jitterFraction;

	// Slide P from P0 to P1, (now-homogeneous) Q from Q0 to Q1, and k from k0 to k1
    vec3 Q = Q0;
    float  k = k0;

	// We track the ray depth at +/- 1/2 pixel to treat pixels as clip-space solid 
	// voxels. Because the depth at -1/2 for a given pixel will be the same as at 
	// +1/2 for the previous iteration, we actually only have to compute one value 
	// per iteration.
	float prevZMaxEstimate = csOrigin.z;
    float stepCount = 0.0;
    float rayZMax = prevZMaxEstimate, rayZMin = prevZMaxEstimate;
    float sceneZMax = rayZMax + 1e4;

    // P1.x is never modified after this point, so pre-scale it by 
    // the step direction for a signed comparison
    float end = P1.x * stepDirection;

    // We only advance the z field of Q in the inner loop, since
    // Q.xy is never used until after the loop terminates.
	for (vec2 P = P0;
        ((P.x * stepDirection) <= end) && 
        (stepCount < maxSteps) &&
        ((rayZMax < sceneZMax - csZThickness) ||
            (rayZMin > sceneZMax)) &&
        (sceneZMax != 0.0);
        P += dP, Q.z += dQ.z, k += dk, stepCount += 1.0) {
                
		hitPixel = permute ? P.yx : P;
		hitPixel  = (hitPixel + 1) / 2 ;
        // The depth range that the ray covers within this loop
        // iteration.  Assume that the ray is moving in increasing z
        // and swap if backwards.  Because one end of the interval is
        // shared between adjacent iterations, we track the previous
        // value and then swap as needed to ensure correct ordering
        rayZMin = prevZMaxEstimate;

        // Compute the value at 1/2 pixel into the future
        rayZMax = (dQ.z * 0.5 + Q.z) / (dk * 0.5 + k);
		prevZMaxEstimate = rayZMax;
        if (rayZMin > rayZMax) { Swap(rayZMin, rayZMax); }

        // Camera-space z of the background
        sceneZMax = LinearizeDepth(texture(u_DepthTexture, hitPixel).r);
    } // pixel on ray
    Q.xy += dQ.xy * stepCount;
	csHitPoint = Q;

    // Matches the new loop condition:
    return true;
}


void main()
{
	vec3 orginPoint = v2f_FragPosInViewSpace;
	vec3 normal = normalize(v2f_Normal);
	vec3 viewDir = normalize(orginPoint);
	vec3 reflectDir = normalize(reflect(viewDir,normal));
//	Color_ = vec4(reflectDir, 1.0f);
//	return ;
	vec2 hitPixel;
	vec3 csHitPoint;
	float stride = 0.01f;
	float jitterFraction = 1.0f;
	float maxSteps = 100.0f;
	float maxRayTraceDistance = 100.0f;
	if(TraceScreenSpaceRay(orginPoint, 
	                       reflectDir,
	                       1.0f,
	                       stride,
	                       jitterFraction,
	                       maxSteps,
	                       maxRayTraceDistance,
	                       hitPixel,
	                       csHitPoint))
	{
//		Color_ = vec4(hitPixel,0, 1.0f);
	csHitPoint.xyz = (csHitPoint.xyz + 1) / 2;
	hitPixel.x = 1.0 - hitPixel.x;
		Color_ = vec4(texture(u_AlbedoTexture, hitPixel).xyz, 1.0f);
	}
		
	else
		Color_ = vec4(1.0,1,1, 1.0f);
}