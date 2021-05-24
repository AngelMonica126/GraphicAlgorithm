#version 430 core

in  vec2 v2f_TexCoords;
out vec4 FragColor_;

uniform sampler2D u_InputImage;
uniform bool u_IsHorizontal;
uniform int u_MipLevel;

const float u_GaussianWeight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);
const int uGaussianRadiusArray[8] = { 1, 3, 13, 35, 54, 54, 54,54 };
const float uWeightArray0[1] = { 1.0 };
const float uWeightArray1[3] = { 0.294118, 0.235294, 0.117647 };
const float uWeightArray2[13] = { 0.122386, 0.116823, 0.101585, 0.0804214, 0.0579034, 0.0378599, 0.0224355, 0.012019, 0.00580229, 0.00251432, 0.000973287, 0.000334567, 0.000101384 };
const float uWeightArray3[35] = { 0.0397701, 0.0395732, 0.0389884, 0.0380328, 0.0367341, 0.0351292, 0.0332625, 0.0311836, 0.0289455, 0.0266023, 0.0242068, 0.021809, 0.019454, 0.0171814, 0.0150237, 0.0130066, 0.0111485, 0.00946092, 0.0079489, 0.00661204, 0.00544521, 0.00443956, 0.0035835, 0.0028636, 0.00226543, 0.00177425, 0.00137563, 0.00105586, 0.000802267, 0.000603444, 0.000449318, 0.000331178, 0.000241632, 0.000174512, 0.000124758 };
const float uWeightArray4[54] = { 0.0249522, 0.0249034, 0.0247578, 0.024517, 0.0241837, 0.0237619, 0.0232563, 0.0226727, 0.0220174, 0.0212976, 0.0205209, 0.0196954, 0.0188292, 0.0179309, 0.0170087, 0.016071, 0.0151256, 0.0141803, 0.0132421, 0.0123176, 0.0114129, 0.0105334, 0.00968359, 0.00886756, 0.00808854, 0.0073491, 0.00665114, 0.00599592, 0.00538409, 0.00481577, 0.00429057, 0.00380769, 0.00336591, 0.00296373, 0.00259938, 0.00227089, 0.00197613, 0.00171289, 0.00147889, 0.00127184, 0.00108949, 0.000929621, 0.000790093, 0.00066887, 0.00056402, 0.000473736, 0.000396339, 0.000330283, 0.000274152, 0.000226665, 0.000186666, 0.000153119, 0.000125106, 0.000101815 };
//
vec4 getColorByRadius(int vTextureIndex)
{
	vec2 TextureOffset = 1.0f / textureSize(u_InputImage, 0);
	vec3 ResultColor;
	if (vTextureIndex == 0)
		ResultColor = textureLod(u_InputImage, v2f_TexCoords, vTextureIndex).rgb * uWeightArray0[0];
	else if (vTextureIndex == 1)
	{
		ResultColor = textureLod(u_InputImage, v2f_TexCoords, vTextureIndex).rgb * uWeightArray1[0];
		if (u_IsHorizontal)
		{
			for (int i = 1; i < uGaussianRadiusArray[vTextureIndex]; ++i)
			{
				ResultColor += textureLod(u_InputImage, v2f_TexCoords + vec2(TextureOffset.x * i, 0.0f), vTextureIndex).rgb * uWeightArray1[i];
				ResultColor += textureLod(u_InputImage, v2f_TexCoords - vec2(TextureOffset.x * i, 0.0f), vTextureIndex).rgb * uWeightArray1[i];
			}
		}
		else
		{
			for (int i = 1; i < uGaussianRadiusArray[vTextureIndex]; ++i)
			{
				ResultColor += textureLod(u_InputImage, v2f_TexCoords + vec2(0.0f, TextureOffset.y * i), vTextureIndex).rgb * uWeightArray1[i];
				ResultColor += textureLod(u_InputImage, v2f_TexCoords - vec2(0.0f, TextureOffset.y * i), vTextureIndex).rgb * uWeightArray1[i];
			}
		}
	}
	else if (vTextureIndex == 2)
	{
		ResultColor = textureLod(u_InputImage, v2f_TexCoords, vTextureIndex).rgb * uWeightArray2[0];
		if (u_IsHorizontal)
		{
			for (int i = 1; i < uGaussianRadiusArray[vTextureIndex]; ++i)
			{
				ResultColor += textureLod(u_InputImage, v2f_TexCoords + vec2(TextureOffset.x * i, 0.0f), vTextureIndex).rgb * uWeightArray2[i];
				ResultColor += textureLod(u_InputImage, v2f_TexCoords - vec2(TextureOffset.x * i, 0.0f), vTextureIndex).rgb * uWeightArray2[i];
			}
		}
		else
		{
			for (int i = 1; i < uGaussianRadiusArray[vTextureIndex]; ++i)
			{
				ResultColor += textureLod(u_InputImage, v2f_TexCoords + vec2(0.0f, TextureOffset.y * i), vTextureIndex).rgb * uWeightArray2[i];
				ResultColor += textureLod(u_InputImage, v2f_TexCoords - vec2(0.0f, TextureOffset.y * i), vTextureIndex).rgb * uWeightArray2[i];
			}
		}
	}
	else if (vTextureIndex == 3)
	{
		ResultColor = textureLod(u_InputImage, v2f_TexCoords, vTextureIndex).rgb * uWeightArray3[0];
		if (u_IsHorizontal)
		{
			for (int i = 1; i < uGaussianRadiusArray[vTextureIndex]; ++i)
			{
				ResultColor += textureLod(u_InputImage, v2f_TexCoords + vec2(TextureOffset.x * i, 0.0f), vTextureIndex).rgb * uWeightArray3[i];
				ResultColor += textureLod(u_InputImage, v2f_TexCoords - vec2(TextureOffset.x * i, 0.0f), vTextureIndex).rgb * uWeightArray3[i];
			}
		}
		else
		{
			for (int i = 1; i < uGaussianRadiusArray[vTextureIndex]; ++i)
			{
				ResultColor += textureLod(u_InputImage, v2f_TexCoords + vec2(0.0f, TextureOffset.y * i), vTextureIndex).rgb * uWeightArray3[i];
				ResultColor += textureLod(u_InputImage, v2f_TexCoords - vec2(0.0f, TextureOffset.y * i), vTextureIndex).rgb * uWeightArray3[i];
			}
		}
	}
	else
	{
		ResultColor = textureLod(u_InputImage, v2f_TexCoords, vTextureIndex).rgb * uWeightArray4[0];
		if (u_IsHorizontal)
		{
			for (int i = 1; i < uGaussianRadiusArray[4]; ++i)
			{
				ResultColor += textureLod(u_InputImage, v2f_TexCoords + vec2(TextureOffset.x * i, 0.0f), vTextureIndex).rgb * uWeightArray4[i];
				ResultColor += textureLod(u_InputImage, v2f_TexCoords - vec2(TextureOffset.x * i, 0.0f), vTextureIndex).rgb * uWeightArray4[i];
			}
		}
		else
		{
			for (int i = 1; i < uGaussianRadiusArray[4]; ++i)
			{
				ResultColor += textureLod(u_InputImage, v2f_TexCoords + vec2(0.0f, TextureOffset.y * i), vTextureIndex).rgb * uWeightArray4[i];
				ResultColor += textureLod(u_InputImage, v2f_TexCoords - vec2(0.0f, TextureOffset.y * i), vTextureIndex).rgb * uWeightArray4[i];
			}
		}
	}
	return vec4(ResultColor, 1.0f);
}

void main()
{
	FragColor_ = getColorByRadius(u_MipLevel);
}