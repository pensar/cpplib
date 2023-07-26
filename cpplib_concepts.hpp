#ifndef CPPLIB_CONCEPTS
#define CPPLIB_CONCEPTS

#include "version.hpp"
#include "constants.hpp"
#include <concepts>
#include <iostream>

namespace pensar_digital
{
	namespace cpplib
	{
		// Assignable concept. Requires a member function assign(const T&).
		template <class T>
		concept Assignable = requires (T t, const T & a)
		{
			{t.assign(a)};
		};

		
		// Constrainable concept. Requires a member function ok() returning something convertible to bool.
		template <typename T, typename... Args>
		concept Constrainable = requires (T t, Args&& ... args)
		{
			{t.ok(std::forward<Args>(args) ...)} noexcept -> std::convertible_to<bool>;
		};
		
		// DefaultConstructible concept. Requires a default constructor.
		template <typename T>
		concept DefaultConstructible = requires
		{
			{T()} noexcept;
		};

		/// Concept for a class with a noexcept initialize method returning something convertible to bool.
		template <typename T, typename... Args>
		concept Initializable = requires (T t, Args... args)
		{
			{T(std::forward<Args>(args) ...)} noexcept;
			{t.initialize(std::forward<Args>(args) ...)} noexcept -> std::convertible_to<bool>;
		};

		// FactoryConstructible concept. Requires Initializable and a static factory method named get returning something convertible to T&.
		template <typename T, typename... Args>
		concept FactoryConstructible = Initializable<T, Args...>&& requires (Args... args)
		{
			{T::get(args ...)} noexcept -> std::convertible_to<T&>;
		};

		// FactoryCloneable concept requires Assignable and FactoryConstructible.
		template <typename T, typename... Args>
		concept FactoryCloneable = Assignable<T> && FactoryConstructible<T, Args...>;

		
		// Negatable. Requires the unary operator ! to be defined returning something convertible to bool.
		template <typename T>
		concept Negatable = requires (T t)
		{
			{!t} noexcept -> std::convertible_to<bool>;
		};

		// Andable. Requires the binary operator && to be defined returning something convertible to bool.
		template <typename T>
		concept Andable = requires (T t)
		{
			{t && t} noexcept -> std::convertible_to<bool>;
		};

		// Orable. Requires the binary operator || to be defined returning something convertible to bool.
		template <typename T>
		concept Orable = requires (T t)
		{
			{t || t} noexcept -> std::convertible_to<bool>;
		};

		// Xorable. Requires the binary operator ^ to be defined returning something convertible to bool.
		template <typename T>
		concept Xorable = requires (T t)
		{
			{t ^ t} noexcept -> std::convertible_to<bool>;
		};

		// Versionable concept requires a inline static const structVersion public member named VERSION.
		template <typename T>	
		concept Versionable = requires (T t)
		{
			{t.VERSION} noexcept -> std::convertible_to<structVersion>;
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

