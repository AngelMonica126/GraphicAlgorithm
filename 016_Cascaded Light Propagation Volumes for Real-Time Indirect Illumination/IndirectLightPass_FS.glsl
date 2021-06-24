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
uniform mat4 u_InverseCameraViewMatrix;

vec4 evalSH_direct( vec3 direction ) {	
	return vec4( SH_C0, -SH_C1 * direction.y, SH_C1 * direction.z, -SH_C1 * direction.x );
}

ivec3 convertPointToGridIndex(vec3 vPos) {
	return ivec3((vPos - u_MinAABB) / u_CellSize);
}
const ivec3 propDirections[8] = {
	ivec3(u_CellSize * 0.5,u_CellSize * 0.5,u_CellSize * 0.5),
	ivec3(u_CellSize * 0.5,u_CellSize * 0.5,-u_CellSize * 0.5),
	ivec3(u_CellSize * 0.5,-u_CellSize * 0.5,u_CellSize * 0.5),
	ivec3(u_CellSize * 0.5,-u_CellSize * 0.5,-u_CellSize * 0.5),
	ivec3(-u_CellSize * 0.5,u_CellSize * 0.5,u_CellSize * 0.5),
	ivec3(-u_CellSize * 0.5,u_CellSize * 0.5,-u_CellSize * 0.5),
	ivec3(-u_CellSize * 0.5,-u_CellSize * 0.5,u_CellSize * 0.5),
	ivec3(-u_CellSize * 0.5,-u_CellSize * 0.5,-u_CellSize * 0.5)
	
};
void main()
{
	vec3 Normal = texture(u_NormalTexture,v2f_TexCoords).xyz;
	vec3 Position = texture(u_PositionTexture, v2f_TexCoords).xyz;
	Position = vec3(u_InverseCameraViewMatrix * vec4(Position,1));
	vec4 SHintensity = evalSH_direct(Normal);
	ivec3 LpvCellCoords = convertPointToGridIndex(Position);
	vec3 LpvIntensity  = vec3(0);
	vec3 LpvCellBasePos = vec3(convertPointToGridIndex(Position)) * u_CellSize +  u_MinAABB;
	vec3 alpha = clamp((Position - LpvCellBasePos) / u_CellSize, vec3(0), vec3(1));
	for (int i = 0; i < 8; ++i) {
		ivec3 offset = ivec3(i, i >> 1, i >> 2) & ivec3(1);
		ivec3 LpvCellCoords = convertPointToGridIndex(Position) + offset;
		vec3 trilinear = mix (1 - alpha, alpha, offset);
		float weight = trilinear.x * trilinear.y * trilinear.z;

		weight = max(0.0002, weight);
		LpvIntensity += weight 
		* vec3( 
				dot(SHintensity, texelFetch(u_RAccumulatorLPV, LpvCellCoords,0)),
				dot(SHintensity, texelFetch(u_GAccumulatorLPV, LpvCellCoords,0)),
				dot(SHintensity, texelFetch(u_BAccumulatorLPV, LpvCellCoords,0))
		);
	}
	Color_ = vec4(max(LpvIntensity, 0 ),1) ;
}