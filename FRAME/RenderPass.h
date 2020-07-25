#pragma once
#include <string>
#include <memory>
#include "FRAME_EXPORTS.h"
#include "Common.h"
class CShader;

class FRAME_DLLEXPORTS IRenderPass
{
public:
	IRenderPass();
	IRenderPass(const std::string& vPassName, int vExcutionOrder);
	IRenderPass(const std::string& vPassName, int vExcutionOrder,ElayGraphics::ERenderPassType vtype);
	virtual ~IRenderPass();

	virtual void initV() = 0;
	virtual void updateV() = 0;

	bool operator<(const IRenderPass& vOtherPass) const;

	const std::string& getPassName() const { return m_PassName; }
	int getExecutionOrder() const { return m_ExecutionOrder; }

	void setPassName(const std::string &vPassName) { m_PassName = vPassName; }
	void setExecutionOrder(int vExecutionOrder) { m_ExecutionOrder = vExecutionOrder; }
	ElayGraphics::ERenderPassType getPassType();
	void setPassType(ElayGraphics::ERenderPassType vType);
	int getExecutionOrder();
	void finishExecute();
protected:
	std::shared_ptr<CShader> m_pShader;

private:
	std::string m_PassName;
	ElayGraphics::ERenderPassType m_Type  = ElayGraphics::ERenderPassType::RenderPassType_Normal;
	int m_ExecutionOrder = -1;
};