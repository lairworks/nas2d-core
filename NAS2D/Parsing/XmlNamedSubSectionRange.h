#pragma once

#include "XmlNamedSubSectionIterator.h"

#include <string>


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}


	class XmlNamedSubSectionRange
	{
	public:
		explicit XmlNamedSubSectionRange(const Xml::XmlElement& parentElement, std::string name);

		XmlNamedSubSectionIterator begin() const;
		XmlNamedSubSectionIterator end() const;

		XmlNamedSubSectionIterator::difference_type size() const;

	protected:
		const Xml::XmlElement& mParentElement;
		std::string mName;
	};
}
