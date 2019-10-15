#include "VtCtrlTab.h"
#include "parser/VtContext.h"

VtCtrlTab::VtCtrlTab()
{
	
}

VtCtrlTab::~VtCtrlTab()
{

}

void VtCtrlTab::Init()
{
	VtFmtManager& fmtManager = m_context->fmtManager;
	fmtManager.AddFmt({ VT_STR_ESC, "H" }, std::bind(&VtCtrlTab::AddTab, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[g" }, std::bind(&VtCtrlTab::RemoveTab, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[0g" }, std::bind(&VtCtrlTab::RemoveTab, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[3g" }, std::bind(&VtCtrlTab::RemoveAllTab, this));
}

void VtCtrlTab::AddTab()
{

}

void VtCtrlTab::RemoveTab()
{

}

void VtCtrlTab::RemoveAllTab()
{

}
