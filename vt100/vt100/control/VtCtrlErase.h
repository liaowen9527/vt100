#pragma once
#include "VtCtrlNode.h"

class VtCtrlErase : public VtCtrlNode
{
public:
	VtCtrlErase();
	~VtCtrlErase();

public:
	void Parts();
	void LineOrParts();

	void EraseLots(bool line_only, bool from_begin, bool to_end);
	
	// Clear the scrollback.
	void ClearSB();

	void SetEraseChar();
	void UseBgcolorErase();
};