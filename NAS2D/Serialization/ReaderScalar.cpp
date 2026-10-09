#include "ReaderScalar.h"

#include <stdexcept>


using namespace NAS2D;


ReaderScalar::ReaderScalar(std::string_view value) :
	mValue{value}
{
}
