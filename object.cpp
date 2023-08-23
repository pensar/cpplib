// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "object.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
        
        inline std::istream& operator >> (std::istream& is,       IObject&   o) { return o.read(is);  }
        inline std::ostream& operator << (std::ostream& os, const IObject&   o) { return o.write(os); }
        inline std::istream& operator >> (std::istream& is,       IObjectPtr o) { return is >> *o;    }
        inline std::ostream& operator << (std::ostream& os, const IObjectPtr o) { return os << *o;    }

        // implements input stream member virtual std::istream& Object::read(std::istream& is)
        std::istream& Object::read(std::istream& is)
        {
			// Check if os is in binary mode.
            if (is.flags() & std::ios::binary)
            {
				String stream_class_name;
				Version stream_version;
				Id stream_id;
				return is >> stream_class_name >> stream_version >> stream_id;
			}
            else
            {
				return read_json<Object>(is, *this);
			}
		};

        std::ostream& Object::write(std::ostream& os) const
        {
            // Check if os is in binary mode.
            if (os.flags() & std::ios::binary)
            {
                os << class_name() << VERSION << id;
            }
            else
            {
                // Write object as json string.
                os << json();
            }
            return os;
        };

        void to_json(Json& j, const Object& o)
        {
            j["class"                      ] = o.class_name();
            j["id"                         ] = o.get_id();
            j["mpublic"   ] = o.VERSION->get_public    ();
            j["mprotected"] = o.VERSION->get_protected ();
            j["mprivate"  ] = o.VERSION->get_private   ();
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