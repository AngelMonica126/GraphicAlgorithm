#include "Utils.h"
#include <initializer_list>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <gli.hpp>
#include <iostream>
#include <numeric>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "common.h"
#include "ResourceManager.h"

//************************************************************************************
//Function:
GLint createVAO(const GLvoid* vVertices, GLint vVerticesSize, std::initializer_list<GLint> vAttribsLength, const GLint vIndices[], GLint vIndicesSize, int *voVBO)
{
	GLint VAO = 0, VBO = 0, EBO = 0;
	glGenVertexArrays(1, &(GLuint&)VAO);
	glBindVertexArray(VAO);
	VBO = genBuffer(GL_ARRAY_BUFFER, vVerticesSize, vVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //In function "genBuffer", bindbuffer(0) is called, so here we must rebind so that VAO is effective
	if (vIndices)
	{
		EBO = genBuffer(GL_ELEMENT_ARRAY_BUFFER, vIndicesSize, vIndices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	}
	GLint AttribOffset = 0;
	GLint i = 0;
	GLint Stride = std::accumulate(vAttribsLength.begin(), vAttribsLength.end(), 0);
	for (auto vAttribLength : vAttribsLength)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, vAttribLength, GL_FLOAT, GL_FALSE, Stride * sizeof(GL_FLOAT), (GLvoid*)(AttribOffset * sizeof(GL_FLOAT)));
		AttribOffset += vAttribLength;
		++i;
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (voVBO)
		*voVBO = VBO;
	return VAO;
}

//************************************************************************************
//Function:
GLint createVAO(int vBufferIndex, std::initializer_list<GLint> vAttribsLength, GLenum vDataType)
{
	GLint VAO = 0;
	glGenVertexArrays(1, &(GLuint&)VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vBufferIndex);
	GLint AttribOffset = 0;
	GLint i = 0;
	GLint Stride = std::accumulate(vAttribsLength.begin(), vAttribsLength.end(), 0);
	for (auto vAttribLength : vAttribsLength)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, vAttribLength, vDataType, GL_FALSE, Stride * sizeof(vDataType), (GLvoid*)(AttribOffset * sizeof(vDataType)));
		AttribOffset += vAttribLength;
		++i;
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return VAO;
}

//************************************************************************************
//Function:
GLint createVAO4ScreenQuad()
{
	GLfloat WindowQuadVertices[] = {
		1.0f, 1.0f,  1.0f,1.0f,
		1.0f,-1.0f,  1.0f,0.0f,
		-1.0f, 1.0f,  0.0f,1.0f,
		-1.0f, 1.0f,  0.0f,1.0f,
		1.0f,-1.0f,  1.0f,0.0f,
		-1.0f,-1.0f,  0.0f,0.0f
	};
	return createVAO(WindowQuadVertices, sizeof(WindowQuadVertices), { 2,2 });
}

//************************************************************************************
//Function:
GLint createVAO4Cube()
{
	GLfloat CubeVertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		// right face
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		// top face
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left      
	};
	return createVAO(CubeVertices, sizeof(CubeVertices), { 3,3,2 });
}

