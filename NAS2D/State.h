// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#pragma once


namespace NAS2D {

class State
{
public:
	State() {}

	virtual ~State() = default;

protected:
	friend class StateManager;

	virtual void initialize() = 0;
	virtual State* update() = 0;
};

} // namespace
