#pragma once

#include <string_view>


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}

	class Document;
	class WriterScalar;
	class WriterArray;


	class WriterObject
	{
	public:
		WriterScalar scalar(std::string_view name);
		WriterObject object(std::string_view name);
		WriterArray array(std::string_view name);

	protected:
		friend Document;
		friend WriterArray;

		explicit WriterObject(Xml::XmlElement& element);

	private:
		Xml::XmlElement& mElement;
	};
}
