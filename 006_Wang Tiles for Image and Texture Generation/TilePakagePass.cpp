#include "TilePakagePass.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Shader.h"
#include <GLM/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
const int m_VerticalNum = 20;
const int m_HorizontalNum = 20;

CTilePakagePass::CTilePakagePass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CTilePakagePass::~CTilePakagePass()
{
}

void CTilePakagePass::initV()
{
	m_InputTextureArray = std::make_shared<ElayGraphics::STexture>();
	int Width = 0, Height = 0, Channels = 0;
	GLvoid* pData = nullptr;
	for (int i = 0; i < m_Vertical; ++i)//ио
		for (int j = 0; j < m_Horizontal; ++j)//ср
			for (int k = 0; k < m_Vertical; ++k)//об
				for (int l = 0; l < m_Vertical; ++l)//вС
				{
					stbi_set_flip_vertically_on_load(true);
					std::string t = (m_VerticalChar[i]) + (m_HorizontalChar[j]) + (m_VerticalChar[k]) + (m_HorizontalChar[l]);
					pData = stbi_load(("Textures/tile_" + (m_VerticalChar[i]) + (m_HorizontalChar[j]) + (m_VerticalChar[k]) + (m_HorizontalChar[l]) + ".bmp").c_str(), &Width, &Height, &Channels, 0);
					m_InputTextureArray->pDataSet.push_back(pData);
					glm::ivec4 edge = glm::ivec4(i, j, k, l);
					m_Edges.push_back(edge);
				}
	m_InputTextureArray->TextureType = ElayGraphics::STexture::ETextureType::Texture2DArray;
	m_InputTextureArray->InternalFormat = m_InputTextureArray->ExternalFormat = (Channels == 4) ? GL_RGBA : GL_RGB;
	m_InputTextureArray->Width = Width;
	m_InputTextureArray->Height = Height;
	m_InputTextureArray->Depth = 16;
	genTexture(m_InputTextureArray);
	for (auto& Item : m_InputTextureArray->pDataSet)
		stbi_image_free(Item);
	int tileSize = m_Edges.size();
	int tilesIndex[m_HorizontalNum][m_VerticalNum];
	tilesIndex[0][0] = rand() % tileSize;
	for (int i = 0; i < m_VerticalNum; i++)
		for (int j = 0; j < m_HorizontalNum; j++)
		{
			if (i == 0 && j == 0) 
				continue;
			else if (i == 0)
				tilesIndex[i][j] = getHorizontal(m_Edges, m_Edges[tilesIndex[i][j - 1]].y);
			else if (j == 0)
				tilesIndex[i][j] = getVertical(m_Edges, m_Edges[tilesIndex[i - 1][j]].x);
			else
				tilesIndex[i][j] = getCross(m_Edges, m_Edges[tilesIndex[i - 1][j]].x, m_Edges[tilesIndex[i][j - 1]].y);
		}
	genBuffer(GL_SHADER_STORAGE_BUFFER, m_HorizontalNum * m_VerticalNum * sizeof(int), tilesIndex, GL_STATIC_DRAW, 0);

	ElayGraphics::ResourceManager::registerSharedData("TextureArray", m_InputTextureArray);
	ElayGraphics::ResourceManager::registerSharedData("TileWidth", ElayGraphics::WINDOW_KEYWORD::getWindowWidth() / m_HorizontalNum);
	ElayGraphics::ResourceManager::registerSharedData("TileHeight", ElayGraphics::WINDOW_KEYWORD::getWindowHeight() / m_VerticalNum);

}

void CTilePakagePass::updateV()
{
}

int CTilePakagePass::getHorizontal(std::vector<glm::ivec4> vEdges, int vType)
{
	int tileSize = vEdges.size();
	while (true)
	{
		int index = rand() % tileSize;
		if (vEdges[index].w == vType)
			return index;
	}
	return 0;
}

int CTilePakagePass::getVertical(std::vector<glm::ivec4> vEdges, int vType)
{
	int tileSize = vEdges.size();
	while (true)
	{
		int index = rand() % tileSize;
		if (vEdges[index].z == vType)
			return index;
	}
	return 0;
}

int CTilePakagePass::getCross(std::vector<glm::ivec4> vEdges, int vVerticalType, int vHorizontalType)
{
	int tileSize = vEdges.size();
	while (true)
	{
		int index = rand() % tileSize;
		if (vEdges[index].z == vVerticalType && vEdges[index].w == vHorizontalType)
			return index;
	}
	return 0;
}
