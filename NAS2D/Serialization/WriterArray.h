#pragma once

#include <string_view>


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}

	class WriterObject;


	class WriterArray
	{
	public:
		WriterObject object();

	protected:
		friend class WriterObject;

		explicit WriterArray(Xml::XmlElement& parentElement, std::string_view arrayElementName);

	private:
		Xml::XmlElement& mParentElement;
		std::string_view mArrayElementName;
	};
}
