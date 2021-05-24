#version 430 core
#define PI 3.141592654
in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D u_Albedo;
uniform sampler2D u_Test;
uniform vec2	g_focus = vec2(0);
uniform	float	g_sigma = 50.0;
uniform int	g_level = 0;
float MipGaussianBlendWeight(vec2 tex)
{
	float sigma = g_sigma;
//	if (uint(g_focus.x) != 0xffffffff)
//	{
//		const vec2 r = (2.0 * tex - 1.0) - g_focus;
//		sigma *= dot(r, r);
//	}
	const float sigma2 = sigma * sigma;
	const float c = 2.0 * PI * sigma2;
	const float numerator = (1 << (g_level << 2)) * log(4.0);
	const float denorminator = c * ((1 << (g_level << 1)) + c);
	return clamp(numerator / denorminator,0,1);

}
void main()
{
	vec3 Color = texture(u_Albedo, v2f_TexCoords,1).rgb;
	vec3 Test = texture(u_Test, v2f_TexCoords).rgb;
	float weight = MipGaussianBlendWeight(v2f_TexCoords);
	Color_ = vec4(Test, 1.0f);
//	Color_ = vec4((1 - weight) * Color + weight * Test, 1.0f);
}