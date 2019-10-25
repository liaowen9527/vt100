#pragma once
#include <algorithm>

struct VtContext;
class VtTerm;
class VtArgument;
class VtCursor;

class VtCtrlNode
{
public:
	VtCtrlNode();
	virtual ~VtCtrlNode();

	void SetContext(VtContext* context);

protected:
	bool HasCompat(int vttype);
	bool CheckCompat(int vttype);
	bool CheckCompat_2(int vttype, int vttype2);

protected:
	VtContext* m_context;
	VtControl* m_ctrl;

	VtTerm* m_term;
	VtArgument* m_args;
	VtCursor* m_cursor;
};

