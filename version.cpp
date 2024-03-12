// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "version.hpp"
#include "type_util.hpp"
#include "io_util.hpp"
#include "concept.hpp"
#include "s.hpp"

#include <iostream>

namespace pensar_digital
{
	namespace cpplib
	{
		InStream& operator >> (InStream& is, Version& v)
		{
			return is >> v.mdata.mpublic >> v.mdata.mprotected >> v.mdata.mprivate;
		}
				
		OutStream& operator << (OutStream& os, const Version& v)
		{
			return os << v.get_public() << W(".") << v.get_protected() << W(".") << v.get_private();
		}

		S Version::debug_string() const noexcept
		{
			return S();
		}
		
		bool Version::equals(const Version& v) const noexcept
		{
			static_assert (Hashable<Version>);
			return (v.id () == id ()) && (v.mdata.mprivate == mdata.mprivate) && (v.mdata.mprotected == mdata.mprotected) && (v.mdata.mpublic == mdata.mpublic);
		}

		const Hash Version::get_hash() const noexcept
		{
			return Hash (897896785686); // todo: implement hash.
		}
	
		InStream& Version::read (InStream& is, const std::endian& byte_order)
		{
				/* String sclass_name;
				binary_read(is, sclass_name, byte_order);
				if (sclass_name != class_name())
					throw new std::runtime_error("Version::read: class name mismatch.");
				binary_read<Id>        (is, mdata.mid, byte_order);
				binary_read<VersionInt>(is, mdata.mpublic, byte_order);
				binary_read<VersionInt>(is, mdata.mprotected, byte_order);
				binary_read<VersionInt>(is, mdata.mprivate, byte_order);
				*/
				is.read((char*)data(), data_size());
			return is;
		}

		OutStream& Version::write (OutStream& os, const std::endian& byte_order) const
		{
				/*
				binary_write(os, class_name(), byte_order);
				binary_write<Id> (os, mid, byte_order);
				binary_write<VersionInt>(os, mpublic);
				binary_write<VersionInt>(os, mprotected);
				binary_write<VersionInt>(os, mprivate);
				return os;
				*/
				os.write((const char*)data(), data_size());
			return os;
		}
	} // namespace cpplib
} // namespace pensar_digital