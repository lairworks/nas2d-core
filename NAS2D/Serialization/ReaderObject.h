#pragma once

#include <string_view>


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}

	class Document;
	class ReaderScalar;
	class ReaderArray;


	class ReaderObject
	{
	public:
		bool hasField(std::string_view name) const;

		void requiredFields(std::initializer_list<std::string_view> required);
		void optionalFields(std::initializer_list<std::string_view> optional);

		ReaderScalar scalar(std::string_view name) const;
		ReaderObject object(std::string_view name) const;
		ReaderArray array(std::string_view name) const;

	protected:
		friend Document;
		friend ReaderArray;

		explicit ReaderObject(const Xml::XmlElement& element);

	private:
		const Xml::XmlElement& mElement;
	};
}
