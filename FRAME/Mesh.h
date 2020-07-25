#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <memory>
#include <map>

#define MESH_ID			"u_MeshId"
#define SHININESS		"u_Shininess"
#define REFRACTI		"u_Refracti"
#define AMBIENT_COLOR	"u_AmbientColor"
#define DIFFUSE_COLOR	"u_DiffuseColor"
#define SPECULAR_COLOR	"u_SpecularColor"
#define DIFFUSE_TEX		"u_DiffuseTexture"
#define SPECULAR_TEX	"u_SpecularTexture"
#define NORMAL_TEX		"u_NormalTexture"
#define ROUGHNESS_TEX	"u_RoughnessTexture"
#define METALLIC_TEX	"u_MetallicTexture"

class CShader;
class CAABB;

struct SMeshVertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;

	SMeshVertex() {}
};

struct SMeshTexture
{
	GLint ID = -1;
	std::string TexturePath;
	std::string TextureUniformName;

	SMeshTexture() {}
};

struct SMeshMatProperties
{
	glm::vec3 AmbientColor;
	glm::vec3 DiffuseColor;
	glm::vec3 SpecularColor;
	float Shininess = 0.0f;
	float Refracti = 0.0f;
};

class CMesh
{
public:
	CMesh() = default;
	CMesh(const std::vector<SMeshVertex>& vVertices, const std::vector<GLint>& vIndices, const std::vector<SMeshTexture>& vTexture, const SMeshMatProperties& vMeshMatProperties, int vMeshId);
	~CMesh();
	GLvoid init(const CShader& vioShader);
	GLvoid update(const CShader& vShader) const;

	std::shared_ptr<CAABB> getOrCreateBounding();

private:
	std::vector<SMeshVertex>	m_Vertices;
	std::vector<SMeshTexture>	m_Textures;
	std::vector<GLint>			m_Indices;
	std::shared_ptr<CAABB>		m_pBounding;
	SMeshMatProperties			m_MeshMatProperties;
	GLint m_VAO = -1;
	GLint m_AmbientColorLoc = -1;
	GLint m_DiffuseColorLoc = -1;
	GLint m_SpecularColorLoc = -1;
	GLint m_MeshId = -1;

	GLvoid __init();
};