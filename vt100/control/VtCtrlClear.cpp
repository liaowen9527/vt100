#include "VtCtrlClear.h"
#include "parser/VtContext.h"

VtCtrlClear::VtCtrlClear()
{
	
}

VtCtrlClear::~VtCtrlClear()
{

}

void VtCtrlClear::Init()
{
	VtFmtManager& fmtManager = m_context->fmtManager;
	fmtManager.AddFmt({ VT_STR_ESC, "[K" }, std::bind(&VtCtrlClear::Right, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[0K" }, std::bind(&VtCtrlClear::Right, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[1K" }, std::bind(&VtCtrlClear::Left, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[2K" }, std::bind(&VtCtrlClear::Line, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[J" }, std::bind(&VtCtrlClear::Down, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[0J" }, std::bind(&VtCtrlClear::Down, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[1J" }, std::bind(&VtCtrlClear::Up, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[2J" }, std::bind(&VtCtrlClear::Screen, this));
	fmtManager.AddFmt({ VT_STR_ESC, "K" }, std::bind(&VtCtrlClear::Eol, this));
	fmtManager.AddFmt({ VT_STR_ESC, "J" }, std::bind(&VtCtrlClear::Eos, this));
}

void VtCtrlClear::Right()
{

}

void VtCtrlClear::Left()
{

}

void VtCtrlClear::Line()
{

}

void VtCtrlClear::Down()
{

}

void VtCtrlClear::Up()
{

}

void VtCtrlClear::Screen()
{

}

void VtCtrlClear::Eol()
{

}

void VtCtrlClear::Eos()
{

}
