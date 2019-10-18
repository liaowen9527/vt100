#pragma once
#include "../VtCtxNode.h"
#include <string>

//The h and l codes are used for setting terminal/display mode, and vary depending on the implementation. Line Wrap is one of the few setup codes that tend to be used consistently:
class VtCtrlSetup : public VtCtxNode
{
public:
	VtCtrlSetup();
	~VtCtrlSetup();

	void Init();

public:
	/*
	Reset Device		<ESC>c
	Reset all terminal settings to default.
	*/
	void Reset();

	/* <ESC>[{attr1};...;{attrn}h Set Mode SM*/
	/* <Esc>[20h	Set new line mode	LMN */
	/* <Esc>[?1h	Set cursor key to application	DECCKM */
	/* <Esc>[?2h	Set ANSI (versus VT52)	DECANM */
	/* <Esc>[?3h	Set number of columns to 132	DECCOLM */
	/* <Esc>[?4h	Set smooth scrolling	DECSCLM */
	/* <Esc>[?5h	Set reverse video on screen	DECSCNM */
	/* <Esc>[?6h	Set origin to relative	DECOM */
	/* <Esc>[?7h	Set auto-wrap mode	DECAWM */
	/* <Esc>[?8h	Set auto-repeat mode	DECARM */
	/* <Esc>[?9h	Set interlacing mode	DECINLM */
	void SetMode();

	/* <ESC>[{attr1};...;{attrn}l Reset Mode RM */
	/* <Esc>[20l	Set line feed mode	LMN */
	/* <Esc>[?1l	Set cursor key to cursor	DECCKM */
	/* <Esc>[?2l	Set VT52 (versus ANSI)	DECANM */
	/* <Esc>[?3l	Set number of columns to 80	DECCOLM */
	/* <Esc>[?4l	Set jump scrolling	DECSCLM */
	/* <Esc>[?5l	Set normal video on screen	DECSCNM */
	/* <Esc>[?6l	Set origin to absolute	DECOM */
	/* <Esc>[?7l	Reset auto-wrap mode	DECAWM */
	/* <Esc>[?8l	Reset auto-repeat mode	DECARM */
	/* <Esc>[?9l	Reset interlacing mode	DECINLM */
	void ResetMode();

protected:
	bool FilterAttrH(const std::string& attr, bool bLast);
	bool FilterAttrL(const std::string& attr, bool bLast);
	bool IsAttr(const std::string& attr);

protected:	
	void DoSetMode(int code, bool high);
	void AppCursorKeys(bool high);
	void Vt52Mode(bool high);
	void Columns(bool high);
	void ReverseVideo(bool high);
	void OriginMode(bool high);
	void AutoWrap(bool high);
	void AutoKeyRepeat(bool high);
	void LocalEditMode(bool high);
	void EnableCursor(bool high);
	void AlternateScreen(bool high);
	void XtermMouse1(bool high);
	void XtermMouse2(bool high);
	void XtermExtendMouse(bool high);
	void UrxvtExtendMouse(bool high);
	void SaveCursor(bool high);
	void Mode1049(bool high);
	void XtermBracketedPaste(bool high);

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