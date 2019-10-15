#pragma once
#include "VtControl.h"

//The following codes are used for reporting terminal/display settings, and vary depending on the implementation:
class VtCtrlDeviceStatus : public VtControl
{
public:
	VtCtrlDeviceStatus();
	~VtCtrlDeviceStatus();

	void Init();

public:
	/* <Esc>[c	Identify what terminal type	DA */
	/* <Esc>[0c	Identify what terminal type (another)	DA */
	void QueryCode();

	/*
	<Esc>[?1;{code}c	Response: terminal type code n	DA
	Option Present					Sequence Sent
	No options						ESC [?1;0c
	Processor option (STP)			ESC [?1;1c
	Advanced video option (AVO)		ESC [?1;2c
	AVO and STP						ESC [?1;3c
	Graphics option (GPO)			ESC [?1;4c
	GPO and STP						ESC [?1;5c
	GPO and AVO						ESC [?1;6c
	GPO, STP and AVO				ESC [?1;7c
	*/
	void ReportCode();

	/*
	Query Device Status	<ESC>[5n
	Requests a Report Device Status response from the device.
	*/
	void QueryStatus();

	/* <Esc>0n	Response: terminal is OK	DSR */
	void ReportOk();

	/* <Esc>3n	Response: terminal is not OK	DSR */
	void ReportNotOk();

	/* <Esc>[6n	Get cursor position	DSR */
	void QueryCursorPos();

	/* <Esc>[{Line};{Column}R	Response: cursor is at v,h	CPR */
	void ReportCursorPos();

protected:
	
};