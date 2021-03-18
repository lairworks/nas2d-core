#pragma once

#include "../StringUtils.h"
#include "Xml.h"
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>

namespace NAS2D {
	// Open an xml document
	// Throws a runtime_error if the xml is ill formed or the root element name is incorrect
	NAS2D::Xml::XmlDocument openXmlFile(std::string filename, std::string rootElementName);

	template<typename T>
	T stringToEnum(const std::unordered_map<std::string, T>& table, std::string value)
	{
		auto it = table.find(value);
		if (it != table.end())
		{
			return it->second;
		}

		throw std::runtime_error("Unable to parse enum with value of " + value);
	}

	// Create an overload to parse specific enums
	template<typename T, std::enable_if_t<!std::is_enum<T>::value, bool> = true>
	void parseElementValue(T& destination, const NAS2D::Xml::XmlElement* element)
	{
		try
		{
			destination = NAS2D::stringTo<T>(element->getText());
		}
		catch (const std::exception& e)
		{
			throw std::logic_error("Unable to parse the value of " + element->getText() + " from XML element " + element->value() + " as type of " + std::string(typeid(T).name()) + ". " + std::string(e.what()));
		}
	}
}