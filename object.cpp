// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "object.hpp"
#include "io_util.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
        
        Object& Object::parse_json (const String& sjson)
        {
            VersionPtr v;
            Id aid;
            Json j;
            pd::read_json<Object> (sjson, *this, &aid, &v, &j);
            mid = aid;

            // todo: check version compatibility.
            if (*VERSION != *v)
                throw std::runtime_error("ObjectFactory::parse_json: version mismatch.");
            return *this;
        }

        // implements input stream member virtual std::istream& Object::read(std::istream& is)
        std::istream& Object::read(std::istream& is, const IO_Mode& amode, const ByteOrder& abyte_order)
        {
            if (amode == BINARY)
            {
                String sclass_name;
                binary_read<String>(is, sclass_name, abyte_order);
                if (sclass_name != class_name())
					throw new std::runtime_error("Object::read: class name mismatch.");
                binary_read<Id>(is, mid, abyte_order);

                Version v;
                v.read (is, amode, abyte_order);
                if (*VERSION != v)
					throw new std::runtime_error("Object::read: version mismatch.");
            }
            else // json format
            {
                String stream_class_name;
				VersionPtr stream_version;
				Id stream_id;
                Json j;
                pd::read_json<Object>(is, *this, &stream_id, &stream_version, &j);
                mid = stream_id;
                if (*VERSION != *stream_version)
                    throw new std::runtime_error("Object::read: version mismatch.");
            }
            return is;
        };

        std::ostream& Object::write (std::ostream& os, const IO_Mode& amode, const ByteOrder& abyte_order) const
        {
            if (amode == BINARY)
            {
                binary_write     (os, class_name(), abyte_order);
                binary_write<Id> (os,           mid, abyte_order);

                VERSION->write   (os, amode, abyte_order);  
            }
            else // json format
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
        
    }
}                                                                                                                                                                                                                                                                                                                                                                                           