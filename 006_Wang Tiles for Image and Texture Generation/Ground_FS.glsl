#version 430 core

in  vec2 v2f_TexCoords;
out vec4 FragColor_;


uniform sampler2DArray u_TileArrayTextures;
uniform int u_TileWidth;
uniform int u_TileHeight;
uniform int u_WindowWidth;
uniform int u_WindowHeight;
layout (std430, binding = 0) buffer WhichTile
{
	int u_WhichTile[20][20];
};
void main()
{
	ivec2 Location = ivec2(v2f_TexCoords * vec2(u_WindowWidth, u_WindowHeight));
	ivec2 TileIndex = ivec2(Location.x / u_TileWidth, Location.y / u_TileHeight);
	vec2 TileTexCoord = vec2((Location.x % u_TileWidth) / float(u_TileWidth), (Location.y % u_TileHeight) / float(u_TileHeight));
	int WhichTile = u_WhichTile[TileIndex.y][TileIndex.x];
	FragColor_ = vec4(texture(u_TileArrayTextures, vec3(TileTexCoord, WhichTile)).xyz, 1.0);
}