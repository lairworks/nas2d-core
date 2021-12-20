#pragma once

#include <string>


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}

	class XmlSection;


	class XmlNamedSubSectionIterator
	{
	public:
		explicit XmlNamedSubSectionIterator(const Xml::XmlElement* currentElement, std::string name);

		XmlNamedSubSectionIterator& operator++();
		bool operator!=(const XmlNamedSubSectionIterator& other);
		XmlSection operator*() const;

	protected:
		const Xml::XmlElement* mXmlElement;
		const std::string mName;
	};
}
