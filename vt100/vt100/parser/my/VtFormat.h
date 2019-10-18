#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>

struct VtContext;

enum EmVtFormat
{
	emString = 0,
	emAttr,
	emRegex
};

class VtFormat;
typedef std::shared_ptr<VtFormat> VtFormatPtr;

class VtFormat
{
public:
	VtFormat();
	virtual ~VtFormat();

	static VtFormatPtr Create(EmVtFormat type);

	void SetFormat(const std::vector<std::string>& arr);
	virtual void SetFormat(const std::string& fmt);

	virtual bool Match(VtContext* context) = 0;
	virtual void GetAttrs(VtContext* context);

protected:
	std::string m_strFmt;
};


class VtStringFmt : public VtFormat
{
protected:
	virtual bool Match(VtContext* context);
};

class VtAttrFmt : public VtFormat
{
public:
	virtual bool Match(VtContext* context);
	virtual void GetAttrs(VtContext* context);
};

class VtLoopFmt : public VtFormat
{
public:
	virtual bool Match(VtContext* context);
	virtual void GetAttrs(VtContext* context);

	void SetFunc(std::function<bool(const std::string&, bool)> func);

protected:
	std::vector<std::string> SplitAttrs(const std::string& str, const std::string& sep);

protected:
	std::function<bool(const std::string& attr, bool)> m_func;
};
