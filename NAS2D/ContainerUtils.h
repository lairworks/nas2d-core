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


#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>


namespace NAS2D
{
	namespace ContainerOperators
	{
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
					[&container2](const T& value) {
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


	template <typename Container, typename UnaryOperation>
	auto mapToVector(const Container& container, UnaryOperation mapFunction)
	{
		using ResultType = decltype(mapFunction(*std::begin(container)));
		using ElementType = std::remove_cv_t<std::remove_reference_t<ResultType>>;

		std::vector<ElementType> results;
		results.reserve(std::size(container));

		std::transform(std::begin(container), std::end(container), std::back_inserter(results), mapFunction);
		return results;
	}

	/**
	 * Determine the number of elements in a collection of collection of elements
	 *
	 * Example:
	 * A std::string is a collection of chars, so a std::vector<std::string> would
	 * be a collection of collection of chars. The result of flattenSize would be
	 * the char length if all those strings were joined into on. That is, the size
	 * of flatten the collection of collection of chars into a collection of
	 * chars.
	 **/
	template <typename Container>
	auto flattenSize(const Container& collectionOfCollection)
	{
		return std::accumulate(
			std::begin(collectionOfCollection),
			std::end(collectionOfCollection),
			std::size_t{0},
			[](std::size_t sum, const typename Container::value_type& innerCollection) noexcept {
				return sum + innerCollection.size();
			}
		);
	}

	template <typename Container>
	auto flatten(const Container& collectionOfCollection)
	{
		using InnerCollectionType = typename Container::value_type;

		InnerCollectionType result;
		result.reserve(flattenSize(collectionOfCollection));

		for (const auto& innerCollection : collectionOfCollection)
		{
			result.insert(result.end(), innerCollection.begin(), innerCollection.end());
		}
		return result;
	}

	template <typename T>
	auto missingValues(const std::vector<T>& values, const std::vector<T>& required)
	{
		using namespace ContainerOperators;
		return required - values;
	}

	template <typename T>
	auto unexpectedValues(const std::vector<T>& values, const std::vector<T>& required, const std::vector<T>& optional = {})
	{
		using namespace ContainerOperators;
		const auto expected = required + optional;
		return values - expected;
	}

	template <typename Container>
	bool has(const Container& container, const typename Container::value_type& value)
	{
		return std::find(std::begin(container), std::end(container), value) != std::end(container);
	}

	template <typename KeyValueContainer>
	std::vector<typename KeyValueContainer::key_type> getKeys(const KeyValueContainer& map)
	{
		return mapToVector(map, [](auto keyValuePair) { return keyValuePair.first; });
	}

	/// Key-wise merge of values from two key/value containers
	template <typename KeyValueContainer>
	KeyValueContainer mergeByKey(const KeyValueContainer& defaults, const KeyValueContainer& priorityValues)
	{
		KeyValueContainer results = defaults;
		for (const auto& [key, value] : priorityValues)
		{
			results[key] += value;
		}
		return results;
	}
}
