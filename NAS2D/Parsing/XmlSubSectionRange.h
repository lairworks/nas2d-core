#pragma once

#include "XmlSubSectionIterator.h"


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}


	class XmlSubSectionRange
	{
	public:
		explicit XmlSubSectionRange(const Xml::XmlElement& parentElement);

		XmlSubSectionIterator begin() const;
		XmlSubSectionIterator end() const;

	protected:
		const Xml::XmlElement& mParentElement;
	};
}
