#version 430 core

layout (location = 0) in vec2 _Position;
flat out ivec3 v2f_volumeCellIndex;
out vec3 v2f_posFromRSM;
out vec3 v2f_normalFromRSM;
out vec4 v2f_fluxFromRSM;
out vec3 v2f_vplToCell;
uniform sampler2D u_RSMRadiantFluxTexture;
uniform sampler2D u_RSMPositionTexture;
uniform sampler2D u_RSMNormalTexture;
uniform float u_CellSize;
uniform vec3 u_MinAABB;
uniform int u_RSMResolution;

ivec3 convertPointToGridIndex(vec3 vPos) {
	return ivec3((vPos - u_MinAABB) / u_CellSize);
}

void main()
{
	ivec2 RSMCoords = ivec2(gl_VertexID % u_RSMResolution, gl_VertexID / u_RSMResolution);
	v2f_posFromRSM = texelFetch(u_RSMPositionTexture, RSMCoords,0).rgb;
	v2f_normalFromRSM = texelFetch(u_RSMNormalTexture, RSMCoords,0).rgb;
	v2f_fluxFromRSM = texelFetch(u_RSMRadiantFluxTexture, RSMCoords,0);
	v2f_volumeCellIndex = convertPointToGridIndex(v2f_posFromRSM);
	vec3 CellCenter = (v2f_volumeCellIndex - 0.5) * u_CellSize + u_MinAABB;
	v2f_vplToCell = normalize(v2f_posFromRSM - CellCenter);
	gl_Position = vec4(_Position, 0.0, 1.0);
}