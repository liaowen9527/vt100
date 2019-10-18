#include "VtCtrlScrolling.h"
#include "../VtContext.h"

VtCtrlScrolling::VtCtrlScrolling()
{
	
}

VtCtrlScrolling::~VtCtrlScrolling()
{

}

void VtCtrlScrolling::Init()
{
	VtFmtManager& fmtManager = m_context->fmtManager;
	fmtManager.AddFmt({ VT_STR_ESC, "[r" }, std::bind(&VtCtrlScrolling::Enable, this));
	fmtManager.AddAttrFmt({ VT_STR_ESC, "[", VT_STR_NUM, "r" }, std::bind(&VtCtrlScrolling::VScroll, this));
	fmtManager.AddFmt({ VT_STR_ESC, "D" }, std::bind(&VtCtrlScrolling::Down, this));
	fmtManager.AddFmt({ VT_STR_ESC, "M" }, std::bind(&VtCtrlScrolling::Up, this));
}

void VtCtrlScrolling::Enable()
{

}

void VtCtrlScrolling::VScroll()
{

}

void VtCtrlScrolling::Down()
{

}

void VtCtrlScrolling::Up()
{

}

void VtCtrlScrolling::Region()
{

}
