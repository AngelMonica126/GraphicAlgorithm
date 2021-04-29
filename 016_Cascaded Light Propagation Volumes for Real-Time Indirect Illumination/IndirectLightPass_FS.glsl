#version 430 core
#define SH_C0 0.282094792f // 1 / 2sqrt(pi)
#define SH_C1 0.488602512f // sqrt(3/pi) / 2
#define PI 3.1415926f


in  vec2 v2f_TexCoords;

layout (location = 0) out vec4 Color_;

uniform sampler3D u_RAccumulatorLPV;
uniform sampler3D u_GAccumulatorLPV;
uniform sampler3D u_BAccumulatorLPV;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_PositionTexture;
uniform float u_CellSize;
uniform vec3 u_MinAABB;

vec4 evalSH_direct( vec3 direction ) {	
	return vec4( SH_C0, -SH_C1 * direction.y, SH_C1 * direction.z, -SH_C1 * direction.x );
}

ivec3 convertPointToGridIndex(vec3 vPos) {
	return ivec3((vPos - u_MinAABB) / u_CellSize);
}
const ivec3 propDirections[6] = {
	//+Z
	ivec3(0,0,1),
	//-Z
	ivec3(0,0,-1),
	//+X
	ivec3(1,0,0),
	//-X
	ivec3(-1,0,0),
	//+Y
	ivec3(0,1,0),
	//-Y
	ivec3(0,-1,0)
};
void main()
{
	vec3 Normal = texture(u_NormalTexture,v2f_TexCoords).xyz;
	vec3 Position = texture(u_PositionTexture, v2f_TexCoords).xyz;
	vec4 SHintensity = evalSH_direct(Normal);
	ivec3 LpvCellCoords = convertPointToGridIndex(Position);
	vec3 LpvIntensity =  vec3( 
				dot(SHintensity, texelFetch(u_RAccumulatorLPV, LpvCellCoords,0)),
				dot(SHintensity, texelFetch(u_GAccumulatorLPV, LpvCellCoords,0)),
				dot(SHintensity, texelFetch(u_BAccumulatorLPV, LpvCellCoords,0))
		) * 0.36;	
	for(int neighbour = 0; neighbour < 6; neighbour++)
	{
		LpvIntensity += vec3( 
				dot(SHintensity, texelFetch(u_RAccumulatorLPV, LpvCellCoords - propDirections[neighbour],0)),
				dot(SHintensity, texelFetch(u_GAccumulatorLPV, LpvCellCoords - propDirections[neighbour],0)),
				dot(SHintensity, texelFetch(u_BAccumulatorLPV, LpvCellCoords - propDirections[neighbour],0))
		) * 0.12;	
	}
	Color_ = vec4(max(LpvIntensity, 0 ),1) ;
}