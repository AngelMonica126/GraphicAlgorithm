#version 430 core

in  vec2 v2f_TexCoords;

layout (location = 0) out vec4 Albedo_;
uniform samplerCube u_Skybox;

float signNotZero(in float k) {
    return k >= 0.0 ? 1.0 : -1.0;
}

vec2 signNotZero(in vec2 v) {
    return vec2( signNotZero(v.x), signNotZero(v.y) );
}

vec3 octEncode(vec2 vTextCoord)
{
	vec3 Direction = vec3(vTextCoord.x, vTextCoord.y, 1.0 - abs(vTextCoord.x) - abs(vTextCoord.y));
    if (Direction.z < 0.0) {
        Direction.xy = (1.0 - abs(Direction.yx)) * signNotZero(Direction.xy);
    }
    return normalize(Direction);
}

void main()
{	
	vec3 Direction = octEncode(v2f_TexCoords * vec2(2.0) - vec2(1.0));
	Albedo_ = texture(u_Skybox, Direction);
}