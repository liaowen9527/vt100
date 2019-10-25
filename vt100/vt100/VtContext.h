#pragma once


#include "control/VtCtrlCharset.h"
#include "control/VtCtrlErase.h"
#include "control/VtCtrlRender.h"
#include "control/VtCtrlCursor.h"
#include "control/VtCtrlDeviceStatus.h"
#include "control/VtCtrlPrint.h"
#include "control/VtCtrlSetup.h"
#include "control/VtCtrlTab.h"
#include "control/VtCtrlUnknown.h"

#include "data/VtTerm.h"

struct VtControl;

struct VtContext
{
	VtContext()
	{
		ctrl->SetContext(this);
	}

	//control
	VtControl ctrl;

	VtTerm term;
};

struct VtControl
{
	void SetContext(VtContext* context)
	{
		charset.SetContext(context);
		erase.SetContext(context);
		render.SetContext(context);
		cursor.SetContext(context);
		status.SetContext(context);
		print.SetContext(context);
		setup.SetContext(context);
		tab.SetContext(context);
		unknown.SetContext(context);
		buffer.SetContext(context);
	}

	VtCtrlCharset charset;
	VtCtrlErase erase;
	VtCtrlRender render;
	VtCtrlCursor cursor;
	VtCtrlDeviceStatus status;
	VtCtrlPrint print;
	VtCtrlSetup setup;
	VtCtrlTab tab;
	VtCtrlUnknown unknown;
	VtCtrlBuffer buffer;
	VtCtrlWindow win;
};
