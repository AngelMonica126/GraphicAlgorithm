#version 430 core
#define LOCAL_GROUP_SIZE 16

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;
uniform sampler2D u_PositionTexture;
uniform sampler2D u_NormalTexture;
layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputImage;

uniform int u_WindowWidth;
uniform int u_WindowHeight;
uniform float u_NormalThreshold = 1.8f;
uniform float u_DepthThreshold = 11.0f;

void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
//	vec2  TexelSize = vec2(1) / vec2(u_WindowWidth, u_WindowHeight);
//	vec2  TexCoord = FragPos * TexelSize + TexelSize / 2.0f;
//
//	vec2 NeighborTexCoords[3];
//	NeighborTexCoords[0] = TexCoord + TexelSize * vec2(0, 1);
//	NeighborTexCoords[1] = TexCoord + TexelSize * vec2(1, 0);
//	NeighborTexCoords[2] = TexCoord + TexelSize * vec2(1, 1);
	ivec2 NeighborTexCoords[3];
	NeighborTexCoords[0] = FragPos + ivec2(0, 1);
	NeighborTexCoords[1] = FragPos + ivec2(1, 0);
	NeighborTexCoords[2] = FragPos + ivec2(1, 1);

//	vec3 Normal = texture(u_NormalTexture, TexCoord).xyz;
//	float Depth = texture(u_PositionTexture, TexCoord).z;
	//vec3 Pos = texture(u_PositionTexture, TexCoord).xyz;
	vec3 Normal = texelFetch(u_NormalTexture, FragPos, 0).xyz;
	float Depth = texelFetch(u_PositionTexture, FragPos, 0).z;

	bool IsEdge = false;
	for(int i = 0; i < 3; ++i)
	{
		//vec3 NeighborNormal = texture(u_NormalTexture, NeighborTexCoords[i]).xyz;
		vec3 NeighborNormal = texelFetch(u_NormalTexture, NeighborTexCoords[i], 0).xyz;
		if(dot(NeighborNormal, Normal) < u_NormalThreshold)
		{
			IsEdge = true;
			break;
		}

		//float NeighborDepth = texture(u_PositionTexture, NeighborTexCoords[i]).z;
		//vec3 NeighborPos = texture(u_PositionTexture, NeighborTexCoords[i]).xyz;
		float NeighborDepth = texelFetch(u_PositionTexture, NeighborTexCoords[i], 0).z;
		if(abs(NeighborDepth - Depth) > abs(u_DepthThreshold * Depth))
		//if(length(NeighborPos - Pos) > abs(u_DepthThreshold))
		{
			IsEdge = true;
			break;
		}
	}

	vec4 FragColor = IsEdge ? vec4(1.0f) : vec4(0.0f);
	imageStore(u_OutputImage, FragPos, FragColor);
	//imageStore(u_OutputImage, FragPos, vec4(Normal2, 1.0f));
}