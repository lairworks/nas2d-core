#pragma once

#include <iterator>
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
		using value_type = XmlSection;
		using reference = XmlSection&;
		using pointer = XmlSection*;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;

		explicit XmlNamedSubSectionIterator(const Xml::XmlElement* currentElement, std::string name);

		XmlNamedSubSectionIterator& operator++();
		bool operator!=(const XmlNamedSubSectionIterator& other);
		XmlSection operator*() const;

	protected:
		const Xml::XmlElement* mXmlElement;
		const std::string mName;
	};
}
