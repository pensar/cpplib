// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "object.hpp"
#include "io_util.hpp"
//#include "header_lib/json.hpp"
//#include "type_util.hpp"
//#include "object_factory.hpp"
//#include "json_util.hpp"

namespace pd = pensar_digital::cpplib;

/*/namespace nlohmann
{
    template <>
    struct adl_serializer<pd::Object>
    {
        // note: the return type is no longer 'void', and the method only takes
        // one argument
        pd::Object from_json (const pd::Json& j) 
        {
            pd::S class_name = pd::class_name<pd::Object> ();
            pd::S json_class = j.at("class");
            if (class_name == json_class)
            {
                return *pd::Object::get.get (j.at("id"));
            }
            else throw new std::runtime_error("Object expected class = " + class_name + " but json has " + json_class);
        }
    };
}
*/
namespace pensar_digital
{
	namespace cpplib
	{
        Object& Object::from_json(const S& sjson)
        {
            VersionPtr v;
            Id aid;
            Json j;
            pd::read_json<Object> (sjson, *this, &aid, &v, &j);
            mdata.mid = aid;

            // todo: check version compatibility.
            if (*VERSION != *v)
                throw std::runtime_error("Factory::parse_json: version mismatch.");
            return *this;
        }
        
        void Object::read_bin_obj(std::istream& is, const std::endian& byte_order)
        {
            S sclass_name;
            binary_read(is, sclass_name, byte_order);
            if (sclass_name != class_name())
                throw new std::runtime_error("Object::read: class name mismatch.");
            binary_read<Id>(is, mdata.mid, byte_order);
        }

        void Object::read_bin_version(std::istream& is, const std::endian& byte_order)
        {
            Version v;
            v.read(is, BINARY, byte_order);
            if (*VERSION != v)
                throw new std::runtime_error("Version mismatch.");
        }

        // implements input stream member virtual std::istream& Object::read(std::istream& is)
        std::istream& Object::read(std::istream& is, const IO_Mode amode, const std::endian& byte_order)
        {
            if (amode == BINARY)
            {
                //read_bin_obj(is, byte_order);

                //read_bin_version(is, byte_order);
                is.read((char*)data (), data_size ());
            }
            else // json format
            {
                S stream_class_name;
				VersionPtr stream_version;
				Id stream_id;
                Json j;
                pd::read_json<Object>(is, *this, &stream_id, &stream_version, &j);
                mdata.mid = stream_id;
                if (*VERSION != *stream_version)
                    throw new std::runtime_error("Object::read: version mismatch.");
            }
            return is;
        };

        std::ostream& Object::write (std::ostream& os, const IO_Mode amode, const std::endian& byte_order) const
        {
            if (amode == BINARY)
            {
                //binary_write     (os, class_name(), byte_order);
                //binary_write<Id> (os, mdata.mid, byte_order);

                //VERSION->write   (os, amode, byte_order);  
                os.write ((const char *)data(), data_size());
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
            j["id"                         ] = o.id();
            to_json (j, *o.VERSION);
        };

        void from_json(const Json& j, Object& o)
        {
            S class_name = o.class_name();
            S json_class = j.at("class");
            if (class_name == json_class)
                o.set_id(j.at("id"));
            else throw new std::runtime_error("Object expected class = " + class_name + " but json has " + json_class);
        }
    }
}                                                                                                                                                                                                                                                                                                                                                                                           