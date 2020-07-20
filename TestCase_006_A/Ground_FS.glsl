#version 430 core

in  vec3 v2f_FragPosInWorldSpace;
in  vec2 v2f_TexCoords;
out vec4 FragColor_;


uniform sampler2D tilesTexture;
uniform sampler2D indexTexture;
uniform float osizeh;
uniform float osizev;
uniform float isizeh;
uniform float isizev;
//uniform samplerRECT indexTexture
layout (std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};
struct FragmentInput
{ 
	vec4 tex; // TEX0;    
	vec4 col; // COL0; 
};  
struct FragmentOutput
{
	vec4 col; // COL; 
}; 

float TileIndex1D(const float e1, const float e2)  
{   
	float result;    
	if (e1 < e2) 
		result = (2 * e1 + e2 * e2);    
	else if (e1 == e2)      
		if (e1 > 0) 
			result = ((e1 + 1) * (e1 + 1) - 2);      
		else result = 0;   
	else        
		if (e2 > 0) result = (e1 * e1 + 2 * e2 - 1);      
		else result = ((e1 + 1) * (e1 + 1) - 1);    
	return result;  
}   
vec2 TileIndex2D(const vec4 e)  
{    
	vec2 result;    // orthogonally compute the horizontal and vertical locations      
	// the x, y, z, w components of e store   
	// the south, east, north, and west edge coding    
	result.x = TileIndex1D(e.w, e.y);    
	result.y = TileIndex1D(e.x, e.z);    
	return result; 
}

vec2 mod(const vec2 a, const vec2 b) 
{   
	// emulate integer mod operations in a float-only environment    
	return floor((fract(a/b)) * b); 
}    
vec4 fragment(FragmentInput inputs)  
{    
	vec4 outputs;      // osizeh and osizev are the number of output tiles     
	// they are compile-time constants    
	vec2 mappingScale = vec2(osizeh, osizev);    // the scaling is required for RECT textures    
	// we will also need its fractional part in the final tex read  
	vec2 mappingAddress = inputs.tex.xy * mappingScale;   
	// figure out which input tile to use     
	vec4 whichTile = texture2D(indexTexture,mod(mappingAddress, mappingScale));      // isizeh and isizev are the number of input tiles  
	// they are compile-time constants   
	vec2 tileScale = vec2(isizeh, isizev);   
	// this is to ensure the correct input mipmap level is accessed     
	vec2 tileScaledTex = inputs.tex.xy * vec2(osizeh/isizeh,osizev/isizev);      // fetch the final texel color  
	// note we need to specify ddx/ddy explicitly   
	outputs = texture2D(tilesTexture,(whichTile.xy + fract(mappingAddress))/tileScale);
	//	outputs.col = texture2D(tilesTexture,((whichTile.xy + fract(mappingAddress))/tileScale,dFdx(tileScaledTex), dFdy(tileScaledTex)));
	return outputs; 
}

void main()
{
	FragColor_ = vec4(1.0,1.0,0, 1.0);
}