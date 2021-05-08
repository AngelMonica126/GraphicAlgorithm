#version 430 core
#extension GL_NV_shader_atomic_float : require
#extension GL_NV_shader_atomic_fp16_vector : require
#extension GL_NV_gpu_shader5 : require

#define PI 3.1415926f

#define SH_C0 0.282094792f // 1 / 2sqrt(pi)
#define SH_C1 0.488602512f // sqrt(3/pi) / 2

#define SH_cosLobe_C0 0.886226925f // sqrt(pi)/2 
#define SH_cosLobe_C1 1.02332671f // sqrt(pi/3) 

layout(rgba16f ,binding = 0) uniform image3D RAccumulatorLPV_;
layout(rgba16f ,binding = 1) uniform image3D GAccumulatorLPV_;
layout(rgba16f ,binding = 2) uniform image3D BAccumulatorLPV_;

layout(rgba16f ,binding = 3) uniform image3D LPVGridR_;
layout(rgba16f ,binding = 4) uniform image3D LPVGridG_;
layout(rgba16f ,binding = 5) uniform image3D LPVGridB_;

uniform sampler3D LPVGridR;
uniform sampler3D LPVGridG;
uniform sampler3D LPVGridB;
uniform sampler3D GeometryVolume;

flat in ivec3 v2f_volumeCellIndex;

uniform vec3 u_GridDim;
uniform bool u_FirstPropStep;
uniform float occlusionAmplifier = 1.0f;

const float directFaceSubtendedSolidAngle =  0.4006696846f; 
const float sideFaceSubtendedSolidAngle = 0.4234413544f; 

vec4 evalSH_direct(vec3 dir) {	
	return vec4(SH_C0, -SH_C1 * dir.y, SH_C1 * dir.z, -SH_C1 * dir.x);
}

vec4 evalCosineLobeToDir(vec3 dir) {
	return vec4(SH_cosLobe_C0, -SH_cosLobe_C1 * dir.y, SH_cosLobe_C1 * dir.z, -SH_cosLobe_C1 * dir.x);
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
const ivec2 cellSides[4] = {ivec2(1.0, 0.0), ivec2(0.0, 1.0), ivec2(-1.0, 0.0), ivec2(0.0, -1.0)};

vec3 getEvalSideDirection(int index, ivec3 orientation) {
	const float smallComponent = 0.4472135; // 1 / sqrt(5)
	const float bigComponent = 0.894427; // 2 / sqrt(5)
	const ivec2 side = cellSides[ index ];
	vec3 tmp = vec3(side.x * smallComponent, side.y * smallComponent, bigComponent);
	return vec3(orientation.x * tmp.x, orientation.y * tmp.y, orientation.z * tmp.z);
}

vec3 getReprojSideDirection(int index, ivec3 orientation) {
	const ivec2 side = cellSides[ index ];
	return vec3(orientation.x*side.x, orientation.y*side.y, 0);
}


void main()
{
	vec4 cR = vec4(0.0);
	vec4 cG = vec4(0.0);
	vec4 cB = vec4(0.0);
	for(int neighbour = 0; neighbour < 6; neighbour++) {
		vec4 RSHcoeffsNeighbour = vec4(0.0);
		vec4 GSHcoeffsNeighbour = vec4(0.0);
		vec4 BSHcoeffsNeighbour = vec4(0.0);
		ivec3 mainDirection = propDirections[neighbour]; 
		ivec3 neighbourCellIndex = v2f_volumeCellIndex - mainDirection;
	
		RSHcoeffsNeighbour = texelFetch(LPVGridR, neighbourCellIndex, 0);
		GSHcoeffsNeighbour = texelFetch(LPVGridG, neighbourCellIndex, 0);
		BSHcoeffsNeighbour = texelFetch(LPVGridB, neighbourCellIndex, 0);

		vec3 occCoord = (vec3(neighbourCellIndex.xyz) + 0.5 * mainDirection) / u_GridDim;
		vec4 occCoeffs = texture(GeometryVolume, occCoord);
		float occlusionValue = 1.0 - clamp(occlusionAmplifier * dot(occCoeffs, evalSH_direct(-mainDirection)),0.0,1.0 );
		float occludedDirectFaceContribution = occlusionValue * directFaceSubtendedSolidAngle;
		vec4 mainDirectionCosineLobeSH = evalCosineLobeToDir(mainDirection);
		vec4 mainDirectionSH = evalSH_direct(mainDirection);
		cR += occludedDirectFaceContribution * max(0.0, dot(RSHcoeffsNeighbour, mainDirectionCosineLobeSH)) * mainDirectionSH;
		cG += occludedDirectFaceContribution * max(0.0, dot(GSHcoeffsNeighbour, mainDirectionCosineLobeSH)) * mainDirectionSH;
		cB += occludedDirectFaceContribution * max(0.0, dot(BSHcoeffsNeighbour, mainDirectionCosineLobeSH)) * mainDirectionSH;
	
		for(int face = 0; face < 4; face++) {
			vec3 evalDirection = getEvalSideDirection(face, mainDirection);
			vec3 reprojDirection = getReprojSideDirection(face, mainDirection);

			vec3 occCoord = (vec3(neighbourCellIndex.xyz) + 0.5 * evalDirection) / u_GridDim;
			vec4 occCoeffs = texture(GeometryVolume, occCoord);
			occlusionValue = 1.0 - clamp(occlusionAmplifier * dot(occCoeffs, evalSH_direct(-evalDirection)),0.0,1.0);
			float occludedSideFaceContribution = occlusionValue * sideFaceSubtendedSolidAngle;
			vec4 reprojDirectionCosineLobeSH = evalSH_direct(reprojDirection);
			vec4 evalDirectionSH = evalCosineLobeToDir(evalDirection);
			
			cR += occludedSideFaceContribution * max(0.0, dot(RSHcoeffsNeighbour, evalDirectionSH)) * reprojDirectionCosineLobeSH;
			cG += occludedSideFaceContribution * max(0.0, dot(GSHcoeffsNeighbour, evalDirectionSH)) * reprojDirectionCosineLobeSH;
			cB += occludedSideFaceContribution * max(0.0, dot(BSHcoeffsNeighbour, evalDirectionSH)) * reprojDirectionCosineLobeSH;
		}
	}

	imageAtomicAdd(LPVGridR_,v2f_volumeCellIndex,f16vec4(cR / PI));
	imageAtomicAdd(LPVGridG_,v2f_volumeCellIndex,f16vec4(cG / PI));
	imageAtomicAdd(LPVGridB_,v2f_volumeCellIndex,f16vec4(cB / PI));

	vec4 R = imageLoad(LPVGridR_, v2f_volumeCellIndex);
	vec4 G = imageLoad(LPVGridG_, v2f_volumeCellIndex);
	vec4 B = imageLoad(LPVGridB_, v2f_volumeCellIndex);

	imageAtomicAdd(RAccumulatorLPV_, v2f_volumeCellIndex, f16vec4(R));
	imageAtomicAdd(GAccumulatorLPV_, v2f_volumeCellIndex, f16vec4(G));
	imageAtomicAdd(BAccumulatorLPV_, v2f_volumeCellIndex, f16vec4(B));
}