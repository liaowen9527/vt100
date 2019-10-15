#pragma once
#include <string>
#include <vector>
#include "VtFmtAction.h"

struct VtContext;

class VtFmtManager
{
public:
	VtFmtManager();
	~VtFmtManager();

	void SetContext(VtContext* context);
	std::vector<VtFmtAction> Filter(bool refilter);

public:
	void AddFmt(const std::vector<std::string>& fmt, std::function<void()> func);
	void AddAttrFmt(const std::vector<std::string>& fmt, std::function<void()> func);
	void AddLoopFmt(const std::vector<std::string>& fmt, std::function<bool(const std::string& attr, bool)> match, std::function<void()> func);

protected:
	std::vector<VtFmtAction> DoFilter(std::vector<VtFmtAction>& orgin);

protected:
	VtContext* m_context;
	
	std::vector<VtFmtAction> m_default;
	std::vector<VtFmtAction> m_actions;
};
