#pragma once

#include "Document.h"


namespace NAS2D
{
	template<typename Type>
	Type readFileData(std::string_view xmlData, std::string_view rootElementName)
	{
		const Document document{xmlData};

		return document.readRootObject<Type>(rootElementName);
	}


	template<typename Type>
	std::string writeFileData(std::string_view rootElementName, const Type& value)
	{
		const Document document{rootElementName, value};

		return document.asString();
	}
}
