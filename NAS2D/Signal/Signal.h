// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
// = SIGNAL.H
// ==================================================================================
// = This file is provided under the terms of the MIT license and is included as part
// = of NAS2D's library interface.
// =
// = Modifications include:
// =	- Used variadic templates for arbitrary parameter lists
// =	- Condensed lines for a more compact file
// =	- Replaced for loops using C++11 range-based loops
// =	- Split client methods into SignalSource.h
// =
// = Created by Patrick Hogan on 5/18/09.
// = https://github.com/pbhogan/Signals
// ==================================================================================
// = LICENSE: MIT (http://www.opensource.org/licenses/mit-license.php)
// ==================================================================================
#pragma once

#include "SignalSource.h"

#include <utility>


namespace NAS2D
{
	/**
	 * Signal with preset number of parameters
	 *
	 * See https://github.com/lairworks/nas2d-core/wiki/Signal-&-Slots for usage documentation.
	 */
	template <typename... Params>
	class Signal : public SignalSource<Params...>
	{
	public:
		void emit(Params... params) const
		{
			// Copy the callback list, in case a callback updates Signal connections
			// Updated signal connections would invalidate iterators to the original list
			const auto delegateListCopy = this->delegateList;
			for (auto& delegate : delegateListCopy)
			{
				delegate(params...);
			}
		}

		void operator()(Params... params) const { emit(std::forward<Params>(params)...); }
	};


	extern template class Signal<>;
	extern template class Signal<bool>;
	extern template class Signal<int>;
	extern template class Signal<int, int>;
	extern template class Signal<int, int, int>;
} // namespace
