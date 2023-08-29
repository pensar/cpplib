#include "name.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
        std::istream& operator >> (std::istream& is, Name& n) { return n.read(is); };

        std::ostream& operator << (std::ostream& os, const Name& n) { return n.write(os); };

        /*
        void to_json(Json& j, const Name& n)
        {
            j["class"                      ] = n.class_name();
            pd::to_json (j, static_cast<const Object&> (n));
            j["name"                       ] = n.get_name();
        }

        void from_json (const Json& j, Name& n)
        {
            String class_name = n.class_name();
            String json_class = j.at("class");
            if (class_name == json_class)
            {
                pd::from_json (j, n);
                n.name = j.at("name");
            }
            else throw new std::runtime_error("Expected class = " + class_name + " but json has " + json_class);
        }
        */
	}
}