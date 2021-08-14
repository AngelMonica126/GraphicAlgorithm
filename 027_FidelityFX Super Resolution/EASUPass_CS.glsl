#version 430 core
#pragma optionNV (unroll all)	
#define LOCAL_GROUP_SIZE 32

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;
layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputEASUTexture;

uniform sampler2D u_InputTexture;
uniform int u_DisplayWidth;
uniform int u_DisplayHeight;
uniform vec4 con0;
uniform vec4 con1;
uniform vec4 con2;
uniform vec4 con3;

vec4 FsrEasuRF(vec2 p) { vec4 res = vec4(textureGather(u_InputTexture, p, 0)); return res; }
vec4 FsrEasuGF(vec2 p) { vec4 res = vec4(textureGather(u_InputTexture, p, 1)); return res; }
vec4 FsrEasuBF(vec2 p) { vec4 res = vec4(textureGather(u_InputTexture, p, 2)); return res; }
vec3 Min3F3(vec3 x,vec3 y,vec3 z){return min(x,min(y,z));}
vec3 Max3F3(vec3 x,vec3 y,vec3 z){return max(x,max(y,z));}

 void FsrEasuTapF(
	 inout vec3 aC, // Accumulated color, with negative lobe.
	 inout float aW, // Accumulated weight.
	 vec2 off, // Pixel offset from resolve position to tap.
	 vec2 dir, // Gradient direction.
	 vec2 len, // Length.
	 float lob, // Negative lobe strength.
	 float clp, // Clipping point.
	 vec3 c) { // Tap color.
	  // Rotate offset by direction.
	 vec2 v;
	 v.x = (off.x * (dir.x)) + (off.y * dir.y);
	 v.y = (off.x * (-dir.y)) + (off.y * dir.x);
	 // Anisotropy.
	 v *= len;
	 // Compute distance^2.
	 float d2 = v.x * v.x + v.y * v.y;
	 // Limit to the window as at corner, 2 taps can easily be outside.
	 d2 = min(d2, clp);
	 // Approximation of lancos2 without sin() or rcp(), or sqrt() to get x.
	 //  (25/16 * (2/5 * x^2 - 1)^2 - (25/16 - 1)) * (1/4 * x^2 - 1)^2
	 //  |_______________________________________|   |_______________|
	 //                   base                             window
	 // The general form of the 'base' is,
	 //  (a*(b*x^2-1)^2-(a-1))
	 // Where 'a=1/(2*b-b^2)' and 'b' moves around the negative lobe.
	 float wB = float(2.0 / 5.0) * d2 + float(-1.0);
	 float wA = lob * d2 + float(-1.0);
	 wB *= wB;
	 wA *= wA;
	 wB = float(25.0 / 16.0) * wB + float(-(25.0 / 16.0 - 1.0));
	 float w = wB * wA;
	 // Do weighted average.
	 aC += c * w; aW += w;
 }

// Accumulate direction and length.
 void FsrEasuSetF(
	 inout vec2 dir,
	 inout float len,
	 vec2 pp,
	 bool biS, bool biT, bool biU, bool biV,
	 float lA, float lB, float lC, float lD, float lE) {
	 // Compute bilinear weight, branches factor out as predicates are compiler time immediates.
	 //  s t
	 //  u v
	 float w = float(0.0);
	 if (biS)w = (float(1.0) - pp.x) * (float(1.0) - pp.y);
	 if (biT)w = pp.x *(float(1.0) - pp.y);
	 if (biU)w = (float(1.0) - pp.x)*           pp.y;
	 if (biV)w = pp.x *           pp.y;
	 // Direction is the '+' diff.
	 //    a
	 //  b c d
	 //    e
	 // Then takes magnitude from abs average of both sides of 'c'.
	 // Length converts gradient reversal to 0, smoothly to non-reversal at 1, shaped, then adding horz and vert terms.
	 float dc = lD - lC;
	 float cb = lC - lB;
	 float lenX = max(abs(dc), abs(cb));
	 lenX = 1.0f / lenX;
	 float dirX = lD - lB;
	 dir.x += dirX * w;
	 lenX = clamp(abs(dirX) * lenX,0,1);
	 lenX *= lenX;
	 len += lenX * w;
	 // Repeat for the y axis.
	 float ec = lE - lC;
	 float ca = lC - lA;
	 float lenY = max(abs(ec), abs(ca));
	 lenY = 1.0f / lenY;
	 float dirY = lE - lA;
	 dir.y += dirY * w;
	 lenY = clamp(abs(dirY) * lenY,0,1);
	 lenY *= lenY;
	 len += lenY * w;
 }

