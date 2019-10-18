#include "VtCtrlUnknown.h"
#include "../VtContext.h"

VtCtrlUnknown::VtCtrlUnknown()
{
	
}

VtCtrlUnknown::~VtCtrlUnknown()
{

}

void VtCtrlUnknown::Init()
{
	VtFmtManager& fmtManager = m_context->fmtManager;

	//DECDHL -- Double Height Line (DEC Private)
	fmtManager.AddFmt({ VT_STR_ESC, "#3" }, std::bind(&VtCtrlUnknown::DoNothing, this));
	fmtManager.AddFmt({ VT_STR_ESC, "#4" }, std::bind(&VtCtrlUnknown::DoNothing, this));
	fmtManager.AddFmt({ VT_STR_ESC, "#5" }, std::bind(&VtCtrlUnknown::DoNothing, this));
	fmtManager.AddFmt({ VT_STR_ESC, "#6" }, std::bind(&VtCtrlUnknown::DoNothing, this));
	fmtManager.AddFmt({ VT_STR_ESC, "#8" }, std::bind(&VtCtrlUnknown::DoNothing, this));

	//DECLL -- Load LEDS (DEC Private)
	fmtManager.AddFmt({ VT_STR_ESC, "[0q" }, std::bind(&VtCtrlUnknown::DoNothing, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[1q" }, std::bind(&VtCtrlUnknown::DoNothing, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[2q" }, std::bind(&VtCtrlUnknown::DoNothing, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[3q" }, std::bind(&VtCtrlUnknown::DoNothing, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[4q" }, std::bind(&VtCtrlUnknown::DoNothing, this));

	//DECKPAM -- Keypad Application Mode (DEC Private)
	fmtManager.AddFmt({ VT_STR_ESC, "=" }, std::bind(&VtCtrlUnknown::DoNothing, this));
	fmtManager.AddFmt({ VT_STR_ESC, ">" }, std::bind(&VtCtrlUnknown::DoNothing, this));
	fmtManager.AddFmt({ VT_STR_ESC, "#6" }, std::bind(&VtCtrlUnknown::DoNothing, this));
	fmtManager.AddFmt({ VT_STR_ESC, "#8" }, std::bind(&VtCtrlUnknown::DoNothing, this));

	//unknown
	fmtManager.AddFmt({ VT_STR_ESC, "N" }, std::bind(&VtCtrlUnknown::DoNothing, this));
	fmtManager.AddFmt({ VT_STR_ESC, "O" }, std::bind(&VtCtrlUnknown::DoNothing, this));

	//DECTST -- Invoke Confidence Test
	fmtManager.AddAttrFmt({ VT_STR_ESC, "[", VT_STR_NUM, ";", VT_STR_NUM, "y" }, std::bind(&VtCtrlUnknown::DoNothing, this));
}

void VtCtrlUnknown::DoNothing()
{

}

