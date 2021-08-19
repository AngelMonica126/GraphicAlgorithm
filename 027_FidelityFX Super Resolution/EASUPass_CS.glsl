#version 430 core
#pragma optionNV (unroll all)	
#define LOCAL_GROUP_SIZE 32

layout(local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;
layout(rgba32f, binding = 0) uniform writeonly image2D u_OutputEASUTexture;

uniform sampler2D u_InputTexture;
uniform int u_DisplayWidth;
uniform int u_DisplayHeight;
uniform vec4 u_Con0;
uniform vec4 u_Con1;
uniform vec4 u_Con2;
uniform vec4 u_Con3;

vec4 fsrEasuRF(vec2 p) { vec4 res = vec4(textureGather(u_InputTexture, p, 0)); return res; }
vec4 fsrEasuGF(vec2 p) { vec4 res = vec4(textureGather(u_InputTexture, p, 1)); return res; }
vec4 fsrEasuBF(vec2 p) { vec4 res = vec4(textureGather(u_InputTexture, p, 2)); return res; }
vec3 min3F3(vec3 x, vec3 y, vec3 z) { return min(x, min(y, z)); }
vec3 max3F3(vec3 x, vec3 y, vec3 z) { return max(x, max(y, z)); }

void fsrEasuTapF(
	inout vec3 aC, // Accumulated color, with negative lobe.
	inout float aW, // Accumulated weight.
	vec2 Off, // Pixel offset from resolve position to tap.
	vec2 Dir, // Gradient direction.
	vec2 Len, // Length.
	float Lob, // Negative lobe strength.
	float Clp, // Clipping point.
	vec3 Color) { // Tap color.
	 // Rotate offset by direction.
	vec2 v;
	v.x = (Off.x * (Dir.x)) + (Off.y * Dir.y);
	v.y = (Off.x * (-Dir.y)) + (Off.y * Dir.x);
	// Anisotropy.
	v *= Len;
	// Compute distance^2.
	float x2 = v.x * v.x + v.y * v.y;
	// Limit to the window as at corner, 2 taps can easily be outside.
	x2 = min(x2, Clp);
	// Approximation of lancos2 without sin() or rcp(), or sqrt() to get x.
	//  (25/16 * (2/5 * x^2 - 1)^2 - (25/16 - 1)) * (1/4 * x^2 - 1)^2
	//  |_______________________________________|   |_______________|
	//                   base                             window
	// The general form of the 'base' is,
	//  (a*(b*x^2-1)^2-(a-1))
	// Where 'a=1/(2*b-b^2)' and 'b' moves around the negative lobe.
	float WindowB = float(2.0 / 5.0) * x2 + float(-1.0);
	float WindowA = Lob * x2 + float(-1.0);
	WindowB *= WindowB;
	WindowA *= WindowA;
	WindowB = float(25.0 / 16.0) * WindowB + float(-(25.0 / 16.0 - 1.0));
	float Window = (WindowB * WindowA);
	// Do Windoweighted average.
	aC += Color * Window; aW += Window;
}

// Accumulate direction and length.
void fsrEasuSetF(
	inout vec2 Dir,
	inout float Feature,
	vec2 P,
	bool BoolS, bool BoolT, bool BoolU, bool BoolV,
	float LumaA, float LumaB, float LumaC, float LumaD, float LumaE) {
	// Compute bilinear weight, branches factor out as predicates are compiler time immediates.
	//  s t
	//  u v
	float Weight = 0.0f;
	if (BoolS) Weight = (1.0f - P.x) * (1.0f - P.y);
	if (BoolT) Weight = P.x * (1.0f - P.y);
	if (BoolU) Weight = (1.0f - P.x) * P.y;
	if (BoolV) Weight = P.x * P.y;
	// Direction is the '+' diff.
	//    a
	//  b c d
	//    e
	// Then takes magnitude from abs average of both sides of 'c'.
	// Length converts gradient reversal to 0, smoothly to non-reversal at 1, shaped, then adding horz and vert terms.
	float DC = LumaD - LumaC;
	float CB = LumaC - LumaB;
	float FeatureX = max(abs(DC), abs(CB));
	float DirX = LumaD - LumaB;
	Dir.x += DirX * Weight;
	FeatureX = clamp(abs(DirX) / FeatureX, 0.0f, 1.0f);
	FeatureX *= FeatureX;
	Feature += FeatureX * Weight;
	// Repeat for the y axis.
	float EC = LumaE - LumaC;
	float CA = LumaC - LumaA;
	float FeatureY = max(abs(EC), abs(CA));
	float DirY = LumaE - LumaA;
	Dir.y += DirY * Weight;
	FeatureY = clamp(abs(DirY) / FeatureY, 0.0f, 1.0f);
	FeatureY *= FeatureY;
	Feature += FeatureY * Weight;
}

vec3 fsrEasuF(ivec2 ip)
{
	//      +---+---+
	//      |   |   |
	//      +--(0)--+
	//      | b | c |
	//  +---F---+---+---+
	//  | e | f | g | h |
	//  +--(1)--+--(2)--+
	//  | i | j | k | l |
	//  +---+---+---+---+
	//      | n | o |
	//      +--(3)--+
	//      |   |   |
	//      +---+---+
	//------------------------------------------------------------------------------------------------------------------------------
	// Get position of 'f'.
	vec2 P = vec2(ip) * u_Con0.xy + u_Con0.zw;
	vec2 F = floor(P);
	P -= F;
	vec2 P0 = F * u_Con1.xy + u_Con1.zw;
	// These are from P0 to avoid pulling two constants on pre-Navi hardware.
	vec2 P1 = P0 + u_Con2.xy;
	vec2 P2 = P0 + u_Con2.zw;
	vec2 P3 = P0 + u_Con3.xy;
	vec4 bczzR = fsrEasuRF(P0);
	vec4 bczzG = fsrEasuGF(P0);
	vec4 bczzB = fsrEasuBF(P0);
	vec4 ijfeR = fsrEasuRF(P1);
	vec4 ijfeG = fsrEasuGF(P1);
	vec4 ijfeB = fsrEasuBF(P1);
	vec4 klhgR = fsrEasuRF(P2);
	vec4 klhgG = fsrEasuGF(P2);
	vec4 klhgB = fsrEasuBF(P2);
	vec4 zzonR = fsrEasuRF(P3);
	vec4 zzonG = fsrEasuGF(P3);
	vec4 zzonB = fsrEasuBF(P3);
	//------------------------------------------------------------------------------------------------------------------------------
	  // Simplest multi-channel approximate luma possible (luma times 2, in 2 FMA/MAD).
	vec4 bczzL = bczzB * 0.5f + (bczzR * 0.5f + bczzG);
	vec4 ijfeL = ijfeB * 0.5f + (ijfeR * 0.5f + ijfeG);
	vec4 klhgL = klhgB * 0.5f + (klhgR * 0.5f + klhgG);
	vec4 zzonL = zzonB * 0.5f + (zzonR * 0.5f + zzonG);
	// Rename.
	float bL = bczzL.x;
	float cL = bczzL.y;
	float iL = ijfeL.x;
	float jL = ijfeL.y;
	float fL = ijfeL.z;
	float eL = ijfeL.w;
	float kL = klhgL.x;
	float lL = klhgL.y;
	float hL = klhgL.z;
	float gL = klhgL.w;
	float oL = zzonL.z;
	float nL = zzonL.w;
	// Accumulate for bilinear interpolation.
	vec2 Dir = vec2(0.0);
	float Feature = 0.0;
	fsrEasuSetF(Dir, Feature, P, true, false, false, false, bL, eL, fL, gL, jL);
	fsrEasuSetF(Dir, Feature, P, false, true, false, false, cL, fL, gL, hL, kL);
	fsrEasuSetF(Dir, Feature, P, false, false, true, false, fL, iL, jL, kL, nL);
	fsrEasuSetF(Dir, Feature, P, false, false, false, true, gL, jL, kL, lL, oL);
	//------------------------------------------------------------------------------------------------------------------------------
	// Normalize with approximation, and cleanup close to zero.
	vec2 Dir2 = Dir * Dir;
	float DirR = Dir2.x + Dir2.y;
	bool Zero = DirR < (1.0 / 32768.0);
	DirR = 1.0f / sqrt(DirR);
	DirR = Zero ? 1.0f : DirR;
	Dir.x = Zero ? 1.0f : Dir.x;
	Dir *= vec2(DirR);
	// Transform from {0 to 2} to {0 to 1} range, and shape with square.
	Feature = Feature * 0.5f;
	Feature *= Feature;
	// Stretch kernel {1.0 vert|horz, to sqrt(2.0) on diagonal}.
	float Stretch = (1.0f / (max(abs(Dir.x), abs(Dir.y))));
	// Anisotropic length after rotation,
	//  x := 1.0 lerp to 'Stretch' on edges
	//  y := 1.0 lerp to 2x on edges
	vec2 Len = vec2(1.0f + (Stretch - 1.0f) * Feature, 1.0f + -0.5f * Feature);
	// Based on the amount of 'edge',
	// the window shifts from +/-{sqrt(2.0) to slightly beyond 2.0}.
	float Lob = 0.5f + ((1.0f / 4.0f - 0.04f) - 0.5f) * Feature;
	// Set distance^2 clipping point to the end of the adjustable window.
	float Clp = 1.0f / Lob;
	//------------------------------------------------------------------------------------------------------------------------------
	  // Accumulation mixed with min/max of 4 nearest.
	  //    b c
	  //  e f g h
	  //  i j k l
	  //    n o
	vec3 Min4 = min(min3F3(vec3(ijfeR.z, ijfeG.z, ijfeB.z), vec3(klhgR.w, klhgG.w, klhgB.w), vec3(ijfeR.y, ijfeG.y, ijfeB.y)),
		vec3(klhgR.x, klhgG.x, klhgB.x));
	vec3 Max4 = max(max3F3(vec3(ijfeR.z, ijfeG.z, ijfeB.z), vec3(klhgR.w, klhgG.w, klhgB.w), vec3(ijfeR.y, ijfeG.y, ijfeB.y)),
		vec3(klhgR.x, klhgG.x, klhgB.x));
	// Accumulation.
	vec3 aC = vec3(0.0);
	float aW = (0.0);
	fsrEasuTapF(aC, aW, vec2(0.0, -1.0) - P, Dir, Len, Lob, Clp, vec3(bczzR.x, bczzG.x, bczzB.x)); // b
	fsrEasuTapF(aC, aW, vec2(1.0, -1.0) - P, Dir, Len, Lob, Clp, vec3(bczzR.y, bczzG.y, bczzB.y)); // c
	fsrEasuTapF(aC, aW, vec2(-1.0, 1.0) - P, Dir, Len, Lob, Clp, vec3(ijfeR.x, ijfeG.x, ijfeB.x)); // i
	fsrEasuTapF(aC, aW, vec2(0.0, 1.0) - P, Dir, Len, Lob, Clp, vec3(ijfeR.y, ijfeG.y, ijfeB.y)); // j
	fsrEasuTapF(aC, aW, vec2(0.0, 0.0) - P, Dir, Len, Lob, Clp, vec3(ijfeR.z, ijfeG.z, ijfeB.z)); // f
	fsrEasuTapF(aC, aW, vec2(-1.0, 0.0) - P, Dir, Len, Lob, Clp, vec3(ijfeR.w, ijfeG.w, ijfeB.w)); // e
	fsrEasuTapF(aC, aW, vec2(1.0, 1.0) - P, Dir, Len, Lob, Clp, vec3(klhgR.x, klhgG.x, klhgB.x)); // k
	fsrEasuTapF(aC, aW, vec2(2.0, 1.0) - P, Dir, Len, Lob, Clp, vec3(klhgR.y, klhgG.y, klhgB.y)); // l
	fsrEasuTapF(aC, aW, vec2(2.0, 0.0) - P, Dir, Len, Lob, Clp, vec3(klhgR.z, klhgG.z, klhgB.z)); // h
	fsrEasuTapF(aC, aW, vec2(1.0, 0.0) - P, Dir, Len, Lob, Clp, vec3(klhgR.w, klhgG.w, klhgB.w)); // g
	fsrEasuTapF(aC, aW, vec2(1.0, 2.0) - P, Dir, Len, Lob, Clp, vec3(zzonR.z, zzonG.z, zzonB.z)); // o
	fsrEasuTapF(aC, aW, vec2(0.0, 2.0) - P, Dir, Len, Lob, Clp, vec3(zzonR.w, zzonG.w, zzonB.w)); // n
  //------------------------------------------------------------------------------------------------------------------------------
	// Normalize and dering.
	return min(Max4, max(Min4, aC / aW));
}

void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	imageStore(u_OutputEASUTexture, FragPos, vec4(fsrEasuF(FragPos), 1));
}