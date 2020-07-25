#version 430 core
#pragma optionNV (unroll all)	//暂时不知道有没有起作用
#define LOCAL_GROUP_SIZE 16

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;
uniform sampler2D u_InputTexture;
uniform sampler2D u_EdgeTexture;
layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputImage;

uniform int u_WindowWidth;
uniform int u_WindowHeight;
uniform bool u_IsBlurVertical;

#define WEIGHT_NUM 5

//uniform float weight[WEIGHT_NUM] = {0.4026, 0.2442, 0.0545};
//uniform float weight[WEIGHT_NUM] = {0.6063, 0.19685};
uniform float weight[WEIGHT_NUM] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	vec2  TexelSize = vec2(1) / vec2(u_WindowWidth, u_WindowHeight);
	vec2  TexCoord = FragPos * TexelSize + TexelSize / 2.0f;

	float CurrentEdgeFlags = texture(u_EdgeTexture, TexCoord).r;

	vec2 uv[9];
	if(u_IsBlurVertical)
	{
		uv[0] = TexCoord;
		uv[1] = TexCoord + vec2(0, 1.5) * TexelSize;
		uv[2] = TexCoord + vec2(0, -1.5) * TexelSize;
		uv[3] = TexCoord + vec2(0, 2.5) * TexelSize;
		uv[4] = TexCoord + vec2(0, -2.5) * TexelSize;
		uv[5] = TexCoord + vec2(0, 3.5) * TexelSize;
		uv[6] = TexCoord + vec2(0, -3.5) * TexelSize;
		uv[7] = TexCoord + vec2(0, 4.5) * TexelSize;
		uv[8] = TexCoord + vec2(0, -4.5) * TexelSize;
	}
	else
	{
		uv[0] = TexCoord;
		uv[1] = TexCoord + vec2(1.5, 0) * TexelSize;
		uv[2] = TexCoord + vec2(-1.5, 0) * TexelSize;
		uv[3] = TexCoord + vec2(2.5, 0) * TexelSize;
		uv[4] = TexCoord + vec2(-2.5, 0) * TexelSize;
		uv[5] = TexCoord + vec2(3.5, 0) * TexelSize;
		uv[6] = TexCoord + vec2(-3.5, 0) * TexelSize;
		uv[7] = TexCoord + vec2(4.5, 0) * TexelSize;
		uv[8] = TexCoord + vec2(-4.5, 0) * TexelSize;
	}

	vec3 ColorSum = texture(u_InputTexture, uv[0]).rgb * weight[0];
	float WeightSum = weight[0];
	if(CurrentEdgeFlags < 0.7f)
	{
		for(int i = 1; i < WEIGHT_NUM; ++i)
		{
			float RightNeighborEdgeFlag = texture(u_EdgeTexture, uv[i*2-1]).r;
			if(RightNeighborEdgeFlag < 0.7f)
			{
				ColorSum += texture(u_InputTexture, uv[i*2-1]).rgb * weight[i];
				WeightSum += weight[i];
			}
			else 
				break;
		}
	}

	for(int i = 1; i < WEIGHT_NUM; ++i)
	{
		float LeftNeighborEdgeFlags = texture(u_EdgeTexture, uv[i*2]).r;
		if(LeftNeighborEdgeFlags < 0.7f)
		{
			ColorSum += texture(u_InputTexture, uv[i*2]).rgb * weight[i];
			WeightSum += weight[i];
		}
		else
			break;
	}
	ColorSum /= WeightSum;

	imageStore(u_OutputImage, FragPos, vec4(ColorSum, 1.0f));
}