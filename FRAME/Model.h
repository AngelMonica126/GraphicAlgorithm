#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <map>
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include <memory>
#include "Mesh.h"
#include "Common.h"

class CShader;
class CAABB;

class CModel
{
public:
	CModel() = default;
	CModel(const std::string &vModelPath);
	~CModel() = default;
	void init(CShader &vioShader);
	void update(const CShader &vShader) const;

	std::shared_ptr<CAABB> getOrCreateBounding();
	std::vector<glm::vec3> getTriangle();
private:
	int						   m_MeshCount = -1;
	std::vector<SMeshTexture>  m_LoadedTextures;
	std::vector<CMesh>         m_Meshes;
	std::string                m_Directory;
	const aiScene             *m_pScene;
	std::shared_ptr<CAABB>     m_pBounding;	//这个地方的设计不好，应该由工厂模式来创建对应的包围盒

	GLvoid __loadModel(const std::string& vPath);
	GLvoid __traverseNodes();
	CMesh  __processMesh(const aiMesh *vAiMesh);
	GLvoid __processVertex(const aiMesh *vAiMesh, std::vector<SMeshVertex> &voVertices);
	GLvoid __processIndices(const aiMesh *vAiMesh, std::vector<GLint> &voIndices);
	GLvoid __processTextures(const aiMesh *vAiMesh, std::vector<SMeshTexture> &voTextures);
	GLvoid __processMatProperties(const aiMesh *vAiMesh, SMeshMatProperties& voMeshMatProperties);
	GLvoid __loadTextureFromMaterial(aiTextureType vTextureType, const aiMaterial *vMat, const std::string& vTextureNamePrefix, std::vector<SMeshTexture>& voTextures, std::shared_ptr<ElayGraphics::STexture> vTexture2D = std::make_shared<ElayGraphics::STexture>());
};