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
	float w, // Negative lobe strength.
	float Clp, // Clipping point.
	vec3 Color)
{ // Tap color.
//公式15
	vec2 v;
	v.x = (Off.x * (Dir.x)) + (Off.y * Dir.y);
	v.y = (Off.x * (-Dir.y)) + (Off.y * Dir.x);
	//公式16
	v *= Len;
	float x2 = v.x * v.x + v.y * v.y;
	//根据w裁剪
	x2 = min(x2, Clp);
	//公式6
	//  (25/16 * (2/5 * x^2 - 1)^2 - (25/16 - 1)) * (1/4 * x^2 - 1)^2
	//  |_______________________________________|   |_______________|
	//                   base                             window
	float WindowB = float(2.0 / 5.0) * x2 + float(-1.0);
	float WindowA = w * x2 + float(-1.0);
	WindowB *= WindowB;
	WindowA *= WindowA;
	WindowB = float(25.0 / 16.0) * WindowB + float(-(25.0 / 16.0 - 1.0));
	float Window = (WindowB * WindowA);
	aC += Color * Window; aW += Window;
}

//根据公式7计算Feature，根据公式14计算梯度Dir
void fsrEasuSetF(
	inout vec2 Dir,
	inout float Feature,
	vec2 P,
	bool BoolS, bool BoolT, bool BoolU, bool BoolV,
	float LumaA, float LumaB, float LumaC, float LumaD, float LumaE)
{
	//  s t
	//  u v
	float Weight = 0.0f;
	if (BoolS) Weight = (1.0f - P.x) * (1.0f - P.y);
	if (BoolT) Weight = P.x * (1.0f - P.y);
	if (BoolU) Weight = (1.0f - P.x) * P.y;
	if (BoolV) Weight = P.x * P.y;
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
	vec2 P = vec2(ip) * u_Con0.xy + u_Con0.zw;
	vec2 F = floor(P);
	P -= F;
	vec2 P0 = F * u_Con1.xy + u_Con1.zw;
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
	vec2 Dir = vec2(0.0);
	float Feature = 0.0;
	// 双线性插值
	fsrEasuSetF(Dir, Feature, P, true, false, false, false, bL, eL, fL, gL, jL);
	fsrEasuSetF(Dir, Feature, P, false, true, false, false, cL, fL, gL, hL, kL);
	fsrEasuSetF(Dir, Feature, P, false, false, true, false, fL, iL, jL, kL, nL);
	fsrEasuSetF(Dir, Feature, P, false, false, false, true, gL, jL, kL, lL, oL);
	//------------------------------------------------------------------------------------------------------------------------------
	// 
	{//归一化梯度向量Dir
		vec2 Dir2 = Dir * Dir;
		float DirR = Dir2.x + Dir2.y;
		bool Zero = DirR < (1.0 / 32768.0);
		DirR = 1.0f / sqrt(DirR);
		DirR = Zero ? 1.0f : DirR;
		Dir.x = Zero ? 1.0f : Dir.x;
		Dir *= vec2(DirR);
	}
	{//公式8
		Feature = Feature * 0.5f;
		Feature *= Feature;
	}
	//公式16
	float Stretch = (1.0f / (max(abs(Dir.x), abs(Dir.y))));
	vec2 Len = vec2(1.0f + (Stretch - 1.0f) * Feature, 1.0f + -0.5f * Feature);

	//公式11
	float w = 0.5f - 0.25f * Feature;

	//公式12
	float Clp = 1.0f / w;

	vec3 Min4 = min(min3F3(vec3(ijfeR.z, ijfeG.z, ijfeB.z), vec3(klhgR.w, klhgG.w, klhgB.w), vec3(ijfeR.y, ijfeG.y, ijfeB.y)),
		vec3(klhgR.x, klhgG.x, klhgB.x));
	vec3 Max4 = max(max3F3(vec3(ijfeR.z, ijfeG.z, ijfeB.z), vec3(klhgR.w, klhgG.w, klhgB.w), vec3(ijfeR.y, ijfeG.y, ijfeB.y)),
		vec3(klhgR.x, klhgG.x, klhgB.x));
	// Accumulation.
	vec3 aC = vec3(0.0);
	float aW = (0.0);
	fsrEasuTapF(aC, aW, vec2(0.0, -1.0) - P, Dir, Len, w, Clp, vec3(bczzR.x, bczzG.x, bczzB.x)); // b
	fsrEasuTapF(aC, aW, vec2(1.0, -1.0) - P, Dir, Len, w, Clp, vec3(bczzR.y, bczzG.y, bczzB.y)); // c
	fsrEasuTapF(aC, aW, vec2(-1.0, 1.0) - P, Dir, Len, w, Clp, vec3(ijfeR.x, ijfeG.x, ijfeB.x)); // i
	fsrEasuTapF(aC, aW, vec2(0.0, 1.0) - P, Dir, Len, w, Clp, vec3(ijfeR.y, ijfeG.y, ijfeB.y)); // j
	fsrEasuTapF(aC, aW, vec2(0.0, 0.0) - P, Dir, Len, w, Clp, vec3(ijfeR.z, ijfeG.z, ijfeB.z)); // f
	fsrEasuTapF(aC, aW, vec2(-1.0, 0.0) - P, Dir, Len, w, Clp, vec3(ijfeR.w, ijfeG.w, ijfeB.w)); // e
	fsrEasuTapF(aC, aW, vec2(1.0, 1.0) - P, Dir, Len, w, Clp, vec3(klhgR.x, klhgG.x, klhgB.x)); // k
	fsrEasuTapF(aC, aW, vec2(2.0, 1.0) - P, Dir, Len, w, Clp, vec3(klhgR.y, klhgG.y, klhgB.y)); // l
	fsrEasuTapF(aC, aW, vec2(2.0, 0.0) - P, Dir, Len, w, Clp, vec3(klhgR.z, klhgG.z, klhgB.z)); // h
	fsrEasuTapF(aC, aW, vec2(1.0, 0.0) - P, Dir, Len, w, Clp, vec3(klhgR.w, klhgG.w, klhgB.w)); // g
	fsrEasuTapF(aC, aW, vec2(1.0, 2.0) - P, Dir, Len, w, Clp, vec3(zzonR.z, zzonG.z, zzonB.z)); // o
	fsrEasuTapF(aC, aW, vec2(0.0, 2.0) - P, Dir, Len, w, Clp, vec3(zzonR.w, zzonG.w, zzonB.w)); // n
  //------------------------------------------------------------------------------------------------------------------------------
	// Normalize and dering.
	return min(Max4, max(Min4, aC / aW));
}

void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	imageStore(u_OutputEASUTexture, FragPos, vec4(fsrEasuF(FragPos), 1));
}
