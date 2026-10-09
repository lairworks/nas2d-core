#pragma once

#include "Types.h"
#include "ReaderObject.h"
#include "WriterObject.h"

#include <string_view>
#include <string>


namespace NAS2D
{
	namespace Xml
	{
		class XmlDocument;
	}


	class Document
	{
	public:
		Document();
		explicit Document(std::string_view data);

		template<typename Type>
		explicit Document(std::string_view rootElementName, const Type& value) :
			Document{}
		{
			write(setRootObject(rootElementName), value);
		}

		Document(const Document&) = delete;
		Document& operator=(const Document&) = delete;

		Document(Document&&) = delete;
		Document& operator=(Document&&) = delete;

		~Document();

		template<typename Type>
		Type read(std::string_view rootElementName)
		{
			return read<Type>(getRootObject(rootElementName));
		}

		std::string asString() const;

	protected:
		ReaderObject getRootObject(std::string_view rootElementName) const;
		WriterObject setRootObject(std::string_view rootElementName);

	private:
		Xml::XmlDocument* mDocument;
	};
}
