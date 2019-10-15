#include "VtFmtAction.h"
#include "parser/VtContext.h"
#include <assert.h>


VtFormat::VtFormat()
{

}

VtFormat::~VtFormat()
{

}

VtFormatPtr VtFormat::Create(EmVtFormat type)
{
	if (emString == type)
	{
		return std::make_shared<VtStringFmt>();
	}
	else if (emAttr == type)
	{
		return std::make_shared<VtAttrFmt>();
	}
	else if (emRegex == type)
	{
		return std::make_shared<VtAttrFmt>();
	}
	else
	{
		assert(false);
		abort();
	}

	return nullptr;
}

void VtFormat::SetFormat(const std::vector<std::string>& arr)
{
	std::string strFmt;
	for (int i = 0; i < arr.size(); ++i)
	{
		strFmt.append(arr[i]);
	}

	SetFormat(strFmt);
}

void VtFormat::SetFormat(const std::string& fmt)
{
	m_strFmt = fmt;
}

bool VtStringFmt::Match(VtContext* context)
{
	const std::string& fmt = context->buffer.m_buffer;

	int len = fmt.size();
	return m_strFmt.compare(0, len, fmt, 0, len) == 0;
}

bool VtAttrFmt::Match(VtContext* context)
{
	const std::string& fmt = context->buffer.m_fmt;

	int len = fmt.size();
	return m_strFmt.compare(0, len, fmt, 0, len) == 0;
}

bool VtLoopFmt::Match(VtContext* context)
{
	const std::string& fmt = context->buffer.m_buffer;
	int len = 0;

	//1, compare before loop
	if (fmt.length() <= m_strFmt.length())
	{
		len = fmt.length();
		return m_strFmt.compare(0, len, fmt, 0, len) == 0;
	}

	len = m_strFmt.length();
	if (!m_strFmt.compare(0, len, fmt, 0, len) != 0)
	{
		return false;
	}
	
	//compare loop
	std::vector<std::string> attrs;
	SplitAttrs(attrs, ";");
	for (int i = 0; i < attrs.size(); ++i)
	{
		bool bLast = (i == attrs.size() - 1);
		if (!m_func(attrs[i], bLast))
		{
			return false;
		}
	}

	return true;
}

void VtLoopFmt::SetFunc(std::function<bool(const std::string&, bool)> func)
{
	m_func = func;
}

void VtLoopFmt::SplitAttrs(std::vector<std::string>& attr, const std::string& sep)
{
	size_t wBegin = str.find_first_not_of(sep, 0);
	size_t wEnd = wBegin;
	while (true)
	{
		if (wBegin >= str.length())
			break;
		wEnd = str.find_first_of(sep, wBegin);
		ret.push_back(str.substr(wBegin, wEnd - wBegin));
		wBegin = str.find_first_not_of(sep, wEnd);
	}
}