//************************************************************************************
//Function:
GLint createVAO4Sphere()
{
	GLuint SphereVAO = 0;
	glGenVertexArrays(1, &SphereVAO);
	GLuint VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	std::vector<glm::vec3> Positions;
	std::vector<glm::vec2> UV;
	std::vector<glm::vec3> Normals;
	std::vector<unsigned int> Indices;
	const unsigned int SEGMENTX = 64;
	const unsigned int SEGMENTY = 64;
	for (unsigned int y = 0; y <= SEGMENTY; ++y)
	{
		for (unsigned int x = 0; x <= SEGMENTX; ++x)
		{
			float XSegment = (float)x / (float)SEGMENTX;
			float YSegment = (float)y / (float)SEGMENTY;
			float PosX = std::cos(XSegment * 2.0f * ElayGraphics::PI) * std::sin(YSegment * ElayGraphics::PI);
			float PosY = std::cos(YSegment * ElayGraphics::PI);
			float PosZ = std::sin(XSegment * 2.0f * ElayGraphics::PI) * std::sin(YSegment * ElayGraphics::PI);
			Positions.push_back(glm::vec3(PosX, PosY, PosZ));
			UV.push_back(glm::vec2(XSegment, YSegment));
			Normals.push_back(glm::vec3(PosX, PosY, PosZ));
		}
	}
	bool OddRow = false;
	for (int y = 0; y < SEGMENTY; ++y)
	{
		if (!OddRow) // even rows: y == 0, y == 2; and so on
		{
			for (int x = 0; x <= SEGMENTX; ++x)
			{
				Indices.push_back(y * (SEGMENTX + 1) + x);
				Indices.push_back((y + 1) * (SEGMENTX + 1) + x);
			}
		}
		else
		{
			for (int x = SEGMENTX; x >= 0; --x)
			{
				Indices.push_back((y + 1) * (SEGMENTX + 1) + x);
				Indices.push_back(y * (SEGMENTX + 1) + x);
			}
		}
		OddRow = !OddRow;
	}
	std::vector<float> Data;
	for (size_t i = 0; i < Positions.size(); ++i)
	{
		Data.push_back(Positions[i].x);
		Data.push_back(Positions[i].y);
		Data.push_back(Positions[i].z);
		if (UV.size() > 0)
		{
			Data.push_back(UV[i].x);
			Data.push_back(UV[i].y);
		}
		if (Normals.size() > 0)
		{
			Data.push_back(Normals[i].x);
			Data.push_back(Normals[i].y);
			Data.push_back(Normals[i].z);
		}
	}
	glBindVertexArray(SphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Data.size() * sizeof(float), &Data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);
	int Stride = static_cast<int>((3 + 2 + 3) * sizeof(float));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Stride, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, Stride, (void*)(5 * sizeof(float)));

	return SphereVAO;
}

