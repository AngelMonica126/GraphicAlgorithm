#pragma once
#define kT 0.618034f 
#include <vector>
#include <array>
#include <opencv2/core.hpp>
#include "util.h"
#include <Eigen/Eigen>

class Harmonics
{
public:
	Harmonics(std::array<std::string, 6> vImageFilenames);
	void Evaluate();
	std::vector<glm::vec3> getCoefficients()const
	{
		return m_Coefs;
	}
	glm::vec3 Render(const glm::vec3& vPos);
	cv::Mat RenderCubemap(int vWidth, int vHeight);

	Eigen::MatrixXf computeGramMatrixSRBF();
private:
	std::vector<glm::vec3> m_Coefs;
	std::vector<float> Basis(const glm::vec3& vPos);
	std::vector<float> RenderBasis(const glm::vec3& vPos);
	std::array<cv::Mat, 6> m_Images;
	std::vector<glm::vec3> m_Vertices = {
		glm::normalize(glm::vec3(1.0, kT, 0.0)),
		glm::normalize(glm::vec3(-1.0, kT, 0.0)),
		glm::normalize(glm::vec3(1.0, -kT, -0.0)),
		glm::normalize(glm::vec3(-1.0, -kT, 0.0)),
		glm::normalize(glm::vec3(0.0, 1.0, kT)),
		glm::normalize(glm::vec3(-0.0, -1.0, kT)),
		glm::normalize(glm::vec3(0.0, 1.0, -kT)),
		glm::normalize(glm::vec3(0.0, -1.0, -kT)),
		glm::normalize(glm::vec3(kT, 0.0, 1.0)),
		glm::normalize(glm::vec3(-kT, 0.0, 1.0)),
		glm::normalize(glm::vec3(kT, -0.0, -1.0)),
		glm::normalize(glm::vec3(-kT, -0.0, -1.0))
	};
}; 