// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef FIELD_HPP
#define FIELD_HPP

#include "object.hpp"
#include "generator.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
		template <typename T>
		class Field : public virtual Object
		{
			private:
			T mvalue;
			bool misnull;
			S mname;
			S mdisplay_name;
			S mdescription;
			static Generator<Field<T>> mgenerator;

			public:
				inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);

			Field(S name, S display_name, S description, bool isnull = true, Id id = NULL_ID) 
				: Object (id == NULL_ID ? mgenerator.get () : id),
				  mname         (name        ),
				  mdisplay_name (display_name),
				  mdescription  (description ),
				  misnull       (isnull      )
			{
			}

			Field(S name, S display_name, S description, T value, bool is_null = false) :
				Object (),
				Field  (name, display_name, description, is_null),
				mvalue (value)
			{
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
					mvalue        = other.value;
					misnull       = other.misnull;
					mname         = other.name;
					mdisplay_name = other.display_name;
					mdescription = other.description;
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
					Object::operator=(std::move (other)            );
					mvalue          = std::move (other.value       );
					misnull         = std::move (other.misnull     );
					mname           = std::move (other.name        );
					mdisplay_name   = std::move (other.display_name);
					mdescription    = std::move (other.description );
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

			virtual void set_value(const T& value) { this->mvalue = value; }

			virtual bool is_null() const { return misnull; }

			// Implicit conversion to T
			operator T() const { return value; }

			virtual S name() const { return mname; }

			virtual S display_name() const { return mdisplay_name; }

			virtual S description() const { return mdescription; }

			virtual S to_string() const noexcept { return value.to_string(); }

			// Implicit conversion to S
			operator S() const { return to_string(); }

			virtual bool operator==(const Field& other) const { return mvalue == other.mvalue; }

			virtual bool operator!=(const Field& other) const { return mvalue != other.mvalue; }

			virtual bool operator<(const Field& other) const { return mvalue < other.mvalue; }	

			virtual bool operator<=(const Field& other) const { return mvalue <= other.mvalue; }

			virtual bool operator>(const Field& other) const { return mvalue > other.mvalue; }
			
			virtual bool operator>=(const Field& other) const { return mvalue >= other.mvalue; }

			// Implicit conversion from value type.
			Field& operator=(const T& value) { this->mvalue = value; return *this; }
		};

		class StringField : public virtual Field<S>
		{
			public:
				inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);
				StringField(S name, S display_name, S description, bool is_null = true) :
				Field<S>(name, display_name, description, is_null)
			{
			}
			StringField(S name, S display_name, S description, S value, bool is_null = false) :
				Field<S>(name, display_name, description, value, is_null)
			{
			}

			/// Copy constructor.
			StringField(const StringField& other) : Field<S>(other)
			{
				assign (other);
			}

			/// Move constructor.
			StringField(StringField&& other) : Field<S>(std::move(other))
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

