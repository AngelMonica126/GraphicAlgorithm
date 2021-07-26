#version 430 core
#define PI 3.141592654
in  vec2 v2f_TexCoords;
out vec2 Color_;

uniform sampler2D u_SourceTexture;
uniform sampler2D u_CoarserTexture;
uniform	float	g_sigma = 2;
uniform int	g_level = 0;
float MipGaussianBlendWeight(vec2 tex)
{
	float sigma = g_sigma;
	const float sigma2 = sigma * sigma;
	const float c = 2.0 * PI * sigma2;
	const float numerator = (1 << (g_level << 2)) * log(4.0);
	const float denorminator = c * ((1 << (g_level << 1)) + c);
	return clamp(numerator / denorminator,0,1);
}
void main()
{
	if(g_level == 7)
	{
		Color_ = textureLod(u_CoarserTexture, v2f_TexCoords,7).rg;
		return ;
	}
	vec2 Color = textureLod(u_SourceTexture, v2f_TexCoords,g_level + 1).rg;
	float weight = MipGaussianBlendWeight(v2f_TexCoords);
	vec2 src = textureLod(u_CoarserTexture, v2f_TexCoords,g_level).rg;
	Color_ = vec2((1 - weight) * Color + weight * src);
}