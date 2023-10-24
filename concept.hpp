#ifndef CONCEPT
#define CONCEPT

#include "constant.hpp"
#include <concepts>
#include <iostream>
#include <memory> // for std::shared_ptr
#include <type_traits> // for std::is_same
#include <span>
#include <cstddef> // for std::byte



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
		template <class T, class FactoryReturnType = std::shared_ptr<T>, typename... Args>
		concept FactoryConstructible = Initializable<T, Args...>&& requires (Args... args)
		{
			{T::get (args ...)} noexcept -> std::convertible_to<FactoryReturnType>;
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

		// Container concept. Requires T to be a container with a public T::value_type defined and with a public method at(size_t i) and an operator[size_t i] returning something convertible to T::value_type&. And a size () returning something convertible to size_t.
		template <typename T>
		concept Container = requires (T t, size_t i)
		{
			typename T::value_type;
			{ t.at(i) } -> std::convertible_to<typename T::value_type&>;
			{ t[i] } -> std::convertible_to<typename T::value_type&>;
			{ t.size() } -> std::convertible_to<size_t>;
		};

		// ContainerV is a concept requiring Container<T> and T::value_type to be of type V.
		template <typename T, typename V>
		concept ContainerV = Container<T> && std::is_same<typename T::value_type, V>::value;



		

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
		
		// Identifiable concept requires a public id () method returning something convertible to Id type.
		template <typename T>
		concept Identifiable = requires(T t) { { t.id() } -> std::convertible_to<Id>; };	

		// SizeableIdentifiable concept requires Identifiable and SizeableType.	
		template <typename T>
		concept SizeableIdentifiable = Identifiable<T> && SizeableType<T>;	

		// SizeableObject requires a public size () method returning something convertible to size_t .
		template <typename T>
		concept SizeableObject = requires(T t) { { t.size() } -> std::convertible_to<size_t>; };

		// Sizeable, requires SizeableType or SizeableObject.
		template <typename T>
		concept Sizeable = SizeableType<T> || SizeableObject<T>;

		// BinaryStreamable concept requires CharCastable, Sizeable and Streamable.
		template <typename T>
		concept BinaryStreamable = CharCastable<T> && Sizeable<T> && Streamable<T>;

		// BinaryConvertible concept requires a public method bytes() returning something convertible to std::span<std::byte>&.
		template <typename T>
		concept BinaryConvertible = requires(T t) { { t.bytes () } -> std::convertible_to<std::span<std::byte>>; };
		
		// BinaryOutputtableObject concept requires BinaryConvertible and SizeableIdentifiable.
		template <class T>
		concept BinaryOutputtableObject = BinaryConvertible<T> && SizeableIdentifiable<T>;

		template <typename T>
		concept BinaryStreamableObject = BinaryConvertible<T> && Streamable<T>;

		// BinaryOutputtable concept requires a public method write (std::span<std::byte>& bytes) returning something convertible to T&.
		template <typename T>
		concept BinaryOutputtable = requires(T t, std::span<std::byte>& bytes) { { t.write (bytes) } -> std::convertible_to<T&>; };

		// ObjectBinaryOutputtable concept requires a type T with a public method write<Obj> (const Obj& object) returning something convertible to T&. Where Obj must comply with BinaryOutputtableObject.
		template <typename T, typename Obj>
		concept ObjectBinaryOutputtable = requires(T t, Obj object) 
		{ 
			{ t.write (object) } -> std::convertible_to<T&>; 
		} && BinaryOutputtableObject<Obj>;

		// BinaryInputtable concept requires a public method read (std::span<std::byte>& bytes) returning something convertible to T&.
		template <typename T>
		concept BinaryInputtable = requires(T t, std::span<std::byte>& bytes) { { t.read (bytes) } -> std::convertible_to<T&>; };

		// ObjectBinaryInputtable concept requires a type T with a public method read (U& source) returning something convertible to std::shared_ptr<T>. Where U must comply with BinaryInputtable.
		template <typename T, typename U, typename... Args>
		concept ObjectBinaryInputtable = requires(T t, U& source) 
		{ 
			{ t.read (source) } -> std::convertible_to<std::shared_ptr<T>>; 
		} && BinaryInputtable<T> && FactoryConstructible<U, Args ...>;

		// Pointable concept requires a type T that supports operator-> returning something convertible to T* and supports *T returning something convertible to T&.
		template<typename T>
		concept Pointable = requires(T t) 
		{ 
			{ t.operator->() } -> std::convertible_to<T*>; 
			{ *t             } -> std::convertible_to<T&>; 
		};

	} // namespace cpplib
} // namespace pensar_digital	
#endif // CONCEPT

