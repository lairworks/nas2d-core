#pragma once

#include <string>
#include <cstddef>


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}

	class XmlSection;
	class XmlNamedSubSectionIterator;


	class XmlNamedSubSectionRange
	{
	public:
		explicit XmlNamedSubSectionRange(const Xml::XmlElement& parentElement, std::string name);

		XmlNamedSubSectionIterator begin() const;
		std::nullptr_t end() const;

	protected:
		const Xml::XmlElement& mParentElement;
		std::string mName;
	};
}
