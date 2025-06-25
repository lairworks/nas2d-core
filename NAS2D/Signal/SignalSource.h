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

#include "Delegate.h"
#include <vector>
#include <algorithm>


namespace NAS2D
{
	template <typename... Params>
	class SignalSource
	{
	public:
		using Source = SignalSource; // Restricted base interface for use in derived
		using DelegateType = Delegate<void(Params...)>;

	public:
		std::size_t count() const { return delegateList.size(); }

		bool isEmpty() const { return delegateList.empty(); }

		bool isConnected(DelegateType delegate) const
		{
			const auto iterator = std::find(delegateList.begin(), delegateList.end(), delegate);
			return (iterator != delegateList.end());
		}

		void connect(DelegateType delegate)
		{
			if (!isConnected(delegate))
			{
				delegateList.push_back(delegate);
			}
		}

		void disconnect(DelegateType delegate)
		{
			std::erase(delegateList, delegate);
		}

		void clear() { delegateList.clear(); }

	protected:
		std::vector<DelegateType> delegateList{};
	};
} // namespace
