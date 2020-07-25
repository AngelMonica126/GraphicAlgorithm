#pragma once
#include <GLM/glm.hpp>
#include <vector>
#include "Bounding.h"
#include "FRAME_EXPORTS.h"

class FRAME_DLLEXPORTS CAABB : public CBounding
{
public:
	CAABB() = default;
	CAABB(const glm::vec3& vMin, const glm::vec3& vMax);
	CAABB(const std::vector<glm::vec3> vPoints);
	virtual ~CAABB() = default;

	virtual float getVolume() const override;

	glm::vec3 getCentre() const;
	glm::vec3 getHalfSize() const;
	glm::vec3 getDiagonal() const;
	const glm::vec3& getMin() const { return m_Min; }
	const glm::vec3& getMax() const { return m_Max; }
	float getSurfaceArea() const;

	void combine(const CAABB& vAnotherAABB);
	void combine(const glm::vec3& vPoint);
	bool overlaps(const CAABB& vAnotherAABB);
	bool inside(const glm::vec3& vPoint);

private:
	glm::vec3 m_Min;
	glm::vec3 m_Max;
};