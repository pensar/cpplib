// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "object.hpp"
#include "io_util.hpp"
//#include "type_util.hpp"
//#include "object_factory.hpp"

namespace pd = pensar_digital::cpplib;

namespace pensar_digital
{
	namespace cpplib
	{
        void Object::read_bin_obj(InStream& is, const std::endian& byte_order)
        {
            S sclass_name;
            binary_read(is, sclass_name, byte_order);
            if (sclass_name != class_name())
                throw new std::runtime_error("Object::read: class name mismatch.");
            binary_read<Id>(is, mdata.mid, byte_order);
        }

        void Object::read_bin_version(InStream& is, const Version& version, const std::endian& byte_order)
        {
            Version v;
            v.read(is, byte_order);
            if (version != v)
                throw new std::runtime_error("Version mismatch.");
        }

        // implements input stream member virtual std::istream& Object::read(std::istream& is)
        InStream& Object::read (InStream& is, const std::endian& byte_order)
        {
            //read_bin_obj(is, byte_order);
            read_bin_version(is, *VERSION, byte_order);
            is.read((C*)data (), data_size ());
            return is;
        };

        InStream& read(std::vector<ObjectPtr>& v, InStream& is, const std::endian& byte_order)
        {
            return is;
        }

        OutStream& write(const std::vector<ObjectPtr>& v, OutStream& os, const std::endian& byte_order)
        {
            return os;
        }

        OutStream& Object::write (OutStream& os, const std::endian& byte_order) const
        {
            //binary_write     (os, class_name(), byte_order);
            //binary_write<Id> (os, mdata.mid, byte_order);
            VERSION->write (os, byte_order);  
            os.write ((const C *)data(), data_size());
            return os;
        };
    }
}                                                                                                                                                                                                                                                                                                                                                                                           