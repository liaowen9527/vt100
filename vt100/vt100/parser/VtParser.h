#pragma once
#include <string>
#include "../data/terminal_tag.h"

#include "VtCharTrans.h"
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
	
	void DoPainting();
	void DoSpecialCase(char& c);

	void GLControl(char& c);

	void NormalControl(char& c);
	void C0Control(char& c);

protected:
	bool IsGLControl(char c);
	bool IsC0Control(char c);

protected:
	VtContext m_context;
	terminal_tag m_term;

	VtCharTrans m_vtTrans;
	VtParserTopLevel m_vtTopLevel;
	VtParserESC m_vtEsc;
	VtParserCSI m_vtCsi;
	VtParserOSC m_vtOsc;
	VtParserOSC_W m_vtOscW;
	VtCharTrans m_vtOscP;
	VtParserOSC_String m_vtOscStr;
	VtParserOSC_MaybeST m_vtMaybeST;
	VtParserVT52Esc m_vt52Esc;
	VtParserVT52_Y1 m_vt52Y1;
	VtParserVT52_FG m_vt52FG;
	VtParserVT52_BG m_vt52BG;
};

