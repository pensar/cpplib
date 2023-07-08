// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef FIELD_HPP
#define FIELD_HPP

#include "object.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
		template <typename T>
		class Field : public Object
		{
			private:
			T value;
			bool is_null;
			String name;
			String display_name;
			String description;

			public:
				const static Version PUBLIC_INTERFACE_VERSION    = 1;
				const static Version PROTECTED_INTERFACE_VERSION = 1;
				const static Version PRIVATE_INTERFACE_VERSION   = 1;

				virtual Version get_public_interface_version    () const noexcept { return PUBLIC_INTERFACE_VERSION; }
				virtual Version get_protected_interface_version () const noexcept { return PROTECTED_INTERFACE_VERSION; }
				virtual Version get_private_interface_version   () const noexcept { return PRIVATE_INTERFACE_VERSION; }

			Field(String name, String display_name, String description, bool is_null = true) : Object()
			{
				name = aname;
				display_name = adisplay_name;
				description = adescription;
				isNull = aisNull;
			}

			Field(String aname, String adisplay_name, String adescription, T avalue, bool ais_null = false) :
				Field(aname, adisplay_name, adescription, ais_null),
				Object()
			{
				value = avalue;
			}
			
			/// Copy constructor.
			Field (const Field &other) : Object(other)
			{
				assign (other);
			}
			
			/// Move constructor.
			Field (Field &&other) : Object(std::move(other))
			{
				assign (other);
			}

			virtual Field & assign (const Field &other)
			{
				if (this != &other)
				{
					Object::operator=(other);
					value = other.value;
					is_null = other.is_null;
					name = other.name;
					display_name = other.display_name;
					description = other.description;
				}
				return *this;
			}

			/// <summary>
			/// Move assignment.
			/// </summary>
			/// <param name="other"></param>
			/// <returns></returns>
			virtual Field& assign (Field &&other)
			{
				if (this != &other)
				{
					Object::operator=(std::move(other));
					value = std::move(other.value);
					is_null = std::move(other.is_null);
					name = std::move(other.name);
					display_name = std::move(other.display_name);
					description = std::move(other.description);
				}
				return *this;
			}

			/// Copy assignment operator.
			/// @param other Object to copy from.
			/// @return Reference to this object.
			virtual Field & operator= (const Field &other) { return assign (other); }

			/// Move assignment operator.
			virtual Field & operator= (Field &&other) {	return assign(other); }
		};

		class StringField : public Field<String>
		{
			public:
			StringField(String name, String display_name, String description, bool is_null = true) :
				Field<String>(name, display_name, description, is_null)
			{
			}
			StringField(String name, String display_name, String description, String value, bool is_null = false) :
				Field<String>(name, display_name, description, value, is_null)
			{
			}
		};
	} // namespace cpplib
}     // namespace pensar_digital
#endif

