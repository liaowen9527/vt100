#pragma once
#include "VtCtrlNode.h"

class VtCtrlLdisc : public VtCtrlNode
{
public:
	VtCtrlLdisc();
	~VtCtrlLdisc();

public:
	void QueryType();
	void QueryType2();
	void QueryCursorPos();

	void ReportCharacter();

public:
	void Send();
	
};