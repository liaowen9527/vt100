#pragma once
#include <string>
#include <vector>
#include <functional>
#include "VtFormat.h"

struct VtContext;


class VtFmtAction
{
public:
	VtFmtAction();
	~VtFmtAction();

	void SetContext(VtContext* context);
	void SetVtFormat(VtFormatPtr fmt, std::function<void()> action);

public:
	bool IsMatch();
	void Active();

protected:
	VtContext* m_context;
	
	std::function<void()> m_action;
	VtFormatPtr m_fmt;
};
