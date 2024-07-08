// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "object.hpp"
#include "version.hpp"

//#include "type_util.hpp"
//#include "object_factory.hpp"

#include <iosfwd>
#include <bit>
#include <stdexcept>
#include <vector>

namespace pd = pensar_digital::cpplib;

namespace pensar_digital
{
	namespace cpplib
	{
        /*
        void Object::read_bin_obj(std::istream& is, const std::endian& byte_order)
        {
            S sclass_name;
            binary_read(is, sclass_name, byte_order);
            if (sclass_name != class_name())
                throw new std::runtime_error("Object::read: class name mismatch.");
            binary_read<Id>(is, mdata.mid, byte_order);
        }
        */

        void Object::read_bin_version(std::istream& is, const Version& version, const std::endian& byte_order)
        {
            Version v;
            v.binary_read(is, byte_order);
            if (version != v)
                throw new std::runtime_error("Version mismatch.");
        }

        // implements input stream member virtual std::istream& Object::read(std::istream& is)
        std::istream& Object::binary_read (std::istream& is, const std::endian& byte_order)
        {
            //read_bin_obj(is, byte_order);
            bin_read (is, byte_order);
            read_bin_version(is, *VERSION, byte_order);
            is.read((char*)data (), data_size ());
            return is;
        };

        std::istream& binary_read(std::vector<ObjectPtr>& v, std::istream& is, const std::endian& byte_order)
        {
            return is;
        }

        std::ostream& binary_write(const std::vector<ObjectPtr>& v, std::ostream& os, const std::endian& byte_order)
        {
            return os;
        }

        std::ostream& Object::binary_write(std::ostream& os, const std::endian& byte_order) const
        {
            //binary_write     (os, class_name(), byte_order);
            //binary_write<Id> (os, mdata.mid, byte_order);
            bin_write (os, byte_order);
            VERSION->binary_write (os, byte_order);
            os.write ((const char *)data(), data_size());
            return os;
        };
    }
}                                                                                                                                                                                                                                                                                                                                                                                           