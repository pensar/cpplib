#ifndef META_HPP
#define META_HPP

#include "version.hpp"
#include "s.hpp"
#include "cs.hpp"

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
			using IdentifierName = CS<0, 1024>;
			using Ptr = std::shared_ptr<MetaBase>; //!< Pointer type for this base meta information class.
		protected:
			Version::Ptr mversion;		//!< Pointer to the version of T class (see class Meta below).
			Id mclass_id;				//!< Unique identifier for the class.
			IdentifierName mnamespace;	//!< Namespace of the class.
			IdentifierName mclass_name; //!< Name of the class.

			// Unique sorted list holding all class IDs.
			inline static SortedList<Id> mIds = SortedList<Id>({}, true);
			inline static bool register_class(const Id& class_id)
			{
				return mIds.add (class_id);
			}
				

			public:
				/// \brief Constructor taking a Version object as a parameter.
				inline MetaBase(const Id class_id, const S& class_namespace, const S& class_name, const Version::Int pub = Version::NULL_VERSION, const Version::Int pro = Version::NULL_VERSION, const Version::Int pri = Version::NULL_VERSION) noexcept
					: mclass_id(class_id), mnamespace(class_namespace), mclass_name(class_name), mversion(Version::get(pub, pro, pri))
				{
				}
				inline virtual ~MetaBase() noexcept = default; // Virtual destructor for cleanup

				// Version of this meta class.
				//inline static const Version::Ptr VERSION = pd::Version::get(1, 1, 1);
				inline virtual const Version::Ptr version() const noexcept { return mversion; }

				/// \brief Returns the class ID.
				inline virtual constexpr Id class_id() const noexcept
				{
					return mclass_id;
				}


				inline virtual bool test_class_id_and_version(MemoryBuffer& mb)
				{
					// Get class_id from mb.
					Id class_id;
					mb.read_known_size((BytePtr)&class_id, sizeof(Id));
					if (class_id != mclass_id)
					{
						throw (W("Meta: Class ID mismatch"));
					}

					Version v(mb);
					if (v != *mversion)
					{
						throw (W("Meta: Version mismatch"));
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
				Meta (const Id class_id, const S& class_namespace, const S& class_name, const Version::Int pub = Version::NULL_VERSION, const Version::Int pro = Version::NULL_VERSION, const Version::Int pri = Version::NULL_VERSION) noexcept
					: MetaBase(class_id, class_namespace, class_name, pub, pro, pri)
				{
				}

				/// \brief Returns the type name of the class.
				static
					constexpr const char* type_name() noexcept
				{
					return typeid(T).name();
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

				/// \brief Returns a tuple of all child types.

				/// \brief Returns a string representation of the type name.
				static std::string type_name_str() noexcept
				{
					return std::string(typeid(T).name());
				}

				/// \brief Returns a string representation of the parent types.
				static std::string parent_types_str() noexcept
				{
					S result;
					result += W("Parents: ");
					((result += typeid(Parents).name(), result += W(" ")), ...);
					return result;
				}

				/// \brief Returns a string representation of the meta information.
				static S to_string() noexcept
				{
					return W("Type: ") + type_name_str() + W("\n") + parent_types_str();
				}

				static Ptr get(const Id class_id, const S& class_namespace, const S& class_name, const Version::Int pub = Version::NULL_VERSION, const Version::Int pro = Version::NULL_VERSION, const Version::Int pri = Version::NULL_VERSION) noexcept
				{
					return std::make_shared<Meta<T, Parents...>>(class_id, class_namespace, class_name, pub, pro, pri);
				}
		};
	}
}
#endif // META_HPP

