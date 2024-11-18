// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "pobject.hpp"
#include "io_util.hpp"

namespace pd = pensar_digital::cpplib;

/*/namespace nlohmann
{
    template <>
    struct adl_serializer<pd::PObject>
    {
        // note: the return type is no longer 'void', and the method only takes
        // one argument
        pd::PObject from_json (const pd::Json& j)
        {
            pd::S class_name = pd::class_name<pd::PObject> ();
            pd::S json_class = j.at("class");
            if (class_name == json_class)
            {
                return *pd::PObject::get.get (j.at("id"));
            }
            else throw new std::runtime_error("PObject expected class = " + class_name + " but json has " + json_class);
        }
    };
}
*/
namespace pensar_digital
{
    namespace cpplib
    {
        PObject& PObject::from_json(const S& sjson)
        {
            VersionPtr v;
            Id aid;
            Json j;
            pd::read_json<PObject>(sjson, *this, &aid, &v, &j);
            mdata.mid = aid;

            // todo: check version compatibility.
            if (*VERSION != *v)
                throw std::runtime_error("Factory::parse_json: version mismatch.");
            return *this;
        }

        void PObject::read_bin_obj(std::istream& is, const std::endian& byte_order)
        {
            S sclass_name;
            binary_read(is, sclass_name, byte_order);
            if (sclass_name != class_name())
                throw new std::runtime_error("PObject::read: class name mismatch.");
            binary_read<Id>(is, mdata.mid, byte_order);
        }

        void PObject::read_bin_version(std::istream& is, const std::endian& byte_order)
        {
            Version v;
            v.read(is, BINARY, byte_order);
            if (*VERSION != v)
                throw new std::runtime_error("Version mismatch.");
        }

        // implements input stream member virtual std::istream& PObject::read(std::istream& is)
        std::istream& PObject::read(std::istream& is, const IO_Mode amode, const std::endian& byte_order)
        {
            if (amode == BINARY)
            {
                read_bin_obj(is, byte_order);

                read_bin_version(is, byte_order);
            }
            else // json format
            {
                S stream_class_name;
                VersionPtr stream_version;
                Id stream_id;
                Json j;
                pd::read_json<PObject>(is, *this, &stream_id, &stream_version, &j);
                mdata.mid = stream_id;
                if (*VERSION != *stream_version)
                    throw new std::runtime_error("PObject::read: version mismatch.");
            }
            return is;
        };

        std::ostream& PObject::write(std::ostream& os, const IO_Mode amode, const std::endian& byte_order) const
        {
            if (amode == BINARY)
            {
                binary_write(os, class_name(), byte_order);
                binary_write<Id>(os, mdata.mid, byte_order);

                VERSION->write(os, amode, byte_order);
            }
            else // json format
            {
                // Write object as json string.
                os << json();
            }
            return os;
        };


        void to_json(Json& j, const PObject& o)
        {
            j["class"] = o.class_name();
            j["id"] = o.id();
            to_json(j, *o.VERSION);
        };

        void from_json(const Json& j, PObject& o)
        {
            S class_name = o.class_name();
            S json_class = j.at("class");
            if (class_name == json_class)
                o.set_id(j.at("id"));
            else throw new std::runtime_error("PObject expected class = " + class_name + " but json has " + json_class);
        }
    }
}