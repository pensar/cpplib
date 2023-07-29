#include "object.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
        
        std::istream& operator >> (std::istream& is, Object& o) { return o.read(is); };
        std::ostream& operator << (std::ostream& os, const Object& o) { return o.write(os); };
        Object::ObjectFactory Object::factory(3, 1, 0);
        Factory<Object, Id> object_factory(0);

        void to_json(Json& j, const Object& o)
        {
            j["class"                      ] = o.class_name();
            j["id"                         ] = o.get_id();
            j["public_interface_version"   ] = o.VERSION.PUBLIC;
            j["protected_interface_version"] = o.VERSION.PROTECTED;
            j["private_interface_version"  ] = o.VERSION.PRIVATE;
        };

        void from_json(const Json& j, Object& o)
        {
            String class_name = o.class_name();
            String json_class = j.at("class");
            if (class_name == json_class)
                o.set_id(j.at("id"));
            else throw new std::runtime_error("Object expected class = " + class_name + " but json has " + json_class);
        }
    }
}                                                                                                                                                                                                                                                                                                                                                                                           