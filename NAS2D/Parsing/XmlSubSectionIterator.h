#pragma once

#include <iterator>


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}

	class XmlSection;


	class XmlSubSectionIterator
	{
	public:
		using value_type = XmlSection;
		using reference = XmlSection&;
		using pointer = XmlSection*;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;

		explicit XmlSubSectionIterator(const Xml::XmlElement* currentElement);

		XmlSubSectionIterator& operator++();
		bool operator!=(const XmlSubSectionIterator& other);
		XmlSection operator*() const;

	protected:
		const Xml::XmlElement* mXmlElement;
	};
}
