#include "VtFmtAction.h"
#include "parser/VtContext.h"

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
	m_action();
}

//void VtFmtAction::IsMatchLoop() const
//{
//	int nLoopIndex = m_strFmt.find(VT_STR_LOOP);
//	const std::string& fmt = m_context->buffer.m_fmt;
//
//	int nIndex = 0;
//	//1, compare before loop
//	if (nLoopIndex > 0)
//	{
//		nIndex = nLoopIndex - 1;
//		if (fmt.length() <= nIndex)
//		{
//			// fmt is not complete
//			int len = fmt.length();
//			return m_strFmt.compare(0, len, fmt, 0, len) == 0;
//		}
//
//		if (!m_strFmt.compare(0, nIndex, fmt, 0, nIndex) != 0)
//		{
//			return false;
//		}
//	}
//	
//	//compare loop
//	for (; nIndex < fmt.length(); ++nIndex)
//	{
//		int offset = nIndex - nLoopIndex;
//		char ch = fmt.at(nIndex);
//		if (!m_funcMatch(ch))
//		{
//			break;
//		}
//	}
//
//	//compare after loop
//	int len = fmt.length() - nIndex;
//	return m_strFmt.compare(nLoopIndex + 1, len, fmt, nIndex, len) == 0;
//}


