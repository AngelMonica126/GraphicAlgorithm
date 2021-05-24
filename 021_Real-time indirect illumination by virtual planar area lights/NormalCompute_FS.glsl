#version 430 core
#define e 2.718281828459
#define PI 3.1415926535898
#define Sqrt2 1.414213562373
in vec2 v2f_TexCoords;
layout (location = 0) out highp vec4 FirstImage_;
layout (location = 1) out highp vec4 SecondImage_;

uniform sampler2D u_PositionImage;
uniform sampler2D u_AlbedoImage;
uniform sampler2D u_NormalImage;
uniform sampler2D u_GPositionImage;
uniform sampler2D u_GAlbedoImage;
uniform sampler2D u_GNormalImage;

uniform sampler2D u_InputFirstImage;
uniform sampler2D u_InputSecondImage;
uniform sampler2D u_PlaneCentreTexture;
uniform int u_MipLevel;
uniform int u_MaxLevel;
uniform int u_RSMResolution;
uniform int u_Block;
uniform vec3 u_LightDir;
uniform float u_LightWidth;

void getColorByRadius(out highp vec3 vFirst,out highp vec3 vSecond)
{	
	vec2 TexCoord = v2f_TexCoords;
	if(u_MipLevel == 0)
	{	
		vec3 normal = texture(u_NormalImage, TexCoord).xyz;
		if((abs(normal.x) < 0.0001f) && (abs(normal.y) < 0.0001f) && (abs(normal.z) < 0.0001f))
		{
			vFirst = vec3(0);
			return;
		}
		vec3 r  =normalize(normal);
		vFirst  = vec3(r.x * r.x, r.x * r.y, r.x * r.z);
		vSecond = vec3(r.y * r.y, r.y * r.z, r.z * r.z);
		return ;
	}
	vec2 TextureOffset = vec2(1.0f / u_RSMResolution * pow(0.5, u_MipLevel));
	
	vec3 f1 = textureLod(u_InputFirstImage, TexCoord, u_MipLevel - 1).rgb;
	vec3 f2 = textureLod(u_InputFirstImage, TexCoord + vec2(TextureOffset.x,0), u_MipLevel - 1).rgb;
	vec3 f3 = textureLod(u_InputFirstImage, TexCoord + vec2(TextureOffset.x,TextureOffset.y), u_MipLevel - 1).rgb;
	vec3 f4 = textureLod(u_InputFirstImage, TexCoord + vec2(0,TextureOffset.y), u_MipLevel - 1).rgb;

	vec3 s1 = textureLod(u_InputSecondImage, TexCoord, u_MipLevel - 1).rgb;
	vec3 s2 = textureLod(u_InputSecondImage, TexCoord + vec2(TextureOffset.x,0), u_MipLevel - 1).rgb;
	vec3 s3 = textureLod(u_InputSecondImage, TexCoord + vec2(TextureOffset.x,TextureOffset.y), u_MipLevel - 1).rgb;
	vec3 s4 = textureLod(u_InputSecondImage, TexCoord + vec2(0,TextureOffset.y), u_MipLevel - 1).rgb;
	vFirst  = f1 + f2 + f3 + f4;
	vSecond = s1 + s2 + s3 + s4;
}
 
void main()
{
	highp vec3 Frist;
	highp vec3 Second;
	Frist = vec3(0); 
	Second = vec3(0); 
	getColorByRadius(Frist,Second);
	FirstImage_ = vec4(Frist, 1);
	SecondImage_ = vec4(Second, 1);
}