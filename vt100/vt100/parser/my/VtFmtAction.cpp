#include "VtFmtAction.h"
#include "../VtContext.h"

VtFmtAction::VtFmtAction()
{

}

VtFmtAction::~VtFmtAction()
{

}

void VtFmtAction::SetContext(VtContext* context)
{
	m_context = context;
}

void VtFmtAction::SetVtFormat(VtFormatPtr fmt, std::function<void()> action)
{
	m_fmt = fmt;
	m_action = action;
}

bool VtFmtAction::IsMatch()
{
	return m_fmt->Match(m_context);
}

void VtFmtAction::Active()
{
	m_fmt->GetAttrs(m_context);
	m_action();
}


