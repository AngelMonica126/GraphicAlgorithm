#version 430 core
#pragma optionNV (unroll all)	//暂时不知道有没有起作用

#define LOCAL_GROUP_SIZE 16

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;
uniform sampler2D u_InputAlbedoTexture;
uniform sampler2D u_InputNormalTexture;
uniform sampler2D u_InputPositionTexture;
layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputAlbedoImage;
layout (rgba32f, binding = 1) uniform writeonly image2D u_OutputNormalImage;
layout (rgba32f, binding = 2) uniform writeonly image2D u_OutputPositionImage;

uniform int u_WindowWidth;
uniform int u_WindowHeight;
uniform int LowWidth;
uniform int LowHeight;

void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);

	vec4 Albedo = texelFetch(u_InputAlbedoTexture, FragPos, 0);
	vec4 Normal = texelFetch(u_InputNormalTexture, FragPos, 0);
	vec4 Position = texelFetch(u_InputPositionTexture, FragPos, 0);
	int BlockX = u_WindowWidth/LowWidth;
	int BlockY = u_WindowHeight/LowHeight;
	if(FragPos.x%BlockX==0||FragPos.y%BlockY==0)
	{
		ivec2 SubdividedFragPos = ivec2(FragPos.x/BlockX,FragPos.y/BlockY);
		imageStore(u_OutputAlbedoImage, SubdividedFragPos, Albedo);
		imageStore(u_OutputNormalImage, SubdividedFragPos, Normal);
		imageStore(u_OutputPositionImage, SubdividedFragPos, Position);
	}
}