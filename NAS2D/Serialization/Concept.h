#pragma once

#include "Types.h"


namespace NAS2D
{
	template<typename Type>
	concept hasReadScalar = requires(const ReaderScalar& reader) { readType<Type>(reader); };

	template<typename Type>
	concept hasReadObject = requires(const ReaderObject& reader) { readType<Type>(reader); };

	template<typename Type>
	concept hasReadArray = requires(const ReaderArray& reader) { readType<Type>(reader); };


	template<typename Type>
	concept hasWriteScalar = requires(WriterScalar& writer, const Type& value) { write(writer, value); };

	template<typename Type>
	concept hasWriteObject = requires(WriterObject& writer, const Type& value) { write(writer, value); };

	template<typename Type>
	concept hasWriteArray = requires(WriterArray& writer, const Type& value) { write(writer, value); };
}
