#include "VtCtrlColor.h"
#include "parser/VtContext.h"

VtCtrlColor::VtCtrlColor()
{
	
}

VtCtrlColor::~VtCtrlColor()
{

}

void VtCtrlColor::Init()
{
	VtFmtManager& fmtManager = m_context->fmtManager;
	fmtManager.AddFmt({ VT_STR_ESC, "[m" }, std::bind(&VtCtrlColor::Reset, this));

	/*m_default.push_back(VtFmtAction({ "^", VT_STR_ESC, "\[([0-9])(;[0-9])?m" }, emRegex, [this]() {
		SetAttribute();
	}));*/
}

void VtCtrlColor::Reset()
{

}

void VtCtrlColor::SetAttribute()
{
	int nCount = m_context->buffer.GetAttrsCount();
	for (int i = 0; i < nCount; ++i)
	{
		SetAttribute(m_context->buffer.GetIntAttr(i));
	}
}

void VtCtrlColor::SetAttribute(char attr)
{

}


