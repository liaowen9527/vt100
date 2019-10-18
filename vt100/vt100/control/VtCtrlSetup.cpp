#include "VtCtrlSetup.h"
#include "../VtContext.h"

VtCtrlSetup::VtCtrlSetup()
{
	
}

VtCtrlSetup::~VtCtrlSetup()
{

}

void VtCtrlSetup::Init()
{
	VtFmtManager& fmtManager = m_context->fmtManager;
	fmtManager.AddFmt({ VT_STR_ESC, "c" }, std::bind(&VtCtrlSetup::Reset, this));

	fmtManager.AddLoopFmt({ VT_STR_ESC, "[" }, std::bind(&VtCtrlSetup::FilterAttrH, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VtCtrlSetup::SetMode, this));
	fmtManager.AddLoopFmt({ VT_STR_ESC, "[" }, std::bind(&VtCtrlSetup::FilterAttrL, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VtCtrlSetup::ResetMode, this));
}

void VtCtrlSetup::Reset()
{
	
}

void VtCtrlSetup::SetMode()
{
	int nCount = m_context->buffer.GetAttrsCount();
	for (int i = 0; i < nCount; ++i)
	{
		DoSetMode(m_context->buffer.GetIntAttr(i), true);
	}
}

void VtCtrlSetup::ResetMode()
{
	int nCount = m_context->buffer.GetAttrsCount();
	for (int i = 0; i < nCount; ++i)
	{
		DoSetMode(m_context->buffer.GetIntAttr(i), false);
	}
}

bool VtCtrlSetup::FilterAttrH(const std::string& attr, bool bLast)
{
	if (bLast)
	{
		if (*attr.rbegin() == 'h')
		{
			std::string temp = attr.substr(0, attr.length() - 1);
			return IsAttr(temp);
		}
	}
	return IsAttr(attr);
}

bool VtCtrlSetup::FilterAttrL(const std::string& attr, bool bLast)
{
	if (bLast)
	{
		if (*attr.rbegin() == 'l')
		{
			std::string temp = attr.substr(0, attr.length() - 1);
			return IsAttr(temp);
		}
	}
	return IsAttr(attr);
}

bool VtCtrlSetup::IsAttr(const std::string& attr)
{
	if (attr.empty())
	{
		return false;
	}
	
	for (int i = 0; i < attr.size(); ++i)
	{
		char ch = attr.at(i);
		if (i == 0 && ch == '?')
		{
			continue;
		}
		if (ch < '0' || ch > '9')
		{
			return false;
		}
	}

	return true;
}

void VtCtrlSetup::DoSetMode(int code, bool high)
{
	switch (code)
	{
	case 1:		       /* DECCKM: application cursor keys */
		AppCursorKeys(high);
		break;
	case 2:		       /* DECANM: VT52 mode */
		Vt52Mode(high);
		break;
	case 3:		       /* DECCOLM: 80/132 columns */
		Columns(high);
		break;
	case 4:				/* IRM: set insert mode */
		break;
	case 5:		       /* DECSCNM: reverse video */
		ReverseVideo(high);
		break;
	case 6:		       /* DECOM: DEC origin mode */
		OriginMode(high);
		break;
	case 7:		       /* DECAWM: auto wrap */
		AutoWrap(high);
		break;
	case 8:		       /* DECARM: auto key repeat */
		AutoKeyRepeat(high);
		break;
	case 10:		       /* DECEDM: set local edit mode */
		LocalEditMode(high);
		break;
	case 12:			/* SRM: set echo mode */
		break;
	case 20:			/* LNM: Return sends ... */
		break;
	case 25:		       /* DECTCEM: enable/disable cursor */
		EnableCursor(high);
		break;
	case 34:			 /* WYULCURM: Make cursor BIG */
		break;
	case 47:		       /* alternate screen */
		AlternateScreen(high);
		break;
	case 1000:		       /* xterm mouse 1 (normal) */
		XtermMouse1(high);
		break;
	case 1002:		       /* xterm mouse 2 (inc. button drags) */
		XtermMouse2(high);
		break;
	case 1006:		       /* xterm extended mouse */
		XtermExtendMouse(high);
		break;
	case 1015:		       /* urxvt extended mouse */
		UrxvtExtendMouse(high);
		break;
	case 1047:                   /* alternate screen */
		AlternateScreen(high);
		break;
	case 1048:                   /* save/restore cursor */
		SaveCursor(high);
		break;
	case 1049:                   /* cursor & alternate screen */
		Mode1049(high);
		break;
	case 2004:		       /* xterm bracketed paste */
		XtermBracketedPaste(high);
		break;
	}
}

void VtCtrlSetup::AppCursorKeys(bool high)
{
	//m_app_cursor_keys = high;
}

void VtCtrlSetup::Vt52Mode(bool high)
{
// 	if (m_vt52_mode) {
// 		m_blink_is_real = false;
// 		m_vt52_bold = false;
// 	}
// 	else {
// 		m_blink_is_real = m_blinktext;
// 	}
}

void VtCtrlSetup::Columns(bool high)
{
// 	m_cols = high ? 132 : 80;
// 	
// 	m_reset_132 = high;
// 	m_alt_t = m_marg_t = 0;
// 	m_alt_b = m_marg_b = m_rows - 1;
// 
// 	m_context->cursor.SetPosition(0, 0);
// 	m_context->erase.Eos();
	/*move(term, 0, 0, 0);
	erase_lots(term, FALSE, TRUE, TRUE);*/
}

void VtCtrlSetup::ReverseVideo(bool high)
{
	/*
	* Toggle reverse video. If we receive an OFF within the
	* visual bell timeout period after an ON, we trigger an
	* effective visual bell, so that ESC[?5hESC[?5l will
	* always be an actually _visible_ visual bell.
	*/

	//m_rvideo = high;
}

void VtCtrlSetup::OriginMode(bool high)
{
	//m_dec_om = high;
}

void VtCtrlSetup::AutoWrap(bool high)
{
	//m_wrap = high;
}

void VtCtrlSetup::AutoKeyRepeat(bool high)
{
	//m_repeat_off = !high;
}

void VtCtrlSetup::LocalEditMode(bool high)
{
	//m_term_editing = high;
}

void VtCtrlSetup::EnableCursor(bool high)
{
	//m_context->cursor.Enable(high);
}

void VtCtrlSetup::AlternateScreen(bool high)
{
	
}

void VtCtrlSetup::XtermMouse1(bool high)
{
	//m_xterm_mouse = high ? 1 : 0;
}

void VtCtrlSetup::XtermMouse2(bool high)
{
	//m_xterm_mouse = high ? 2 : 0;
}

void VtCtrlSetup::XtermExtendMouse(bool high)
{
	//m_xterm_extended_mouse = high ? 1 : 0;
}

void VtCtrlSetup::UrxvtExtendMouse(bool high)
{
	//m_urxvt_extended_mouse = high ? 1 : 0;
}

void VtCtrlSetup::SaveCursor(bool high)
{
	
}

void VtCtrlSetup::Mode1049(bool high)
{
	
}

void VtCtrlSetup::XtermBracketedPaste(bool high)
{
	//m_bracketed_paste = high;
}

