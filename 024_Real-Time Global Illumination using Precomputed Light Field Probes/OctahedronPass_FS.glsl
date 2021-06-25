#version 430 core

in  vec2 v2f_TexCoords;
layout (location = 0) out vec4 AlbedoAndMetallic_;
layout (location = 1) out vec4 Normal_;
layout (location = 2) out vec2 Chebyshevs_;

uniform samplerCube u_BakeAlbedoTextures;
uniform samplerCube u_BakeNormalTextures;
uniform samplerCube u_BakeChebyshevsTextures;

float signNotZero(float f)
{
	return (f>=0.0) ? 1.0:-1.0;
}

vec2 signNotZero(vec2 v)
{
	return vec2(signNotZero(v.x),signNotZero(v.y));
}
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
	vec3 Dir = (octDecode(v2f_TexCoords) + 1 ) / 2.0;
	AlbedoAndMetallic_= texture(u_BakeAlbedoTextures, Dir);
	Normal_= texture(u_BakeNormalTextures, Dir);
	Chebyshevs_= texture(u_BakeChebyshevsTextures, Dir).xy;
}