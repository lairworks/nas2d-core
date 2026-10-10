#include "ReaderObject.h"

#include "../Xml/XmlAttribute.h"
#include "../Xml/XmlElement.h"

#include <stdexcept>


using namespace NAS2D;


namespace
{
	const Xml::XmlAttribute* findAttribute(const Xml::XmlElement& element, std::string_view name)
	{
		for (const auto* attribute = element.firstAttribute(); attribute; attribute = attribute->next())
		{
			if (attribute->name() == name)
			{
				return attribute;
			}
		}
		return nullptr;
	}

	std::string_view findScalarValue(const Xml::XmlElement& element, std::string_view name)
	{
		const auto* attribute = findAttribute(element, name);
		if (attribute)
		{
			return attribute->value();
		}

		const auto* childElement = element.firstChildElement(name);
		if (childElement)
		{
			return childElement->getText();
		}

		// **TODO** Better exception type?
		throw std::runtime_error("Required scalar not found: " + std::string{name});
	}
}


ReaderObject::ReaderObject(const Xml::XmlElement& element) :
	mElement{element}
{
}


ReaderScalar ReaderObject::scalar(std::string_view name) const
{
	return ReaderScalar{findScalarValue(mElement, name)};
}


ReaderObject ReaderObject::object(std::string_view name) const
{
	const auto* childElement = mElement.firstChildElement(name);
	if (!childElement)
	{
		throw std::runtime_error("Required object not found: " + std::string{name});
	}
	return ReaderObject{*childElement};
}


ReaderArray ReaderObject::array(std::string_view name) const
{
	return ReaderArray{mElement, name};
}
