#pragma once

#include <cstddef>


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}

	class XmlSubSectionRange;
	class XmlSection;


	class XmlSubSectionIterator
	{
	public:
		explicit XmlSubSectionIterator(const Xml::XmlElement& currentElement);

		XmlSubSectionIterator& operator++();
		bool operator!=(std::nullptr_t);
		XmlSection operator*() const;

	protected:
		const Xml::XmlElement* mXmlElement;
	};
}
