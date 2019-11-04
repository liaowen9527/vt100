#pragma once
#include <string>
#include "../define/VtType.h"

struct VtContext;
struct VtTerm;

class VtCtrlCharset;
class VtCtrlErase;
class VtCtrlRender;
class VtCtrlCursor;
class VtCtrlDeviceStatus;
class VtCtrlPrint;
class VtCtrlScrolling;
class VtCtrlSetup;
class VtCtrlTab;
class VtCtrlUnknown;
class VtCtrlBuffer;

class VtParserBase
{
public:
	VtParserBase();
	virtual ~VtParserBase();

public:
	void SetContext(VtContext* context);
	void SetTerm(VtTerm* p);

	unsigned long GetChar();

	void Parse(unsigned long c);

protected:
	virtual void Parse();

protected:
	bool CheckCompat(int vttype);

	int GetCharWidth(char c);

protected:
	unsigned long m_char;
	VtContext* m_context;
	VtTerm* m_term;

protected:
	VtCtrlTerm* m_ctrlTerm;
	VtCtrlCharset* m_charset;
	VtCtrlErase* m_erase;
	VtCtrlRender* m_render;
	VtCtrlCursor* m_cursor;
	VtCtrlDeviceStatus* m_status;
	VtCtrlPrint* m_print;
	VtCtrlScrolling* m_scoller;
	VtCtrlSetup* m_setup;
	VtCtrlTab* m_tab;
	VtCtrlUnknown* m_unknown;
	VtCtrlBuffer* m_buffer;
	VtCtrlLdisc* m_ldisc;
	VtCtrlWindow* m_win;
};

