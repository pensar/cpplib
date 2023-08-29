// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "object.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
        
        // implements input stream member virtual std::istream& Object::read(std::istream& is)
        std::istream& Object::read(std::istream& is, const IO_Mode& amode, const ByteOrder& abyte_order)
        {
            if (amode == BINARY)
            {
                // todo: implement binary read.
            }
            else // json format
            {
                String stream_class_name;
				IVersionPtr stream_version;
				Id stream_id;
                Json j;
                pd::read_json<Object>(is, *this, &stream_id, stream_version, &j);
            }
            return is;
        };

        std::ostream& Object::write (std::ostream& os, const IO_Mode& amode, const ByteOrder& abyte_order) const
        {
            if (amode == BINARY)
            {
                // todo: implement binary read.
            }
            else // json format
            {
                // Write object as json string.
                os << json();
            }
            return os;
        };

        /*
        void to_json(Json& j, const Object& o)
        {
            j["class"                      ] = o.class_name();
            j["id"                         ] = o.get_id();
            to_json (j, *o.VERSION);
        };

        void from_json(const Json& j, Object& o)
        {
            String class_name = o.class_name();
            String json_class = j.at("class");
            if (class_name == json_class)
                o.set_id(j.at("id"));
            else throw new std::runtime_error("Object expected class = " + class_name + " but json has " + json_class);
        }
        */
    }
}                                                                                                                                                                                                                                                                                                                                                                                           