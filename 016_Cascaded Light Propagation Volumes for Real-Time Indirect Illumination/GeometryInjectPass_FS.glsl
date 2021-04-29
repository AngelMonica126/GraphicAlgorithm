#version 430 core
#extension GL_NV_shader_atomic_float : require
#extension GL_NV_shader_atomic_fp16_vector : require
#extension GL_NV_gpu_shader5 : require

#define PI 3.1415926f

#define SH_C0 0.282094792f // 1 / 2sqrt(pi)
#define SH_C1 0.488602512f // sqrt(3/pi) / 2

#define SH_cosLobe_C0 0.886226925f // sqrt(pi)/2 
#define SH_cosLobe_C1 1.02332671f // sqrt(pi/3) 

layout(rgba16f ,binding = 0) uniform image3D GeometryVolume_;

flat in ivec3 v2f_volumeCellIndex;
in vec3 v2f_posFromRSM;
in vec3 v2f_normalFromRSM;
in float surfelArea;

uniform float u_CellSize;
uniform vec3 u_LightDir;

vec4 evalCosineLobeToDir(vec3 dir) {
	return vec4(SH_cosLobe_C0, -SH_cosLobe_C1 * dir.y, SH_cosLobe_C1 * dir.z, -SH_cosLobe_C1 * dir.x);
}

float calculateBlockingPotencial(vec3 dir, vec3 normal) {
	return clamp((surfelArea * clamp(dot(normal,dir),0.0,1.0))/(u_CellSize * u_CellSize),0.0,1.0);
}

void main()
{
	float BlockingPotencial = calculateBlockingPotencial(u_LightDir, v2f_normalFromRSM);
	vec4 SHCoeffGV = evalCosineLobeToDir(v2f_normalFromRSM) * BlockingPotencial;
	imageAtomicAdd(GeometryVolume_,v2f_volumeCellIndex,f16vec4(SHCoeffGV));
}