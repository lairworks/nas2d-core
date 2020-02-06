#pragma once

#include "NAS2D/StringUtils.h"

#include <string>

namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;

		void validateXMLElement(const XmlElement& element,
								const std::string& name,
								const std::string& requiredChildElements,
								const std::string& requiredAttributes,
								const std::string& optionalChildElements = std::string{},
								const std::string& optionalAttributes = std::string{});

		StringList getChildElementNames(const NAS2D::Xml::XmlElement& element);
		StringList getAttributeNames(const NAS2D::Xml::XmlElement& element);

	} // namespace Xml

} // namespace DataUtils