//************************************************************************************
//Function:
void drawQuad()
{
	glBindVertexArray(CResourceManager::getOrCreateInstance()->getOrCreateScreenQuadVAO());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

//************************************************************************************
//Function:
void drawCube()
{
	glBindVertexArray(CResourceManager::getOrCreateInstance()->getOrCreateCubeVAO());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

//************************************************************************************
//Function:
void drawSphere()
{
	glBindVertexArray(CResourceManager::getOrCreateInstance()->getOrCretaeSphereVAO());
	glDrawElements(GL_TRIANGLE_STRIP, 8320, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

//************************************************************************************
//Function: 
GLint genBuffer(GLenum vTarget, GLsizeiptr vSize, const GLvoid *vData, GLenum vUsage, GLint vBindingIndex)
{
	GLint BufferID;
	glGenBuffers(1, &(GLuint&)BufferID);
	glBindBuffer(vTarget, BufferID);
	glBufferData(vTarget, vSize, vData, vUsage);
	glBindBuffer(vTarget, 0);
	if (vBindingIndex != -1)
		glBindBufferBase(vTarget, vBindingIndex, BufferID);
	return BufferID;
}

//************************************************************************************
//Function: 
GLvoid   getSSBOBuffer(GLint BufferID, GLsizeiptr vSize, GLvoid **vData)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, BufferID);
	GLvoid* p = (GLvoid*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
	memcpy(*vData, p, vSize);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}
//************************************************************************************
//Function:
GLvoid  updateSSBOBuffer(GLint BufferID, GLsizeiptr vSize, const GLvoid *vData, GLenum vUsage)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, BufferID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, vSize, vData, vUsage);
}
//************************************************************************************
//Function:
void transferData2Buffer(GLenum vTarget,GLint vTargetID,std::vector<GLintptr> vOffsets, std::vector<GLsizeiptr> vSizes,std::vector<const GLvoid*> vDatas)
{
	_ASSERT(vOffsets.size() == vSizes.size() && vOffsets.size() == vDatas.size());
	GLint OffsetSize = static_cast<GLint>(vOffsets.size());
	glBindBuffer(vTarget, vTargetID);
	for (auto i = 0; i < OffsetSize; ++i)
	{
		glBufferSubData(vTarget, vOffsets[i], vSizes[i], vDatas[i]);
	}
	glBindBuffer(vTarget, 0);
}


GLvoid ClearTexture(std::shared_ptr<ElayGraphics::STexture> vioTexture, GLuint TextureType)
{
	std::vector<GLfloat> emptyData(vioTexture->Width * vioTexture->Height * vioTexture->Depth * sizeof(float), 0.0);
	glBindTexture(TextureType, vioTexture->TextureID);
	//glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, MAX_GRID_SIZE, MAX_GRID_SIZE, MAX_GRID_SIZE, 0, GL_RGBA, GL_FLOAT, &emptyData[0]);
	//or
	glTexSubImage3D(TextureType, 0, 0, 0, 0, vioTexture->Width , vioTexture->Height , vioTexture->Depth, vioTexture->ExternalFormat, vioTexture->DataType, &emptyData[0]);
	glBindTexture(TextureType, 0);
}
//************************************************************************************
//Function:
GLvoid genTexture(std::shared_ptr<ElayGraphics::STexture> vioTexture)
{
	GLint TextureID;
	glGenTextures(1, &(GLuint&)TextureID);

	GLuint TextureType = -1;
	switch (vioTexture->TextureType)
	{
	case ElayGraphics::STexture::ETextureType::Texture2D:
		TextureType = GL_TEXTURE_2D;
		glBindTexture(TextureType, TextureID);
		glTexImage2D(TextureType, 0, vioTexture->InternalFormat, vioTexture->Width, vioTexture->Height, 0, vioTexture->ExternalFormat, vioTexture->DataType, vioTexture->pDataSet.size() > 0 ? vioTexture->pDataSet[0] : nullptr);
		break;
	case ElayGraphics::STexture::ETextureType::Texture2DArray:
		TextureType = GL_TEXTURE_2D_ARRAY;
		glBindTexture(TextureType, TextureID);
		glTexImage3D(TextureType, 0, vioTexture->InternalFormat, vioTexture->Width, vioTexture->Height, vioTexture->Depth, 0, vioTexture->ExternalFormat, vioTexture->DataType, nullptr);
		for (int i = 0; i < static_cast<int>(vioTexture->pDataSet.size()); ++i)
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, vioTexture->Width, vioTexture->Height, 1, vioTexture->ExternalFormat, vioTexture->DataType, vioTexture->pDataSet[i]);
		break;
	case ElayGraphics::STexture::ETextureType::Texture3D:
		TextureType = GL_TEXTURE_3D;
		glBindTexture(TextureType, TextureID);
		glTexImage3D(TextureType, 0, vioTexture->InternalFormat, vioTexture->Width, vioTexture->Height, vioTexture->Depth, 0, vioTexture->ExternalFormat, vioTexture->DataType, vioTexture->pDataSet.size() > 0 ? vioTexture->pDataSet[0] : nullptr);
		break;
	case ElayGraphics::STexture::ETextureType::TextureCubeMap:
		TextureType = GL_TEXTURE_CUBE_MAP;
		glBindTexture(TextureType, TextureID);
		for (int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, vioTexture->InternalFormat, vioTexture->Width, vioTexture->Height, 0, vioTexture->ExternalFormat, vioTexture->DataType, int(vioTexture->pDataSet.size()) > i ? vioTexture->pDataSet[i] : nullptr);
		break;
	case ElayGraphics::STexture::ETextureType::DepthCubeMap:
		TextureType = GL_TEXTURE_CUBE_MAP;
		glBindTexture(TextureType, TextureID);
		for (int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, vioTexture->Width, vioTexture->Height, 0, GL_DEPTH_COMPONENT, vioTexture->DataType, int(vioTexture->pDataSet.size()) > i ? vioTexture->pDataSet[i] : nullptr);
		break;
	default:
		break;
	}
	glTexParameterfv(TextureType, GL_TEXTURE_BORDER_COLOR, vioTexture->BorderColor.data());
	glTexParameteri(TextureType, GL_TEXTURE_WRAP_S, vioTexture->Type4WrapS);
	glTexParameteri(TextureType, GL_TEXTURE_WRAP_T, vioTexture->Type4WrapT);
	glTexParameteri(TextureType, GL_TEXTURE_WRAP_R, vioTexture->Type4WrapR);
	if (vioTexture->isMipmap && vioTexture->Type4MinFilter == GL_LINEAR)
		vioTexture->Type4MinFilter = GL_LINEAR_MIPMAP_LINEAR;
	glTexParameteri(TextureType, GL_TEXTURE_MIN_FILTER, vioTexture->Type4MinFilter);
	glTexParameteri(TextureType, GL_TEXTURE_MAG_FILTER, vioTexture->Type4MagFilter);
	//glTexParameteri(TextureType, GL_TEXTURE_MAX_LEVEL, 4);
	if (vioTexture->isMipmap) 
		glGenerateMipmap(TextureType);
	glBindTexture(TextureType, 0);
	if (vioTexture->ImageBindUnit != -1)
		glBindImageTexture(vioTexture->ImageBindUnit, TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, vioTexture->InternalFormat);
	vioTexture->TextureID = TextureID;
}

