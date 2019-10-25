#pragma once


class VtArgument
{
public:
	VtArgument();
	~VtArgument();

public:
	int Count();

	unsigned int GetArg(int index);
	unsigned int GetArg(int index, unsigned int _default);
	unsigned int GetClampArg(int index, unsigned int max);
	unsigned int GetArg(int index, unsigned int max, unsigned int _default);

protected:
	unsigned m_esc_args[ARGS_MAX];
};