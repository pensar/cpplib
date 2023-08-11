// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef GENERATOR_HPP_INCLUDED
#define GENERATOR_HPP_INCLUDED

#include "object.hpp"
#include "igenerator.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
      /// Generator is meant to be used as unique identifier generator for other classes.
      ///
      /// Usually this is used as a static member for an entity class in conjunction with the Id mixin class like in the Person class:
      /// \code {.cpp}
      ///  template <class String = std::wstring, class T = int> class Person : public Name<String>, public Id<T>
      ///  {
      ///    public:
      ///      Person(const String& aname = L"", const T aid = -1): Name<String>(aname), Id<T>((aid == -1) ? generator.get() : aid){};
      ///      virtual ~Person(){};
      ///
      ///    static Generator<T> generator;
      ///
      ///  };
      /// \endcode
      template <class T>
      class Generator : public Object
      {
        public:
            inline static const Version VERSION = Version(1, 1, 1);

            typedef IGenerator<T>     I; // Interface type.
            typedef IGeneratorRO<T> IRO; // Read only interface type.

            /// \brief Constructs a Generator.
            /// \param [in] initial_value Initial value for the generator, defaults to 0.
            /// \param [in] astep Step to be used when incrementing the generator, defaults to 1.
            Generator(Id aid = NULL_ID, Id initial_value = 0, Id step = 1) : Object(aid), fvalue(initial_value), fstep(step) {};

            virtual ~Generator () = default;

            /// \brief Increments value and return the new value.
            /// \return The new value.
            virtual const Id get() { fvalue += fstep; return fvalue; }

            /// \brief Gets the next value without incrementing the current one.
            /// \return The next value.
            virtual const Id get_next() { return (fvalue + fstep); }

            /// \brief Gets the current value.
            /// \return The current value.
            virtual const Id get_current () const { return fvalue; }

            /// \brief Set value. Next call to get will get value + 1.
            /// \param val New value to set
            virtual void set_value(Id val) { fvalue = val; }

            // Conversion to json string.
            virtual String json() const noexcept
            {
                return pd::json<Generator>(*this);
            }

            virtual std::istream& read(std::istream& is)
            {
                return read_json<Generator<T>>(is, *this);
            };

            virtual std::ostream& write(std::ostream& os) const
            {
                return write_json<Generator<T>>(os, *this);
            };
            //friend void to_json(Json& j, const Generator<T>& g);
            //friend void from_json(const Json& j, Generator<T>& d);

        private:
            Id fvalue; //!< Member variable "id"
            Id fstep;  //!< Step to increment value.
      };
      /*
      template <typename T>
      void to_json(Json& j, const Generator<T>& g)
      {
          j["class"] = g.class_name();
          j["id"] = g.get_id();
          j["private_interface_version"] = g.get_private_interface_version();
          j["protected_interface_version"] = g.get_protected_interface_version();
          j["public_interface_version"] = g.get_public_interface_version();
          j["name"] = g.get_name();
      };

      template <typename T>
      void from_json(const Json& j, Generator<T>& d)
      {
      };
      */
    }
}
/// \example GeneratorTest.cpp

#endif // GENERATOR_HPP_INCLUDED
