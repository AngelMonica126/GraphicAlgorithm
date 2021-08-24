#version 430 core
#pragma optionNV (unroll all)	
#define LOCAL_GROUP_SIZE 32

layout(local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;
layout(rgba32f, binding = 3) uniform writeonly image2D u_OutputEASUTexture;

uniform sampler2D u_InputTexture;
uniform vec4 u_Con0;
uniform vec4 u_Con1;
uniform vec4 u_Con2;
uniform vec4 u_Con3;
uniform float u_InvDisplayWidth;
uniform float u_InvDisplayHeight;

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
	vec3 Color) { // Tap color.
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
	float LumaA, float LumaB, float LumaC, float LumaD, float LumaE) {
	//  s t
	//  u v
	float DC = LumaD - LumaC;
	float CB = LumaC - LumaB;
	float FeatureX = max(abs(DC), abs(CB));
	float DirX = LumaD - LumaB;
	Dir.x += DirX;
	FeatureX = clamp(abs(DirX) / FeatureX, 0.0f, 1.0f);
	FeatureX *= FeatureX;
	Feature += FeatureX;
	// Repeat for the y axis.
	float EC = LumaE - LumaC;
	float CA = LumaC - LumaA;
	float FeatureY = max(abs(EC), abs(CA));
	float DirY = LumaE - LumaA;
	Dir.y += DirY;
	FeatureY = clamp(abs(DirY) / FeatureY, 0.0f, 1.0f);
	FeatureY *= FeatureY;
	Feature += FeatureY;
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
	vec2 FF = F * u_Con1.xy;
	vec2 bT = FF + vec2(0.5, -0.5) * vec2(u_InvDisplayWidth, u_InvDisplayHeight);
	vec2 jT = FF + vec2(0.5, 1.5)  * vec2(u_InvDisplayWidth, u_InvDisplayHeight);
	vec2 fT = FF + vec2(0.5, 0.5)  * vec2(u_InvDisplayWidth, u_InvDisplayHeight);	
	vec2 eT = FF + vec2(-0.5, 0.5) * vec2(u_InvDisplayWidth, u_InvDisplayHeight);	
	vec2 gT = FF + vec2(1.5, 0.5)  * vec2(u_InvDisplayWidth, u_InvDisplayHeight);	
	vec2 kT = FF + vec2(1.5, 1.5)  * vec2(u_InvDisplayWidth, u_InvDisplayHeight);	

	vec3 b =  texture(u_InputTexture,bT).xyz;
	vec3 e =  texture(u_InputTexture,eT).xyz;
	vec3 f =  texture(u_InputTexture,fT).xyz;
	vec3 g =  texture(u_InputTexture,gT).xyz;
	vec3 j =  texture(u_InputTexture,jT).xyz;
	vec3 k =  texture(u_InputTexture,kT).xyz;
	float bL = b.z * 0.5f + (b.x * 0.5f + b.y);
	float eL = e.z * 0.5f + (e.x * 0.5f + e.y);
	float fL = f.z * 0.5f + (f.x * 0.5f + f.y);
	float gL = g.z * 0.5f + (g.x * 0.5f + g.y);
	float jL = j.z * 0.5f + (j.x * 0.5f + j.y);
	vec2 Dir = vec2(0.0);
	float Feature = 0.0;
	fsrEasuSetF(Dir, Feature, P, bL, eL, fL, gL, jL);
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
	float w = (4.0 - Feature) / 12.0;
	
	//公式12
	float Clp = 1.0f / w;
	vec3 aC = vec3(0.0);
	float aW = (0.0);
	fsrEasuTapF(aC, aW, vec2(-1.0, 0.0) - P, Dir, Len, w, Clp, e); //  e
	fsrEasuTapF(aC, aW, vec2(0.0, -1.0) - P, Dir, Len, w, Clp, b); //  b
	fsrEasuTapF(aC, aW, vec2(0.0, 1.0) - P,  Dir, Len, w, Clp, j); //  j
	fsrEasuTapF(aC, aW, vec2(0.0, 0.0) - P,  Dir, Len, w, Clp, f); //  f
	fsrEasuTapF(aC, aW, vec2(1.0, 1.0) - P,  Dir, Len, w, Clp, k); //  k
	fsrEasuTapF(aC, aW, vec2(1.0, 0.0) - P,  Dir, Len, w, Clp, g); //  g
  //------------------------------------------------------------------------------------------------------------------------------
	// Normalize and dering.
	return aC / aW;
}

void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	imageStore(u_OutputEASUTexture, FragPos, vec4(fsrEasuF(FragPos), 1));
}