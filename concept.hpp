#ifndef CONCEPT
#define CONCEPT

#include "constant.hpp"
#include <concepts>
#include <iostream>

namespace pensar_digital
{
	namespace cpplib
	{
		// Assignable concept. Requires a member function T& assign(const T&).

		template <typename T>
		concept Assignable = requires (T t)
		{
			{t.assign(t)} noexcept -> std::convertible_to<T&>;
		};
		
		// Checkable concept. Requires a member function ok() returning something convertible to bool.
		template <typename T, typename... Args>
		concept Checkable = requires (T t, Args&& ... args)
		{
			{t.ok(args ...)} -> std::convertible_to<bool>;
		};

		// RangeCheckable concept. Requires Checkable and operators >= and <=.
		template <typename T>
		concept RangeCheckable = requires (T t)
		{
			{t >= t} -> std::convertible_to<bool>;
			{t <= t} -> std::convertible_to<bool>;
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

		// AFactory concept requires a virtual get(const Args& ... args) const public method that returns
		// something convertible to std::shared_ptr<T>.
		template <typename T, typename... Args>
		concept AFactory = requires (T t, Args... args)
		{
			{t.get(args ...)} noexcept -> std::convertible_to<std::shared_ptr<T>>;
		};
		
		// Interfaceable concept two public typedefs named i_type and i_type_ro.
		template <typename T>
		concept Interfaceable = requires
		{
			typename T::I;
			typename T::I_RO;
		};	

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

		// OutputStreamable concept.
		template<typename T>
		concept OutputStreamable = requires(T a, std::ostream & os) { { os << a } -> std::same_as<std::ostream&>; };

		// InputStreamable concept.
		template<typename T>
		concept InputStreamable = requires(T a, std::istream & is) 	{ { is >> a } -> std::same_as<std::istream&>; };

		// Streamable concept.
		template<typename T>
		concept Streamable = OutputStreamable<T> && InputStreamable<T>;

		// A concept requiring a type that supports reinterpret_cast<char*>(*t).
		template <typename T>
		concept CharCastable = requires(T * t) {
			{ reinterpret_cast<char*>(*t) } -> std::same_as<char*>;
		};

		// A concept requiring a type that is castable to std::byte.
		template <typename T>
		concept ByteCastable = requires(T * t) {
			{ reinterpret_cast<std::byte*>(*t) } -> std::same_as<std::byte*>;
		};	

		// SizeableType, a concept requiring a type that supports sizeof(t).
		template <typename T>
		concept SizeableType = requires(T t)
		{
			{ sizeof(t) } -> std::same_as<size_t>;
		};

		// SizeableObject requires a public size () method returning something convertible to size_t .
		template <typename T>
		concept SizeableObject = requires(T t) { { t.size() } -> std::convertible_to<size_t>; };

		// Sizeable, requires SizeableType or SizeableObject.
		template <typename T>
			concept Sizeable = SizeableType<T> || SizeableObject<T>;

		// BinaryStreamable concept requires CharCastable, Sizeable and Streamable.
		template <typename T>
		concept BinaryStreamable = CharCastable<T> && Sizeable<T> && Streamable<T>;

		// BinaryConvertible concept requires SizeableObject and a public method to_binary() returning something convertible to char *.
		template <typename T>
		concept BinaryConvertible = requires(T t) { { t.to_binary () } -> std::convertible_to<char*>; } && SizeableObject<T>;

		template <typename T>
		concept BinaryStreamableObject = BinaryConvertible<T> && Streamable<T>;


	} // namespace cpplib
} // namespace pensar_digital	
#endif // CONCEPT

