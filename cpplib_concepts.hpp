#ifndef CPPLIB_CONCEPTS
#define CPPLIB_CONCEPTS

#include <concepts>
#include <iostream>

namespace pensar_digital
{
	namespace cpplib
	{
		/// Concept for a class with a noexcept initialize method returning something convertible to bool.
		template <typename T, typename... Args>
		concept Initializable = requires (T t, Args&& ... args)
		{
			{T(std::forward<Args>(args) ...)} noexcept;
			{t.initialize (std::forward<Args>(args) ...)} noexcept -> std::convertible_to<bool>;
		};

		// OutputStreamable concept.
		template<typename T>
		concept OutputStreamable = requires(T a, std::ostream & os) { { os << a } -> std::same_as<std::ostream&>; };

		// InputStreamable concept.
		template<typename T>
		concept InputStreamable = requires(T a, std::istream & is) 	{ { is >> a } -> std::same_as<std::istream&>; };

		// Streamable concept.
		template<typename T>
		concept Streamable = OutputStreamable<T> && InputStreamable<T>;
	}
}
#endif // CPPLIB_CONCEPTS

