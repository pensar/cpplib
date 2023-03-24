#ifndef GENERATOR_HPP_INCLUDED
#define GENERATOR_HPP_INCLUDED

#include "version.hpp"

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
      template <class T = int>
      class Generator : public Version
      {
        public:
        /// \brief Constructs a Generator.
        /// \param [in] initial_value Initial value for the generator, defaults to 0.
        /// \param [in] astep Step to be used when incrementing the generator, defaults to 1.
        Generator(T initial_value = 0, T astep = 1): Version(0), value(initial_value), step(astep) {};
        virtual ~Generator(){};

        /// \brief Increments value and return the new value.
        /// \return The new value.
        T get () { value += step; return value; }

        /// \brief Gets the next value without incrementing the current one.
        /// \return The next value.
        T get_next () const { return (value + step); }

        /// \brief Gets the current value.
        /// \return The current value.
        T get_current () const { return value; }

        /// \brief Set value. Next call to get will get value + 1.
        /// \param val New value to set
        void set_value(T val) { value = val; }
        using Version::operator <<;
        using Version::operator >>;

        virtual std::istream& ReadFromStream (std::istream& is, const Version::V v)
        {
            Version::V version;
            is >> version;
            switch (version)
            {
                case 0:
                  is >> step >> value;
                default:
                   throw UnsupportedVersion (version);
            };
            return is;
        };

        virtual std::ostream& WriteToStream (std::ostream& os, const Version::V v) const
        {
            switch (v)
            {
                case 0:
                    os << value << step << v;
                default:
                   throw UnsupportedVersion (v);
            };
            return os;
        };

        private:

        T value; //!< Member variable "id"
        T step;  //!< Step to increment value.
      };
    }
}
/// \example GeneratorTest.cpp

#endif // GENERATOR_HPP_INCLUDED
