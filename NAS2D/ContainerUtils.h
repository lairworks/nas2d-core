#pragma once


#include <vector>
#include <algorithm>


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

		template <typename T>
		std::vector<T>& operator-=(std::vector<T>& container1, const std::vector<T>& container2)
		{
			container1.erase(
				std::remove_if(
					container1.begin(),
					container1.end(),
					[&container2](const T& value){
						return std::find(container2.begin(), container2.end(), value) != container2.end();
					}
				),
				container1.end()
			);
			return container1;
		}

		template <typename T>
		std::vector<T> operator-(std::vector<T> container1, const std::vector<T>& container2)
		{
			return container1 -= container2;
		}
	}


	template <typename Container>
	bool has(const Container& container, const typename Container::value_type& value)
	{
		return std::find(std::begin(container), std::end(container), value) != std::end(container);
	}

	template <typename KeyValueContainer>
	std::vector<typename KeyValueContainer::key_type> getKeys(const KeyValueContainer& map)
	{
		std::vector<typename KeyValueContainer::key_type> result;
		result.reserve(map.size());
		for (const auto& pair : map)
		{
			result.push_back(pair.first);
		}
		return result;
	}


	template <typename T>
	struct MissingAndUnexpected {
		std::vector<T> missing;
		std::vector<T> unexpected;

		bool operator==(const MissingAndUnexpected& other) const {
			return missing == other.missing && unexpected == other.unexpected;
		}
		bool operator!=(const MissingAndUnexpected& other) const {
			return !(*this == other);
		}
	};

	template <typename T>
	MissingAndUnexpected<T> findMissingAndUnexpected(
		const std::vector<T>& names,
		const std::vector<T>& required,
		const std::vector<T>& optional = {}
	) {
		using namespace ContainerOperators;

		const auto expected = required + optional;
		return {required - names, names - expected};
	}
}
