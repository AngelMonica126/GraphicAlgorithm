//--------------------------------------------------------------------------------------
// Copyright (c) Elay Pu. All rights reserved.
//--------------------------------------------------------------------------------------

#include "RenderPass.h"
#include "Shader.h"

IRenderPass::IRenderPass()
{
}

IRenderPass::IRenderPass(const std::string& vPassName, int vExcutionOrder):m_PassName(vPassName),m_ExecutionOrder(vExcutionOrder)
{
}
IRenderPass::IRenderPass(const std::string& vPassName, int vExcutionOrder, ElayGraphics::ERenderPassType vtype) : m_PassName(vPassName), m_ExecutionOrder(vExcutionOrder),m_Type(vtype)
{

}
IRenderPass::~IRenderPass()
{
}

//************************************************************************************
//Function:
bool IRenderPass::operator<(const IRenderPass& vOtherPass) const
{
 	return m_ExecutionOrder <= vOtherPass.getExecutionOrder();//保证排序稳定性
}

//************************************************************************************
//Function:
ElayGraphics::ERenderPassType IRenderPass::getPassType()
{
	return m_Type;
}
//************************************************************************************
//Function:
void IRenderPass::setPassType(ElayGraphics::ERenderPassType vType)
{
	m_Type = vType;
}

//************************************************************************************
//Function:
int IRenderPass::getExecutionOrder()
{
	return m_ExecutionOrder;
}

//************************************************************************************
//Function:
void IRenderPass::finishExecute()
{
	 m_ExecutionOrder = -1;
}