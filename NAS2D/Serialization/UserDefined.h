#pragma once

#include "Types.h"


namespace NAS2D
{
	template<typename Type>
	Type read(ReaderScalar& reader) = delete;

	template<typename Type>
	Type read(ReaderObject& reader) = delete;

	template<typename Type>
	Type read(ReaderArray& reader) = delete;


	template<typename Type>
	void write(WriterScalar& writer, const Type& value) = delete;

	template<typename Type>
	void write(WriterObject& writer, const Type& value) = delete;

	template<typename Type>
	void write(WriterArray& writer, const Type& value) = delete;
}
