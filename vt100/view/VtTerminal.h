#pragma once

class VtTerminal
{
public:
	VtTerminal();
	~VtTerminal();

public:
	void SendMessage(const std::string& str);

public:
	void AltScreen();

protected:
	bool m_no_alt_screen;
	VtScreen m_screen;
	VtScreen m_alt_screen;
};

