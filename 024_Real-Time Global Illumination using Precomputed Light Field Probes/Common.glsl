#version 430 core

vec2 signNotZero(vec2 v) {
	return vec2((v.x >= 0.0) ? +1.0 : -1.0, (v.y >= 0.0) ? +1.0 : -1.0);
}

vec2 float32x3_to_oct(in vec3 v) {
	vec2 p = v.xy * (1.0 / (abs(v.x) + abs(v.y) + abs(v.z)));
	return (v.z <= 0.0) ? ((1.0 - abs(p.yx)) * signNotZero(p)) : p;
}

float signNotZero(float f)
{
	return (f>=0.0) ? 1.0:-1.0;
}

vec2 octEncode(vec3 vDirection)
{
	float NormalTotal = abs(vDirection.x) + abs(vDirection.y) + abs(vDirection.z);
	vec2 Result = vDirection.xy * (1.0 / NormalTotal);
	if(vDirection.z < 0.0)
		Result = (1.0 - abs(Result.yx)) * signNotZero(Result.xy);
	return Result;
}

vec3 octDecode(vec2 vTextCoord)
{
	vec3 Direction = vec3(vTextCoord.x, vTextCoord.y, 1.0 - abs(vTextCoord.x) - abs(vTextCoord.y));
    if (Direction.z < 0.0) {
        Direction.xy = (1.0 - abs(Direction.yx)) * signNotZero(Direction.xy);
    }
    return normalize(Direction);
}