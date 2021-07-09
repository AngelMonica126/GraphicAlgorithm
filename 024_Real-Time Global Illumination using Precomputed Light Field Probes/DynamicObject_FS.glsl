#version 430 core

in  vec3 v2f_FragPosInWorldSpace;
in  vec3 v2f_Normal;

layout(location = 0) out vec4 Color_;

uniform vec3 u_DiffuseColor;
uniform sampler2D u_PositionTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_AlbedoTexture;
uniform sampler2DArray u_OutputOctRadianceImage;
uniform sampler2DArray u_OutputOctNormalImage;
uniform sampler2DArray u_OutputOctChebyshevsImage;
uniform sampler2DArray u_OutputOctIrradianceImage;
uniform float u_Exposure = 2.0f;
uniform mat4 u_InverseViewMat;
uniform float u_CellSize;
uniform vec3 u_MinAABB;
uniform ivec3 u_CellNum;

ivec3 convertPointToGridIndex(vec3 vPos) {
	return ivec3((vPos - u_MinAABB) / u_CellSize);
}


float signNotZero(in float k) {
    return k >= 0.0 ? 1.0 : -1.0;
}

vec2 signNotZero(in vec2 v) {
    return vec2( signNotZero(v.x), signNotZero(v.y) );
}

vec2 octDecode(vec3 vDirection)
{
	float NormalTotal = abs(vDirection.x)+abs(vDirection.y)+abs(vDirection.z);
	vec2 Result = vDirection.xy * (1.0 / NormalTotal);
	if(vDirection.z < 0.0)
		Result =(1.0 - abs(Result.yx)) * signNotZero(Result.xy);
	return Result;
}
void main()
{

	vec3 Albedo = vec3(1);
	vec3 Position = v2f_FragPosInWorldSpace;
	vec3 Normal = normalize(v2f_Normal);
	ivec3 CellGridIndex = convertPointToGridIndex(Position);
	vec3 Intensity  = vec3(0);
	vec3 CellBasePos = vec3(convertPointToGridIndex(Position)) * u_CellSize +  u_MinAABB;
	vec3 alpha = clamp((Position - CellBasePos) / u_CellSize, vec3(0), vec3(1));
	vec2 Texcoord = octDecode(Normal);
	for(int i = 0; i < 8 ;i++)
	{
		ivec3 offset = ivec3(i, i >> 1, i >> 2) & ivec3(1);
		ivec3 CellCoords = convertPointToGridIndex(Position) + offset;
		vec3 trilinear = mix (1 - alpha, alpha, offset);
		float weight = trilinear.x * trilinear.y * trilinear.z;		int Index = CellCoords.x * u_CellNum.y * u_CellNum.z + CellCoords.y * u_CellNum.z + CellCoords.z;
		Intensity += weight  * texture(u_OutputOctIrradianceImage,vec3(Texcoord,Index)).xyz;
	}
	Intensity *= Albedo;
	vec3 Mapped = vec3(1.0) - exp(-Intensity * u_Exposure);
	Mapped = pow(Mapped, vec3(1.0f / 2.2f));
	Color_ = vec4(Mapped, 1.0f);

}