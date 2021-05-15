#version 450
#define TriangleNum 17
#define TriangleVerticesNum 51
layout (triangles) in;
layout (triangle_strip, max_vertices = 51) out;


layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};
uniform mat4 u_ModelMatrix;
layout (std430, binding = 1) buffer SkSSBO
{
	float u_Sk[];
};

layout (std430, binding = 2) buffer NoiseSSBO
{
	float u_Noise[];
};

layout (std430, binding = 3) buffer SkpSSBO
{
	float u_Skp[];
};

struct STriangle
{
	vec3 mPositions[3];
	vec3 mNormals;
	vec2 mTexCoords;
	vec2 mAreaAndLevel;
};

in VS_GS_VEXTEX
{
	vec4 PositionS;
	vec3 NormalW;
	vec2 TexCoord;
}vs_gs_in[];

out GS_FS_VERTEX
{
	STriangle TriangleInfo;
}fragment_out;

uniform int u_NoiseNum;
uniform int u_NumS;
uniform float InWidth = 1.0 / 1920.0;
uniform float InHeight = 1.0 / 1080.0;
float getArea(vec3 vPositions[3])
{
	vec3 V1 = vPositions[1] - vPositions[0];
	vec3 V2 = vPositions[2] - vPositions[0];
	vec3 c = cross(V1,V2);
	return 0.5 * sqrt(dot(c,c));
}

int getLevel(float vArea, float vRand)
{
	int level = 0;
	for(int i = 1; i < u_NumS; i++)
	{
		float AS = vArea / u_Skp[i];
		if(vRand < AS)
		{
			level = i - 1;
			break;
		}
	}
	return level;
}


int getMaxEdge(vec3 vEdge[3])
{
	float edge[3];
	edge[0] = dot(vEdge[0],vEdge[0]);
	edge[1] = dot(vEdge[1],vEdge[1]);
	edge[2] = dot(vEdge[2],vEdge[2]);
	if(edge[0] > edge[1] && edge[0] > edge[2])
		return 0;
	if(edge[1] > edge[0] && edge[1] > edge[2])
		return 1;
	if(edge[2] > edge[0] && edge[2] > edge[1])
		return 2;

}

void splitTriangle(vec3 vPositions[3], out vec3 TriangleOne[3],out vec3 TriangleTwo[3])
{
	vec3 V[3];
	V[0] = vPositions[1] - vPositions[0];
	V[1] = vPositions[2] - vPositions[1];
	V[2] = vPositions[2] - vPositions[0];
	int maxEdge = getMaxEdge(V);
	if(maxEdge == 0)
	{
		vec3 mid = 0.5 * (vPositions[0] + vPositions[1]);
		TriangleOne[0] = vPositions[0];
		TriangleOne[1] = vPositions[2];
		TriangleOne[2] = mid;

		TriangleTwo[0] = vPositions[2];
		TriangleTwo[1] = vPositions[1];
		TriangleTwo[2] = mid;
	}
	else if(maxEdge == 1)
	{
		vec3 mid = 0.5 * (vPositions[2] + vPositions[1]);
		TriangleOne[0] = vPositions[1];
		TriangleOne[1] = vPositions[0];
		TriangleOne[2] = mid;

		TriangleTwo[0] = vPositions[0];
		TriangleTwo[1] = vPositions[2];
		TriangleTwo[2] = mid;
	}
	else
	{
		vec3 mid = 0.5 * (vPositions[2] + vPositions[0]);
		TriangleOne[0] = vPositions[1];
		TriangleOne[1] = vPositions[0];
		TriangleOne[2] = mid;

		TriangleTwo[0] = vPositions[2];
		TriangleTwo[1] = vPositions[1];
		TriangleTwo[2] = mid;
	}
}

vec3 getNormal(vec3 vTriangle[3])
{
	vec3 V1 = vTriangle[1] - vTriangle[0];
	vec3 V2 = vTriangle[2] - vTriangle[0];
	return normalize(cross(V1,V2));
}

vec2 getTexcoord(vec3 P)
{
	vec3 P0 = vs_gs_in[0].PositionS.xyz;
	vec3 P1 = vs_gs_in[1].PositionS.xyz;
	vec3 P2 = vs_gs_in[2].PositionS.xyz;

	float b = ((P0.x - P2.x) * (P1.y - P2.y) - (P0.y - P2.y) * (P1.x - P2.x));
	float u = (P.x *(P1.y - P2.y) - P.y * (P1.x - P2.x)) / b;
	float v = (P.x *(P0.y - P2.y) - P.y * (P0.x - P2.x)) / (-b);
	
	vec2 T0 = vs_gs_in[0].TexCoord - vs_gs_in[2].TexCoord;
	vec2 T1 = vs_gs_in[1].TexCoord - vs_gs_in[2].TexCoord;
	return T0 * u + T1 * v;

}
void sendTriangle(vec3 vTriangle[3],float vArea,float vLevel,int temp)
{
	STriangle TempTriangle;
	TempTriangle.mPositions[0] = vTriangle[0];
	TempTriangle.mPositions[1] = vTriangle[1];
	TempTriangle.mPositions[2] = vTriangle[2];
	TempTriangle.mNormals = getNormal(vTriangle);
	vec3 Center = (TempTriangle.mPositions[0] + TempTriangle.mPositions[1] + TempTriangle.mPositions[2]) / 3.0;
	TempTriangle.mTexCoords = getTexcoord(Center);
	TempTriangle.mAreaAndLevel = vec2(vArea,vLevel);
	for (int i = 0; i < 3; ++i)
	{
		fragment_out.TriangleInfo = TempTriangle;
		gl_Position = vec4(temp * InWidth,0,0,1);
//		gl_Position = u_ProjectionMatrix * u_ViewMatrix  * u_ModelMatrix * vec4(vTriangle[i],1);
		EmitVertex();
	}

}
void main()
{
	vec3 stack[TriangleVerticesNum];
	int top = -1;
	stack[++top] = vs_gs_in[0].PositionS.xyz;
	stack[++top] = vs_gs_in[1].PositionS.xyz;
	stack[++top] = vs_gs_in[2].PositionS.xyz;
	float rand = u_Noise[gl_PrimitiveIDIn % u_NoiseNum];
	float maxS = u_Skp[u_NumS - 1];
	int temp = 0;
	while(top > 0 && top < TriangleVerticesNum)
	{
		vec3 Triangle[3];
		Triangle[0] = stack[top--];
		Triangle[1] = stack[top--];
		Triangle[2] = stack[top--];
		float area = getArea(Triangle);
		if(area <= rand * u_Skp[0])continue;
		if(rand * maxS > area)
		{
			//·Ö¸î
			if(top >= TriangleVerticesNum) continue;
			vec3 TriangleOne[3];
			vec3 TriangleTwo[3];
			splitTriangle(Triangle,TriangleOne,TriangleTwo);
			if(top < TriangleVerticesNum)
			{
				stack[++top] = TriangleOne[0];
				stack[++top] = TriangleOne[1];
				stack[++top] = TriangleOne[2];
			}
			if(top < TriangleVerticesNum)
			{
				stack[++top] = TriangleTwo[0];
				stack[++top] = TriangleTwo[1];
				stack[++top] = TriangleTwo[2];
			}
		}
		else
		{
			int level = getLevel(area,rand);
			sendTriangle(Triangle,area,level,temp);
			temp++;
		}
	}
	gl_Position = vec4(0,InHeight,0,1);
	EmitVertex();
	EndPrimitive();

}