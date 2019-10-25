#pragma once

#include "VtData.h"

enum TriState {
	/*
	 * Several different bits of the PuTTY configuration seem to be
	 * three-way settings whose values are `always yes', `always
	 * no', and `decide by some more complex automated means'. This
	 * is true of line discipline options (local echo and line
	 * editing), proxy DNS, proxy terminal logging, Close On Exit, and
	 * SSH server bug workarounds. Accordingly I supply a single enum
	 * here to deal with them all.
	 */
	FORCE_ON = 0, 
	FORCE_OFF, 
	AUTO
};

struct Ldisc_tag 
{
	Terminal *term;
	Backend *backend;
	Seat *seat;

	/*
	 * Values cached out of conf.
	 */
	bool telnet_keyboard, telnet_newline;
	int protocol, localecho, localedit;

	char *buf;
	size_t buflen, bufsiz;
	bool quotenext;
};

class VtLdisc : public VtData
{
public:
	VtLdisc();
	~VtLdisc();

public:
	void Send(const char* buf, int len, bool interactive);

	void UpdateEchoEdit();

protected:
	bool IsEdit();
	bool IsEcho();

protected:
	Ldisc_tag m_tag;
};