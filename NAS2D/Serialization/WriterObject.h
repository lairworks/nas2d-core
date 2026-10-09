#pragma once

#include "Concept.h"
#include "WriterScalar.h"
#include "WriterArray.h"

#include <string_view>


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}


	class WriterObject
	{
	public:
		explicit WriterObject(Xml::XmlElement& element);

		template<typename Type>
		void write(std::string_view name, const Type& value)
		{
			if constexpr (hasWriteScalar<Type>)
			{
				write(scalar(name), value);
			}
			else if constexpr (hasWriteObject<Type>)
			{
				write(object(name), value);
			}
			else if constexpr (hasWriteArray<Type>)
			{
				write(array(name), value);
			}
			else
			{
				static_assert(false, "No writer for object field");
			}
		}

	protected:
		WriterScalar scalar(std::string_view name);
		WriterObject object(std::string_view name);
		WriterArray array(std::string_view name);

	private:
		Xml::XmlElement& mElement;
	};
}
