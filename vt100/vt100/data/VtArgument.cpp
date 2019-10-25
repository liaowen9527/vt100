#include "Argument.h"



VtArgument::VtArgument()
{

}

VtArgument::~VtArgument()
{

}

int VtArgument::Count()
{
	return 0;
}

unsigned int VtArgument::GetArg(int index)
{
	return m_esc_args[index];
}

unsigned int VtArgument::GetArg(int index, unsigned int _default)
{
	unsigned int arg = m_esc_args[index];

	return arg == ARG_DEFAULT ? _default : arg;
}

unsigned int VtArgument::GetClampArg(int index, unsigned int max)
{
	unsigned int arg = std::min<unsigned int>(m_esc_args[index], max);
	m_esc_args[index] = arg;

	return arg;
}

unsigned int VtArgument::GetArg(int index, unsigned int max, unsigned int _default)
{
	unsigned int arg = std::min<unsigned int>(m_esc_args[index], max);
	m_esc_args[index] = arg;

	return arg == ARG_DEFAULT ? _default : arg;
}

