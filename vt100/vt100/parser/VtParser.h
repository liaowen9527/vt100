#pragma once
#include <string>
#include "../data/terminal_tag.h"

#include "../translate/term_translate.h"
#include "VtParserTopLevel.h"
#include "VtParserESC.h"
#include "VtParserCSI.h"
#include "VtParserOSC.h"
#include "VtParserOSC_W.h"
#include "VtParserOSC_P.h"
#include "VtParserOSC_String.h"
#include "VtParserOSC_MaybeST.h"
#include "VtParserVT52Esc.h"
#include "VtParserVT52_Y1.h"
#include "VtParserVT52_FG.h"
#include "VtParserVT52_BG.h"

class VtParser
{
public:
	VtParser();
	~VtParser();

public:
	void Parse(const std::string& str);
	void Parse(const char* str, int len);

protected:
	void InputChar(char ch);
	
	void C1_Control(unsigned long& c);
	void GL_Control(unsigned long& c);
	void NormalControl(unsigned long& c);

protected:
	bool IsGL_Control(unsigned long c);
	bool IsC0_Control(unsigned long c);

protected:
	VtContext m_context;
	VtTerm* m_term;
	term_translate m_vtTrans;

protected:
	VtParserC0 m_vtC0;
	VtParserTopLevel m_vtTopLevel;
	VtParserESC m_vtEsc;
	VtParserCSI m_vtCsi;
	VtParserOSC m_vtOsc;
	VtParserOSC_W m_vtOscW;
	VtParserOSC_P m_vtOscP;
	VtParserOSC_String m_vtOscStr;
	VtParserOSC_MaybeST m_vtMaybeST;
	VtParserVT52Esc m_vt52Esc;
	VtParserVT52_Y1 m_vt52Y1;
	VtParserVT52_FG m_vt52FG;
	VtParserVT52_BG m_vt52BG;
};

