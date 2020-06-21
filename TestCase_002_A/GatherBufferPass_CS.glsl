#version 430 core
#define LOCAL_GROUP_SIZE 16

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;
uniform sampler2D u_LightedSplitedTexture;
layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputImage;

uniform int u_WindowWidth;
uniform int u_WindowHeight;
uniform int u_SubBufferNumX;
uniform int u_SubBufferNumY;

void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	//vec2  TexCoord = vec2(float(FragPos.x + 0.5f) / u_WindowWidth, float(FragPos.y + 0.5f) / u_WindowHeight);
	ivec2 SubBufferSize = ivec2(u_WindowWidth / u_SubBufferNumX, u_WindowHeight / u_SubBufferNumY);
	ivec2 SubBufferIndex2 = ivec2(FragPos.x / SubBufferSize.x, FragPos.y / SubBufferSize.y);
	ivec2 CoordsInSubBuffer = ivec2(FragPos.x % SubBufferSize.x, FragPos.y % SubBufferSize.y);

	ivec2 CoordsInOriginalBuffer = ivec2(CoordsInSubBuffer.x * u_SubBufferNumX + SubBufferIndex2.x, CoordsInSubBuffer.y * u_SubBufferNumY + SubBufferIndex2.y);

	//vec4 FragColor = texture(u_LightedSplitedTexture, TexCoord);
	vec4 FragColor = texelFetch(u_LightedSplitedTexture, FragPos, 0);
	imageStore(u_OutputImage, CoordsInOriginalBuffer, FragColor);
}