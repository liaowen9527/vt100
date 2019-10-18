#pragma once
#include "../VtCtxNode.h"

class VtCtrlTab : public VtCtxNode
{
public:
	VtCtrlTab();
	~VtCtrlTab();

	void Init();

public:
	/* <ESC>H	Set a tab at the current column	HTS */
	void AddTab();

	/* <ESC>[g	Clear a tab at the current column	TBC */
	/* <ESC>[0g	Clear a tab at the current column	TBC */
	void RemoveTab();

	/*<ESC>[3g Clear all tabs	TBC */
	void RemoveAllTab();
};