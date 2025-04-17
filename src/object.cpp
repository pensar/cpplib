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
            read_bin_version(is, *(version ()), byte_order);
            is.read((char*)data (), data_size ());
            return is;
        };

        std::ostream& Object::binary_write(std::ostream& os, const std::endian& byte_order) const
        {
            bin_write (os, byte_order);                   // Writes Object.
            VersionPtr v = version();
            v->binary_write(os, byte_order);     // Writes the polymorphic Version.
            os.write ((const char *)data(), data_size()); // Writes the polymorphic data.
            return os;
        };
    }
}                                                                                                                                                                                                                                                                                                                                                                                           