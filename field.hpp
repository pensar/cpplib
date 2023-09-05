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
		class Field : public virtual Object
		{
			private:
			T value;
			bool isnull;
			String name;
			String display_name;
			String description;

			public:
				inline static const VersionPtr VERSION = pd::versionf.get (1, 1, 1);

			Field(String name, String adisplay_name, String adescription, bool aisnull = true) : Object()
			{
				name = aname;
				display_name = adisplay_name;
				description = adescription;
				isnull = aisnull;
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
					isnull = other.isnull;
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
					isnull = std::move(other.isnull);
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

			virtual ~Field() {}

			virtual T value() const { return value; }

			virtual void set_value(const T& value) { this->value = value; }

			virtual bool is_null() const { return isnull; }

			// Implicit conversion to T
			operator T() const { return value; }

			virtual String name() const { return name; }

			virtual String display_name() const { return display_name; }

			virtual String description() const { return description; }

			virtual String to_string() const { return value.to_string(); }

			// Implicit conversion to String
			operator String() const { return to_string(); }

			virtual bool operator==(const Field& other) const { return value == other.value; }

			virtual bool operator!=(const Field& other) const { return value != other.value; }

			virtual bool operator<(const Field& other) const { return value < other.value; }	

			virtual bool operator<=(const Field& other) const { return value <= other.value; }

			virtual bool operator>(const Field& other) const { return value > other.value; }
			
			virtual bool operator>=(const Field& other) const { return value >= other.value; }

			// Implicit conversion from value type.
			Field& operator=(const T& value) { this->value = value; return *this; }
		};

		class StringField : public virtual Field<String>
		{
			public:
				inline static const VersionPtr VERSION = pd::versionf.get (1, 1, 1);
				StringField(String name, String display_name, String description, bool is_null = true) :
				Field<String>(name, display_name, description, is_null)
			{
			}
			StringField(String name, String display_name, String description, String value, bool is_null = false) :
				Field<String>(name, display_name, description, value, is_null)
			{
			}

			/// Copy constructor.
			StringField(const StringField& other) : Field<String>(other)
			{
				assign (other);
			}

			/// Move constructor.
			StringField(StringField&& other) : Field<String>(std::move(other))
			{
				assign (other);
			}

			virtual StringField& assign(const StringField& other)
			{
				if (this != &other)
				{
					Field<String>::assign(other);
				}
				return *this;
			}

			virtual ~StringField() {}

			/// Copy assignment operator.
			/// @param other Object to copy from.
			/// @return Reference to this object.
			virtual StringField& operator=(const StringField& other) { return assign(other); }

			/// Move assignment operator.
			virtual StringField& operator=(StringField&& other) { return assign(other); }


		};
	} // namespace cpplib
}     // namespace pensar_digital
#endif

