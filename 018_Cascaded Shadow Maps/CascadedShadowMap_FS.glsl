#version 430 core

layout(rgba16f ,binding = 0) uniform writeonly image2DArray u_OutputImage;
uniform int u_Index;

void main()
{
	imageStore(u_OutputImage, ivec3(gl_FragCoord.xy,u_Index), vec4(gl_FragCoord.z,0,0, 1.0));
}