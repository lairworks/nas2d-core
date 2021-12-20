#pragma once

#include <cstddef>


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}

	class XmlSubSectionIterator;


	class XmlSubSectionRange
	{
	public:
		explicit XmlSubSectionRange(const Xml::XmlElement& parentElement);

		XmlSubSectionIterator begin() const;
		std::nullptr_t end() const;

	protected:
		const Xml::XmlElement& mParentElement;
	};
}
