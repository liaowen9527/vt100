#pragma once
#include "VtCtrlNode.h"
#include <string>

//The h and l codes are used for setting terminal/display mode, and vary depending on the implementation. Line Wrap is one of the few setup codes that tend to be used consistently:
class VtCtrlSetup : public VtCtrlNode
{
public:
	VtCtrlSetup();
	~VtCtrlSetup();

public:
	void ToggleModeH();
	void ToggleModeL();

protected:	
	void ToggleMode(int code, int query, bool state);
	void ToggleMode_q0(int code, bool state);
	void ToggleMode_q1(int code, bool state);

	void AppCursorKeys(bool state);
	void Vt52Mode(bool state);
	void Columns(bool state);
	void ReverseVideo(bool state);
	void OriginMode(bool state);
	void AutoWrap(bool state);
	void AutoKeyRepeat(bool state);
	void EnableCursor(bool state);
	void AlternateScreen(bool state, bool reset, bool keep_cur_pos);
	void XtermMouse1(bool state);
	void XtermMouse2(bool state);
	void XtermExtendMouse(bool state);
	void UrxvtExtendMouse(bool state);
	void SaveCursor(bool state);
	void Mode1049(bool state);
	void XtermBracketedPaste(bool state);

protected:
	bool m_app_cursor_keys;
	bool m_vt52_mode;
	bool m_vt52_bold;
	bool m_blink_is_real;
	bool m_blinktext;
	bool m_no_remote_resize;
	bool m_reset_132;

	//terminal size
	int m_rows;
	int m_cols;

	int m_alt_t;
	int m_alt_b;
	int m_marg_t;
	int m_marg_b;

	bool m_rvideo;
	unsigned long m_rvbell_startpoint;

	bool m_dec_om;
	bool m_wrap;
	bool m_repeat_off;
	bool m_term_editing;

	bool m_no_alt_screen;
	int m_xterm_mouse;
	int m_xterm_extended_mouse;
	int m_urxvt_extended_mouse;

	bool m_bracketed_paste;
};