#version 430 core
#pragma optionNV (unroll all)	
#define LOCAL_GROUP_SIZE 32

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;
layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputEASUTexture;

uniform sampler2D u_InputTexture;
uniform int u_DisplayWidth;
uniform int u_DisplayHeight;


void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	vec2 UV = vec2(FragPos.x / float(u_DisplayWidth),FragPos.y / float(u_DisplayHeight));
	imageStore(u_OutputEASUTexture,FragPos,texture(u_InputTexture,UV));
}