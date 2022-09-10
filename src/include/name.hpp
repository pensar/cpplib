#ifndef NAMED_HPP_INCLUDED
#define NAMED_HPP_INCLUDED

#include "string_def.hpp"
#include "version.hpp"

#include <string>
#include <iostream>

namespace pensar_digital
{
    namespace cpplib
    {
        /// Inherit from ReadOnlyName to have a read only name atributte.
        /// \tparam String The string class used to hold the name attribute.
        template <class String = string>
        class ReadOnlyName : public Version
        {
            protected:
            ~ReadOnlyName (){};

            public:
            ReadOnlyName (const String& aname): Version(0), name(aname){};

            /// Access name
            /// \return The current value of name
            String get_name () const { return name; }

            bool operator == (const ReadOnlyName<String>& aname) const {return (aname.name == name);}
            bool operator != (const ReadOnlyName<String>& aname) const {return (aname.name != name);}
            using Version::operator <<;
            using Version::operator >>;

            virtual std::istream& ReadFromStream (std::istream& is, const Version::V v)
            {
                switch (v)
                {
                    case 0:
                        return is >> name;
                };
                return is;
            }

            virtual std::ostream& WriteToStream (std::ostream& os, const Version::V v) const
            {
                switch (v)
                {
                    case 0:
                        return os << name;
                };
                return os;
            }

            protected:
            String name; //!< Member variable "name"
       };

        /// Inherit from Name to have a read/write name atributte.
        /// \tparam String The string class used to hold the name attribute.
        template <class String = string>
        class Name : public ReadOnlyName<String>
        {
            public:
            Name (const String& aname): ReadOnlyName<String>(aname) {};

            using ReadOnlyName<String>::operator ==;
            using ReadOnlyName<String>::operator !=;
            using ReadOnlyName<String>::operator <<;
            using ReadOnlyName<String>::operator >>;

            /// Set name
            /// \param val New value to set
            void set_name (const String& val) { ReadOnlyName<String>::name = val; }
        };
    }
}

#endif // NAMED_HPP_INCLUDED
