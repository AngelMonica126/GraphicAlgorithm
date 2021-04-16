#version 460  core
in  vec2 v2f_TexCoords;

layout (location = 0) out vec4 Color_;

uniform sampler2D u_OpacityAlbedoTexture;
uniform sampler2D u_TansparentTexture;
uniform sampler2D u_CoefficientOneTex;

void main()
{
   vec3  opaqueColor = texelFetch(u_OpacityAlbedoTexture, ivec2(gl_FragCoord.xy), 0).rgb;
   vec4  translucentColor = texelFetch(u_TansparentTexture, ivec2(gl_FragCoord.xy), 0).rgba;
   float totalOpticalDepth = 0.5 * texelFetch(u_CoefficientOneTex, ivec2(gl_FragCoord.xy), 0).y;
   float totalTransmittance = exp(-totalOpticalDepth);

   vec3 finalColor = translucentColor.rgb / translucentColor.a * (1.0 - totalTransmittance) + opaqueColor * totalTransmittance;
   if(isnan(finalColor.x))
	   finalColor = opaqueColor;
   Color_ = vec4(finalColor,1.0);
}