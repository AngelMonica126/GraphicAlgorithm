#version 430 core

layout (location = 0) in vec2 _Position;
flat out ivec3 v2f_volumeCellIndex;
out vec3 v2f_posFromRSM;
out vec3 v2f_normalFromRSM;
out float surfelArea;


uniform sampler2D u_RSMPositionTexture;
uniform sampler2D u_RSMNormalTexture;
uniform float u_CellSize;
uniform vec3 u_MinAABB;
uniform int u_RSMResolution;
uniform float u_RSMArea = 4.0;
uniform mat4 u_LightViewMat;


ivec3 convertPointToGridIndex(vec3 vPos) {
	return ivec3((vPos - u_MinAABB) / u_CellSize);
}

float calculateSurfelAreaLightOrtho(vec3 lightPos) {
	return (u_RSMArea)/(u_RSMResolution * u_RSMResolution);
}

void main()
{
	ivec2 RSMCoords = ivec2(gl_VertexID % u_RSMResolution, gl_VertexID / u_RSMResolution);
	v2f_posFromRSM = texelFetch(u_RSMPositionTexture, RSMCoords,0).rgb;
	v2f_normalFromRSM = texelFetch(u_RSMNormalTexture, RSMCoords,0).rgb;

	vec4 viewPos = u_LightViewMat * vec4(v2f_posFromRSM,1.0);
	surfelArea = calculateSurfelAreaLightOrtho(viewPos.xyz);

	v2f_volumeCellIndex = convertPointToGridIndex(v2f_posFromRSM);

	gl_Position = vec4(_Position, 0.0, 1.0);
}