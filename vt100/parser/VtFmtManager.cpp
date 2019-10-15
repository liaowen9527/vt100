#include "VtFmtManager.h"
#include "parser/VtContext.h"

VtFmtManager::VtFmtManager()
{

}

VtFmtManager::~VtFmtManager()
{

}

void VtFmtManager::SetContext(VtContext* context)
{
	m_context = context;
}

std::vector<VtFmtAction> VtFmtManager::Filter(bool refilter)
{
	std::vector<VtFmtAction> actions;
	if (refilter)
	{
		actions = DoFilter(m_default);
	}
	else
	{
		actions = DoFilter(m_actions);
	}

	m_actions = actions;
	/*m_actions.clear();
	m_actions.insert(actions.begin(), actions.end()));*/

	return std::move(actions);
}

std::vector<VtFmtAction> VtFmtManager::DoFilter(std::vector<VtFmtAction>& orgin)
{
	std::vector<VtFmtAction> actions;
	for (VtFmtAction& act : orgin)
	{
		if (act.IsMatch())
		{
			actions.push_back(act);
		}
	}

	return std::move(actions);
}

void VtFmtManager::AddFmt(const std::vector<std::string>& fmt, std::function<void()> func)
{
	auto vtFmt = std::make_shared<VtStringFmt>();
	vtFmt->SetFormat(fmt);

	VtFmtAction action;
	action.SetVtFormat(vtFmt, func);
	action.SetContext(m_context);

	m_default.push_back(action);
}

void VtFmtManager::AddAttrFmt(const std::vector<std::string>& fmt, std::function<void()> func)
{
	auto vtFmt = std::make_shared<VtAttrFmt>();
	vtFmt->SetFormat(fmt);

	VtFmtAction action;
	action.SetVtFormat(vtFmt, func);

	m_default.push_back(action);
}

void VtFmtManager::AddLoopFmt(const std::vector<std::string>& fmt, std::function<bool(const std::string& attr, bool)> match, std::function<void()> func)
{
	auto vtFmt = std::make_shared<VtLoopFmt>();
	vtFmt->SetFormat(fmt);
	vtFmt->SetFunc(match);

	VtFmtAction action;
	action.SetVtFormat(vtFmt, func);

	m_default.push_back(action);
}
