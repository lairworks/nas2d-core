#pragma once

#include <array>
#include <cmath>
#include <limits>
#include <random>
#include <type_traits>

using Rng_t = std::mt19937;

namespace NAS2D
{
	class Random
	{
		public:
			Random(const Random&) = delete;
			Random& operator=(const Random&) = delete;
			Random(Random&&) = delete;
			Random& operator=(Random&&) = delete;

			/**
			* \fn static void seed(std::seed_seq& seedValue)
			* 
			* Seeds the random number generator with a specific sequence.
			* 
			* \param seq: The sequence with which to seed the generator.
			* \return Nothing
			*/
			static void seed(std::seed_seq& seq) noexcept
			{
				get().seed_impl(seq);
			}

			/**
			* \fn static std::size_t seed() noexcept
			* 
			* Gets the current seed value.
			* 
			* \return The seed value.
			*/
			static const std::array<uint32_t, Rng_t::state_size>& seed() noexcept
			{
				return get().seed_impl();
			}

			/**
			* \fn static T GetRandomValueInRange(T lower, T upper)
			* 
			* Generates a random value in the closed range [lower, upper].
			*
			* \param inclusiveLowerBound: The inclusive lower bound to generate from.
			* \param inclusiveUpperBound: The inclusive upper bound to generate from.
			* \return A value in the closed range [lower, upper]
			* \remarks
			* The inequality `lower < upper` must hold. It is undefined behavior otherwise.
			*/
			template<typename T>
			static T GetRandomValueInRange(T inclusiveLowerBound, T inclusiveUpperBound) noexcept
			{
				return get().GetRandomValueInRange_impl<T>(inclusiveLowerBound, inclusiveUpperBound);
			}

			/**
			* \fn static T GetRandomValueLessThan(T upper)
			* 
			* Generates a random value in the half open range [0, upper).
			*
			* \param exclusiveUpperBound: The exclusive upper bound to generate from.
			* \return A value in the half open range [0, upper)
			*/
			template<typename T>
			static T GetRandomValueLessThan(T exclusiveUpperBound) noexcept
			{
				return get().GetRandomValueLessThan_impl<T>(exclusiveUpperBound);
			}

			/**
			* \fn static bool GetRandomBool()
			* 
			* Generates a random true or false value with a probability of `0.5`.
			* 
			* \return A boolean value.
			*/
			static bool GetRandomBool() noexcept
			{
				return get().GetRandomValueLessThan(2) == 0;
			}

			/**
			* \fn static bool GetRandomBool()
			* 
			* Generates a random true or false value given a specific probability of being true.
			* 
			* \param percentChance: A value in the range [0.0, 1.0] that represents the probability of returning `true`.
			* \return A boolean value.
			*/
			template<typename T>
			static bool GetRandomBool(T percentChance) noexcept
			{
				return get().GetRandomFloatZeroToOne_impl() < percentChance;
			}

			/**
			* \fn static T GetRandomFloatZeroToOne()
			* 
			* Generates a random floating-point value in the closed range [0.0, 1.0].
			* 
			* \return A floating-point value in the closed range [0.0, 1.0]
			*/
			template<typename T>
			static T GetRandomFloatZeroToOne() noexcept
			{
				return get().GetRandomFloatZeroToOne_impl<T>();
			}

			/**
			* \fn static T GetRandomFloatLessThanOne()
			* 
			* Generates a random floating-point value in the half open range [0.0, 1.0).
			* 
			* \return A floating-point value in the half open range [0, 1.0)
			*/
			template<typename T>
			static T GetRandomFloatLessThanOne() noexcept
			{
				return get().GetRandomFloatLessThanOne_impl<T>();
			}

			/**
			* \fn static T GetRandomFloatNegOneToOne()
			* 
			* Generates a random floating-point value in the closed range [-1.0, 1.0].
			* 
			* \return A floating-point value in the closed range range [-1.0, 1.0]
			*/
			template<typename T>
			static T GetRandomFloatNegOneToOne() noexcept
			{
				return get().GetRandomValueInRange_impl<T>(T{-1.0}.T{1.0});
			}

		protected:
		private:
			std::array<uint32_t, Rng_t::state_size> seed_value{};
			Rng_t generator{};

			Random()
			{
				std::random_device rdev;
				std::generate_n(std::begin(seed_value), Rng_t::state_size, std::ref(rdev));
				generator.seed(seed_value);
			}

			static Random& get() noexcept
			{
				static Random instance;
				return instance;
			}

			void seed_impl(std::seed_seq& seq) noexcept
			{
				generator.seed(seq);
			}

			const std::array<uint32_t, Rng_t::state_size>& seed_impl() const noexcept
			{
				return seed_value;
			}

			template<typename T>
			T GetRandomValueInRange_impl(T inclusiveLowerBound, T inclusiveUpperBound) const noexcept
			{
				if constexpr (std::is_floating_point_v<T>)
				{
					const auto upper_bound = std::nextafter(inclusiveUpperBound, std::numeric_limits<T>::max());
					std::uniform_real_distribution<T> dist(inclusiveLowerBound, upper_bound);
					return dist(generator);
				}
				else if constexpr (std::is_integral_v<T>)
				{
					std::uniform_int_distribution<T> dist(inclusiveLowerBound, inclusiveUpperBound);
					return dist(generator);
				}
			}

			template<typename T>
			T GetRandomValueLessThan_impl(T exclusiveUpperBound) const noexcept
			{
				if constexpr (std::is_floating_point_v<T>)
				{
					std::uniform_real_distribution<T> dist(T{0.0}, exclusiveUpperBound);
					return dist(generator);
				}
				else if constexpr (std::is_integral_v<T>)
				{
					std::uniform_int_distribution<T> dist(T{0}, exclusiveUpperBound - T{1});
					return dist(generator);
				}
			}

			template<typename T>
			T GetRandomFloatZeroToOne_impl() const noexcept
			{
				return GetRandomValueInRange_impl(T{0.0}, T{1.0});
			}

			template<typename T>
			T GetRandomFloatLessThanOne_impl() const noexcept
			{
				return GetRandomValueLessThan_impl(T{1.0});
			}
	};

} // namespace NAS2D