//************************************************************************************
//Function:
GLvoid genTextureByBuffer(std::shared_ptr<ElayGraphics::STexture> vioTexture, GLint vBuffer)
{
	GLint TextureID;
	glGenTextures(1, &(GLuint&)TextureID);
	glBindTexture(GL_TEXTURE_BUFFER, TextureID);
	glTexBuffer(GL_TEXTURE_BUFFER, vioTexture->InternalFormat, vBuffer);
	glBindTexture(GL_TEXTURE_BUFFER, 0);
	vioTexture->TextureID = TextureID;
}

//************************************************************************************
//Function:
GLvoid genGenerateMipmap(std::shared_ptr<ElayGraphics::STexture> vioTexture)
{
	GLint TextureID = vioTexture->TextureID;
	GLuint TextureType = -1;
	switch (vioTexture->TextureType)
	{
	case ElayGraphics::STexture::ETextureType::Texture2D:
		TextureType = GL_TEXTURE_2D;
		glBindTexture(TextureType, TextureID);
		break;
	case ElayGraphics::STexture::ETextureType::Texture2DArray:
		TextureType = GL_TEXTURE_2D_ARRAY;
		glBindTexture(TextureType, TextureID);
		break;
	case ElayGraphics::STexture::ETextureType::Texture3D:
		TextureType = GL_TEXTURE_3D;
		glBindTexture(TextureType, TextureID);
		break;
	case ElayGraphics::STexture::ETextureType::TextureCubeMap:
		TextureType = GL_TEXTURE_CUBE_MAP;
		glBindTexture(TextureType, TextureID);
		break;
	default:
		break;
	}
	glGenerateMipmap(TextureType);
	glBindTexture(TextureType, 0);
}

//************************************************************************************
//Function:
void configureDDSTexture(const std::string& vFilePath, gli::texture &voGLITexture, std::shared_ptr<ElayGraphics::STexture>& voTexture2D)
{
	voGLITexture = gli::load(vFilePath);
	GLvoid* pImageData = voGLITexture.data();
	_ASSERT(pImageData);
	gli::gl GL(gli::gl::PROFILE_GL33);
	gli::gl::format const GLIFormat = GL.translate(voGLITexture.format(), voGLITexture.swizzles());
	voTexture2D->InternalFormat = GLIFormat.Internal;
	voTexture2D->ExternalFormat = GLIFormat.External;
	voTexture2D->DataType = GLIFormat.Type;
	voTexture2D->Width = voGLITexture.extent().x;
	voTexture2D->Height = voGLITexture.extent().y;
	voTexture2D->pDataSet.push_back(pImageData);
}

//************************************************************************************
//Function:
void configureCommonTexture(const std::string& vFilePath, std::shared_ptr<ElayGraphics::STexture>& voTexture2D)
{
	stbi_set_flip_vertically_on_load(true);
	GLint NumChannels = 0, ImageWidth = 0, ImageHeight = 0;
	GLvoid *pImageData = stbi_load(vFilePath.c_str(), &ImageWidth, &ImageHeight, &NumChannels, 0);
	_ASSERT(pImageData);
	switch (NumChannels)
	{
	case 1:
		voTexture2D->InternalFormat = voTexture2D->ExternalFormat = GL_RED;
		break;
	case 2:
		voTexture2D->InternalFormat = voTexture2D->ExternalFormat = GL_RG;
		break;
	case 3:
		voTexture2D->InternalFormat = voTexture2D->isSRGBSpace ? GL_SRGB : GL_RGB;
		voTexture2D->ExternalFormat = GL_RGB;
		break;
	case 4:
		voTexture2D->InternalFormat = voTexture2D->isSRGBSpace ? GL_SRGB_ALPHA : GL_RGBA;
		voTexture2D->ExternalFormat = GL_RGBA;
		break;
	default:
		break;
	}
	voTexture2D->DataType = GL_UNSIGNED_BYTE;
	voTexture2D->Width = ImageWidth;
	voTexture2D->Height = ImageHeight;
	voTexture2D->pDataSet.push_back(pImageData);
}

