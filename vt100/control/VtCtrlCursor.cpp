#include "VtCtrlCursor.h"
#include "parser/VtContext.h"

VtCtrlCursor::VtCtrlCursor()
{
 	
}

VtCtrlCursor::~VtCtrlCursor()
{

}

void VtCtrlCursor::Init()
{
	VtFmtManager& fmtManager = m_context->fmtManager;

	fmtManager.AddAttrFmt({ VT_STR_ESC, "[", VT_STR_NUM, "A" }, std::bind(&VtCtrlCursor::Up, this));
	fmtManager.AddAttrFmt({ VT_STR_ESC, "[", VT_STR_NUM, "B" }, std::bind(&VtCtrlCursor::Down, this));
	fmtManager.AddAttrFmt({ VT_STR_ESC, "[", VT_STR_NUM, "C" }, std::bind(&VtCtrlCursor::Forward, this));
	fmtManager.AddAttrFmt({ VT_STR_ESC, "[", VT_STR_NUM, "D" }, std::bind(&VtCtrlCursor::Backward, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[H" }, std::bind(&VtCtrlCursor::Home, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[;H" }, std::bind(&VtCtrlCursor::Home, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[f" }, std::bind(&VtCtrlCursor::Home, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[;f" }, std::bind(&VtCtrlCursor::Home, this));
	fmtManager.AddAttrFmt({ VT_STR_ESC, "[", VT_STR_NUM, ";", VT_STR_NUM, "H" }, std::bind(&VtCtrlCursor::SetPosition, this));
	fmtManager.AddAttrFmt({ VT_STR_ESC, "[", VT_STR_NUM, ";", VT_STR_NUM, "f" }, std::bind(&VtCtrlCursor::SetPosition, this));

	fmtManager.AddFmt({ VT_STR_ESC, "[s" }, std::bind(&VtCtrlCursor::Save, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[u" }, std::bind(&VtCtrlCursor::Restore, this));
	fmtManager.AddFmt({ VT_STR_ESC, "7" }, std::bind(&VtCtrlCursor::Save, this));
	fmtManager.AddFmt({ VT_STR_ESC, "8" }, std::bind(&VtCtrlCursor::Restore, this));

	fmtManager.AddFmt({ VT_STR_ESC, "I" }, std::bind(&VtCtrlCursor::ReverseLineFeed, this));
}

void VtCtrlCursor::Up()
{

}

void VtCtrlCursor::Down()
{

}

void VtCtrlCursor::Forward()
{

}

void VtCtrlCursor::Backward()
{

}

void VtCtrlCursor::Home()
{

}

void VtCtrlCursor::SetPosition()
{

}

void VtCtrlCursor::NextLine()
{

}

void VtCtrlCursor::Save()
{

}

void VtCtrlCursor::Restore()
{

}

void VtCtrlCursor::ReverseLineFeed()
{

}
