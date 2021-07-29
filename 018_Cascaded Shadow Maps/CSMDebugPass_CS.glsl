#version 430 core
#pragma optionNV (unroll all)	//暂时不知道有没有起作用

#define LOCAL_GROUP_SIZE 32

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;
uniform sampler2D u_InputPositionTexture;

layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputCSMDebugImage;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform mat4  u_LightVPMatrixMulInverseCameraViewMatrix[5];
uniform int u_SpiltNum = 5;

uniform vec3 u_Color[5] = vec3[](vec3(1,0,0),vec3(1,1,0),vec3(1,1,1),vec3(0,1,1),vec3(1,0,1));

void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	vec4 Position = texelFetch(u_InputPositionTexture, FragPos, 0);
	vec3 FragViewPos = Position.xyz;
	for(int i = 0; i < u_SpiltNum; i++)
	{
		vec4 FragPosInLightSpace = u_LightVPMatrixMulInverseCameraViewMatrix[i] * vec4(FragViewPos, 1);
		FragPosInLightSpace /= FragPosInLightSpace.w;
		FragPosInLightSpace.xyz = (FragPosInLightSpace.xyz + 1) / 2;
		if(FragPosInLightSpace.x > 1.0f || FragPosInLightSpace.y > 1.0f || FragPosInLightSpace.x < 0.0f || FragPosInLightSpace.y < 0.0f)
			continue;
		else
		{
			imageStore(u_OutputCSMDebugImage, FragPos, vec4(u_Color[i], 1));
			break;
		}
	}

}