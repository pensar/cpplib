// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef IGENERATOR_HPP_INCLUDED
#define IGENERATOR_HPP_INCLUDED

#include "iobject.hpp"
#include "version_factory.hpp"

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
        class IGeneratorRO : public virtual IObjectRO
        {
            public:
                inline static const IVersionPtr VERSION = pd::versionf.get (1, 1, 1);

            virtual ~IGeneratorRO() noexcept = default;

            /// \brief Gets the current value.
            /// \return The current value.
            virtual const Id get_current() const = 0;

            // Conversion to json string.
            virtual String json() const noexcept = 0;

            virtual std::ostream& write(std::ostream& os) const = 0;
        };

        template <class T>
        class IGenerator : public virtual IGeneratorRO<T>, public virtual IObject
        {
			public:
				inline static const IVersionPtr VERSION = pd::versionf.get (1, 1, 1);
                
                virtual ~IGenerator() noexcept = default;

                /// \brief Increments value and return the new value.
                /// \return The new value.
                virtual const Id get() = 0;

                /// \brief Gets the next value without incrementing the current one.
                /// \return The next value.
                virtual const Id get_next() = 0;
                
                /// \brief Set value. Next call to get will get value + 1.
                /// \param val New value to set
                virtual void set_value(Id val) = 0;

                virtual std::istream& read(std::istream& is) = 0;
        };
    }   // namespace cpplib 
}   // namespace pensar_digital


#endif // IGENERATOR_HPP_INCLUDED
