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
uniform int u_SubBufferNumX;
uniform int u_SubBufferNumY;

void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	//vec2  TexCoord = vec2(float(FragPos.x + 0.5f) / u_WindowWidth, float(FragPos.y + 0.5) / u_WindowHeight);
	ivec2 SubBufferSize = ivec2(u_WindowWidth / u_SubBufferNumX, u_WindowHeight / u_SubBufferNumY);

	ivec2 SubBufferIndex = ivec2(FragPos.x % u_SubBufferNumX, FragPos.y % u_SubBufferNumY);
	ivec2 CoordInSubBuffer = ivec2(FragPos.x / u_SubBufferNumX, FragPos.y / u_SubBufferNumY);

	ivec2 SubdividedFragPos = SubBufferSize * SubBufferIndex + CoordInSubBuffer;

//	vec4 Albedo = texture(u_InputAlbedoTexture, TexCoord);
//	vec4 Normal = texture(u_InputNormalTexture, TexCoord);
//	vec4 Position = texture(u_InputPositionTexture, TexCoord);
	vec4 Albedo = texelFetch(u_InputAlbedoTexture, FragPos, 0);
	vec4 Normal = texelFetch(u_InputNormalTexture, FragPos, 0);
	vec4 Position = texelFetch(u_InputPositionTexture, FragPos, 0);

	imageStore(u_OutputAlbedoImage, SubdividedFragPos, Albedo);
	imageStore(u_OutputNormalImage, SubdividedFragPos, Normal);
	imageStore(u_OutputPositionImage, SubdividedFragPos, Position);
}