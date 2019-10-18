#pragma once
#include <algorithm>

struct VtContext;

class VtCtxNode
{
public:
	VtCtxNode();
	virtual ~VtCtxNode();

	void SetContext(VtContext* context);

protected:
	VtContext* m_context;
};

