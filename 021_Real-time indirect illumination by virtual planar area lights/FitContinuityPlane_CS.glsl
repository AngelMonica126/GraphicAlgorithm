#version 430 core
#pragma optionNV (unroll all)	

#define LOCAL_GROUP_SIZE 1
#define PI 3.1415926

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;

struct SPlane	//newnew
{
	vec4 P0;
	vec4 P1;
	vec4 P2;
	vec4 P3;
	vec4 Normal;
	vec4 ProportionAndUV;
	ivec4 Index;
};

layout (std430, binding = 1) buffer SPlanes
{
	SPlane u_Planes[];
};
layout (std430, binding = 2) buffer SVALs
{
	SPlane u_VALs[];
};
layout (std430, binding = 3) buffer SVALCount
{
	int u_VALCount;
};
uniform int u_Block;
uniform float u_DistThresh;
uniform float u_NormalThresh;
uniform float u_Area;
int findVAL(int vBegin, int vEnd, int vBeginRow, int vParent,vec3 vNormal, inout int vCount)
{
	int rows = 1;
	for(int i = vBeginRow; i < u_Block; i++)
	{
		for(int j = vBegin; j <= vEnd; j++)
		{
			vec3 Temp1 = u_Planes[j + (rows - 1) * u_Block].P3.xyz - u_Planes[j + rows * u_Block].P2.xyz;
			vec3 Temp2 = u_Planes[j + (rows - 1) * u_Block].P0.xyz - u_Planes[j + rows * u_Block].P1.xyz;
			vec3 Normal1 = u_Planes[j + rows * u_Block].Normal.xyz;
			vec3 Normal2 =  u_Planes[j + (rows - 1) * u_Block].Normal.xyz;
			float maxDistThresh = u_DistThresh * max( u_Planes[j + (rows - 1) * u_Block].Normal.w, u_Planes[j + rows * u_Block].Normal.w);
			if((dot(Normal1,Normal2) > u_NormalThresh) && (dot(Temp1, Temp1) < maxDistThresh * maxDistThresh &&
			dot(Temp2, Temp2) < maxDistThresh * maxDistThresh) &&  u_Planes[j + (rows) * u_Block].Index.x == -1 )
				 u_Planes[j + rows * u_Block].Index.x = vParent;
			else 
			{
				for(int k = vBegin; k <= j; k++)
					u_Planes[k + rows * u_Block].Index.x = -1;
				return rows;
			}
		}
		vCount +=(vEnd - vBegin + 1);
		rows++;
	}
	return rows;
}


void main()
{
	const int PlaneSize = u_Block * u_Block;
	const int StackSize =  u_Block;
	int PtrBegin = -1;
	int PtrEnd = -1;
	int Index = 0;
	int Count = 0;
	for(int i = 0; i < PlaneSize; i++)
	{
		if(u_Planes[i].Index.x != -1)
			continue;
		PtrBegin = PtrEnd = i;
		u_Planes[i].Index.x = i;
		Count++;
		int end =  int(ceil(float(i + 1) / float(u_Block))) * u_Block;
		for(int j = i + 1; j < end; j++)
		{
			vec3 Temp1 = u_Planes[j - 1].P0.xyz -  u_Planes[j].P3.xyz;
			vec3 Temp2 = u_Planes[j - 1].P1.xyz -  u_Planes[j].P2.xyz;
			vec3 Normal1 = u_Planes[j - 1].Normal.xyz;
			vec3 Normal2 = u_Planes[j].Normal.xyz;
			float maxDistThresh = u_DistThresh * max(u_Planes[j - 1].Normal.w,u_Planes[j].Normal.w);
			if(u_Planes[j].Index.x == -1 && (dot(Normal1,Normal2) > u_NormalThresh &&
			(dot(Temp1, Temp1) < maxDistThresh * maxDistThresh && dot(Temp2, Temp2) < maxDistThresh * maxDistThresh )))
			{
				 u_Planes[j].Index.x = i;
				 PtrEnd = j;
				 Count++;
			}
			else
			{
				int row = findVAL(PtrBegin,PtrEnd,int(ceil(float(i + 1) / float(u_Block))),i,u_Planes[i].Normal.xyz, Count);
				u_VALs[Index].P0 = u_Planes[PtrEnd + (row - 1) * u_Block].P0;
				u_VALs[Index].P1 = u_Planes[PtrEnd].P1;
				u_VALs[Index].P2 = u_Planes[PtrBegin].P2;
				u_VALs[Index].P3 = u_Planes[PtrBegin + (row - 1) * u_Block].P3;
				float Area = u_Area * Count; 
				vec3 V1 = u_VALs[Index].P3.xyz - u_VALs[Index].P0.xyz;
				vec3 V2 = u_VALs[Index].P1.xyz - u_VALs[Index].P0.xyz;
				vec3 PlaneOrtho = cross(V1, V2);
				float PlaneArea = length(PlaneOrtho);
				u_VALs[Index].Normal = vec4(normalize(PlaneOrtho),Area / PlaneArea);
				u_VALs[Index].ProportionAndUV = vec4(u_Planes[i].ProportionAndUV.x * (PtrEnd  - PtrBegin+ 1.0), u_Planes[i].ProportionAndUV.y * row, u_Planes[i].ProportionAndUV.zw);
				PtrBegin = PtrEnd = -1;
				Index++;
				Count = 0;
				break;
			}
		}
		if(PtrBegin != -1)
		{
			int row = findVAL(PtrBegin,PtrEnd,int(ceil(float(i + 1) / float(u_Block))),i,u_Planes[i].Normal.xyz, Count);
			u_VALs[Index].P0 = u_Planes[PtrEnd + (row - 1) * u_Block].P0;
			u_VALs[Index].P1 = u_Planes[PtrEnd].P1;
			u_VALs[Index].P2 = u_Planes[PtrBegin].P2;
			u_VALs[Index].P3 = u_Planes[PtrBegin + (row - 1) * u_Block].P3;
			float Area = u_Area * Count; 
			vec3 V1 = u_VALs[Index].P3.xyz - u_VALs[Index].P0.xyz;
			vec3 V2 = u_VALs[Index].P1.xyz - u_VALs[Index].P0.xyz;
			vec3 PlaneOrtho = cross(V1, V2);
			float PlaneArea = length(PlaneOrtho);
			u_VALs[Index].Normal = vec4(normalize(PlaneOrtho),Area / PlaneArea);
			u_VALs[Index].ProportionAndUV = vec4(u_Planes[i].ProportionAndUV.x * (PtrEnd  - PtrBegin + 1.0), u_Planes[i].ProportionAndUV.y * row, u_Planes[i].ProportionAndUV.zw);
			PtrBegin = PtrEnd = -1;
			Index++;
			Count = 0;
		}
	}
	u_VALCount = Index;
}