#include "VtCtrlColor.h"
#include "../VtContext.h"

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
	fmtManager.AddLoopFmt({ VT_STR_ESC, "[" }, std::bind(&VtCtrlColor::FilterAttr, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VtCtrlColor::SetAttributes, this));
}

void VtCtrlColor::Reset()
{

}

void VtCtrlColor::SetAttributes()
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

bool VtCtrlColor::FilterAttr(const std::string& attr, bool bLast)
{
	if (bLast)
	{
		if (*attr.rbegin() == 'm')
		{
			std::string temp = attr.substr(0, attr.length() - 1);
			return IsAttr(temp);
		}
	}
	return IsAttr(attr);
}

bool VtCtrlColor::IsAttr(const std::string& attr)
{
	if (attr.empty())
	{
		return false;
	}

	for (int i = 0; i < attr.size(); ++i)
	{
		char ch = attr.at(i);
		if (ch < '0' || ch > '9')
		{
			return false;
		}
	}

	return true;
}

