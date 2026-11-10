#pragma once

#include <string_view>
#include <string>


namespace NAS2D
{
	namespace Xml
	{
		class XmlDocument;
	}

	class ReaderObject;
	class WriterObject;


	class Document
	{
	public:
		Document();
		explicit Document(std::string_view data);

		Document(const Document&) = delete;
		Document& operator=(const Document&) = delete;

		Document(Document&&) = delete;
		Document& operator=(Document&&) = delete;

		~Document();

		std::string asString() const;

		ReaderObject getRootObject(std::string_view rootElementName) const;
		WriterObject setRootObject(std::string_view rootElementName);

	private:
		Xml::XmlDocument* mDocument;
	};
}
