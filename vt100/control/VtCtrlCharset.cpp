#include "VtCtrlCharset.h"
#include "parser/VtContext.h"

VtCtrlCharset::VtCtrlCharset()
{
	
}

VtCtrlCharset::~VtCtrlCharset()
{

}

void VtCtrlCharset::Init()
{
	VtFmtManager& fmtManager = m_context->fmtManager;
	fmtManager.AddFmt({ VT_STR_ESC, "(A" }, std::bind(&VtCtrlCharset::Set_ukg0, this));
	fmtManager.AddFmt({ VT_STR_ESC, ")A" }, std::bind(&VtCtrlCharset::Set_ukg1, this));
	fmtManager.AddFmt({ VT_STR_ESC, "(B" }, std::bind(&VtCtrlCharset::Set_usg0, this));
	fmtManager.AddFmt({ VT_STR_ESC, ")B" }, std::bind(&VtCtrlCharset::Set_usg1, this));
	fmtManager.AddFmt({ VT_STR_ESC, "(0" }, std::bind(&VtCtrlCharset::Set_specg0, this));
	fmtManager.AddFmt({ VT_STR_ESC, ")0" }, std::bind(&VtCtrlCharset::Set_specg1, this));
	fmtManager.AddFmt({ VT_STR_ESC, "(1" }, std::bind(&VtCtrlCharset::Set_altg0, this));
	fmtManager.AddFmt({ VT_STR_ESC, ")1" }, std::bind(&VtCtrlCharset::Set_altg1, this));
	fmtManager.AddFmt({ VT_STR_ESC, "(2" }, std::bind(&VtCtrlCharset::Set_altspecg0, this));
	fmtManager.AddFmt({ VT_STR_ESC, ")2" }, std::bind(&VtCtrlCharset::Set_altspecg1, this));
}

void VtCtrlCharset::Set_ukg0()
{

}

void VtCtrlCharset::Set_ukg1()
{

}

void VtCtrlCharset::Set_usg0()
{

}

void VtCtrlCharset::Set_usg1()
{

}

void VtCtrlCharset::Set_specg0()
{

}

void VtCtrlCharset::Set_specg1()
{

}

void VtCtrlCharset::Set_altg0()
{

}

void VtCtrlCharset::Set_altg1()
{

}

void VtCtrlCharset::Set_altspecg0()
{

}

void VtCtrlCharset::Set_altspecg1()
{

}