//************************************************************************************
//Function:
void configureHDRTexture(const std::string& vFilePath, std::shared_ptr<ElayGraphics::STexture>& voTexture2D)
{
	stbi_set_flip_vertically_on_load(true);
	GLint NumChannels = 0, ImageWidth = 0, ImageHeight = 0;
	GLvoid *pImageData = stbi_loadf(vFilePath.c_str(), &ImageWidth, &ImageHeight, &NumChannels, 0);
	_ASSERT(pImageData);
	switch (NumChannels)
	{
	case 1:
		voTexture2D->InternalFormat = GL_R16F;
		voTexture2D->ExternalFormat = GL_RED;
		break;
	case 2:
		voTexture2D->InternalFormat = GL_RG16F;
		voTexture2D->ExternalFormat = GL_RG;
		break;
	case 3:
		voTexture2D->InternalFormat = GL_RGB16F;
		voTexture2D->ExternalFormat = GL_RGB;
		break;
	case 4:
		voTexture2D->InternalFormat = GL_RGBA16F;
		voTexture2D->ExternalFormat = GL_RGBA;
		break;
	default:
		break;
	}
	voTexture2D->DataType = GL_FLOAT;
	voTexture2D->Width = ImageWidth;
	voTexture2D->Height = ImageHeight;
	voTexture2D->pDataSet.push_back(pImageData);
}

GLvoid loadCubeTextureFromFile(const std::vector<std::string>& vFilePath, std::shared_ptr<ElayGraphics::STexture> voTexture2D)
{
	if(vFilePath.size()  != 6)
		_ASSERT(vFilePath.size());
	int width, height, nrChannels;
	for (int i = 0; i < vFilePath.size(); i++)
	{
		unsigned char *data = stbi_load((vFilePath[i]).c_str(), &width, &height, &nrChannels, 0);
		voTexture2D->pDataSet.push_back(data);
	}
	voTexture2D->Width = width;
	voTexture2D->Height = height;
	genTexture(voTexture2D);
}
//************************************************************************************
//Function:
GLvoid loadTextureFromFile(const std::string& vFilePath, std::shared_ptr<ElayGraphics::STexture> voTexture2D)
{
	GLint ImageWidth = 0, ImageHeight = 0;
	
	std::vector<std::string> SplitStringSet;
	boost::split(SplitStringSet, vFilePath, boost::is_any_of(".")); 
	bool IsUseStbi = true;
	gli::texture GLITexture;
	if (*(SplitStringSet.end() - 1) == "dds")
	{
		configureDDSTexture(vFilePath, GLITexture, voTexture2D);
		IsUseStbi = false;
	}
	else if (*(SplitStringSet.end() - 1) == "hdr")
		configureHDRTexture(vFilePath, voTexture2D);
	else
		configureCommonTexture(vFilePath, voTexture2D);
	voTexture2D->isMipmap = true;
	genTexture(voTexture2D);

	if (IsUseStbi) stbi_image_free(voTexture2D->pDataSet[0]);
}

//************************************************************************************
//Function:
//GLvoid loadTextureFromFile(const std::string& vFilePath)
//{
//	auto Texture2D = std::make_shared<ElayGraphics::STexture>();
//	loadTextureFromFile(vFilePath, Texture2D);
//}

