#include "VtCtrlPrint.h"
#include "parser/VtContext.h"

VtCtrlPrint::VtCtrlPrint()
{
	
}

VtCtrlPrint::~VtCtrlPrint()
{

}

void VtCtrlPrint::Init()
{
	VtFmtManager& fmtManager = m_context->fmtManager;
	fmtManager.AddFmt({ VT_STR_ESC, "[i" }, std::bind(&VtCtrlPrint::Screen, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[1i" }, std::bind(&VtCtrlPrint::Line, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[4i" }, std::bind(&VtCtrlPrint::DisableLog, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[5i" }, std::bind(&VtCtrlPrint::PrintLog, this));
}

void VtCtrlPrint::Screen()
{

}

void VtCtrlPrint::Line()
{

}

void VtCtrlPrint::DisableLog()
{

}

void VtCtrlPrint::PrintLog()
{

}
