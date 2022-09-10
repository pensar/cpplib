#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <iro_object.hpp>


class Object : public IROObject
{
    public:
        using VersionType                   = int16_t;
        const VersionType VERSION           = 1;
        const VersionType INTERFACE_VERSION = 1;

        /** Default constructor */
        Object(int64_t aid = NULL_ID) : IROObject<T>(aid){};

        /** Default destructor */
        virtual ~Object() {}

        /** Copy constructor
         *  \param other Object to copy from
         */
        Object(const Object& other) {id = other.id}

        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        Object& operator=(const Object& other) { return *this; }

        int64_t get_id() const { return id; }

        /** Set id
         * \param val New value to set
         */
        void set_id (int64_t value) { id = value; }

        const VersionType get_version           () const { return VERSION          ; };
        const VersionType get_interface_version () const { return INTERFACE_VERSION; };

        bool operator == (const IROObject<T>& o) const {return (o.id == id);}
        bool operator != (const IROObject<T>& o) const {return (o.id != id);}
        std::istream& operator >> (std::istream& is)       { return ReadFromStream (is);};
        std::ostream& operator << (std::ostream& os) const { return WriteToStream  (os);};

    protected:
        virtual std::istream& ReadFromStream (std::istream& is, const VersionType v)
        {
            switch (v)
            {
                case 1:
                    return is >> id;
            };
            return is;
        };

        virtual std::ostream& WriteToStream (std::ostream& os, const VersionType v) const
        {
            switch (v)
            {
                case 1:
                    return os << id;
            };
            return os;
        };

    private:
        int64_t id; //!< Member variable "id"
};

#endif // OBJECT_HPP