//************************************************************************************
//Function:
GLint genFBO(const std::initializer_list< std::shared_ptr<ElayGraphics::STexture>>& vioTextureAttachments)
{
	GLint FBO;
	glGenFramebuffers(1, &(GLuint&)FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	GLint i = -1;
	GLboolean HasDepthTextureAttachment = GL_FALSE, HasStencilTextureAttachment = GL_FALSE;
	std::vector<GLenum> Attachments;
	Attachments.reserve(vioTextureAttachments.size());
	for (auto& vioTexture : vioTextureAttachments)
	{
		_ASSERT(vioTexture->TextureID != -1);
		switch (vioTexture->TextureAttachmentType)
		{
		case ElayGraphics::STexture::ETextureAttachmentType::DepthArrayTexture:
			for (int k = 0; k < vioTexture->Depth; ++k)
			{
				glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, vioTexture->TextureID, 0, k);
			}
			HasDepthTextureAttachment = GL_TRUE;
			HasStencilTextureAttachment = GL_TRUE;	//FIXME£ºthere is a problem: if set it as GL_FALSE, the stencil render buffer will be added, then result in incomplete fbo 
			break;
		case ElayGraphics::STexture::ETextureAttachmentType::DepthTexture:
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, vioTexture->TextureID, 0);
			HasDepthTextureAttachment = GL_TRUE;
			HasStencilTextureAttachment = GL_TRUE;	//FIXME£ºthere is a problem: if set it as GL_FALSE, the stencil render buffer will be added, then result in incomplete fbo 
			break;
		case ElayGraphics::STexture::ETextureAttachmentType::CubeDepthTexture:
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, vioTexture->TextureID, 0);
			HasDepthTextureAttachment = GL_TRUE;
			HasStencilTextureAttachment = GL_TRUE;	//FIXME£ºthere is a problem: if set it as GL_FALSE, the stencil render buffer will be added, then result in incomplete fbo 
			break;
		case ElayGraphics::STexture::ETextureAttachmentType::StencilTexture:
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, vioTexture->TextureID, 0);
			HasDepthTextureAttachment = GL_TRUE;
			HasStencilTextureAttachment = GL_TRUE;	//FIXME: there is also same problem as above
			break;
		case ElayGraphics::STexture::ETextureAttachmentType::DepthAndStencilTexture:
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, vioTexture->TextureID, 0);
			HasDepthTextureAttachment = GL_TRUE;
			HasStencilTextureAttachment = GL_TRUE; 
			break;
		default:
			switch (vioTexture->TextureType)
			{
			case ElayGraphics::STexture::ETextureType::Texture2D:
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (++i), GL_TEXTURE_2D, vioTexture->TextureID, 0);
				Attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
				break;
			case ElayGraphics::STexture::ETextureType::Texture2DArray:
				for (int k = 0; k < vioTexture->Depth; ++k)
				{
					glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (++i), vioTexture->TextureID, 0, k);
				}
				Attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
				break;
			case ElayGraphics::STexture::ETextureType::TextureCubeMap:
				glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (++i), vioTexture->TextureID, 0);
				Attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
				HasDepthTextureAttachment = GL_TRUE;	//FIXME: there is a problem: if set it as GL_FALSE, the depth render buffer will be added, then result in incomplete fbo.
				break;
			case ElayGraphics::STexture::ETextureType::Texture3D:
				glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (++i), GL_TEXTURE_3D, vioTexture->TextureID,0, 0);
				Attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
				break;
			default:
				break;
			}
			HasStencilTextureAttachment = GL_TRUE;
			break;
		}
		vioTexture->FrameBufferID = FBO;
		vioTexture->AttachmentID = i;
	}
	if (Attachments.size())
		glDrawBuffers(static_cast<int>(Attachments.size()), &Attachments[0]);

	int RenderBufferWidth = vioTextureAttachments.size() > 0 ? (*vioTextureAttachments.begin())->Width : ElayGraphics::WINDOW_KEYWORD::WINDOW_WIDTH;
	int RenderBufferHeight = vioTextureAttachments.size() > 0 ? (*vioTextureAttachments.begin())->Height : ElayGraphics::WINDOW_KEYWORD::WINDOW_HEIGHT;
	auto genRenderBufferFunc = [=](GLenum vInternelFormat, GLenum vAttachmentType)
	{
		GLint RenderBuffer;
		glGenRenderbuffers(1, &(GLuint&)RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, RenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, vInternelFormat, RenderBufferWidth, RenderBufferHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, vAttachmentType, GL_RENDERBUFFER, RenderBuffer);
	};
	if (!HasDepthTextureAttachment)
	{
		genRenderBufferFunc(GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
	}
	if (!HasStencilTextureAttachment)
	{
		genRenderBufferFunc(GL_STENCIL_INDEX, GL_STENCIL_ATTACHMENT);
	}
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Error::FBO:: Framebuffer Is Not Complete." << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return FBO;
}

