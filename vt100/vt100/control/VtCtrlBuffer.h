#pragma once
#include "VtCtrlNode.h"

class VtCtrlBuffer : public VtCtrlNode
{
public:
	VtCtrlBuffer();
	~VtCtrlBuffer();

public:
	void DisplayChar();

	void InsertLines();
	void DeleteLines();

	void InsertChars();
	void DeleteChars();

	void ClearTabs();

	void WriteSpaces();
};