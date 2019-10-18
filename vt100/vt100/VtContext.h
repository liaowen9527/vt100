#pragma once

#include "parser/VtBufChain.h"
#include "parser/VtFmtManager.h"

#include "control/VtCtrlCharset.h"
#include "control/VtCtrlClear.h"
#include "control/VtCtrlColor.h"
#include "control/VtCtrlCursor.h"
#include "control/VtCtrlDeviceStatus.h"
#include "control/VtCtrlPrint.h"
#include "control/VtCtrlScrolling.h"
#include "control/VtCtrlSetup.h"
#include "control/VtCtrlTab.h"
#include "control/VtCtrlUnknown.h"

#include "view/VtTerminal.h"

struct VtContext
{
	VtContext()
	{
		fmtManager.SetContext(this);

		charsetCtrl.SetContext(this);
		eraseCtrl.SetContext(this);
		colorsCtrl.SetContext(this);
		cursorCtrl.SetContext(this);
		statusCtrl.SetContext(this);
		printCtrl.SetContext(this);
		scollerCtrl.SetContext(this);
		setupCtrl.SetContext(this);
		tabCtrl.SetContext(this);
		unknownCtrl.SetContext(this);

		charsetCtrl.Init();
		eraseCtrl.Init();
		colorsCtrl.Init();
		cursorCtrl.Init();
		statusCtrl.Init();
		printCtrl.Init();
		scollerCtrl.Init();
		setupCtrl.Init();
		tabCtrl.Init();
		unknownCtrl.Init();
	}

	VtBufChain buffer;
	VtFmtManager fmtManager;

	//control
	VtCtrlCharset charsetCtrl;
	VtCtrlClear eraseCtrl;
	VtCtrlColor colorsCtrl;
	VtCtrlCursor cursorCtrl;
	VtCtrlDeviceStatus statusCtrl;
	VtCtrlPrint printCtrl;
	VtCtrlScrolling scollerCtrl;
	VtCtrlSetup setupCtrl;
	VtCtrlTab tabCtrl;
	VtCtrlUnknown unknownCtrl;

	VtTerm terminal;
};