#include "VtCtrlDeviceStatus.h"
#include "../VtContext.h"


VtCtrlDeviceStatus::VtCtrlDeviceStatus()
{
	
}

VtCtrlDeviceStatus::~VtCtrlDeviceStatus()
{

}

void VtCtrlDeviceStatus::Init()
{
	VtFmtManager& fmtManager = m_context->fmtManager;
	fmtManager.AddFmt({ VT_STR_ESC, "[c" }, std::bind(&VtCtrlDeviceStatus::QueryCode, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[0c" }, std::bind(&VtCtrlDeviceStatus::QueryCode, this));
	fmtManager.AddAttrFmt({ VT_STR_ESC, "[?1;", VT_STR_NUM, "c" }, std::bind(&VtCtrlDeviceStatus::ReportCode, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[5n" }, std::bind(&VtCtrlDeviceStatus::QueryStatus, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[0n" }, std::bind(&VtCtrlDeviceStatus::ReportOk, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[3n" }, std::bind(&VtCtrlDeviceStatus::ReportNotOk, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[6n" }, std::bind(&VtCtrlDeviceStatus::QueryCursorPos, this));
	fmtManager.AddAttrFmt({ VT_STR_ESC, "[", VT_STR_NUM, ";", VT_STR_NUM, "R" }, std::bind(&VtCtrlDeviceStatus::ReportCursorPos, this));
}

void VtCtrlDeviceStatus::QueryCode()
{

}

void VtCtrlDeviceStatus::ReportCode()
{

}

void VtCtrlDeviceStatus::QueryStatus()
{

}

void VtCtrlDeviceStatus::ReportOk()
{

}

void VtCtrlDeviceStatus::ReportNotOk()
{

}

void VtCtrlDeviceStatus::QueryCursorPos()
{

}

void VtCtrlDeviceStatus::ReportCursorPos()
{

}
