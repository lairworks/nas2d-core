#pragma once


#include <vector>
#include <type_traits>


namespace NAS2D {
	namespace ContainerOperators {
		template <typename T>
		std::vector<T>& operator+=(std::vector<T>& container1, const std::vector<T>& container2)
		{
			container1.reserve(container1.size() + container2.size());
			container1.insert(container1.end(), container2.begin(), container2.end());
			return container1;
		}

		template <typename T>
		std::vector<T> operator+(std::vector<T> container1, const std::vector<T>& container2)
		{
			return container1 += container2;
		}
	}
}
