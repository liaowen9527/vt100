#pragma once
#include "VtCtrlNode.h"
#include <string>

class VtCtrlRender : public VtCtrlNode
{
public:
	VtCtrlRender();
	~VtCtrlRender();

public:
	void Render();

protected:
	void Restore();

	void EnableAttr(int attr, bool enable);

	void Blink();
	void SetBlink();
	void SetBlinkIsReal();

	void SetAcsStatus(int status);

	void Foreground(int i);
	void AixtermForeground(int i);
	void DefaultForeground();
	void NormalForeground();

	void Background(int i);
	void AixtermBackground(int i);
	void DefaultBackground();
	void NomalBackground();

	int Attr_38(int i);
	int Attr_48(int i);
};