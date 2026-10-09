#pragma once

#include "Concept.h"
#include "ReaderObject.h"

#include <string_view>


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}


	class ReaderArray
	{
	public:
		explicit ReaderArray(const Xml::XmlElement& parentElement, std::string_view arrayElementName);

		bool hasNext() const;

		template<typename Type>
		Type read()
		{
			if constexpr(hasReadObject<Type>)
			{
				return read<Type>(object());
			}
			else
			{
				static_assert(false, "No object reader for element type: XML backend only supports arrays of objects");
			}
		}

	protected:
		ReaderObject object();

	private:
		const Xml::XmlElement& mParentElement;
		std::string_view mArrayElementName;
		const Xml::XmlElement* mCurrentElement;
	};
}
