#pragma once

#include "../StringFrom.h"

#include <string_view>


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}


	class WriterScalar
	{
	public:
		WriterScalar(Xml::XmlElement& parentElement, std::string_view attributeName);

		template<typename Type>
		void write(const Type& value)
		{
			write(stringFrom(value));
		}

	protected:
		void write(std::string_view value);

	private:
		Xml::XmlElement& mParentElement;
		std::string_view mAttributeName;
	};
}
