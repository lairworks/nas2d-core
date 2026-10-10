#pragma once

#include "Concept.h"
#include "ReaderScalar.h"
#include "ReaderArray.h"

#include <string_view>


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}

	class ReaderArray;


	class ReaderObject
	{
	public:
		explicit ReaderObject(const Xml::XmlElement& element);

		template<typename Type>
		Type read(std::string_view name) const
		{
			if constexpr (hasReadScalar<Type>)
			{
				return readType<Type>(scalar(name));
			}
			else if constexpr (hasReadObject<Type>)
			{
				return readType<Type>(object(name));
			}
			else if constexpr (hasReadArray<Type>)
			{
				return readType<Type>(array(name));
			}
			else
			{
				static_assert(false, "No reader for object field");
			}
		}

		template<typename Type>
		Type read(std::string_view name, const Type& defaultValue) const
		{
			if (!hasField(name))
			{
				return defaultValue;
			}

			return read<Type>(name);
		}

		bool hasField(std::string_view name) const;

		void requiredFields(std::initializer_list<std::string_view> required);
		void optionalFields(std::initializer_list<std::string_view> optional);

	protected:
		ReaderScalar scalar(std::string_view name) const;
		ReaderObject object(std::string_view name) const;
		ReaderArray array(std::string_view name) const;

	private:
		const Xml::XmlElement& mElement;
	};
}
