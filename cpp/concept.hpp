#ifndef CONCEPT
#define CONCEPT

#include "constant.hpp"
#include "factory.hpp" // for NewFactory

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
		// Checkable concept. Requires a member function ok() returning something convertible to bool.
		template <typename T, typename... Args>
		concept Checkable = requires (T t, Args&& ... args)
		{
			{t.ok(args ...)} -> std::convertible_to<bool>;
		};

		// Hashable concept. Requires a member function hash() returning something convertible to Hash type.
		template <typename T>
		concept Hashable = requires (T t)
		{
			{t.hash()} -> std::convertible_to<Hash>;
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

		// Factorable concept requires a public type defined named Factory and a descendent of NewFactory.
		template <typename T>
		concept Factorable = requires
		{
			typename T::Factory;
			std::derived_from<typename T::Factory, NewFactory<T>>;
		};


		// FactoryConstructible concept. Requires Initializable and a static factory method named get returning something convertible to T&.
		template <class T, typename... Args>
		concept FactoryConstructible = Initializable<T, Args...>&& requires (Args... args)
		{
			{T::get(args ...)} noexcept -> std::convertible_to<typename T::Factory::P>;
		} && Factorable<T>;
	
		// FactoryCloneable concept requires Assignable and FactoryConstructible.
		//template <typename T, typename... Args>
		//concept FactoryCloneable = Assignable<T> && FactoryConstructible<T, Args...>;

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

		// NarrowOutputStreamable concept.
		template<typename T>
		concept NarrowOutputStreamable = requires(T a, std::ostream & os) { { os << a } -> std::same_as<std::ostream&>; };

		// WideOutputStreamable concept.
		template<typename T>
		concept WideOutputStreamable = requires(T a, std::wostream & os) { { os << a } -> std::same_as<std::wostream&>; };

		// OutputStreamable concept. NarrowOutputStreamable or WideOutputStreamable.
		template<typename T>
		concept OutputStreamable = NarrowOutputStreamable<T> || WideOutputStreamable<T>;

		// NarrowInputStreamable concept.
		template<typename T>
		concept NarrowInputStreamable = requires(T a, std::istream & is) 	{ { is >> a } -> std::same_as<std::istream&>; };

		// WideInputStreamable concept.
		template<typename T>
		concept WideInputStreamable = requires(T a, std::wistream & is) { { is >> a } -> std::same_as<std::wistream&>; };

		// InputStreamable concept. NarrowInputStreamable or WideInputStreamable.
		template<typename T>
		concept InputStreamable = NarrowInputStreamable<T> || WideInputStreamable<T>;

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

		// Sizeofable, a concept requiring a type that supports sizeof(t).
		template <typename T>
		concept Sizeofable = requires(T t)
		{
			{ sizeof(t) } -> std::same_as<size_t>;
		};
		
		// Identifiable concept requires a public id () method returning something convertible to Id type.
		template <typename T>
		concept Identifiable = requires(T t) { { t.id() } -> std::convertible_to<Id>; };	

		// SizeableIdentifiable concept requires Identifiable and SizeableType.	
		template <typename T>
		concept SizeableIdentifiable = Identifiable<T> && Sizeofable<T>;

		// Countable requires a public count () method returning something convertible to size_t .
		template <typename T>
		concept Countable = requires(T t) { { t.count() } -> std::convertible_to<size_t>; };

		// Sizeable, requires SizeableType or SizeableObject.
		template <typename T>
		concept Sizeable = requires(T t) { { t.size() } -> std::convertible_to<size_t>; };

		// BinaryStreamable concept requires CharCastable, Sizeable and Streamable.
		template <typename T>
		concept BinaryStreamable = CharCastable<T> && Sizeofable<T> && Streamable<T>;

		// BinaryConvertible concept requires a public method bytes() returning something convertible to std::span<std::byte>&.
		template <typename T>
		concept BinaryConvertible = requires(T t) { { t.wbytes () } -> std::convertible_to<std::span<std::byte>>; };
		
		// BinaryOutputtableObject concept requires BinaryConvertible and SizeableIdentifiable.
		template <class T>
		concept BinaryWriteableObject = BinaryConvertible<T> && Sizeofable<T>;

		template <typename T>
		concept BinaryStreamableObject = BinaryConvertible<T> && Streamable<T>;

		// BinaryWriteable concept requires a public method write (std::span<std::byte>& wbytes).
		template <typename T>
		concept BinaryWriteable = requires(T t, std::span<std::byte>& wbytes) { { t.write (wbytes) } -> std::convertible_to<void>; };

		// ObjectBinaryWriteable concept requires a type T with a public method write<Obj> (const Obj& object). Where Obj must comply with BinaryWriteableObject.
		template <typename T, typename Obj>
		concept ObjectBinaryWriteable = requires(T t, const Obj & object)
		{
			{ t.template write<Obj>(object) } -> std::convertible_to<void>;
		}&& BinaryWriteableObject<Obj>;

		// FactoryObjectBinaryWriteable concept requires a type T with a public method write<Obj> (const Obj& object). Where Obj must comply with BinaryOutputtableObject.
		template <typename T, typename Obj, typename... Args>
		concept FactoryObjectBinaryWriteable = requires(T t,const Obj& object)
		{ 
			{ t.template write<Obj, Args...> (object) } -> std::convertible_to<void>;
		} && ObjectBinaryWriteable<T, Obj> && FactoryConstructible<Obj, Args ...>;

		// BinaryReadable concept requires a public method read (std::span<std::byte>& bytes) returning something convertible to T&.
		template <typename T>
		concept BinaryReadable = requires(T t, Bytes& bytes) { { t.read (bytes) } -> std::convertible_to<void>; };

		// ObjectBinaryReadable concept requires a type T with a public method template <class Obj> void read(Obj* p). Where Obj must comply with BinaryReadable.
		template <typename T, typename Obj>
		concept ObjectBinaryReadable = requires(T t, Obj* p)
		{
			{ t.template read<Obj> (p) } -> std::convertible_to<void>;
		} && BinaryReadable<T>;

		// FactoryObjBinaryReadable concept requires a type T reader of U objects. T must have a public method read with signature: U::Factory::P read<U, Args...> () . U must comply with BinaryInputtable && FactoryConstructible.
		template <typename T, typename Obj, typename... Args>
		concept FactoryObjBinaryReadable = requires(T t)
		{
			{ t.template read<Obj, Args...> () } -> std::convertible_to<typename Obj::Factory::P>;
		} && ObjectBinaryReadable<T, Obj>&& FactoryConstructible<Obj, Args ...>;
		
		// Pointable concept requires a type T that supports operator-> returning something convertible to T* and supports *T returning something convertible to T&.
		template<typename T>
		concept Pointable = requires(T t) 
		{ 
			{ t.operator->() } -> std::convertible_to<T*>; 
			{ *t             } -> std::convertible_to<T&>; 
		};

		// StandardLayout concept requires a type T that is standard layout.
		template<typename T>
		concept StandardLayout = std::is_standard_layout_v<T>;

		// TriviallyCopyable concept requires a type T that is trivially copyable.
		template<typename T>
		concept TriviallyCopyable = std::is_trivially_copyable_v<T>;

		// StdLayoutTriviallyCopyable concept requires a type T that is standard layout and trivially copyable.
		template<typename T>
		concept StdLayoutTriviallyCopyable = StandardLayout<T> && TriviallyCopyable<T>;	
		// Memcpyasble concept. Requires a function data() returning something convertible to void*. Usually a pointer to std::byte. And a data_size() returning something convertible to size_t.

		template <class T>
		concept Persistable = requires (T t)
		{
			{t.data()     } -> std::convertible_to<const Data*>;
			{t.data_size()} -> std::convertible_to<size_t>;
		} && TriviallyCopyable<typename T::DataType> && Identifiable<T> && Hashable<T>;

		// TriviallyDestructible concept requires a type T that is trivially destructible.
		template<typename T>
		concept TriviallyDestructible = std::is_trivially_destructible_v<T>;

		// TriviallyConstructible concept requires a type T that is trivially constructible.
		template<typename T>
		concept TriviallyConstructible = std::is_trivially_constructible_v<T>;

		// TriviallyAssignable concept requires a type T that is trivially assignable.
		template<typename T, typename U>
		concept TriviallyAssignable = std::is_trivially_assignable_v<T, U>;

		// TriviallyCopyAssignable concept requires a type T that is trivially copy assignable.
		template<typename T>
		concept TriviallyCopyAssignable = std::is_trivially_copy_assignable_v<T>;

		// TriviallyMoveAssignable concept requires a type T that is trivially move assignable.
		template<typename T>
		concept TriviallyMoveAssignable = std::is_trivially_move_assignable_v<T>;

		// TriviallyMoveConstructible concept requires a type T that is trivially move constructible.
		template<typename T>
		concept TriviallyMoveConstructible = std::is_trivially_move_constructible_v<T>;

		// TriviallyCopyConstructible concept requires a type T that is trivially copy constructible.
		template<typename T>
		concept TriviallyCopyConstructible = std::is_trivially_copy_constructible_v<T>;

		// TriviallyMovable concept requires a type T that is trivially movable.
		template<typename T>
		concept TriviallyMovable = TriviallyMoveConstructible<T> && TriviallyMoveAssignable<T>;

	} // namespace cpplib
} // namespace pensar_digital	
#endif // CONCEPT
