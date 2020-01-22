// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include "Resource.h"

#include <map>

namespace NAS2D {

/**
 *  \class Music
 *  \brief Music Resource.
 */
class Music : public Resource
{
public:
	Music() = default;
	explicit Music(const std::string& filePath);

	Music(const Music& rhs);
	Music& operator=(const Music& rhs);

	virtual ~Music();

private:
	void load();
};

} // namespace
