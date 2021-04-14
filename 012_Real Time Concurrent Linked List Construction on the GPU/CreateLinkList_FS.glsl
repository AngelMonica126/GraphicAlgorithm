#version 460  core
#define MAX_FRAGMENTS 400 * 400 * 64
in  vec2 v2f_TexCoords;

layout (location = 0) out vec4 Color_;

struct ListNode
{
	uint packedColor;
	uint transmittance;
	uint depth;
	uint next;
};
layout(binding = 0, r32ui) uniform uimage2D u_ListHeadPtrTex;
layout(binding = 0, offset = 0) uniform atomic_uint u_ListNodeCounter;
layout(binding = 0, std430) buffer linkedLists { 
	ListNode nodes[]; 
};

uniform sampler2D u_DiffuseTexture;
uniform vec4 u_DiffuseColor;
uniform bool u_isDiffuseColor = false;
uniform float u_NearPlane;
uniform float u_FarPlane;

uint packColor(vec4 color)
{
	uvec4 bytes = uvec4(color * 255.0);
	uint pack = (bytes.r << 24) | (bytes.g << 16) | (bytes.b << 8) | (bytes.a);
	return pack;
}
float LinearizeDepth(float vDepth)
{
	float z = vDepth * 2.0 - 1.0; 
	return (2.0 * u_NearPlane * u_FarPlane) / (u_FarPlane + u_NearPlane - z * (u_FarPlane - u_NearPlane));
}
void main()
{
    
	vec4 Albedo;
	if(u_isDiffuseColor)
	    Albedo = u_DiffuseColor;
	else
	{
	    Albedo = texture(u_DiffuseTexture, v2f_TexCoords);
	}
	Color_ = Albedo;

	uint transmittance = packColor(vec4(Albedo.w,0,0, 0.0));
	uint packedColor = packColor(Albedo);

	uint nodeIndex = atomicCounterIncrement(u_ListNodeCounter) + 1u;
	if (nodeIndex < MAX_FRAGMENTS)
	{
	    uint nextIndex = imageAtomicExchange(u_ListHeadPtrTex, ivec2(gl_FragCoord.xy), nodeIndex);
	    uint currentDepth = packHalf2x16(vec2(LinearizeDepth(gl_FragCoord.z), 0));
	    nodes[nodeIndex] = ListNode(packedColor, transmittance, currentDepth, nextIndex);
	}
}