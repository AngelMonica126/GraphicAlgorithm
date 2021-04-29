#version 430 core

#define SH_C0 0.282094792f // 1 / 2sqrt(pi)
#define SH_C1 0.488602512f // sqrt(3/pi) / 2
#define PI 3.1415926f
in vec3 v2f_FragPosInViewSpace;
in vec2 v2f_TexCoords;
in vec3 v2f_Normal;
flat in ivec3 v2f_volumeCellIndex;

out vec4 Color_;
uniform float u_CellSize;
uniform vec3 u_MinAABB;

vec4 evalSH_direct( vec3 direction ) {	
	return vec4( SH_C0, -SH_C1 * direction.y, SH_C1 * direction.z, -SH_C1 * direction.x );
}

ivec3 convertPointToGridIndex(vec3 vPos) {
	return ivec3((vPos - u_MinAABB) / u_CellSize);
}

uniform sampler3D u_RAccumulatorLPV;
uniform sampler3D u_GAccumulatorLPV;
uniform sampler3D u_BAccumulatorLPV;


void main()
{
	vec4 SHintensity = evalSH_direct(normalize(v2f_Normal));
	ivec3 LpvCellCoords = convertPointToGridIndex(v2f_FragPosInViewSpace);
	vec3 LpvIntensity = vec3( 
			dot(SHintensity, texelFetch(u_RAccumulatorLPV, LpvCellCoords,0) ),
			dot(SHintensity, texelFetch(u_GAccumulatorLPV, LpvCellCoords,0) ),
			dot(SHintensity, texelFetch(u_BAccumulatorLPV, LpvCellCoords,0) )
		);
	Color_ = vec4(max(LpvIntensity, 0 ) * 10,1) ;
}