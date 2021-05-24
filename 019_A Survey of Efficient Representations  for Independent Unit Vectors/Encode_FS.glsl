#version 430 core

in  vec2 v2f_TexCoords;

layout (location = 0) out vec4 Albedo_;
uniform samplerCube u_Skybox;

float signNotZero(float f)
{
	return (f>=0.0) ? 1.0:-1.0;
}

vec2 signNotZero(vec2 v)
{
	return vec2(signNotZero(v.x),signNotZero(v.y));
}

/** Assumes that v is a unit vector. The result is an octahedral vector on the [-1, +1] square. */
vec2 octEncode(vec3 vDirection)
{
	float NormalTotal = abs(vDirection.x)+abs(vDirection.y)+abs(vDirection.z);
	vec2 Result = vDirection.xy*(1.0/NormalTotal);
	if(vDirection.z<0.0)
		Result =(1.0-abs(Result.yx))*signNotZero(Result.xy);
	return Result;
}

/** Returns a unit vector. Argument o is an octahedral vector packed via octEncode,on the [-1, +1] square*/
vec3 octDecode(vec2 vTextCoord)
{
	 vec3 Direction = vec3(vTextCoord.x, vTextCoord.y, 1.0 - abs(vTextCoord.x) - abs(vTextCoord.y));
    if (Direction.z < 0.0) {
        Direction.xy = (1.0 - abs(Direction.yx)) * signNotZero(Direction.xy);
    }
    return normalize(Direction);
}
void main()
{	
	vec3 Direction =octDecode(v2f_TexCoords * vec2(2.0)-vec2(1.0));
	Albedo_ = texture(u_Skybox, Direction);
}