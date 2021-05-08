#version 430 core
#extension GL_NV_shader_atomic_float : require
#extension GL_NV_shader_atomic_fp16_vector : require
#extension GL_NV_gpu_shader5 : require

#define PI 3.1415926f

#define SH_C0 0.282094792f // 1 / 2sqrt(pi)
#define SH_C1 0.488602512f // sqrt(3/pi) / 2

#define SH_cosLobe_C0 0.886226925f // sqrt(pi)/2 
#define SH_cosLobe_C1 1.02332671f // sqrt(pi/3) 

layout(rgba16f ,binding = 0) uniform image3D LPVGridR_;
layout(rgba16f ,binding = 1) uniform image3D LPVGridG_;
layout(rgba16f ,binding = 2) uniform image3D LPVGridB_;

flat in ivec3 v2f_volumeCellIndex;
in vec3 v2f_posFromRSM;
in vec3 v2f_normalFromRSM;
in vec4 v2f_fluxFromRSM;
in vec3 v2f_vplToCell;

vec4 evalSH_direct(vec3 dir) {	
	return vec4(SH_C0, -SH_C1 * dir.y, SH_C1 * dir.z, -SH_C1 * dir.x);
}


void main()
{
	vec4 SHCoeffsR = evalSH_direct(v2f_vplToCell) * v2f_fluxFromRSM.r;
	vec4 SHCoeffsG = evalSH_direct(v2f_vplToCell) * v2f_fluxFromRSM.g;
	vec4 SHCoeffsB = evalSH_direct(v2f_vplToCell) * v2f_fluxFromRSM.b;

	imageAtomicAdd(LPVGridR_,v2f_volumeCellIndex,f16vec4(SHCoeffsR));
	imageAtomicAdd(LPVGridG_,v2f_volumeCellIndex,f16vec4(SHCoeffsG));
	imageAtomicAdd(LPVGridB_,v2f_volumeCellIndex,f16vec4(SHCoeffsB));
}