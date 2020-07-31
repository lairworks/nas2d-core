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

#include <string>
#include <map>


namespace NAS2D {

/**
 *  \class Music
 *  \brief Music Resource.
 */
class Music
{
public:
	explicit Music(const std::string& filePath);

	Music(const Music& rhs);
	Music& operator=(const Music& rhs);

	~Music();

	const std::string& name() const { return mResourceName; }

private:
	void load();

	std::string mResourceName{"Default Resource"}; /**< File path and internal identifier. */
};

} // namespace
