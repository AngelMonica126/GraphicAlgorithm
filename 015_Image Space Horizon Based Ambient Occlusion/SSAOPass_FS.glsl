#version 430 core

in  vec2 v2f_TexCoords;
layout (location = 0) out float FragColor_;

uniform sampler2D u_PositionTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_NoiseTexture;
uniform float u_Near = 0.1;
uniform float u_Far = 100.0f;
uniform float u_WindowWidth;
uniform float u_WindowHeight;
uniform vec3 u_Samples[64];

int KernelSize = 64;
float Radius = 1.0;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

float LinearizeDepth(float vDepth)
{
    float z = vDepth * 2.0 - 1.0; 
    return (2.0 * u_Near * u_Far) / (u_Far + u_Near - z * (u_Far - u_Near));    
}

void main()
{
	vec2 NoiseScale = vec2(u_WindowWidth/4.0f, u_WindowHeight/4.0f);
    vec3 FragPos = texture(u_PositionTexture, v2f_TexCoords).xyz;
    vec3 Normal = texture(u_NormalTexture, v2f_TexCoords).rgb;
    vec3 RandomVec = texture(u_NoiseTexture, NoiseScale * v2f_TexCoords).xyz;
    vec3 Tangent = normalize(RandomVec - Normal * dot(RandomVec, Normal));
    vec3 Bitangent = cross(Normal, Tangent);
    mat3 TBN = mat3(Tangent, Bitangent, Normal);
    float Occlusion = 0.0;
    for(int i = 0; i < KernelSize; ++i)
    {
        vec3 samples = TBN * u_Samples[i]; 
        samples = FragPos + samples * Radius; 
        vec4 Offset = vec4(samples, 1.0);
        Offset = u_ProjectionMatrix * Offset; 
        Offset.xyz /= Offset.w; 
        Offset.xyz = Offset.xyz * 0.5 + 0.5; 
        
        float SampleDepth = -texture(u_PositionTexture, Offset.xy).w; 
        
        float RangeCheck = smoothstep(0.0, 1.0, Radius / abs(FragPos.z - SampleDepth ));
        Occlusion += (SampleDepth >= samples.z ? 1.0 : 0.0) * RangeCheck;           
    }
    Occlusion = 1.0 - (Occlusion / KernelSize);
	FragColor_  = Occlusion;
}