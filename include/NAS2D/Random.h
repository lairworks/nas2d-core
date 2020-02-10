#pragma once

#include <cmath>
#include <limits>
#include <random>
#include <type_traits>

#if defined(_WIN64) || defined(__MINGW64__)
using Rng_t = std::mt19937_64;
#else
using Rng_t = std::mt19937;
#endif

namespace NAS2D
{
	namespace MathUtils
	{
		class Random
		{
		  public:
			Random(const Random&) = delete;
			Random& operator=(const Random&) = delete;
			Random(Random&&) = delete;
			Random& operator=(Random&&) = delete;

			/**
			* \fn static void seed(std::size_t seedValue)
			* 
			* Seeds the random number generator with a specific value.
			* 
			* \param seedValue: The value with which to seed the generator.
			* \return Nothing
			* \remarks
			* If not called prior to any other function, then the RNG is seeded with a non-deterministic random value.
			*/
			static void seed(std::size_t seedValue) noexcept
			{
				get().seed_impl(seedValue);
			}

			/**
			* \fn static std::size_t seed() noexcept
			* 
			* Gets the current seed value.
			* 
			* \return The seed value.
			*/
			static std::size_t seed() noexcept
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
			Random()
			: seed_value(std::random_device{}())
			, generator(seed_value)
			{}

			std::size_t seed_value{};
			Rng_t generator;

			static Random& get() noexcept
			{
				static Random instance;
				return instance;
			}


			void seed_impl(std::size_t seedValue) noexcept
			{
				generator.seed(seedValue);
			}

			std::size_t seed_impl() const noexcept
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

	} // namespace MathUtils

} // namespace NAS2D
