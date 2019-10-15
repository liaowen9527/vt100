#pragma once

struct VtContext;

class VtControl
{
public:
	VtControl();
	virtual ~VtControl();

	void SetContext(VtContext* context);

protected:
	VtContext* m_context;
};

