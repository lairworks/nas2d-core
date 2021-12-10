#include "XmlSection.h"

#include "../Xml/XmlElement.h"
#include "../ContainerUtils.h"

#include <stdexcept>


using namespace NAS2D;


namespace
{
	void throwErrorMissingUnexpected(const XmlSection& xmlSection, const std::string& context, const std::vector<std::string>& missing, const std::vector<std::string>& unexpected)
	{
		const auto missingString = !missing.empty() ? "Missing names: {" + join(missing, ", ") + "}" : "";
		const auto unexpectedString = !unexpected.empty() ? "Unexpected names: {" + join(unexpected, ", ") + "}" : "";
		const auto joinString = (!missingString.empty() && !unexpectedString.empty()) ? "\n" : "";
		xmlSection.throwError(context + missingString + joinString + unexpectedString);
	}


	void verifyNames(const XmlSection& xmlSection, const std::string& context, const std::vector<std::string>& names, const std::vector<std::string>&required, const std::vector<std::string>& optional)
	{
		const auto missing = missingValues(names, required);
		const auto unexpected = unexpectedValues(names, required, optional);
		if (!missing.empty() || !unexpected.empty())
		{
			throwErrorMissingUnexpected(xmlSection, "Failed to validate " + context, missing, unexpected);
		}
	}
}


XmlSection::XmlSection(const NAS2D::Xml::XmlElement& xmlElement) :
	mXmlElement{xmlElement}
{
}


const std::string& XmlSection::name() const
{
	return mXmlElement.value();
}


void XmlSection::throwError(const std::string& errorMessage) const
{
	const auto positionString = " at (line " + std::to_string(mXmlElement.row()) + ", column " + std::to_string(mXmlElement.column()) + ")";
	throw std::runtime_error(errorMessage + positionString);
}


void XmlSection::verifySubSections(const std::vector<std::string>& required, const std::vector<std::string>& optional) const
{
	verifyNames(*this, "Sub-sections", subSectionNames(), required, optional);
}


void XmlSection::verifyKeys(const std::vector<std::string>& required, const std::vector<std::string>& optional) const
{
	verifyNames(*this, "Keys", keys(), required, optional);
}


std::vector<std::string> XmlSection::subSectionNames() const
{
	std::vector<std::string> results;
	for (const auto* subElement = mXmlElement.firstChildElement(); subElement; subElement = subElement->nextSiblingElement())
	{
		results.push_back(subElement->value());
	}
	return results;
}


std::vector<std::string> XmlSection::keys() const
{
	std::vector<std::string> results;
	for (const auto* attribute = mXmlElement.firstAttribute(); attribute; attribute = attribute->next())
	{
		results.push_back(attribute->name());
	}
	return results;
}


bool XmlSection::hasSubSection(const std::string& name) const
{
	const auto names = subSectionNames();
	return std::find(names.begin(), names.end(), name) != names.end();
}


bool XmlSection::hasKey(const std::string& key) const
{
	const auto names = keys();
	return std::find(names.begin(), names.end(), key) != names.end();
}


XmlSection XmlSection::subSection(const std::string& name) const
{
	const auto* subElement = mXmlElement.firstChildElement(name);
	if (!subElement)
	{
		throwError("Required sub-section name not found: " + name);
	}
	return XmlSection{*subElement};
}


NAS2D::StringValue XmlSection::valueOrEmpty(const std::string& key) const
{
	return NAS2D::StringValue{mXmlElement.attribute(key)};
}
