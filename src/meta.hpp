#ifndef META_HPP
#define META_HPP

#include "version.hpp"
#include "s.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
		// Create a variadic template class called Meta that has the following template parameters: T (the type of this meta information class refers to), all parent classes (provide a mechanism for no parent classes case) and all children classes (provide a checanism for no children classes case). Also provide public methods to access these info in a modern C++ 23 way.
		template < class T, class... Parents>
		class Meta
		{
			private:
				Version::Ptr version; //!< Pointer to the version of T class.
				Id class_id; //!< Unique identifier for the class.
			public:
				using Ptr = std::shared_ptr<Meta<T, Parents...>>; //!< Pointer type for this meta information class.
				using Type = T; //!< The type of this meta information class refers to.
				using ParentTypes = std::tuple<Parents...>; //!< Tuple of parent types.
				
				// Version of this meta class.
				inline static const Version::Ptr VERSION = pd::Version::get(1, 1, 1);
				virtual const Version::Ptr version() const noexcept { return VERSION; }

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

				/// \brief Constructor taking a Version object as a parameter.
				Meta(const Version::Ptr& version) noexcept
					: VERSION(version)
				{
					if (!VERSION)
					{
						log_throw(W("Version is null."));
					}
					this->version = version;
				}

				/// \brief version accessor.
				inline const Version::Ptr version() const noexcept
				{
					return version;
				}
		};
	}
}
#endif // META_HPP

