#ifndef ID_HPP_INCLUDED
#define ID_HPP_INCLUDED

#include "version.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        /// Inherit from ReadOnlyId to have a read only id atributte.
        /// \tparam T The type used to hold the id attribute.
        template <class T = int64_t>
        class ReadOnlyId : public Version
        {
            protected:
            ~ReadOnlyId() {}

            public:
            typedef T value_type;
            ReadOnlyId (const T aid = -1): Version(0), id(aid) {}

            /// Access id
            /// \return The unique identifier.
            T get_id () const { return id;}

            bool operator == (const ReadOnlyId<T>& aid) const {return (aid.id == id);}
            bool operator != (const ReadOnlyId<T>& aid) const {return (aid.id != id);}
            using Version::operator <<;
            using Version::operator >>;

            virtual std::istream& ReadFromStream (std::istream& is, const Version::V v)
            {
                switch (v)
                {
                    case 0:
                        return is >> id;
                };
                return is;
            };

            virtual std::ostream& WriteToStream (std::ostream& os, const Version::V v) const
            {
                switch (v)
                {
                    case 0:
                        return os << id;
                };
                return os;
            };

            protected:
            T id; ///< Member variable "id"
        };

        /// Inherit from Id to have a read/write id atributte.
        /// \tparam T The type used to hold the id attribute.
        template <class T = int64_t>
        class Id : public ReadOnlyId<T>
        {
            protected:
            ~Id() {};

            public:
            Id (const T aid = -1): ReadOnlyId<T>(aid) {}

            /// Set id
            /// \param val New value to set
            ///
            void set_id(T val) { ReadOnlyId<T>::id = val;}

            using ReadOnlyId<T>::operator ==;
            using ReadOnlyId<T>::operator !=;
            using ReadOnlyId<T>::operator <<;
            using ReadOnlyId<T>::operator >>;
        };

/// \example id_test.cpp
    }
}
#endif // ID_HPP_INCLUDED
