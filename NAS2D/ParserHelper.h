#pragma once

#include "Dictionary.h"
#include "Xml/Xml.h"

#include <string>
#include <vector>
#include <map>


namespace NAS2D {
	void reportMissingOrUnexpected(const std::vector<std::string>& missing, const std::vector<std::string>& unexpected);
	void reportMissingOrUnexpected(const std::vector<std::string>& names, const std::vector<std::string>& required, const std::vector<std::string>& optional);

	Dictionary attributesToDictionary(const Xml::XmlElement& element);
	std::map<std::string, Dictionary> subTagsToDictionaryMap(const Xml::XmlElement& element);
}
