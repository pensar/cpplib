// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef GENERATOR_HPP_INCLUDED
#define GENERATOR_HPP_INCLUDED

#include "object.hpp"
#include "error.hpp"

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

            const static Version PUBLIC_INTERFACE_VERSION    = 1;
            const static Version PROTECTED_INTERFACE_VERSION = 1;
            const static Version PRIVATE_INTERFACE_VERSION   = 1;

            virtual Version get_public_interface_version    () const noexcept { return PUBLIC_INTERFACE_VERSION;    }
            virtual Version get_protected_interface_version () const noexcept { return PROTECTED_INTERFACE_VERSION; }
            virtual Version get_private_interface_version   () const noexcept { return PRIVATE_INTERFACE_VERSION;   }

        /// \brief Constructs a Generator.
        /// \param [in] initial_value Initial value for the generator, defaults to 0.
        /// \param [in] astep Step to be used when incrementing the generator, defaults to 1.
            Generator(Id aid = NULL_ID, Id initial_value = 0, Id step = 1) : Object(aid), fvalue(initial_value), fstep(step) {};
        virtual ~Generator(){};

        /// \brief Increments value and return the new value.
        /// \return The new value.
        Id get () { fvalue += fstep; return fvalue; }

        /// \brief Gets the next value without incrementing the current one.
        /// \return The next value.
        Id get_next () const { return (fvalue + fstep); }

        /// \brief Gets the current value.
        /// \return The current value.
        Id get_current () const { return fvalue; }

        /// \brief Set value. Next call to get will get value + 1.
        /// \param val New value to set
        void set_value(Id val) { fvalue = val; }

        virtual std::istream& ReadFromStream (std::istream& is)
        {
            Version version;
            is >> version;
            switch (version)
            {
                case PUBLIC_INTERFACE_VERSION:
                  is >> fstep >> fvalue;
                default:
                   throw UnsupportedVersion (version);
            };
            return is;
        };

        virtual std::ostream& WriteToStream (std::ostream& os) const
        {
            Version v = get_public_interface_version ();
            switch (get_public_interface_version ())
            {
                case PUBLIC_INTERFACE_VERSION:
                    os << fvalue << fstep << PUBLIC_INTERFACE_VERSION;
                default:
                   throw UnsupportedVersion (v);
            };
            return os;
        };

        private:

        Id fvalue; //!< Member variable "id"
        Id fstep;  //!< Step to increment value.
      };
    }
}
/// \example GeneratorTest.cpp

#endif // GENERATOR_HPP_INCLUDED