void FsrEasuF(out vec3 pix,ivec2 ip) {

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
	 // Veerzeng F对应pp p0对应0...

//------------------------------------------------------------------------------------------------------------------------------
  // Get position of 'f'.
	 vec2 pp = vec2(ip) * con0.xy + con0.zw;
	 vec2 fp = floor(pp);
	 pp -= fp;
	 //------------------------------------------------------------------------------------------------------------------------------
	   // 12-tap kernel.
	   //    b c
	   //  e f g h
	   //  i j k l
	   //    n o
	   // Gather 4 ordering.
	   //  a b
	   //  r g
	   // For packed FP16, need either {rg} or {ab} so using the following setup for gather in all versions,
	   //    a b    <- unused (z)
	   //    r g
	   //  a b a b
	   //  r g r g
	   //    a b
	   //    r g    <- unused (z)
	   // Allowing dead-code removal to remove the 'z's.
	 vec2 p0 = fp * con1.xy + con1.zw;//Veerzeng 这里是将其转换到纹理空间(0,1)
	 // These are from p0 to avoid pulling two constants on pre-Navi hardware.
	 vec2 p1 = p0 + con2.xy;
	 vec2 p2 = p0 + con2.zw;
	 vec2 p3 = p0 + con3.xy;
	 vec4 bczzR = FsrEasuRF(p0);
	 vec4 bczzG = FsrEasuGF(p0);
	 vec4 bczzB = FsrEasuBF(p0);
	 vec4 ijfeR = FsrEasuRF(p1);
	 vec4 ijfeG = FsrEasuGF(p1);
	 vec4 ijfeB = FsrEasuBF(p1);
	 vec4 klhgR = FsrEasuRF(p2);
	 vec4 klhgG = FsrEasuGF(p2);
	 vec4 klhgB = FsrEasuBF(p2);
	 vec4 zzonR = FsrEasuRF(p3);
	 vec4 zzonG = FsrEasuGF(p3);
	 vec4 zzonB = FsrEasuBF(p3);
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
	 vec2 dir = vec2(0.0);
	 float len = 0.0;
	 FsrEasuSetF(dir, len, pp, true, false, false, false, bL, eL, fL, gL, jL);
	 FsrEasuSetF(dir, len, pp, false, true, false, false, cL, fL, gL, hL, kL);
	 FsrEasuSetF(dir, len, pp, false, false, true, false, fL, iL, jL, kL, nL);
	 FsrEasuSetF(dir, len, pp, false, false, false, true, gL, jL, kL, lL, oL);
	 //------------------------------------------------------------------------------------------------------------------------------
	   // Normalize with approximation, and cleanup close to zero.
	 vec2 dir2 = dir * dir;
	 float dirR = dir2.x + dir2.y;
	 bool zro = dirR < (1.0 / 32768.0);
	 dirR = 1.0f / sqrt(dirR);
	 dirR = zro ? 1.0f : dirR;
	 dir.x = zro ? 1.0f : dir.x;
	 dir *= vec2(dirR);
	 // Transform from {0 to 2} to {0 to 1} range, and shape with square.
	 len = len * 0.5f;
	 len *= len;
	 // Stretch kernel {1.0 vert|horz, to sqrt(2.0) on diagonal}.
	 float stretch = (dir.x * dir.x + dir.y * dir.y) * (1.0f / (max(abs(dir.x), abs(dir.y))));
	 // Anisotropic length after rotation,
	 //  x := 1.0 lerp to 'stretch' on edges
	 //  y := 1.0 lerp to 2x on edges
	 vec2 len2 = vec2(1.0f + (stretch - 1.0f) * len, 1.0f + -0.5f * len);
	 // Based on the amount of 'edge',
	 // the window shifts from +/-{sqrt(2.0) to slightly beyond 2.0}.
	 float lob = 0.5f + ((1.0f / 4.0f - 0.04f) - 0.5f) * len;
	 // Set distance^2 clipping point to the end of the adjustable window.
	 float clp = 1.0f / lob;
	 //------------------------------------------------------------------------------------------------------------------------------
	   // Accumulation mixed with min/max of 4 nearest.
	   //    b c
	   //  e f g h
	   //  i j k l
	   //    n o
	 vec3 min4 = min(Min3F3(vec3(ijfeR.z, ijfeG.z, ijfeB.z), vec3(klhgR.w, klhgG.w, klhgB.w), vec3(ijfeR.y, ijfeG.y, ijfeB.y)),
		 vec3(klhgR.x, klhgG.x, klhgB.x));
	 vec3 max4 = max(Max3F3(vec3(ijfeR.z, ijfeG.z, ijfeB.z), vec3(klhgR.w, klhgG.w, klhgB.w), vec3(ijfeR.y, ijfeG.y, ijfeB.y)),
		 vec3(klhgR.x, klhgG.x, klhgB.x));
	 // Accumulation.
	 vec3 aC = vec3(0.0);
	 float aW = (0.0);
	 FsrEasuTapF(aC, aW, vec2(0.0, -1.0) - pp, dir, len2, lob, clp, vec3(bczzR.x, bczzG.x, bczzB.x)); // b
	 FsrEasuTapF(aC, aW, vec2(1.0, -1.0) - pp, dir, len2, lob, clp, vec3(bczzR.y, bczzG.y, bczzB.y)); // c
	 FsrEasuTapF(aC, aW, vec2(-1.0, 1.0) - pp, dir, len2, lob, clp, vec3(ijfeR.x, ijfeG.x, ijfeB.x)); // i
	 FsrEasuTapF(aC, aW, vec2(0.0, 1.0) - pp, dir, len2, lob, clp, vec3(ijfeR.y, ijfeG.y, ijfeB.y)); // j
	 FsrEasuTapF(aC, aW, vec2(0.0, 0.0) - pp, dir, len2, lob, clp, vec3(ijfeR.z, ijfeG.z, ijfeB.z)); // f
	 FsrEasuTapF(aC, aW, vec2(-1.0, 0.0) - pp, dir, len2, lob, clp, vec3(ijfeR.w, ijfeG.w, ijfeB.w)); // e
	 FsrEasuTapF(aC, aW, vec2(1.0, 1.0) - pp, dir, len2, lob, clp, vec3(klhgR.x, klhgG.x, klhgB.x)); // k
	 FsrEasuTapF(aC, aW, vec2(2.0, 1.0) - pp, dir, len2, lob, clp, vec3(klhgR.y, klhgG.y, klhgB.y)); // l
	 FsrEasuTapF(aC, aW, vec2(2.0, 0.0) - pp, dir, len2, lob, clp, vec3(klhgR.z, klhgG.z, klhgB.z)); // h
	 FsrEasuTapF(aC, aW, vec2(1.0, 0.0) - pp, dir, len2, lob, clp, vec3(klhgR.w, klhgG.w, klhgB.w)); // g
	 FsrEasuTapF(aC, aW, vec2(1.0, 2.0) - pp, dir, len2, lob, clp, vec3(zzonR.z, zzonG.z, zzonB.z)); // o
	 FsrEasuTapF(aC, aW, vec2(0.0, 2.0) - pp, dir, len2, lob, clp, vec3(zzonR.w, zzonG.w, zzonB.w)); // n
   //------------------------------------------------------------------------------------------------------------------------------
	 // Normalize and dering.
	 pix = min(max4, max(min4, aC / aW));
 }



void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	vec3 color = vec3(0);
	FsrEasuF(color,FragPos);
	imageStore(u_OutputEASUTexture,FragPos,vec4(color,1));
}