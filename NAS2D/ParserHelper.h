// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#pragma once

#include "Dictionary.h"

#include <string>
#include <vector>
#include <map>


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}


	void reportMissingOrUnexpected(const std::vector<std::string>& missing, const std::vector<std::string>& unexpected);
	void reportMissingOrUnexpected(const std::vector<std::string>& names, const std::vector<std::string>& required, const std::vector<std::string>& optional);

	Dictionary attributesToDictionary(const Xml::XmlElement& element);
	std::map<std::string, Dictionary> subTagsToDictionaryMap(const Xml::XmlElement& element);

	Xml::XmlElement* dictionaryToAttributes(const std::string& tagName, const Dictionary& dictionary);
	Xml::XmlElement* dictionaryMapToElement(const std::string& tagName, const std::map<std::string, Dictionary>& sections);
}
