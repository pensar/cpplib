#ifndef META_HPP
#define META_HPP
#include "version.hpp"
#include "s.hpp"
#include "cs.hpp"
#include "stream_util.hpp"

#include "memory_buffer.hpp"
#include "sorted_list.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
		// Create a variadic template class called Meta that has the following template parameters: T (the type of this meta information class refers to), all parent classes (provide a mechanism for no parent classes case) and all children classes (provide a checanism for no children classes case). Also provide public methods to access these info in a modern C++ 23 way.
		
		class MetaBase
		{
			public:
				static const int8_t MAX_IDENTIFIER_LENTGH = 100; //!< Maximum size for identifier names.
				using IdentifierName = CS<0, MAX_IDENTIFIER_LENTGH>;
				using Ptr = std::shared_ptr<MetaBase>; //!< Pointer type for this base meta information class.
			protected:
				Version::Ptr mversion;		//!< Pointer to the version of T class (see class Meta below).
				IdentifierName mnamespace;	//!< Namespace of the class.
				IdentifierName mclass_name; //!< Name of the class.

				// Unique sorted list holding all class IDs.
				inline static SortedList<S> mclasses = SortedList<S>({}, true);
				inline static void register_class(const S& anamespace, const S& class_name)
				{
					if (!mclasses.add(anamespace + W("::") + class_name))
					{
						// Sends a message to the console (taking into consideration if WIDE_CHAR is defined or not) informing the class ID is already registered. And prints the next available class_id.
						SStream ss;
						ss << W("Meta: class ") << anamespace << W("::") << class_name << W(" is already registered.") << std::endl;
						S errmsg = ss.str();
						out () << errmsg;
						throw (errmsg);
					}

				}
			public:

				inline IdentifierName class_namespace () const noexcept
				{
					return mnamespace;
				}

				inline IdentifierName class_name () const noexcept
				{
					return mclass_name;
				}

			public:
				/// \brief Constructor taking a Version object as a parameter.
				inline MetaBase(const S& class_namespace, const S& class_name, const Version::Int pub = Version::NULL_VERSION, const Version::Int pro = Version::NULL_VERSION, const Version::Int pri = Version::NULL_VERSION) noexcept
					: mnamespace(class_namespace), mclass_name(class_name), mversion(Version::get(pub, pro, pri))
				{
					register_class (class_namespace, class_name);
				}
				inline virtual ~MetaBase() noexcept = default; // Virtual destructor for cleanup

				// Version of this meta class.
				//inline static const Version::Ptr VERSION = pd::Version::get(1, 1, 1);
				inline virtual const Version::Ptr version() const noexcept { return mversion; }

				inline virtual bool test_class_name_and_version(MemoryBuffer& mb)
				{
					// Get class_namespace from mb.
					IdentifierName class_namespace;
					mb.read_known_size((BytePtr)&class_namespace, sizeof(IdentifierName));
					if (class_namespace != mnamespace)
					{
						throw (W("Meta: Class namespace mismatch"));
					}
					// Get class_name from mb.
					IdentifierName class_name;
					mb.read_known_size((BytePtr)&class_name, sizeof(IdentifierName));
					if (class_name != mclass_name)
					{
						throw (W("Meta: Class name mismatch"));
					}

					Version v(mb);
					if (v != *mversion)
					{
						throw (W("Meta: Class version mismatch"));
					}
					return true;
				}
		};
		
		template < class T, class... Parents>
		class Meta : public MetaBase
		{
			public:
				using Ptr = std::shared_ptr<Meta<T, Parents...>>; //!< Pointer type for this meta information class.
				using Type = T; //!< The type of this meta information class refers to.
				using ParentTypes = std::tuple<Parents...>; //!< Tuple of parent types.
				

				/// \brief Constructor for the Meta class.
				Meta (const S& class_namespace, const S& class_name, const Version::Int pub = Version::NULL_VERSION, const Version::Int pro = Version::NULL_VERSION, const Version::Int pri = Version::NULL_VERSION) noexcept
					: MetaBase(class_namespace, class_name, pub, pro, pri)
				{
				}

				inline const S sclass_name () const noexcept
				{
					return mclass_name;
				}

				inline const S sclass_namespace () const noexcept
				{
					return mnamespace;
				}

				/// \brief Returns the type name of the class.
				inline const S type_name() const noexcept
				{
					S stype_name = sclass_namespace ();
					stype_name += W("::");
					stype_name += sclass_name ();
					return stype_name;
				}

				/// \brief Returns the number of parent classes.
				static constexpr size_t parent_count() noexcept
				{
					return sizeof...(Parents);
				}

				/// \brief Returns the parent type at the given index.
				template <size_t Index>
				static constexpr auto parent_type() noexcept
				{
					static_assert(Index < sizeof...(Parents), W("Index out of bounds for parent types."));
					return std::tuple_element_t<Index, ParentTypes>();
				}

				/// \brief Returns a tuple of all parent types.
				static constexpr ParentTypes parent_types() noexcept
				{
					return ParentTypes{};
				}

				/// \brief Returns a string representation of the meta information.
				auto to_s() const noexcept
				{
					return type_name () + W("v") + mversion->to_s ();
				}

				static Ptr get(const S& class_namespace, const S& class_name, const Version::Int pub = Version::NULL_VERSION, const Version::Int pro = Version::NULL_VERSION, const Version::Int pri = Version::NULL_VERSION) noexcept
				{
					return std::make_shared<Meta<T, Parents...>>(class_namespace, class_name, pub, pro, pri);
				}
		};
	}
}
#endif // META_HPP

