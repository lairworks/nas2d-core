#pragma once

#include "Concept.h"
#include "WriterObject.h"

#include <string_view>


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}


	class WriterArray
	{
	public:
		explicit WriterArray(Xml::XmlElement& parentElement, std::string_view arrayElementName);

		template<typename Type>
		void write(const Type& value)
		{
			if constexpr(hasWriteObject<Type>)
			{
				write(object(), value);
			}
			else
			{
				static_assert(false, "No object writer for element type: XML backend only supports arrays of objects");
			}
		}

	protected:
		WriterObject object();

	private:
		Xml::XmlElement& mParentElement;
		std::string_view mArrayElementName;
	};
}
