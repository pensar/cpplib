// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef IOBJECT_HPP
#define IOBJECT_HPP

#include "constant.hpp"
#include "string_def.hpp"
#include "header_lib/json.hpp"


#include <iostream>
#include <memory> // for std::shared_ptr

namespace pensar_digital
{
    namespace cpplib
    {
        class IObject;
        class IObjectRO;
        typedef std::shared_ptr<IObjectRO> IObjectROPtr;
        typedef std::shared_ptr<IObject> IObjectPtr;

        // / \brief Base class for all objects. Read only interface of Object.
        class IObjectRO
        {
            public:
                //inline static const Version VERSION = Version(1, 1, 1);

                virtual ~IObjectRO() noexcept = default;

                virtual String class_name() const = 0;
                    
                /// Debug string.
                /// \return A string with the object id.
                virtual String debug_string() const noexcept = 0;

                /// Check if passed object equals self.
                /// Derived classes must implement the _equals method. The hash compare logic is made on equals.
                /// _equals is called from template method equals and should only implement the specific comparison.
                /// \see _equals
                /// \return true if objects have the same id, false otherwise.
                virtual bool equals(const IObjectRO& o) const noexcept = 0;

                /// Access object id
                /// \return The current value of id
                ///
                virtual const Id get_id() const noexcept = 0;

                /// \brief Access hash
                ///
                /// \return  The current value of hash
                virtual const Hash get_hash() const noexcept = 0;

                // Conversion to json string.
                virtual String json() const noexcept = 0;

                /// Conversion to string.
                /// \return A string with the object id.
                virtual String to_string() const noexcept = 0;

                virtual std::ostream& write(std::ostream& os) const = 0;
        
                // Conversion to xml string.s
                virtual String xml() const noexcept = 0;

                // Comparison operators.
                virtual bool operator == (const IObjectRO& o) const = 0;
                virtual bool operator != (const IObjectRO& o) const = 0;

                /// Implicit conversion to string.
                /// \return A string with the object id.
                virtual operator String () const noexcept = 0;   
        };

        class IObject : public virtual IObjectRO
        {
            public:
                //inline static const Version VERSION = Version(1, 1, 1);

                virtual ~IObject() noexcept = default;

                /// Sets id value.
                /// \param new value for id.
                virtual void set_id (const Id& value) = 0;

                /// Reads object from stream.
                virtual std::istream& read(std::istream& is) = 0;

                // Conversion from xml string.
                virtual void from_xml(const String & sxml) = 0;
        };
        //extern Factory<IObject, Id> object_factory;

    }  // namespace cpplib
}      // namespace pensar_digital
#endif // OBJECT_HPP

