#pragma once


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
		explicit XmlSubSectionIterator(const Xml::XmlElement* currentElement);

		XmlSubSectionIterator& operator++();
		bool operator!=(const XmlSubSectionIterator& other);
		XmlSection operator*() const;

	protected:
		const Xml::XmlElement* mXmlElement;
	};
}
