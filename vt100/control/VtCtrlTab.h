#pragma once
#include "VtControl.h"

class VtCtrlTab : public VtControl
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