//************************************************************************************
//Function:
bool floatEqual(float vFloatNum1, float vFloatNum2)
{
	return std::abs(vFloatNum1 - vFloatNum2) < 0.00001;
}

//************************************************************************************
//Function:
int captureScreen2Img(const std::string& vFileName, int vQuality)
{
	size_t Start = vFileName.find_last_of('.');
	std::string FilePostfix = vFileName.substr(Start + 1, vFileName.size() - Start);
	int WindowWidth = 0, WindowHeight = 0;
	WindowWidth = ElayGraphics::WINDOW_KEYWORD::WINDOW_WIDTH;
	WindowHeight = ElayGraphics::WINDOW_KEYWORD::WINDOW_HEIGHT;
	std::shared_ptr<GLbyte> pScreenData;
	pScreenData.reset(new GLbyte[WindowWidth * WindowHeight * 4]);
	glReadPixels(0, 0, WindowWidth, WindowHeight, GL_RGBA, GL_UNSIGNED_BYTE, pScreenData.get());
	stbi_flip_vertically_on_write(true);

	if (FilePostfix == "tga")
		return stbi_write_tga(vFileName.c_str(), WindowWidth, WindowHeight, 4, pScreenData.get());
	else if (FilePostfix == "png")
		return stbi_write_png(vFileName.c_str(), WindowWidth, WindowHeight, 4, pScreenData.get(), WindowWidth * 4);
	else if (FilePostfix == "bmp")
		return stbi_write_bmp(vFileName.c_str(), WindowWidth, WindowHeight, 4, pScreenData.get());
	else if (FilePostfix == "jpg")
		return stbi_write_jpg(vFileName.c_str(), WindowWidth, WindowHeight, 4, pScreenData.get(), vQuality);
	return -1;
}

//************************************************************************************
//Function:
void hueToRGB(float vHue, glm::vec4& voRGB)
{
	const float s = vHue * 6.0f;
	float r0 = glm::clamp(s - 4.0f, 0.0f, 1.0f);
	float g0 = glm::clamp(s - 0.0f, 0.0f, 1.0f);
	float b0 = glm::clamp(s - 2.0f, 0.0f, 1.0f);

	float r1 = glm::clamp(2.0f - s, 0.0f, 1.0f);
	float g1 = glm::clamp(4.0f - s, 0.0f, 1.0f);
	float b1 = glm::clamp(6.0f - s, 0.0f, 1.0f);

	voRGB = { r0 + r1, g0 * g1, b0 * b1, 1.0f };
}

//************************************************************************************
//Function:
void blitFrameBufferTexture(std::shared_ptr<ElayGraphics::STexture> vioSrcTexture, std::shared_ptr<ElayGraphics::STexture> vioDestTexture, GLenum vFilter/* = GL_NEAREST*/, std::array<bool, 4> vChannels/* = { true, true, true, true }*/)
{
	if (vioSrcTexture->FrameBufferID == -1)	//blited texture need to be binded to some frame buffer
		genFBO({ vioSrcTexture });
	if (vioDestTexture->FrameBufferID == -1)
		genFBO({ vioDestTexture });

	glColorMaski(vioDestTexture->FrameBufferID, vChannels[0], vChannels[1], vChannels[2], vChannels[3]);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, vioSrcTexture->FrameBufferID);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + vioSrcTexture->AttachmentID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, vioDestTexture->FrameBufferID);
	glDrawBuffer(GL_COLOR_ATTACHMENT0 + vioDestTexture->AttachmentID);
	glBlitFramebuffer(0, 0, vioSrcTexture->Width, vioSrcTexture->Height, 0, 0, vioSrcTexture->Width, vioSrcTexture->Height, GL_COLOR_BUFFER_BIT, vFilter);

	glColorMaski(vioDestTexture->FrameBufferID, true, true, true, true);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}