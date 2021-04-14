#version 460  core
//#extension GL_ARB_fragment_shader_interlock : require
#define MAX_FRAGMENTS 64
in  vec2 v2f_TexCoords;

layout (location = 0) out vec4 Color_;

struct ListNode
{
	uint packedColor;
	uint transmittance;
	uint depth;
	uint next;
};
struct NodeData
{
	uint packedColor;
	uint transmittance;
	float depth;
};
layout(binding = 0, r32ui) uniform uimage2D u_ListHeadPtrTex;
layout(binding = 0) uniform atomic_uint u_ListNodeCounter;
layout(binding = 0, std430) buffer linkedLists { 
	ListNode nodes[]; 
};

vec4 unpackColor(uint color)
{
	vec4 c;
	c.r = float((color >> 24) & 0x000000ff) / 255.0f;
	c.g = float((color >> 16) & 0x000000ff) / 255.0f;
	c.b = float((color >> 8) & 0x000000ff) / 255.0f;
	c.a = float(color & 0x000000ff) / 255.0f;
	return c;
}

void insertionSort(inout NodeData sortedFragments[MAX_FRAGMENTS], int size)
{
	for (int k = 1; k < size; k++)
	{
		int j = k;
		NodeData t = sortedFragments[k];
			
		while (sortedFragments[j - 1].depth < t.depth)
		{
			sortedFragments[j] = sortedFragments[j - 1];
			j--;
			if (j <= 0) { break; }
		}

		if (j != k) { sortedFragments[j] = t; }
	}
}
void main()
{
    uint index = imageLoad(u_ListHeadPtrTex, ivec2(gl_FragCoord.xy)).x;
	if (index == 0) { Color_ = vec4(0.0, 1.0, 0.0, 1.0); return; }
	NodeData fragments[MAX_FRAGMENTS];
	int counter = 0;
	while(index != -1 && counter < MAX_FRAGMENTS)
	{
		fragments[counter] = NodeData(0, 0, 0.0f);
		fragments[counter].packedColor = nodes[index].packedColor;
		fragments[counter].transmittance = nodes[index].transmittance;
		fragments[counter].depth = unpackHalf2x16(nodes[index].depth).x;
		counter++;
		index = nodes[index].next;
	}
	insertionSort(fragments, counter);
	vec3 color = vec3(0.0);
	for (int i = 0; i < counter; i++)
	{
		vec4 c = unpackColor(fragments[i].packedColor);
		color = (1.0 - c.a) * color + c.a * (c.rgb);
	}
	Color_ = vec4(color,1);
}